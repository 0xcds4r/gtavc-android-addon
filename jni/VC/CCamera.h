#pragma once

#include "CCommon.h"
#define DEFAULT_NEAR (0.9f)

enum
{
	NONE = 0,
	INTERPOLATION,
	JUMP_CUT
};

enum
{
	NUMBER_OF_VECTORS_FOR_AVERAGE = 2,
	MAX_NUM_OF_SPLINETYPES = 4,
	MAX_NUM_OF_NODES = 800	// for trains
};

class CCamPathSplines
{
public:
	enum {MAXPATHLENGTH=800};
//	float m_arr_PathData[MAXPATHLENGTH];
	float *m_arr_PathData;
	CCamPathSplines(void);
};

struct CTrainCamNode
{
	float m_cvecCamPosition[3];
	float m_cvecPointToLookAt[3];
	float m_cvecMinPointInRange[3];
	float m_cvecMaxPointInRange[3];
	float m_fDesiredFOV;
	float m_fNearClip;
};

struct CQueuedMode
{
	uint16_t Mode;
	float Duration;
	uint16_t MinZoom;
	uint16_t MaxZoom;
};

enum
{
	LOOKING_BEHIND,
	LOOKING_LEFT,
	LOOKING_RIGHT,
	LOOKING_FORWARD,
};

class CCamera : public CPlaceable
{
public:
	bool m_bAboveGroundTrainNodesLoaded;
	bool m_bBelowGroundTrainNodesLoaded;
	bool m_bCamDirectlyBehind;
	bool m_bCamDirectlyInFront;
	bool m_bCameraJustRestored;
	bool m_bcutsceneFinished;
	bool m_bCullZoneChecksOn;
	bool m_bFirstPersonBeingUsed;
	bool m_bJustJumpedOutOf1stPersonBecauseOfTarget;
	bool m_bIdleOn;
	bool m_bInATunnelAndABigVehicle;
	bool m_bInitialNodeFound;
	bool m_bInitialNoNodeStaticsSet;
	bool m_bIgnoreFadingStuffForMusic;
	bool m_bPlayerIsInGarage;
	bool m_bPlayerWasOnBike;
	bool m_bJustCameOutOfGarage;
	bool m_bJustInitalised;
	bool m_bJust_Switched;
	bool m_bLookingAtPlayer;
	bool m_bLookingAtVector;
	bool m_bMoveCamToAvoidGeom;
	bool m_bObbeCinematicPedCamOn;
	bool m_bObbeCinematicCarCamOn;
	bool m_bRestoreByJumpCut;
	bool m_bUseNearClipScript;
	bool m_bStartInterScript;
	bool m_bStartingSpline;
	bool m_bTargetJustBeenOnTrain;
	bool m_bTargetJustCameOffTrain;
	bool m_bUseSpecialFovTrain;
	bool m_bUseTransitionBeta;
	bool m_bUseScriptZoomValuePed;
	bool m_bUseScriptZoomValueCar;
	bool m_bWaitForInterpolToFinish;
	bool m_bItsOkToLookJustAtThePlayer;
	bool m_bWantsToSwitchWidescreenOff;
	bool m_WideScreenOn;
	bool m_1rstPersonRunCloseToAWall;
	bool m_bHeadBob;
	bool m_bVehicleSuspenHigh;
	bool m_bEnable1rstPersonCamCntrlsScript;
	bool m_bAllow1rstPersonWeaponsCamera;
	bool m_bFailedCullZoneTestPreviously;

	bool m_FadeTargetIsSplashScreen;

	bool WorldViewerBeingUsed;
	uint8_t ActiveCam;
	uint32_t m_uiCamShakeStart;
	uint32_t m_uiFirstPersonCamLastInputTime;

	uint32_t m_uiLongestTimeInMill;
	uint32_t m_uiNumberOfTrainCamNodes;
	uint8_t   m_uiTransitionJUSTStarted;
	uint8_t   m_uiTransitionState;        // 0:one mode 1:transition

	uint32_t m_uiTimeLastChange;
	uint32_t m_uiTimeWeLeftIdle_StillNoInput;
	uint32_t m_uiTimeWeEnteredIdle;
	uint32_t m_uiTimeTransitionStart;
	uint32_t m_uiTransitionDuration;
	uint32_t m_uiTransitionDurationTargetCoors;
	int m_BlurBlue;
	int m_BlurGreen;
	int m_BlurRed;
	int m_BlurType;

	int m_iWorkOutSpeedThisNumFrames;
	int m_iNumFramesSoFar;


	int m_iCurrentTrainCamNode;
	int m_motionBlur;
	int m_imotionBlurAddAlpha;
	int m_iCheckCullZoneThisNumFrames;
	int m_iZoneCullFrameNumWereAt;
	int WhoIsInControlOfTheCamera;

	float CamFrontXNorm;
	float CamFrontYNorm;
    uint32_t CarZoomIndicator;
	float CarZoomValue;
	float CarZoomValueSmooth;

