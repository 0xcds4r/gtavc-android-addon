#pragma once

#include "CCommon.h"

class CCam
{
public:
	enum
	{
		MODE_NONE = 0,
		MODE_TOPDOWN,
		MODE_GTACLASSIC,
		MODE_BEHINDCAR,
		MODE_FOLLOWPED,
		MODE_AIMING,
		MODE_DEBUG,
		MODE_SNIPER,
		MODE_ROCKETLAUNCHER,
		MODE_MODELVIEW,
		MODE_BILL,
		MODE_SYPHON,
		MODE_CIRCLE,
		MODE_CHEESYZOOM,
		MODE_WHEELCAM,
		MODE_FIXED,
		MODE_1STPERSON,
		MODE_FLYBY,
		MODE_CAM_ON_A_STRING,
		MODE_REACTION,
		MODE_FOLLOW_PED_WITH_BIND,
		MODE_CHRIS,
		MODE_BEHINDBOAT,
		MODE_PLAYER_FALLEN_WATER,
		MODE_CAM_ON_TRAIN_ROOF,
		MODE_CAM_RUNNING_SIDE_TRAIN,
		MODE_BLOOD_ON_THE_TRACKS,
		MODE_IM_THE_PASSENGER_WOOWOO,
		MODE_SYPHON_CRIM_IN_FRONT,
		MODE_PED_DEAD_BABY,
		MODE_PILLOWS_PAPS,
		MODE_LOOK_AT_CARS,
		MODE_ARRESTCAM_ONE,
		MODE_ARRESTCAM_TWO,
		MODE_M16_1STPERSON,
		MODE_SPECIAL_FIXED_FOR_SYPHON,
		MODE_FIGHT_CAM,
		MODE_TOP_DOWN_PED,
		MODE_LIGHTHOUSE,
		MODE_SNIPER_RUNABOUT,
		MODE_ROCKETLAUNCHER_RUNABOUT,
		MODE_1STPERSON_RUNABOUT,
		MODE_M16_1STPERSON_RUNABOUT,
		MODE_FIGHT_CAM_RUNABOUT,
		MODE_EDITOR,
		MODE_HELICANNON_1STPERSON,
		MODE_CAMERA,
	};

	bool    bBelowMinDist; //used for follow ped mode 
	bool    bBehindPlayerDesired; //used for follow ped mode
	bool    m_bCamLookingAtVector;
	bool    m_bCollisionChecksOn;
	bool    m_bFixingBeta; //used for camera on a string
	bool    m_bTheHeightFixerVehicleIsATrain;
	bool    LookBehindCamWasInFront;
	bool    LookingBehind;
	bool    LookingLeft;
	bool    LookingRight;
	bool    ResetStatics; //for interpolation type stuff to work
	bool    Rotating;

	uint16_t   Mode;                   // CameraMode
	uint32_t  m_uiFinishTime;

	int     m_iDoCollisionChecksOnFrameNum; 
	int     m_iDoCollisionCheckEveryNumOfFrames;
	int     m_iFrameNumWereAt;
	int     m_iRunningVectorArrayPos;
	int     m_iRunningVectorCounter;
	int     DirectionWasLooking;

	float   f_max_role_angle; //=DEGTORAD(5.0f);    
	float   f_Roll; //used for adding a slight roll to the camera in the
	float	f_rollSpeed;
	float   m_fSyphonModeTargetZOffSet;
	float   m_fAmountFractionObscured;
	float   m_fAlphaSpeedOverOneFrame;
	float   m_fBetaSpeedOverOneFrame;
	float   m_fBufferedTargetBeta;
	float   m_fBufferedTargetOrientation;
	float   m_fBufferedTargetOrientationSpeed;
	float   m_fCamBufferedHeight;
	float   m_fCamBufferedHeightSpeed;
	float   m_fCloseInPedHeightOffset;
	float   m_fCloseInPedHeightOffsetSpeed;
	float   m_fCloseInCarHeightOffset;
	float   m_fCloseInCarHeightOffsetSpeed;
	float   m_fDimensionOfHighestNearCar;       
	float   m_fDistanceBeforeChanges;
	float   m_fFovSpeedOverOneFrame;
	float   m_fMinDistAwayFromCamWhenInterPolating;
	float   m_fPedBetweenCameraHeightOffset;
	float   m_fPlayerInFrontSyphonAngleOffSet;
	float   m_fRadiusForDead;
	float   m_fRealGroundDist; //used for follow ped mode
	float   m_fTargetBeta;
	float   m_fTimeElapsedFloat;  
	float   m_fTilt;
	float   m_fTiltSpeed;
	float   m_fTransitionBeta;
	float   m_fTrueBeta;
	float   m_fTrueAlpha;
	float   m_fInitialPlayerOrientation; //used for first person

	float   Alpha;
	float   AlphaSpeed;
	float   FOV;
	float   FOVSpeed;
	float   Beta;
	float   BetaSpeed;
	float   Distance;
	float   DistanceSpeed;
	float   CA_MIN_DISTANCE;
	float   CA_MAX_DISTANCE;
	float   SpeedVar;

