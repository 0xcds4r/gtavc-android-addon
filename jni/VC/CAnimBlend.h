#pragma once

#include "CCommon.h"

template<typename T>
class CLink
{
public:
	T item;
	CLink<T> *prev;
	CLink<T> *next;
};

class CAnimBlendSequence
{
public:
	enum {
		KF_ROT = 1,
		KF_TRANS = 2
	};
	uint32_t type;
	char name[24];
	uint32_t numFrames;
	uint16_t boneTag;
	void *keyFrames;
	void *keyFramesCompressed;
};

class CAnimBlendHierarchy
{
public:
    char name[24];
	CAnimBlendSequence *sequences;
	uint16_t numSequences;
	bool compressed;
	bool keepCompressed;
	float totalLength;
	CLink<CAnimBlendHierarchy*> *linkPtr;
};

class CAnimBlendLink
{
public:
	CAnimBlendLink *next;
	CAnimBlendLink *prev;
};

class CAnimBlendAssociation;
class CAnimBlendNode
{
public:
	// for slerp
	float theta;		// angle between quaternions 
	float invSin;		// 1/Sin(theta) 
	// indices into array in sequence 
	uint32_t frameA;		// next key frame 
	uint32_t frameB;		// previous key frame 
	float remainingTime;	// time until frames have to advance 
	CAnimBlendSequence *sequence; // 24
	CAnimBlendAssociation *association;
};

class CAnimBlendAssociation
{
public:
	enum {
		// callbackType
		CB_NONE,
		CB_FINISH,
		CB_DELETE
	};

	CAnimBlendLink link;

	uint16_t numNodes;			// taken from CAnimBlendClumpData::numFrames
	uint16_t groupId;		// ID of CAnimBlendAssocGroup this is in
	// NB: Order of these depends on order of nodes in Clump this was built from
	CAnimBlendNode *nodes;
	CAnimBlendHierarchy *hierarchy;
	float blendAmount;
	float blendDelta;	// how much blendAmount changes over time
	float currentTime;
	float speed;
	float timeStep;
	uint16_t animId;
	uint16_t flags;
	uint32_t callbackType;
};

