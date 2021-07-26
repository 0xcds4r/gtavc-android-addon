#include "main.h"
#include "crashhandler.h"

#include <dlfcn.h>
#include <map>
#include <set>

extern utils::storage  client_storage;
extern utils::log      client_log;
extern utils::library  arm_library;
extern utils::arm      arm_tools;

struct SCrashAvertedInfo
{
    uint uiTickCount;
    int  uiUsageCount;
};

static uint ms_uiInCrashZone = 0;
static std::map<int, SCrashAvertedInfo>    ms_CrashAvertedMap;

void crashdump::OnCrashAverted(uint uiId)
{
	
}

void crashdump::OnEnterCrashZone(uint id)
{
	ms_uiInCrashZone = id;
}

void handler1(int signum, siginfo_t *info, void* contextPtr)
{
	ucontext* context = (ucontext_t*)contextPtr;

	if(info->si_signo == SIGSEGV) {
		crashdump::report(context, info, "SIGSEGV");
	}

	return;
}

void handler2(int signum, siginfo_t *info, void* contextPtr)
{
	ucontext* context = (ucontext_t*)contextPtr;

	if(info->si_signo == SIGABRT) {
		// crashdump::report(context, info, "SIGABRT");
	}

	return;
}

void handler3(int signum, siginfo_t *info, void* contextPtr)
{
	ucontext* context = (ucontext_t*)contextPtr;

	if(info->si_signo == SIGFPE) {
		crashdump::report(context, info, "SIGFPE");
	}

	return;
}

void handler4(int signum, siginfo_t *info, void* contextPtr)
{
	ucontext* context = (ucontext_t*)contextPtr;

	if(info->si_signo == SIGBUS) {
		crashdump::report(context, info, "SIGBUS");
	}

	return;
}

void handler5(int signum, siginfo_t *info, void* contextPtr)
{
	ucontext* context = (ucontext_t*)contextPtr;

	if(info->si_signo == SIGILL) {
		crashdump::report(context, info, "SIGILL");
	}

	return;
}

void handler6(int signum, siginfo_t *info, void* contextPtr)
{
	ucontext* context = (ucontext_t*)contextPtr;

	if(info->si_signo == SIGPIPE) {
		crashdump::report(context, info, "SIGPIPE");
	}

	return;
}

void handler7(int signum, siginfo_t *info, void* contextPtr)
{
	ucontext* context = (ucontext_t*)contextPtr;

	if(info->si_signo == SIGSTKFLT) {
		crashdump::report(context, info, "SIGSTKFLT");
	}

	return;
}

void crashdump::initialize()
{
    #ifdef VC_DEBUG
    client_log.push(LOG_CAT_TAG, "initializing crashdump..");
    #endif
    
    struct sigaction act;
	act.sa_sigaction = handler1;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &act, 0);
	
	struct sigaction act2;
	act2.sa_sigaction = handler2;
	sigemptyset(&act2.sa_mask);
	act2.sa_flags = SA_SIGINFO;
	sigaction(SIGABRT, &act2, 0);
	
	struct sigaction act3;
	act3.sa_sigaction = handler3;
	sigemptyset(&act3.sa_mask);
	act3.sa_flags = SA_SIGINFO;
	sigaction(SIGFPE, &act3, 0);
	
	struct sigaction act4;
	act4.sa_sigaction = handler4;
	sigemptyset(&act4.sa_mask);
	act4.sa_flags = SA_SIGINFO;
	sigaction(SIGBUS, &act4, 0);
	
	struct sigaction act5;
	act5.sa_sigaction = handler5;
	sigemptyset(&act5.sa_mask);
	act5.sa_flags = SA_SIGINFO;
	sigaction(SIGILL, &act5, 0);
	
	struct sigaction act6;
	act6.sa_sigaction = handler6;
	sigemptyset(&act6.sa_mask);
	act6.sa_flags = SA_SIGINFO;
	sigaction(SIGPIPE, &act6, 0);
	
	struct sigaction act7;
	act7.sa_sigaction = handler7;
	sigemptyset(&act7.sa_mask);
	act7.sa_flags = SA_SIGINFO;
	sigaction(SIGSTKFLT, &act7, 0);
}

void crashdump::printSymbols(void* pc, void* lr)
{
	Dl_info info_pc, info_lr;

	if (dladdr(pc, &info_pc) != 0)
	{
		client_log.push(LOG_CAT_TAG, "> %s", info_pc.dli_sname);
	}

	if (dladdr(lr, &info_lr) != 0)
	{
		client_log.push(LOG_CAT_TAG, "> %s", info_lr.dli_sname);
	}
}

