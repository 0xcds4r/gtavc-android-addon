#include "main.h"
#include "javawrapper.h"
#include "scripting.h"
#include <dlfcn.h>

JavaVM *javaVM = NULL;
capture_callback *m_pHandler;

utils::storage  client_storage;
utils::log      client_log;
utils::library  arm_library;
utils::arm      arm_tools;

uintptr_t g_pVCMP = 0;
uintptr_t g_pGTAVC = 0;
uintptr_t g_pImmEmu = 0;

void registerHandler(capture_callback *handler)
{
    if(handler == nullptr) return;
    m_pHandler = handler;
}

void* handlerThread(void* p)
{
    m_pHandler();
    pthread_exit(0);
}

void* handlerThreadRepeating(void* p)
{
    while(true) {
        m_pHandler();
    }

    pthread_exit(0);
}

void callHandler(uint8_t async)
{
    if(async == 1) 
    {
        pthread_t asyncThread;
        pthread_create(&asyncThread, 0, handlerThread, 0);
        return;
    }

    if(async == 2) 
    {
        pthread_t asyncThreadRep;
        pthread_create(&asyncThreadRep, 0, handlerThreadRepeating, 0);
        return;
    }
    
    m_pHandler();
}

void onLibraryLoad_handle()
{
    // ...
}

void (*CStreaming__Init)();
void CStreaming__Init_hook()
{
	CStreaming__Init();
	*(uint32_t*)(VC(0x6C78B0)) = 0x40000000;
}

void PrintSymbols(void* pc, void* lr)
{
	Dl_info info_pc, info_lr;
	if (dladdr(pc, &info_pc) != 0)
	{
		client_log.push(LOG_CAT_TAG, "PC: %s", info_pc.dli_sname);
	}
	if (dladdr(lr, &info_lr) != 0)
	{
		client_log.push(LOG_CAT_TAG, "LR: %s", info_lr.dli_sname);
	}
}

struct sigaction act_old;
struct sigaction act1_old;
struct sigaction act2_old;
struct sigaction act3_old;

void handler3(int signum, siginfo_t* info, void* contextPtr)
{
	ucontext* context = (ucontext_t*)contextPtr;

	if (act3_old.sa_sigaction)
	{
		act3_old.sa_sigaction(signum, info, contextPtr);
	}

	if (info->si_signo == SIGBUS)
	{
		client_log.push(LOG_CAT_TAG, "game crashed!");
		client_log.push(LOG_CAT_TAG, "SIGBUS | Fault address: 0x%X", info->si_addr);
		client_log.push(LOG_CAT_TAG, "register states:");
		client_log.push(LOG_CAT_TAG, "r0: 0x%X, r1: 0x%X, r2: 0x%X, r3: 0x%X",context->uc_mcontext.arm_r0, context->uc_mcontext.arm_r1, context->uc_mcontext.arm_r2, context->uc_mcontext.arm_r3);
		client_log.push(LOG_CAT_TAG, "r4: 0x%x, r5: 0x%x, r6: 0x%x, r7: 0x%x", context->uc_mcontext.arm_r4, context->uc_mcontext.arm_r5, context->uc_mcontext.arm_r6, context->uc_mcontext.arm_r7);
		client_log.push(LOG_CAT_TAG, "r8: 0x%x, r9: 0x%x, sl: 0x%x, fp: 0x%x", context->uc_mcontext.arm_r8, context->uc_mcontext.arm_r9, context->uc_mcontext.arm_r10, context->uc_mcontext.arm_fp);
		client_log.push(LOG_CAT_TAG, "ip: 0x%x, sp: 0x%x, lr: 0x%x, pc: 0x%x", context->uc_mcontext.arm_ip, context->uc_mcontext.arm_sp, context->uc_mcontext.arm_lr, context->uc_mcontext.arm_pc);
		client_log.push(LOG_CAT_TAG, "backtrace:");
		client_log.push(LOG_CAT_TAG, "1: libGTAVC.so + 0x%X", context->uc_mcontext.arm_pc - g_pGTAVC);
		client_log.push(LOG_CAT_TAG, "2: libGTAVC.so + 0x%X", context->uc_mcontext.arm_lr - g_pGTAVC);
		client_log.push(LOG_CAT_TAG, "1: libmultiplayer.so + 0x%X", context->uc_mcontext.arm_pc - g_pVCMP);
		client_log.push(LOG_CAT_TAG, "2: libmultiplayer.so + 0x%X", context->uc_mcontext.arm_lr - g_pVCMP);
		client_log.push(LOG_CAT_TAG, "1: libc.so + 0x%X", context->uc_mcontext.arm_pc - arm_library.handle("libc.so"));
		client_log.push(LOG_CAT_TAG, "2: libc.so + 0x%X", context->uc_mcontext.arm_lr - arm_library.handle("libc.so"));

		exit(0);
	}

	return;
}

