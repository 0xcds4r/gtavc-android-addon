#pragma once

#define MAX_SCRIPT_VARS 16
#define MAX_SCRIPT_SIZE	255

struct GAME_SCRIPT_THREAD
{
	uint8_t Pad1[20];			// +00
	uintptr_t dwScriptIP;		// +20
	uint8_t Pad2[36];			// +24
	uint32_t dwLocalVar[32];	// +60
	uint32_t dwTimers[2];		// +188
	uint8_t Pad3[33];			// +196
	uint8_t condResult;			// +229
	uint8_t Pad4[10];			// +230
	uint16_t logicalOp;			// +240
	uint8_t notFlag;			// +242
	uint8_t Pad5[13];			// +243
	// STRUCT SIZE = 256
};

struct SCRIPT_COMMAND
{
	uint16_t OpCode;
	char Params[MAX_SCRIPT_VARS];
};

int ScriptCommand(const SCRIPT_COMMAND* pScriptCommand, ...);

const SCRIPT_COMMAND clear_player_wanted_level = { 0x0110, "i" };
const SCRIPT_COMMAND create_car                = { 0x00A5, "ifffv" };
const SCRIPT_COMMAND store_player_pos          = { 0x0054, "ivvv" };