void crashdump::report(ucontext* context, siginfo_t *info, const char* typeName)
{
    #ifdef VC_DEBUG
    uintptr_t pGTAVC = arm_library.handle("libGTAVC.so");
    uintptr_t pClientLibrary = arm_library.handle("libmultiplayer.so");
    uintptr_t pIMM = arm_library.handle("libImmEmulatorJ.so");

	uintptr_t pCSysLibrary = arm_library.handle("libc.so");
	uintptr_t pSTDCPPSysLibrary = arm_library.handle("libstdc++.so");
	uintptr_t pMSysLibrary = arm_library.handle("libm.so");
    
	client_log.push(LOG_CAT_TAG_CRASH, "Game crashed!");
	client_log.push(LOG_CAT_TAG_CRASH, "Crash ID: %d", rand() % 99999);
	
	if(ms_uiInCrashZone != 0) {
		client_log.push(LOG_CAT_TAG_CRASH, "Crash zone ID: %d", ms_uiInCrashZone);
	}

	client_log.push(LOG_CAT_TAG_CRASH, "SIGNO: %s | Fault address: 0x%X", typeName, info->si_addr);
	client_log.push(LOG_CAT_TAG_CRASH, "android_sigaction signal %d (si_code: %d si_errno: %d si_signo: %d)", info, info->si_code, info->si_errno, info->si_signo);

	if(!pGTAVC || !pClientLibrary) {
		return;
	}

	client_log.push(LOG_CAT_TAG_CRASH, "libGTAVC.so base address: 0x%X", pGTAVC);
	client_log.push(LOG_CAT_TAG_CRASH, "libImmEmulatorJ.so base address: 0x%X", pIMM);
	client_log.push(LOG_CAT_TAG_CRASH, "libc.so base address: 0x%X", pCSysLibrary);
	client_log.push(LOG_CAT_TAG_CRASH, "libstdc++.so base address: 0x%X", pSTDCPPSysLibrary);
	client_log.push(LOG_CAT_TAG_CRASH, "libm.so base address: 0x%X", pMSysLibrary);
	client_log.push(LOG_CAT_TAG_CRASH, "libmultiplayer.so base address: 0x%X", pClientLibrary);

	client_log.push(LOG_CAT_TAG_CRASH, "Lib crashed at:");

	uintptr_t contextResult[20] = {
		context->uc_mcontext.arm_pc - pCSysLibrary, context->uc_mcontext.arm_lr - pCSysLibrary, // c
		context->uc_mcontext.arm_pc - pSTDCPPSysLibrary, context->uc_mcontext.arm_lr - pSTDCPPSysLibrary, // cpp
		context->uc_mcontext.arm_pc - pMSysLibrary, context->uc_mcontext.arm_lr - pMSysLibrary, // m
		context->uc_mcontext.arm_pc - pGTAVC, context->uc_mcontext.arm_lr - pGTAVC, // game
		context->uc_mcontext.arm_pc - pIMM, context->uc_mcontext.arm_lr - pIMM, // imm
		context->uc_mcontext.arm_pc - pClientLibrary, context->uc_mcontext.arm_lr - pClientLibrary, // client
	};

	client_log.push(LOG_CAT_TAG_CRASH, "[libc.so] pc: 0x%X | lr: 0x%X", contextResult[0], contextResult[1]);
	client_log.push(LOG_CAT_TAG_CRASH, "[libstdc++.so] pc: 0x%X | lr: 0x%X", contextResult[2], contextResult[3]);
	client_log.push(LOG_CAT_TAG_CRASH, "[libm.so] pc: 0x%X | lr: 0x%X", contextResult[4], contextResult[5]);

	client_log.push(LOG_CAT_TAG_CRASH, "[libGTAVC.so] pc: 0x%X | lr: 0x%X", contextResult[6], contextResult[7]);
	client_log.push(LOG_CAT_TAG_CRASH, "[libImmEmulatorJ.so] pc: 0x%X | lr: 0x%X", contextResult[8], contextResult[9]);
	client_log.push(LOG_CAT_TAG_CRASH, "[libmultiplayer.so] pc: 0x%X | lr: 0x%X", contextResult[10], contextResult[11]);

	client_log.push(LOG_CAT_TAG_CRASH, "register states:");
	client_log.push(LOG_CAT_TAG_CRASH, "r0: 0x%X, r1: 0x%X, r2: 0x%X, r3: 0x%X", context->uc_mcontext.arm_r0, context->uc_mcontext.arm_r1, context->uc_mcontext.arm_r2, context->uc_mcontext.arm_r3);
  	client_log.push(LOG_CAT_TAG_CRASH, "r4: 0x%x, r5: 0x%x, r6: 0x%x, r7: 0x%x", context->uc_mcontext.arm_r4, context->uc_mcontext.arm_r5, context->uc_mcontext.arm_r6, context->uc_mcontext.arm_r7);
  	client_log.push(LOG_CAT_TAG_CRASH, "r8: 0x%x, r9: 0x%x, r10: 0x%x, fp: 0x%x", context->uc_mcontext.arm_r8, context->uc_mcontext.arm_r9, context->uc_mcontext.arm_r10, context->uc_mcontext.arm_fp);
  	client_log.push(LOG_CAT_TAG_CRASH, "ip: 0x%x, sp: 0x%x, lr: 0x%x, pc: 0x%x", context->uc_mcontext.arm_ip, context->uc_mcontext.arm_sp, context->uc_mcontext.arm_lr, context->uc_mcontext.arm_pc);

	crashdump::printSymbols((void*)(context->uc_mcontext.arm_pc), (void*)(context->uc_mcontext.arm_lr));
    #endif

	exit(0);
}