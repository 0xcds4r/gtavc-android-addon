#include "main.h"

// -----------------------------------------------------------------------
void utils::log::push(const char* tag, const char* fmt, ...)
{
    char buffer[256*4];
	memset(buffer, 0, sizeof(buffer));

	va_list arg;
	va_start(arg, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, arg);
	va_end(arg);

	__android_log_write(ANDROID_LOG_INFO, tag, buffer);
}

// -----------------------------------------------------------------------
uintptr_t utils::library::handle(const char* name)
{
    char filename[0xFF] = {0},
    buffer[2048] = {0};
    FILE *fp = 0;
    uintptr_t address = 0;

    sprintf( filename, "/proc/%d/maps", getpid() );

    fp = fopen( filename, "rt" );
    if(fp == 0) {
        goto done;
    }

    while(fgets(buffer, sizeof(buffer), fp))
    {
        if( strstr( buffer, name ) )
        {
            address = (uintptr_t)strtoul( buffer, 0, 16 );
            break;
        }
    }

done:
    if(fp) {
        fclose(fp);
    }

    return address;
}

// -----------------------------------------------------------------------
#include <sys/mman.h>
#include <assert.h> // assert()
#include <dlfcn.h> // dlopen

#define HOOK_PROC "\x01\xB4\x01\xB4\x01\x48\x01\x90\x01\xBD\x00\xBF\x00\x00\x00\x00"

uintptr_t mmap_start 	= 0;
uintptr_t mmap_end		= 0;
uintptr_t memlib_start	= 0;
uintptr_t memlib_end	= 0;

extern utils::log client_log;

