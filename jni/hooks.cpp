#include "main.h"
#include "gui/gui.h"
#include "VC/CCommon.h"

extern utils::storage  client_storage;
extern utils::log      client_log;
extern utils::library  arm_library;
extern utils::arm      arm_tools;

extern gui::events     gui_events;
extern gui::wrapper    gui_wrapper;
extern gui::drawer     gui_drawer;

bool bGameStarted = false;
bool bDuck = false;

#include "scripting.h"
void InitScripting();

void (*TouchEvent)(eTouchType, int, int, int);
void TouchEvent_hook(eTouchType type, int num, int posX, int posY)
{
    if(!bGameStarted) goto skip;
    
    if(!gui_events.touch(type, posX, posY)) {
        return;
    }

skip:
    return TouchEvent(type, num, posX, posY);
}

void doDuckModProc()
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

void weaponCheat()
{
    // DoMobileCheat
    ((int(*)(int))(VC(0x15A5E0 + 1)))(2); 
}

// ==========================================================================================================

void (*CStreaming__Init)();
void CStreaming__Init_hook()
{
	CStreaming__Init();
	*(uint32_t*)(VC(0x6C78B0)) = 0x40000000;
}

void (*CRunningScript_Process)(uintptr_t thiz);
void CRunningScript_Process_hook(uintptr_t thiz)
{
    CRunningScript_Process(thiz);

    if(!bGameStarted)
    {
        // scripting
        InitScripting();

        // graphics
        gui_wrapper.initialise();
        gui_events.initialise();
        gui_drawer.initialise();

        bGameStarted = true;
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
    CCam__Process(pCam);
}

void (*CCamera__Init)(CCamera* pCamera);
void CCamera__Init_hook(CCamera* pCamera)
{
    CCamera__Init(pCamera);
    pCamera->Init2();
}

uintptr_t (*RwFrameAddChild)(uintptr_t parent, uintptr_t child);
uintptr_t RwFrameAddChild_hook(uintptr_t parent, uintptr_t child)
{
    return (parent && child) ? RwFrameAddChild(parent, child) : 0;
}

uintptr_t (*RwFrameRemoveChild)(uintptr_t child);
uintptr_t RwFrameRemoveChild_hook(uintptr_t child)
{
    return child ? RwFrameRemoveChild(child) : 0;
}

uint32_t (*RwFrameDestroy)(uintptr_t frame);
uint32_t RwFrameDestroy_hook(uintptr_t frame)
{
    return frame ? RwFrameDestroy(frame) : 0;
}

uintptr_t (*RwFrameUpdateObjects)(uintptr_t frame);
uintptr_t RwFrameUpdateObjects_hook(uintptr_t frame)
{
    return frame ? RwFrameUpdateObjects(frame) : 0;
}

void (*CAnimManager__UncompressAnimation)(CAnimBlendHierarchy *hier);
void CAnimManager__UncompressAnimation_hook(CAnimBlendHierarchy *hier)
{
    if(hier) {
        CAnimManager__UncompressAnimation(hier);
    }
}

void (*CAnimBlendHierarchy__CalcTotalTimeCompressed)(CAnimBlendHierarchy* hier);
void CAnimBlendHierarchy__CalcTotalTimeCompressed_hook(CAnimBlendHierarchy* hier)
{
    /**
     * we need to add a check to the loop -> hier->sequences[i].numFrames == 0 (todo: full fix)
    */

    if(hier) {
        CAnimBlendHierarchy__CalcTotalTimeCompressed(hier);
    }
}

void (*CAnimBlendHierarchy__CalcTotalTime)(CAnimBlendHierarchy* hier);
void CAnimBlendHierarchy__CalcTotalTime_hook(CAnimBlendHierarchy* hier)
{
    /**
     * we need to add a check to the loop -> hier->sequences[i].numFrames == 0 (todo: full fix)
    */

    if(hier) {
        CAnimBlendHierarchy__CalcTotalTime(hier);
    }
}

bool (*CUpsideDownCarCheck__IsCarUpsideDown)(uintptr_t thiz, uintptr_t pVehicle);
bool CUpsideDownCarCheck__IsCarUpsideDown_hook(uintptr_t thiz, uintptr_t pVehicle)
{
    if(pVehicle)
    {
        if (*(uintptr_t*)(pVehicle + 20)) {
		    return CUpsideDownCarCheck__IsCarUpsideDown(thiz, pVehicle);
	    }
    }
	
    return false;
}

typedef uintptr_t RwFrame;
typedef uintptr_t RpClump;
RwFrame* CClumpModelInfo__GetFrameFromId_Post(RwFrame* pFrameResult, RpClump* pClump, int id)
{
	if (pFrameResult) {
	    return pFrameResult;
    }

	uintptr_t calledFrom = 0;
	__asm__ volatile ("mov %0, lr" : "=r" (calledFrom));
	calledFrom -= g_pGTAVC;

	if (calledFrom == 0x001FC784) { // CVehicleModelInfo::CreateInstance           
		return NULL;
    }

	for (uint i = 2; i < 40; i++)
	{
		RwFrame* pNewFrameResult = NULL;
		uint     uiNewId = id + (i / 2) * ((i & 1) ? -1 : 1);
		
		pNewFrameResult = ((RwFrame * (*)(RpClump * pClump, int id))(g_pGTAVC + 0x1C58B0 + 1))(pClump, i);

		if (pNewFrameResult) {
			return pNewFrameResult;
		}
	}

	return NULL;
}

RwFrame* (*CClumpModelInfo__GetFrameFromId)(RpClump*, int);
RwFrame* CClumpModelInfo__GetFrameFromId_hook(RpClump* clump, int id)
{
	return CClumpModelInfo__GetFrameFromId_Post(CClumpModelInfo__GetFrameFromId(clump, id), clump, id);
}

int (*_rwFreeListFreeReal)(int a1, unsigned int a2);
int _rwFreeListFreeReal_hook(int a1, unsigned int a2)
{
	if (a1) {
		return _rwFreeListFreeReal(a1, a2);
	}

	return 0;
}

typedef uintptr_t CEntity;
bool (*CWorld__ProcessLineOfSight)(const CVector &point1, const CVector &point2, CColPoint &point, CEntity *&entity,
                           bool checkBuildings, bool checkVehicles, bool checkPeds, bool checkObjects,
                           bool checkDummies, bool ignoreSeeThrough, bool ignoreSomeObjects, bool ignoreShootThrough);
bool CWorld__ProcessLineOfSight_hook(const CVector &point1, const CVector &point2, CColPoint &point, CEntity *&entity,
                           bool checkBuildings, bool checkVehicles, bool checkPeds, bool checkObjects,
                           bool checkDummies, bool ignoreSeeThrough, bool ignoreSomeObjects, bool ignoreShootThrough)
{
    uintptr_t calledFrom = 0;
	__asm__ volatile ("mov %0, lr" : "=r" (calledFrom));
	calledFrom -= g_pGTAVC;

    // disable camera shittiest collision
    if(calledFrom == 0x122E23) {
        return false;
    }

    return CWorld__ProcessLineOfSight(point1, point2, point, entity, checkBuildings, checkVehicles, checkPeds, checkObjects, checkDummies, ignoreSeeThrough, ignoreSomeObjects, ignoreShootThrough);
}

void (*CHud__DrawAfterFade)();
void CHud__DrawAfterFade_hook() 
{
    CHud__DrawAfterFade();

    static bool bShowPoolsInfoDebug = false;

    if(bShowPoolsInfoDebug)
    {
        CFont::SetFontStyle(FONT_STANDARD);
	    CFont::SetBackgroundOff();
	    CFont::SetWrapx(500.0f);
	    CFont::SetScale(0.7f, 1.0f);
	    CFont::SetCentreOff();
	    // CFont::SetCentreSize(640.0f);
	    CFont::SetJustifyOff();
	    CFont::SetPropOn();
	    CFont::SetColor(0xFFFFFFFF);
	    CFont::SetBackGroundOnlyTextOff();
	    CFont::SetDropShadowPosition(1);

        // PrintMemoryUsage
        ((int(*)())(VC(0x142A14 + 1)))();
    }
}

void init_hooks()
{
    #ifdef VC_DEBUG
    client_log.push(LOG_CAT_TAG, "Initializing hooks..");
    #endif
    
    // hooks
    arm_tools.initialise(g_pGTAVC, 0x25D6C8, 0x1A36); 

    /* fix */
    arm_tools.installHook(VC(0x1F6B70), (uintptr_t)CStreaming__Init_hook, (uintptr_t*)&CStreaming__Init);
    arm_tools.installHook(VC(0x27E508), (uintptr_t)RenderQueue__ProcessCommand_hook, (uintptr_t*)&RenderQueue__ProcessCommand);
    arm_tools.installHook(VC(0x13C964), (uintptr_t)CFileLoader__SetRelatedModelInfoCB_hook, (uintptr_t*)&CFileLoader__SetRelatedModelInfoCB);
    arm_tools.installHook(VC(0x2AF6A8), (uintptr_t)RpClumpRemoveAtomic_hook, (uintptr_t*)&RpClumpRemoveAtomic);
    arm_tools.installHook(VC(0x176258), (uintptr_t)CWorld__Remove_hook, (uintptr_t*)&CWorld__Remove);
    arm_tools.installHook(VC(0x283BC0), (uintptr_t)RwFrameAddChild_hook, (uintptr_t*)&RwFrameAddChild);
    arm_tools.installHook(VC(0x283A04), (uintptr_t)RwFrameRemoveChild_hook, (uintptr_t*)&RwFrameRemoveChild);
    arm_tools.installHook(VC(0x283B88), (uintptr_t)RwFrameDestroy_hook, (uintptr_t*)&RwFrameDestroy);
    arm_tools.installHook(VC(0x28395C), (uintptr_t)RwFrameUpdateObjects_hook, (uintptr_t*)&RwFrameUpdateObjects);
    arm_tools.installHook(VC(0x0B1584), (uintptr_t)CAnimManager__UncompressAnimation_hook, (uintptr_t*)&CAnimManager__UncompressAnimation);
    arm_tools.installHook(VC(0x0AE76C), (uintptr_t)CAnimBlendHierarchy__CalcTotalTimeCompressed_hook, (uintptr_t*)&CAnimBlendHierarchy__CalcTotalTimeCompressed);
    arm_tools.installHook(VC(0x0AE690), (uintptr_t)CAnimBlendHierarchy__CalcTotalTime_hook, (uintptr_t*)&CAnimBlendHierarchy__CalcTotalTime);
    arm_tools.installHook(VC(0x1011E4), (uintptr_t)CUpsideDownCarCheck__IsCarUpsideDown_hook, (uintptr_t*)&CUpsideDownCarCheck__IsCarUpsideDown);
    arm_tools.installHook(VC(0x1C58B0), (uintptr_t)CClumpModelInfo__GetFrameFromId_hook, (uintptr_t*)&CClumpModelInfo__GetFrameFromId);
    arm_tools.installHook(VC(0x28EB00), (uintptr_t)_rwFreeListFreeReal_hook, (uintptr_t*)&_rwFreeListFreeReal);

    /* camera */
    arm_tools.installHook(VC(0x12B5C8), (uintptr_t)CCamera__Init_hook, (uintptr_t*)&CCamera__Init);
    arm_tools.installHook(VC(0x11F284), (uintptr_t)CCam__Init_hook, (uintptr_t*)&CCam__Init);
    arm_tools.installHook(VC(0x133068), (uintptr_t)CCam__Process_hook, (uintptr_t*)&CCam__Process);
    arm_tools.installHook(VC(0x17B0AC), (uintptr_t)CWorld__ProcessLineOfSight_hook, (uintptr_t*)&CWorld__ProcessLineOfSight);

    /* game */
    arm_tools.installHook(VC(0x2F6E64), (uintptr_t)TouchEvent_hook, (uintptr_t*)&TouchEvent);
    arm_tools.installHook(VC(0x106150), (uintptr_t)CRunningScript_Process_hook, (uintptr_t*)&CRunningScript_Process);
    arm_tools.installHook(VC(0x216C40), (uintptr_t)CText__Get_hook, (uintptr_t*)&CText__Get);
    arm_tools.installHook(VC(0x174F70), (uintptr_t)CTimer__StartUserPause_hook, (uintptr_t*)&CTimer__StartUserPause);
    arm_tools.installHook(VC(0x174F80), (uintptr_t)CTimer__EndUserPause_hook, (uintptr_t*)&CTimer__EndUserPause);
    arm_tools.installHook(VC(0x1DC708), (uintptr_t)CHud__DrawAfterFade_hook, (uintptr_t*)&CHud__DrawAfterFade);
}