void handler(int signum, siginfo_t *info, void* contextPtr)
{
	ucontext* context = (ucontext_t*)contextPtr;

	if (act_old.sa_sigaction)
	{
		act_old.sa_sigaction(signum, info, contextPtr);
	}

	if(info->si_signo == SIGSEGV)
	{
		client_log.push(LOG_CAT_TAG, "game crashed!");
		client_log.push(LOG_CAT_TAG, "SIGSEGV | Fault address: 0x%X", info->si_addr);
		client_log.push(LOG_CAT_TAG, "register states:");
		client_log.push(LOG_CAT_TAG, "r0: 0x%X, r1: 0x%X, r2: 0x%X, r3: 0x%X",context->uc_mcontext.arm_r0, context->uc_mcontext.arm_r1, context->uc_mcontext.arm_r2, context->uc_mcontext.arm_r3);
		client_log.push(LOG_CAT_TAG, "r4: 0x%x, r5: 0x%x, r6: 0x%x, r7: 0x%x", context->uc_mcontext.arm_r4, context->uc_mcontext.arm_r5, context->uc_mcontext.arm_r6, context->uc_mcontext.arm_r7);
		client_log.push(LOG_CAT_TAG, "r8: 0x%x, r9: 0x%x, sl: 0x%x, fp: 0x%x", context->uc_mcontext.arm_r8, context->uc_mcontext.arm_r9, context->uc_mcontext.arm_r10, context->uc_mcontext.arm_fp);
		client_log.push(LOG_CAT_TAG, "ip: 0x%x, sp: 0x%x, lr: 0x%x, pc: 0x%x", context->uc_mcontext.arm_ip, context->uc_mcontext.arm_sp, context->uc_mcontext.arm_lr, context->uc_mcontext.arm_pc);
		client_log.push(LOG_CAT_TAG, "backtrace:");
		client_log.push(LOG_CAT_TAG, "1: libGTAVC.so + 0x%X", context->uc_mcontext.arm_pc - g_pGTAVC);
		client_log.push(LOG_CAT_TAG, "2: libGTAVC.so + 0x%X", context->uc_mcontext.arm_lr - g_pGTAVC);
		client_log.push(LOG_CAT_TAG, "1: libmultiplayer.so + 0x%X", context->uc_mcontext.arm_pc - g_pVCMP);
		client_log.push(LOG_CAT_TAG, "2: libmultiplayer.so + 0x%X", context->uc_mcontext.arm_lr - g_pVCMP);
		client_log.push(LOG_CAT_TAG, "1: libc.so + 0x%X", context->uc_mcontext.arm_pc - arm_library.handle("libc.so"));
		client_log.push(LOG_CAT_TAG, "2: libc.so + 0x%X", context->uc_mcontext.arm_lr - arm_library.handle("libc.so"));

		exit(0);
	}

	return;
}