void utils::arm::initialise(uintptr_t lib, uintptr_t start, uintptr_t end)
{
    #ifdef VC_DEBUG
    client_log.push(LOG_CAT_TAG, "Initializing hook system | start at: 0x%X | size: 0x%X", start, end);
    #endif

    memlib_start = lib + start;
	memlib_end = memlib_start + end;

	mmap_start = (uintptr_t)mmap(0, PAGE_SIZE, PROT_WRITE | PROT_READ | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	mprotect((void*)(mmap_start & 0xFFFFF000), PAGE_SIZE, PROT_READ | PROT_EXEC | PROT_WRITE);
	mmap_end = (mmap_start + PAGE_SIZE);

    utils::arm::unprotect(lib + 0x25D0E0);
}

void utils::arm::unprotect(uintptr_t ptr, size_t dwSize)
{
	if (dwSize)
	{
		unsigned char* to_page = (unsigned char*)((unsigned int)(ptr) & 0xFFFFF000);
		size_t page_size = 0;

		for (int i = 0, j = 0; i < dwSize; ++i)
		{
			page_size = j * 4096;
			if (&((unsigned char*)(ptr))[i] >= &to_page[page_size])
				++j;
		}

		mprotect(to_page, page_size, PROT_READ | PROT_WRITE | PROT_EXEC);
		return;
	}
}

void utils::arm::makeNOP(uintptr_t addr, unsigned int count)
{
    #ifdef VC_DEBUG
    client_log.push(LOG_CAT_TAG, "NOP Calling: 0x%X | size: %d", addr, count);
    #endif

    utils::arm::unprotect(addr);

    for(uintptr_t ptr = addr; ptr != (addr+(count*2)); ptr += 2)
    {
        *(char*)ptr = 0x00;
        *(char*)(ptr+1) = 0xBF;
    }

    // cacheflush(addr, (uintptr_t)(addr + count*2), 0);
}

void utils::arm::writeMemory(uintptr_t dest, uintptr_t src, size_t size)
{
	utils::arm::unprotect(dest);
	memcpy((void*)dest, (void*)src, size);
	cacheflush(dest, dest+size, 0);
}

void utils::arm::readMemory(uintptr_t dest, uintptr_t src, size_t size)
{
    utils::arm::unprotect(src);
    memcpy((void*)dest, (void*)src, size);
}

void utils::arm::jump(uintptr_t func, uintptr_t addr)
{
	uint32_t code = ((addr-func-4) >> 12) & 0x7FF | 0xF000 | ((((addr-func-4) >> 1) & 0x7FF | 0xB800) << 16);
    utils::arm::writeMemory(func, (uintptr_t)&code, 4);
}

void utils::arm::writeHookProc(uintptr_t addr, uintptr_t func)
{
    char code[16];
    memcpy(code, HOOK_PROC, 16);
    *(uint32_t*)&code[12] = (func | 1);
    utils::arm::writeMemory(addr, (uintptr_t)code, 16);
}

void utils::arm::installHook(uintptr_t addr, uintptr_t func, uintptr_t *orig)
{
    #ifdef VC_DEBUG
    client_log.push(LOG_CAT_TAG, "installHook: 0x%X -> 0x%X", addr, func);
    #endif

    if(memlib_end < (memlib_start + 0x10) || mmap_end < (mmap_start + 0x20))
    {
        #ifdef VC_DEBUG
        client_log.push(LOG_CAT_TAG, "installHook: space limit reached");
        #endif

        std::terminate();
    }

    utils::arm::readMemory(mmap_start, addr, 4);
    utils::arm::writeHookProc(mmap_start+4, addr+4);
    *orig = mmap_start+1;
    mmap_start += 32;

    utils::arm::jump(addr, memlib_start);
    utils::arm::writeHookProc(memlib_start, func);
    memlib_start += 16;
}

void utils::arm::installPLTHook(uintptr_t addr, uintptr_t func, uintptr_t *orig)
{
    #ifdef VC_DEBUG
    client_log.push(LOG_CAT_TAG, "installPLTHook: 0x%X -> 0x%X", addr, func);
    #endif

    utils::arm::unprotect(addr);
    *orig = *(uintptr_t*)addr;
    *(uintptr_t*)addr = func;
}

void utils::arm::redirectCall(uintptr_t addr, uintptr_t func)
{
	utils::arm::jump(addr, memlib_start);
	utils::arm::writeHookProc(memlib_start, func);
	memlib_start += 16;
}

void utils::arm::installMethodHook(uintptr_t addr, uintptr_t func)
{
    #ifdef VC_DEBUG
    client_log.push(LOG_CAT_TAG, "installMethodHook: 0x%X -> 0x%X", addr, func);
    #endif

    utils::arm::unprotect(addr);
    *(uintptr_t*)addr = func;
}

void utils::arm::injectCode(uintptr_t addr, uintptr_t func, int reg)
{
    #ifdef VC_DEBUG
    client_log.push(LOG_CAT_TAG, "injectCode: 0x%X -> 0x%x (register: r%d)", addr, func, reg);
    #endif

    char injectCode[12];

    injectCode[0] = 0x01;
    injectCode[1] = 0xA0 + reg;
    injectCode[2] = (0x08 * reg) + reg;
    injectCode[3] = 0x68;
    injectCode[4] = 0x87 + (0x08 * reg);
    injectCode[5] = 0x46;
    injectCode[6] = injectCode[4];
    injectCode[7] = injectCode[5];
    
    *(uintptr_t*)&injectCode[8] = func;

    utils::arm::writeMemory(addr, (uintptr_t)injectCode, 12);
}

void utils::arm::MOVW(uintptr_t addr, int word, int reg) 
{
    utils::arm::unprotect(addr);
    utils::arm::unprotect(addr + 4);

    *(uint8_t*)(addr + 2) = ((uint8_t*)&word)[0]; // lobyte
    auto hibyte = ((uint8_t*)&word)[1];
    *(uint8_t*)(addr) = 0x40 + (hibyte / 0x10);

    if (hibyte - ((hibyte / 0x10) * 0x10) < 0x8) 
    {
        *(uint8_t*)(addr + 1) = 0xF2;
        *(uint8_t*)(addr + 3) = (hibyte - ((hibyte / 0x10) * 0x10)) * 0x10 + reg;
    }
    else 
    {
        *(uint8_t*)(addr + 1) = 0xF6;
        *(uint8_t*)(addr + 3) = ((hibyte - ((hibyte / 0x10) * 0x10)) - 0x8) * 0x10 + reg;
    }

    cacheflush(addr, addr + 4, 0);
}

// -----------------------------------------------------------------------