	float m_fTargetZoomGroundOne;
	float m_fTargetZoomGroundTwo;
	float m_fTargetZoomGroundThree;
	float m_fTargetZoomOneZExtra;
	float m_fTargetZoomTwoZExtra;
	float m_fTargetZoomThreeZExtra;
	float m_fTargetZoomZCloseIn;
	float m_fMinRealGroundDist;
	float m_fTargetCloseInDist;

	float m_cvecSourceSpeedOverOneFrame[3];
	float m_cvecTargetSpeedOverOneFrame[3];
	float m_cvecUpOverOneFrame[3];

	float m_cvecTargetCoorsForFudgeInter[3];
	float m_cvecCamFixedModeVector[3];
	float m_cvecCamFixedModeSource[3];
	float m_cvecCamFixedModeUpOffSet[3];
	float m_vecLastAboveWaterCamPosition[3];  //helper for when the player has gone under the water
	float m_vecBufferedPlayerBodyOffset[3];

	// The three vectors that determine this camera for this frame
	float Front[3];                                                  // Direction of looking in
	float Source[3];                                                 // Coors in world space
	float SourceBeforeLookBehind[3];
	float Up[3];                                                     // Just that
	float m_arrPreviousVectors[2];    // used to average stuff
	uintptr_t CamTargetEntity;

	float       m_fCameraDistance;
	float       m_fIdealAlpha;
	float       m_fPlayerVelocity;
	uintptr_t   m_pLastCarEntered; // So interpolation works
	uintptr_t   m_pLastPedLookedAt;// So interpolation works 
	bool        m_bFirstPersonRunAboutActive;

    void Init() 
    {
        this->Mode = CCam::MODE_FOLLOWPED;
        this->Front[0] = 0.0f;
        this->Front[1] = 0.0f;
        this->Front[2] = -1.0f;
        this->Up[0] = 0.0f;
        this->Up[1] = 0.0f;
        this->Up[2] = 1.0f;
        this->Rotating = false;
        this->m_iDoCollisionChecksOnFrameNum = 1;
        this->m_iDoCollisionCheckEveryNumOfFrames = 9;
        this->m_iFrameNumWereAt = 0;
        this->m_bCollisionChecksOn = false;
        this->m_fRealGroundDist = 0.0f;
        this->BetaSpeed = 0.0f;
        this->AlphaSpeed = 0.0f;
        this->DistanceSpeed = 0.0f;
        this->f_max_role_angle = DEGTORAD(5.0f);
        this->Distance = 30.0f;
        this->DistanceSpeed = 0.0f;
        this->m_pLastCarEntered = 0;
        this->m_pLastPedLookedAt = 0;
        this->ResetStatics = true;
        this->Beta = 0.0f;
        this->m_fTilt = 0.0f;
        this->m_fTiltSpeed = 0.0f;
        this->m_bFixingBeta = false;
        this->CA_MIN_DISTANCE = 0.0f;
        this->CA_MAX_DISTANCE = 0.0f;
        this->LookingBehind = false;
        this->LookingLeft = false;
        this->LookingRight = false;
        this->m_fPlayerInFrontSyphonAngleOffSet = DEGTORAD(20.0f);
        this->m_fSyphonModeTargetZOffSet = 0.5f;
        this->m_fRadiusForDead = 1.5f;
        this->DirectionWasLooking = 3;
        this->LookBehindCamWasInFront = false;
        this->f_Roll = 0.0f;
        this->f_rollSpeed = 0.0f;
        this->m_fCloseInPedHeightOffset = 0.0f;
        this->m_fCloseInPedHeightOffsetSpeed = 0.0f;
        this->m_fCloseInCarHeightOffset = 0.0f;
        this->m_fCloseInCarHeightOffsetSpeed = 0.0f;
        this->m_fPedBetweenCameraHeightOffset = 0.0f;
        this->m_fTargetBeta = 0.0f;
        this->m_fBufferedTargetBeta = 0.0f;
        this->m_fBufferedTargetOrientation = 0.0f;
        this->m_fBufferedTargetOrientationSpeed = 0.0f;
        this->m_fDimensionOfHighestNearCar = 0.0f;
        this->m_fMinRealGroundDist = 1.85f;
        this->m_fTargetCloseInDist = 2.0837801f - this->m_fMinRealGroundDist;
        this->m_fTargetZoomGroundOne = 0.25f;
        this->m_fTargetZoomGroundTwo = 1.5f;
        this->m_fTargetZoomGroundThree = 4.0f;
        this->m_fTargetZoomOneZExtra = -0.14f;
        this->m_fTargetZoomTwoZExtra = 0.16f;
        this->m_fTargetZoomThreeZExtra = 0.25f;
        this->m_fTargetZoomZCloseIn = 0.90040702f;
        this->CamTargetEntity = 0;
        this->CamTargetEntity = 0;
        this->CamTargetEntity = 0;
        this->m_fCamBufferedHeight = 0.0f;
        this->m_fCamBufferedHeightSpeed = 0.0f;
        this->m_fCamBufferedHeight = 0.0f;
        this->m_fCamBufferedHeightSpeed = 0.0f;
        this->m_bCamLookingAtVector = false;
        this->m_bCamLookingAtVector = false;
        this->m_bCamLookingAtVector = false;
        this->m_fPlayerVelocity = 0.0f;
        this->m_fPlayerVelocity = 0.0f;
        this->m_fPlayerVelocity = 0.0f;
    };
};