void handler2(int signum, siginfo_t* info, void* contextPtr)
{
	ucontext* context = (ucontext_t*)contextPtr;

	if (act2_old.sa_sigaction)
	{
		act2_old.sa_sigaction(signum, info, contextPtr);
	}

	if (info->si_signo == SIGFPE)
	{
		client_log.push(LOG_CAT_TAG, "game crashed!");
		client_log.push(LOG_CAT_TAG, "SIGFPE | Fault address: 0x%X", info->si_addr);
		client_log.push(LOG_CAT_TAG, "register states:");
		client_log.push(LOG_CAT_TAG, "r0: 0x%X, r1: 0x%X, r2: 0x%X, r3: 0x%X",context->uc_mcontext.arm_r0, context->uc_mcontext.arm_r1, context->uc_mcontext.arm_r2, context->uc_mcontext.arm_r3);
		client_log.push(LOG_CAT_TAG, "r4: 0x%x, r5: 0x%x, r6: 0x%x, r7: 0x%x", context->uc_mcontext.arm_r4, context->uc_mcontext.arm_r5, context->uc_mcontext.arm_r6, context->uc_mcontext.arm_r7);
		client_log.push(LOG_CAT_TAG, "r8: 0x%x, r9: 0x%x, sl: 0x%x, fp: 0x%x", context->uc_mcontext.arm_r8, context->uc_mcontext.arm_r9, context->uc_mcontext.arm_r10, context->uc_mcontext.arm_fp);
		client_log.push(LOG_CAT_TAG, "ip: 0x%x, sp: 0x%x, lr: 0x%x, pc: 0x%x", context->uc_mcontext.arm_ip, context->uc_mcontext.arm_sp, context->uc_mcontext.arm_lr, context->uc_mcontext.arm_pc);
		client_log.push(LOG_CAT_TAG, "backtrace:");
		client_log.push(LOG_CAT_TAG, "1: libGTAVC.so + 0x%X", context->uc_mcontext.arm_pc - g_pGTAVC);
		client_log.push(LOG_CAT_TAG, "2: libGTAVC.so + 0x%X", context->uc_mcontext.arm_lr - g_pGTAVC);
		client_log.push(LOG_CAT_TAG, "1: libmultiplayer.so + 0x%X", context->uc_mcontext.arm_pc - g_pVCMP);
		client_log.push(LOG_CAT_TAG, "2: libmultiplayer.so + 0x%X", context->uc_mcontext.arm_lr - g_pVCMP);
		client_log.push(LOG_CAT_TAG, "1: libc.so + 0x%X", context->uc_mcontext.arm_pc - arm_library.handle("libc.so"));
		client_log.push(LOG_CAT_TAG, "2: libc.so + 0x%X", context->uc_mcontext.arm_lr - arm_library.handle("libc.so"));

		exit(0);
	}

	return;
}

void handler1(int signum, siginfo_t* info, void* contextPtr)
{
	ucontext* context = (ucontext_t*)contextPtr;

	if (act1_old.sa_sigaction)
	{
		act1_old.sa_sigaction(signum, info, contextPtr);
	}

	if (info->si_signo == SIGABRT)
	{
        client_log.push(LOG_CAT_TAG, "game crashed!");
		client_log.push(LOG_CAT_TAG, "SIGABRT | Fault address: 0x%X", info->si_addr);
		client_log.push(LOG_CAT_TAG, "register states:");
		client_log.push(LOG_CAT_TAG, "r0: 0x%X, r1: 0x%X, r2: 0x%X, r3: 0x%X",context->uc_mcontext.arm_r0, context->uc_mcontext.arm_r1, context->uc_mcontext.arm_r2, context->uc_mcontext.arm_r3);
		client_log.push(LOG_CAT_TAG, "r4: 0x%x, r5: 0x%x, r6: 0x%x, r7: 0x%x", context->uc_mcontext.arm_r4, context->uc_mcontext.arm_r5, context->uc_mcontext.arm_r6, context->uc_mcontext.arm_r7);
		client_log.push(LOG_CAT_TAG, "r8: 0x%x, r9: 0x%x, sl: 0x%x, fp: 0x%x", context->uc_mcontext.arm_r8, context->uc_mcontext.arm_r9, context->uc_mcontext.arm_r10, context->uc_mcontext.arm_fp);
		client_log.push(LOG_CAT_TAG, "ip: 0x%x, sp: 0x%x, lr: 0x%x, pc: 0x%x", context->uc_mcontext.arm_ip, context->uc_mcontext.arm_sp, context->uc_mcontext.arm_lr, context->uc_mcontext.arm_pc);
		client_log.push(LOG_CAT_TAG, "backtrace:");
		client_log.push(LOG_CAT_TAG, "1: libGTAVC.so + 0x%X", context->uc_mcontext.arm_pc - g_pGTAVC);
		client_log.push(LOG_CAT_TAG, "2: libGTAVC.so + 0x%X", context->uc_mcontext.arm_lr - g_pGTAVC);
		client_log.push(LOG_CAT_TAG, "1: libmultiplayer.so + 0x%X", context->uc_mcontext.arm_pc - g_pVCMP);
		client_log.push(LOG_CAT_TAG, "2: libmultiplayer.so + 0x%X", context->uc_mcontext.arm_lr - g_pVCMP);
		client_log.push(LOG_CAT_TAG, "1: libc.so + 0x%X", context->uc_mcontext.arm_pc - arm_library.handle("libc.so"));
		client_log.push(LOG_CAT_TAG, "2: libc.so + 0x%X", context->uc_mcontext.arm_lr - arm_library.handle("libc.so"));
	}

	return;
}