	float DistanceToWater;
	float FOVDuringInter;
	float LODDistMultiplier;
	float GenerationDistMultiplier;
	float m_fAlphaSpeedAtStartInter;
	float m_fAlphaWhenInterPol;
	float m_fAlphaDuringInterPol;
	float m_fBetaDuringInterPol;
	float m_fBetaSpeedAtStartInter;
	float m_fBetaWhenInterPol;
	float m_fFOVWhenInterPol;
	float m_fFOVSpeedAtStartInter;
	float m_fStartingBetaForInterPol;
	float m_fStartingAlphaForInterPol;
	float m_PedOrientForBehindOrInFront;
	float m_CameraAverageSpeed;
	float m_CameraSpeedSoFar;
	float m_fCamShakeForce;
	float m_fCarZoomValueScript;
	float m_fFovForTrain;
	float m_fFOV_Wide_Screen;
	float m_fNearClipScript;
	float m_fOldBetaDiff;
	float m_fPedZoomValue;

	float m_fPedZoomValueScript;
	float m_fPedZoomValueSmooth;
	float m_fPositionAlongSpline;
	float m_ScreenReductionPercentage;
	float m_ScreenReductionSpeed;
	float m_AlphaForPlayerAnim1rstPerson;
	float Orientation;
    uint32_t PedZoomIndicator;
	float PlayerExhaustion;
	float SoundDistUp;
	float SoundDistUpAsRead;
	float SoundDistUpAsReadOld;
	float m_fAvoidTheGeometryProbsTimer;
	uint16_t m_nAvoidTheGeometryProbsDirn;
	float m_fWideScreenReductionAmount;
	float m_fStartingFOVForInterPol;

	float m_fMouseAccelHorzntl;// acceleration multiplier for 1st person controls
	float m_fMouseAccelVertical;// acceleration multiplier for 1st person controls
	float m_f3rdPersonCHairMultX;
	float m_f3rdPersonCHairMultY;

	CCam Cams[3];
	uintptr_t pToGarageWeAreIn;
	uintptr_t pToGarageWeAreInForHackAvoidFirstPerson;
	CQueuedMode m_PlayerMode;
	CQueuedMode PlayerWeaponMode;
	float m_PreviousCameraPosition[3];
	float m_RealPreviousCameraPosition[3];
	float m_cvecAimingTargetCoors[3];
	float m_vecFixedModeVector[3];
	float m_vecFixedModeSource[3];
	float m_vecFixedModeUpOffSet[3];
	float m_vecCutSceneOffset[3];

	float m_cvecStartingSourceForInterPol[3];
	float m_cvecStartingTargetForInterPol[3];
	float m_cvecStartingUpForInterPol[3];
	float m_cvecSourceSpeedAtStartInter[3];
	float m_cvecTargetSpeedAtStartInter[3];
	float m_cvecUpSpeedAtStartInter[3];
	float m_vecSourceWhenInterPol[3];
	float m_vecTargetWhenInterPol[3];
	float m_vecUpWhenInterPol[3];
	float m_vecClearGeometryVec[3];
	float m_vecGameCamPos[3];
	float SourceDuringInter[3];
	float TargetDuringInter[3];
	float UpDuringInter[3];
	uintptr_t m_pRwCamera;
	uintptr_t pTargetEntity;
	CCamPathSplines m_arrPathArray[MAX_NUM_OF_SPLINETYPES];
	CMatrix m_cameraMatrix;
	bool m_bGarageFixedCamPositionSet;
	bool m_vecDoingSpecialInterPolation;
	bool m_bScriptParametersSetForInterPol;
	bool m_bFading;
	bool m_bMusicFading;
	CMatrix m_viewMatrix;
	float m_vecFrustumNormals[4*3];
	float m_vecOldSourceForInter[3];
	float m_vecOldFrontForInter[3];
	float m_vecOldUpForInter[3];
	float m_vecOldFOVForInter;
	float m_fFLOATingFade;
	float m_fFLOATingFadeMusic;
	float m_fTimeToFadeOut;
	float m_fTimeToFadeMusic;
	float m_fFractionInterToStopMoving;
	float m_fFractionInterToStopCatchUp;
	float m_fFractionInterToStopMovingTarget;
	float m_fFractionInterToStopCatchUpTarget;
	float m_fGaitSwayBuffer;
	float m_fScriptPercentageInterToStopMoving;
	float m_fScriptPercentageInterToCatchUp;

	uint32_t  m_fScriptTimeForInterPolation;


	uint16_t   m_iFadingDirection;
	int     m_iModeObbeCamIsInForCar;
	uint16_t   m_iModeToGoTo;
	uint16_t   m_iMusicFadingDirection;
	uint16_t   m_iTypeOfSwitch;

	uint32_t m_uiFadeTimeStarted;
	uint32_t m_uiFadeTimeStartedMusic;

	static bool m_bUseMouse3rdPerson;
    static bool bFreeCam;

