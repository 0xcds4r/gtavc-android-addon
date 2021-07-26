#pragma once

#define MAX_SCRIPT_VARS 16
#define MAX_SCRIPT_SIZE	255

struct GAME_SCRIPT_THREAD
{
	void* pNext;			// 0x00
	void* pPrev;			// 0x04
	char strName[8];		// 0x08
	uint32_t dwScriptIP;		// 0x10
	uint32_t dwReturnStack[6];	// 0x14
	uint32_t dwStackPointer;	// 0x2C
	uint32_t dwLocalVar[18];	// 0x30
	uint8_t bStartNewScript;	// 0x78
	uint8_t bJumpFlag;			// 0x79
	uint8_t bWorky;			// 0x7A
	uint8_t bAwake;			// 0x7B
	uint32_t dwWakeTime;		// 0x7C
	uint16_t wIfParam;			// 0x80
	uint8_t bNotFlag;			// 0x82
	uint8_t bWastedBustedCheck;// 0x83
	uint8_t bWastedBustedFlag;	// 0x84
	uint8_t bMissionThread;	// 0x85
	uint8_t pad[2];			// 0x86
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

const SCRIPT_COMMAND toggle_player_infinite_run = { 0x0330, "ii" };
const SCRIPT_COMMAND toggle_player_controllable = { 0x01B4, "ii" };
const SCRIPT_COMMAND toggle_player_fast_reload  = { 0x0331, "ii" };
const SCRIPT_COMMAND set_player_armed_weapon	= { 0x01b8, "ii" };
const SCRIPT_COMMAND get_player_armed_weapon	= { 0x046f, "iv" };
const SCRIPT_COMMAND remove_player_weapons		= { 0x03b8, "i"  };
const SCRIPT_COMMAND put_player_in_car			= { 0x0369, "ii" };
const SCRIPT_COMMAND put_player_at_and_remove_from_car = { 0x012a, "ifff" };
const SCRIPT_COMMAND set_actor_immunities		= { 0x02ab, "iiiiii" };
const SCRIPT_COMMAND set_car_immunities			= { 0x02ac, "iiiiii" };
const SCRIPT_COMMAND create_object				= { 0x0107, "ifffv" };
const SCRIPT_COMMAND set_object_z_angle			= { 0x0177, "if" };
const SCRIPT_COMMAND set_object_collision		= { 0x0382, "ii" }; 
const SCRIPT_COMMAND play_sound					= { 0x018c, "fffi" };
const SCRIPT_COMMAND create_sound				= { 0x018d,	"fffiv" };
const SCRIPT_COMMAND get_player_weapon_ammo		= { 0x0419, "iiv" };
const SCRIPT_COMMAND get_car_z_angle			= { 0x0174, "iv" };