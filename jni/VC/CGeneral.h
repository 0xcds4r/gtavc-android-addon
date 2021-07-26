#pragma once

#include <ctype.h>
#include "CCommon.h"

#define Clamp(v, low, high) ((v)<(low) ? (low) : (v)>(high) ? (high) : (v))

class CGeneral
{
public:
	static float GetATanOfXY(float x, float y){
		if(x == 0.0f && y == 0.0f)
			return 0.0f;
		float xabs = ABS(x);
		float yabs = ABS(y);

		if(xabs < yabs){
			if(y > 0.0f){
				if(x > 0.0f)
					return 0.5f*PI - atan2(x / y, 1.0f);
				else
					return 0.5f*PI + atan2(-x / y, 1.0f);
			}else{
				if(x > 0.0f)
					return 1.5f*PI + atan2(x / -y, 1.0f);
				else
					return 1.5f*PI - atan2(-x / -y, 1.0f);
			}
		}else{
			if(y > 0.0f){
				if(x > 0.0f)
					return atan2(y / x, 1.0f);
				else
					return PI - atan2(y / -x, 1.0f);
			}else{
				if(x > 0.0f)
					return 2.0f*PI - atan2(-y / x, 1.0f);
				else
					return PI + atan2(-y / -x, 1.0f);
			}
		}
	}

	static float LimitAngle(float angle)
	{
		float result = angle;

		while (result >= 180.0f) {
			result -= 2 * 180.0f;
		}

		while (result < -180.0f) {
			result += 2 * 180.0f;
		}

		return result;
	}

    
	static float LimitRadianAngle(float angle)
	{
		float result = Clamp(angle, -25.0f, 25.0f);

		while (result >= PI) {
			result -= 2 * PI;
		}

		while (result < -PI) {
			result += 2 * PI;
		}
	
		return result;
	}

	// Returns an angle such that x2/y2 looks at x1/y1 with its forward vector if rotated by that angle
	static float GetRadianAngleBetweenPoints(float x1, float y1, float x2, float y2)
	{
		float x = x2 - x1;
		float y = y2 - y1;

		if (y == 0.0f)
			y = 0.0001f;

		if (x > 0.0f) {
			if (y > 0.0f)
				return PI - atan2(x / y, 1.0f);
			else
				return -atan2(x / y, 1.0f);
		} else {
			if (y > 0.0f)
				return -(PI + atan2(x / y, 1.0f));
			else
				return -atan2(x / y, 1.0f);
		}
	}
	
	static float GetAngleBetweenPoints(float x1, float y1, float x2, float y2)
	{
		return RADTODEG(GetRadianAngleBetweenPoints(x1, y1, x2, y2));
	}

	// should return direction in 0-8 range. fits perfectly to peds' path directions.
	static int GetNodeHeadingFromVector(float x, float y)
	{
		float angle = CGeneral::GetRadianAngleBetweenPoints(x, y, 0.0f, 0.0f);
		if (angle < 0.0f)
			angle += TWOPI;

		angle = DEGTORAD(22.5f) + TWOPI - angle;

		if (angle >= TWOPI)
			angle -= TWOPI;

		return (int)floor(angle / DEGTORAD(45.0f));
	}

	// Unlike usual string comparison functions, these don't care about greater or lesser
	static bool faststrcmp(const char *str1, const char *str2)
	{
		for (; *str1; str1++, str2++) {
			if (*str1 != *str2)
				return true;
		}
		return *str2 != '\0';
	}

	static bool faststrncmp(const char *str1, const char *str2, uint32_t count)
	{
		for(uint32_t i = 0; *str1 && i < count; str1++, str2++, i++) {
			if (*str1 != *str2)
				return true;
		}
		return false;
	}

	static bool faststricmp(const char *str1, const char *str2)
	{
		for (; *str1; str1++, str2++) {
#ifndef ASCII_STRCMP
			if (toupper(*str1) != toupper(*str2))
#else
			if (__ascii_toupper(*str1) != __ascii_toupper(*str2))
#endif
				return true;
		}
		return *str2 != '\0';
	}

	// not too sure about all these...
	static uint16_t GetRandomNumber(void)
		{ return rand() & MYRAND_MAX; }
	static bool GetRandomTrueFalse(void)
		{ return GetRandomNumber() < MYRAND_MAX / 2; }
	// Probably don't want to ever reach high
	static float GetRandomNumberInRange(float low, float high)
		{ return low + (high - low)*(GetRandomNumber()/float(MYRAND_MAX + 1)); }
		
	static uint32_t GetRandomNumberInRange(uint32_t low, uint32_t high)
		{ return low + (high - low)*(GetRandomNumber()/float(MYRAND_MAX + 1)); }
};