#include "main.h"
#include "VC/CCommon.h"

extern utils::storage  client_storage;
extern utils::log      client_log;
extern utils::library  arm_library;
extern utils::arm      arm_tools;

bool bGameStarted = false;

void (*CStreaming__Init)();
void CStreaming__Init_hook()
{
	CStreaming__Init();
	*(uint32_t*)(VC(0x6C78B0)) = 0x40000000;
}

enum eTouchType
{
	TOUCH_POP = 1,
	TOUCH_PUSH = 2,
	TOUCH_MOVE = 3
};

#include "scripting.h"
void InitScripting();

bool bDuck = false;
bool bUseFiveCam = false;
int iProcCameraMode = 0;
void processWeapon();

void (*TouchEvent)(eTouchType, int, int, int);
void TouchEvent_hook(eTouchType type, int num, int posX, int posY)
{
    if(!bGameStarted) goto skip;
    static bool bWannaClick = false;

    switch (type)
    {
        case TOUCH_POP:
        #ifdef VC_DEBUG
        client_log.push(LOG_CAT_TAG_TOUCH, "touch pop at %d %d", posX, posY);
        #endif

        if(!bWannaClick) break;
        if(posX >= 1312 && posX <= 1933)
        {
            if(posY <= 300)
            {
                static int i = 0;

                #ifdef VC_DEBUG
                client_log.push(LOG_CAT_TAG, "count fun: %d", i++);   
                #endif
                
                bUseFiveCam ^= true;
                
                // ScriptCommand(&toggle_player_controllable, 1, 0);

                // VehicleCheat
                // ((int(*)(int))(VC(0x155CC0 + 1)))(130);

                // processWeapon();

                bDuck = !bDuck;
            }
        }

        bWannaClick = false;
        break;
    
        case TOUCH_PUSH:
        #ifdef VC_DEBUG
        client_log.push(LOG_CAT_TAG_TOUCH, "touch push at %d %d", posX, posY);
        #endif

        if(posX >= 1312 && posX <= 1933)
        {
            if(posY <= 300)
            {
                bWannaClick = true;
            }
        }
        break;

        case TOUCH_MOVE:
        #ifdef VC_DEBUG
        client_log.push(LOG_CAT_TAG_TOUCH, "touch move at %d %d", posX, posY);
        #endif
        break;
    
        default:
            break;
    }

skip:
    return TouchEvent(type, num, posX, posY);
}

void processPlayerDuckMod()
{
    uintptr_t ped = ((int(*)())(VC(0x162A00 + 1)))(); // FindPlayerPed
    if(!ped) { 
        return;
    }

    // todo: add the ability to move/jump/strafe on haunches
        
    static bool bInDuck = false;

    if(bDuck && !bInDuck) 
    {
        // CPed::SetDuck
        ((int(*)(uintptr_t, int, int))(VC(0x1A4EC8 + 1)))(ped, 60000, 1); 
        bInDuck = true;
    }
    else if(!bDuck && bInDuck)
    {
        // CPed::ClearDuck
        ((int(*)(uintptr_t, int, int))(VC(0x1A53D8 + 1)))(ped, 0, *(uint32_t *)(ped + 1368));
        bInDuck = false;
    }
}

void processWeapon()
{
    static bool onceWeapon = false;

    if(!onceWeapon)
    {
        // DoMobileCheat
        ((int(*)(int))(VC(0x15A5E0 + 1)))(2); 
        onceWeapon = true;
    }
}

void processScriptingInit()
{
    static bool once = false;
    if(!once) 
    {
        InitScripting();
        once = true;
    }
}

void processFPV()
{
    // .data:003A57D4 00 00 00 3F                          fMobilePedChaseDistance DCD 0x3F000000  ; DATA XREF: LOAD:0003BD84?o
    // .data:003A57D8 00 00 00 3F                          fMobileCarChaseDistance DCD 0x3F000000  ; DATA XREF: LOAD:0001AA
}

void (*CRunningScript_Process)(uintptr_t thiz);
void CRunningScript_Process_hook(uintptr_t thiz)
{
    CRunningScript_Process(thiz);

    if(!bGameStarted)
    {
        bGameStarted = true;
    }
    else
    {
        processScriptingInit();
        // processPlayerDuckMod();
        processFPV();
    }
}

char* (*RenderQueue__ProcessCommand)(uintptr_t thiz, char* a2);
char* RenderQueue__ProcessCommand_hook(uintptr_t thiz, char* a2)
{
	if (thiz && a2) {
        return RenderQueue__ProcessCommand(thiz, a2);
    }
    
    return nullptr;
}

void (*CFileLoader__SetRelatedModelInfoCB)(int a1, int a2);
void CFileLoader__SetRelatedModelInfoCB_hook(int a1, int a2)
{
    if(a1 && a2) {
        return CFileLoader__SetRelatedModelInfoCB(a1, a2);
    }
}

void (*RpClumpRemoveAtomic)(int a1, int a2);
void RpClumpRemoveAtomic_hook(int a1, int a2)
{
    if(a2) {
        return RpClumpRemoveAtomic(a1, a2);
    }
}

void (*CWorld__Remove)(uintptr_t entity);
void CWorld__Remove_hook(uintptr_t entity)
{
    if(!entity) {
        return;
    }

    return CWorld__Remove(entity);
}

void skipSplashScreen()
{
    arm_tools.unprotect(VC(0x58A77C));
    arm_tools.unprotect(VC(0x58A780));
    *(uint8_t*)(VC(0x58A77C)) = 2;  // legalScreenState
    *(uint8_t*)(VC(0x58A780)) = 1;  // shownLegalScreen
}