void onJNI_OnLoad_handle()
{
    #ifdef VC_DEBUG
    client_log.push(LOG_CAT_TAG, "------------------------------------------------------------");
    client_log.push(LOG_CAT_TAG, "VC-MP library loaded! Build time: " __DATE__ " " __TIME__);
    #endif

    g_pVCMP = arm_library.handle("libmultiplayer.so");
    g_pGTAVC = arm_library.handle("libGTAVC.so");
    g_pImmEmu = arm_library.handle("libImmEmulatorJ.so");

    arm_library.push(ID_LIB_HANDLE_CURRENT, g_pVCMP);
    arm_library.push(ID_LIB_HANDLE_GAME, g_pGTAVC);
    arm_library.push(ID_LIB_HANDLE_IMM, g_pImmEmu);

    #ifdef VC_DEBUG
    client_log.push(LOG_CAT_TAG, "Images of libraries:\nlibmultiplayer.so - 0x%X \nlibGTAVC.so - 0x%X \nlibImmEmulatorJ.so - 0x%X", arm_library.handle_by_id(ID_LIB_HANDLE_CURRENT), arm_library.handle_by_id(ID_LIB_HANDLE_GAME), arm_library.handle_by_id(ID_LIB_HANDLE_IMM));
    #endif

    // crashes
    struct sigaction act;
	act.sa_sigaction = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &act, &act_old);

	struct sigaction act1;
	act1.sa_sigaction = handler1;
	sigemptyset(&act1.sa_mask);
	act1.sa_flags = SA_SIGINFO;
	sigaction(SIGABRT, &act1, &act1_old);

	struct sigaction act2;
	act2.sa_sigaction = handler2;
	sigemptyset(&act2.sa_mask);
	act2.sa_flags = SA_SIGINFO;
	sigaction(SIGFPE, &act2, &act2_old);

	struct sigaction act3;
	act3.sa_sigaction = handler3;
	sigemptyset(&act3.sa_mask);
	act3.sa_flags = SA_SIGINFO;
	sigaction(SIGBUS, &act3, &act3_old);

    // scripting
    InitScripting();

    // hooks
    arm_tools.initialise(g_pGTAVC, 0x25D6C8, 0x1A36); 
    arm_tools.installHook(VC(0x1F6B70), (uintptr_t)CStreaming__Init_hook, (uintptr_t*)&CStreaming__Init);

    // nop calling useless shit
    arm_tools.makeNOP(VC(0x140D8C), 2); // CStreaming::Shutdown
    arm_tools.makeNOP(VC(0x10628A), 2); // CUpsideDownCarCheck::UpdateTimers
    arm_tools.makeNOP(VC(0x141078), 2); // CRoadBlocks::GenerateRoadBlocks
    
    // skip gay movies
    arm_tools.writeMemory(VC(0x304B78), (uintptr_t)"\xF7\x46", 2);

    // disable peds/vehicles (single)
    arm_tools.writeMemory(VC(0x1C0860), (uintptr_t)"\x00\x20\xF7\x46", 4); // CPopulation::AddPed
    arm_tools.writeMemory(VC(0x1C34E0), (uintptr_t)"\x4F\xF0\x00\x00\xF7\x46", 6); // CPopulation::AddToPopulation
    arm_tools.makeNOP(VC(0x1410E4), 2); // CCarCtrl::GenerateRandomCars
    arm_tools.makeNOP(VC(0x141096), 2); // CTheCarGenerators::Process 

    // fix radar crash
    arm_tools.writeMemory(VC(0x31847C), (uintptr_t)"\xF7\x46", 2);
}

void initVCMP(JNIEnv* pEnv, jobject thiz)
{
    client_log.push(LOG_CAT_TAG, "Initializing VC-MP..");

    client_storage.set(DATA_STORAGE);

    #ifdef VC_DEBUG
    client_log.push(LOG_CAT_TAG, "Storage: %s", client_storage.get());
    #endif

    g_pJavaWrapper = new CJavaWrapper(pEnv, thiz);
    if(!g_pJavaWrapper) 
    {
        #ifdef VC_DEBUG
        client_log.push(LOG_CAT_TAG, "errno: JavaWrapper is not initialized!");
        #endif

        exit(0);
        return;
    }
}  

void updateScreen()
{
    if(!g_pJavaWrapper) {
        return;
    }

    g_pJavaWrapper->hideLayoutCut();
    g_pJavaWrapper->hideNavBar();
} 

extern "C"
{
	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_initVCMP(JNIEnv* pEnv, jobject thiz)
	{
        initVCMP(pEnv, thiz);
    }

    JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_updateScreen(JNIEnv* pEnv, jobject thiz)
	{
        updateScreen();
    }
}

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{ 
    javaVM = vm;

    registerHandler(&onJNI_OnLoad_handle);
    callHandler(1);

    return JNI_VERSION_1_4;
}