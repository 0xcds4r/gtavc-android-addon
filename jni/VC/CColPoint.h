#pragma once

#include "CCommon.h"

struct CColPoint
{
	CVector point;
	int pad1;
	// the surface normal on the surface of point
	CVector normal;
	int pad2;
	uint8_t surfaceA;
	uint8_t pieceA;
	uint8_t surfaceB;
	uint8_t pieceB;
	float depth;
};