void forcedStartGame()
{
    // CMenuManager::DoSettingsBeforeStartingAGame
    ((int(*)())(VC(0x145F14 + 1)))();

    // 58A198 ?? ?? ?? ??                          dword_58A198
    arm_tools.unprotect(VC(0x58A198));
    *(uint32_t*)(VC(0x58A198)) = 0;    

    // 58A19D ??                                   byte_58A19D
    arm_tools.unprotect(VC(0x58A19D));
    *(uint32_t*)(VC(0x58A19D)) = 0;  

    // more ??  
}

uint16_t* (*CText__Get)(uintptr_t thiz, const char* text);
uint16_t* CText__Get_hook(uintptr_t thiz, const char* text)
{
    uintptr_t dwRetAddr = 0;
	__asm__ volatile ("mov %0, lr" : "=r" (dwRetAddr));
	dwRetAddr -= g_pGTAVC;

    // skip splash screen
	if(strcmp(text, "SPLASH") == 0) { // 'Tap for continue' screen and etc.
        skipSplashScreen();
    }

    // skip main menu
    if(strcmp(text, "FEN_STA") == 0) {
        #ifdef SKIP_MAIN_MENU
        forcedStartGame(); // BETA
        #endif
    }

	return CText__Get(thiz, text);
}

void (*CTimer__StartUserPause)();
void CTimer__StartUserPause_hook()
{
    uintptr_t dwRetAddr = 0;
	__asm__ volatile ("mov %0, lr" : "=r" (dwRetAddr));
	dwRetAddr -= g_pGTAVC;
    // client_log.push(LOG_CAT_TAG, "CTimer__StartUserPause ret addr: 0x%X", dwRetAddr);

    // 0x2139AF - radar map
    // 0x145639 - menu

    // changing the action of the pressed back button
    if(dwRetAddr != 0x2139AF) 
    {
        // 0x6F5794 - GRadarMap
        if(*(uintptr_t*)(VC(0x6F5794))) 
        {
            #ifdef ENABLE_BP_FORCE_EXIT
                // forced exit
                exit(0);
            #endif
        }
    }

    // 595DB6                                      ; CTimer::m_UserPause
    *(uint8_t*)(VC(0x595DB6)) = 1;
}

void (*CTimer__EndUserPause)();
void CTimer__EndUserPause_hook()
{
    // 595DB6                                      ; CTimer::m_UserPause
    *(uint8_t*)(VC(0x595DB6)) = 0;
}

void (*CCam__Init)(CCam* pCam);
void CCam__Init_hook(CCam* pCam)
{
    pCam->Init();
}

void (*CCam__Process)(CCam* pCam);
void CCam__Process_hook(CCam* pCam)
{
    if(bUseFiveCam) {
        // pCam->Mode = CCam::MODE_SYPHON;
    }

    // todo: rewrite process

    // client_log.push(LOG_CAT_TAG, "mode: %d", thiz->Mode);

    CCam__Process(pCam);
}

void (*CCamera__Init)(CCamera* pCamera);
void CCamera__Init_hook(CCamera* pCamera)
{
    CCamera__Init(pCamera);
    pCamera->Init2();
}

void init_hooks()
{
    #ifdef VC_DEBUG
    client_log.push(LOG_CAT_TAG, "Initializing hooks..");
    #endif
    
    // hooks
    arm_tools.initialise(g_pGTAVC, 0x25D6C8, 0x1A36); 

    arm_tools.installHook(VC(0x1F6B70), (uintptr_t)CStreaming__Init_hook, (uintptr_t*)&CStreaming__Init);
    arm_tools.installHook(VC(0x2F6E64), (uintptr_t)TouchEvent_hook, (uintptr_t*)&TouchEvent);
    arm_tools.installHook(VC(0x106150), (uintptr_t)CRunningScript_Process_hook, (uintptr_t*)&CRunningScript_Process);
    arm_tools.installHook(VC(0x27E508), (uintptr_t)RenderQueue__ProcessCommand_hook, (uintptr_t*)&RenderQueue__ProcessCommand);
    arm_tools.installHook(VC(0x13C964), (uintptr_t)CFileLoader__SetRelatedModelInfoCB_hook, (uintptr_t*)&CFileLoader__SetRelatedModelInfoCB);
    arm_tools.installHook(VC(0x2AF6A8), (uintptr_t)RpClumpRemoveAtomic_hook, (uintptr_t*)&RpClumpRemoveAtomic);
    arm_tools.installHook(VC(0x176258), (uintptr_t)CWorld__Remove_hook, (uintptr_t*)&CWorld__Remove);

    arm_tools.installHook(VC(0x216C40), (uintptr_t)CText__Get_hook, (uintptr_t*)&CText__Get);
    arm_tools.installHook(VC(0x174F70), (uintptr_t)CTimer__StartUserPause_hook, (uintptr_t*)&CTimer__StartUserPause);
    arm_tools.installHook(VC(0x174F80), (uintptr_t)CTimer__EndUserPause_hook, (uintptr_t*)&CTimer__EndUserPause);

    arm_tools.installHook(VC(0x12B5C8), (uintptr_t)CCamera__Init_hook, (uintptr_t*)&CCamera__Init);
    arm_tools.installHook(VC(0x11F284), (uintptr_t)CCam__Init_hook, (uintptr_t*)&CCam__Init);
    arm_tools.installHook(VC(0x133068), (uintptr_t)CCam__Process_hook, (uintptr_t*)&CCam__Process);
}