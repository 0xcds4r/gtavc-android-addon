#include "main.h"

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

                // VehicleCheat
                ((int(*)(int))(VC(0x155CC0 + 1)))(130);

                processWeapon();

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
        processPlayerDuckMod();
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
}