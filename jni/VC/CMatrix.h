#pragma once

#include "CCommon.h"

class CMatrix
{
public:
    float right[3];		    
	uint32_t flags;	    
	float up[3];			
	float  pad_u;		    
	float at[3];			
	float  pad_a;		    
	float pos[3];			
	float  pad_p;	
};