    void Init2()
    {
        this->m_pRwCamera = 0;
        this->m_bPlayerWasOnBike = false;
        this->m_1rstPersonRunCloseToAWall = false;
        this->m_fPositionAlongSpline = 0.0f;
        this->m_bCameraJustRestored = false;
        this->m_bEnable1rstPersonCamCntrlsScript = false;
        this->m_bAllow1rstPersonWeaponsCamera = false;
        this->m_bVehicleSuspenHigh = false;
        this->m_bMoveCamToAvoidGeom = false;
        this->m_bInATunnelAndABigVehicle = false;
        this->m_iModeObbeCamIsInForCar = 21;
        this->m_bHeadBob = false;
        this->m_fFractionInterToStopMoving = 0.25f;
        this->m_fFractionInterToStopCatchUp = 0.75f;
        this->m_fGaitSwayBuffer = 0.85f;
        this->m_bScriptParametersSetForInterPol = false;
        this->m_uiCamShakeStart = 0;
        this->m_fCamShakeForce = 0.0f;
        this->m_iModeObbeCamIsInForCar = 21;
        this->m_bIgnoreFadingStuffForMusic = false;
        this->m_bWaitForInterpolToFinish = false;
        this->pToGarageWeAreIn = 0;
        this->pToGarageWeAreInForHackAvoidFirstPerson = 0;
        this->m_bPlayerIsInGarage = false;
        this->m_bJustCameOutOfGarage = false;
        this->m_fNearClipScript = DEFAULT_NEAR;
        this->m_bUseNearClipScript = false;
        this->m_vecDoingSpecialInterPolation = false;
        this->m_bAboveGroundTrainNodesLoaded = false;
        this->m_bBelowGroundTrainNodesLoaded = false;
        this->m_WideScreenOn = false;
        this->m_fFOV_Wide_Screen = 0.0f;
        this->m_bRestoreByJumpCut = false;
        this->CarZoomIndicator = 2;
        this->PedZoomIndicator = 2;
        this->CarZoomValueSmooth = 0.0f;
        this->m_fPedZoomValueSmooth = 0.0f;
        this->pTargetEntity = 0;
        this->m_bInitialNodeFound = false;
        this->m_ScreenReductionPercentage = 0.0f;
        this->m_ScreenReductionSpeed = 0.0f;
        this->m_WideScreenOn = false;
        this->m_bWantsToSwitchWidescreenOff = false;
        this->WorldViewerBeingUsed = false;
        this->PlayerExhaustion = 1.0f;
        this->m_PedOrientForBehindOrInFront = 0.0f;
        this->m_bStartingSpline = false;
        this->m_iTypeOfSwitch = INTERPOLATION;
        this->m_bUseScriptZoomValuePed = false;
        this->m_bUseScriptZoomValueCar = false;
        this->m_fPedZoomValueScript = 0.0f;
        this->m_fCarZoomValueScript = 0.0f;
        this->m_bUseSpecialFovTrain = false;
        this->m_fFovForTrain = 70.0f;	// or DefaultFOV from Cam.cpp
        this->m_iModeToGoTo = CCam::MODE_FOLLOWPED;
        this->m_bJust_Switched = false;
        this->m_bUseTransitionBeta = false;
        this->m_bTargetJustBeenOnTrain = false;
        this->m_bInitialNoNodeStaticsSet = false;
        this->m_uiLongestTimeInMill = 5000;
        this->m_uiTimeLastChange = 0;
        this->m_uiTimeWeEnteredIdle = 0;
        this->m_bIdleOn = false;
        this->m_uiTimeWeLeftIdle_StillNoInput = 0;
        this->m_uiTimeWeEnteredIdle = 0;
        this->LODDistMultiplier = 1.0f;
        this->m_bCamDirectlyBehind = false;
        this->m_bCamDirectlyInFront = false;
        this->m_motionBlur = 0;
        this->m_bGarageFixedCamPositionSet = false;
        this->m_bCullZoneChecksOn = false;
        this->m_bFailedCullZoneTestPreviously = false;
        this->m_iCheckCullZoneThisNumFrames = 6;
        this->m_iZoneCullFrameNumWereAt = 0;
        this->m_CameraAverageSpeed = 0.0f;
        this->m_CameraSpeedSoFar = 0.0f;
        this->m_PreviousCameraPosition[0] = 0.0f;
        this->m_PreviousCameraPosition[1] = 0.0f;
        this->m_PreviousCameraPosition[2] = 0.0f;
        this->m_iWorkOutSpeedThisNumFrames = 4;
        this->m_iNumFramesSoFar = 0;
        this->m_bJustInitalised = true;
        this->m_uiTransitionState = 0;
        this->m_uiTimeTransitionStart = 0;
        this->m_bLookingAtPlayer = true;
        this->m_f3rdPersonCHairMultX = 0.53f;
        this->m_fAvoidTheGeometryProbsTimer = 0.0f;
        this->m_nAvoidTheGeometryProbsDirn = 0;

        // todo: fix
        // pCamera->m_f3rdPersonCHairMultY = 0.4f;
    }
};