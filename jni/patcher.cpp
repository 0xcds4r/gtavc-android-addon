#include "main.h"

extern utils::storage  client_storage;
extern utils::log      client_log;
extern utils::library  arm_library;
extern utils::arm      arm_tools;

void applyCameraPatches()
{
    // CMenuManager::m_ControlMethod
    arm_tools.unprotect(VC(0x37BA10));
    *(uint32_t*)(VC(0x37BA10)) = 1;

    // patching camera motion
    arm_tools.unprotect(VC(0x12C8CC));
    *(uint32_t*)(VC(0x12C8CC)) = 0;
    arm_tools.unprotect(VC(0x12C9A8));
    *(uint32_t*)(VC(0x12C9A8)) = 0;

    // patching lineOfsight (vehicle)
    arm_tools.unprotect(VC(0x127098));
    *(uint32_t*)(VC(0x127098)) = 0;

    // patching camera
    arm_tools.unprotect(VC(0x1317D0));
    *(uint32_t*)(VC(0x1317D0)) = 0;

    arm_tools.unprotect(VC(0x131D6E));
    *(uint32_t*)(VC(0x131D6E)) = 0;

    arm_tools.unprotect(VC(0x131D60));
    *(uint32_t*)(VC(0x131D60)) = 0;

    arm_tools.unprotect(VC(0x1325FC));
    *(uint32_t*)(VC(0x1325FC)) = 0;

    arm_tools.unprotect(VC(0x1325FE));
    *(uint32_t*)(VC(0x1325FE)) = 0;

    arm_tools.unprotect(VC(0x132600));
    *(uint32_t*)(VC(0x132600)) = 0;

    arm_tools.unprotect(VC(0x1314E6));
    *(uint32_t*)(VC(0x1314E6)) = 0;

    arm_tools.unprotect(VC(0x13085C));
    *(float*)(VC(0x13085C)) = 0.0f;

    // nop calling useless camera stuff
    arm_tools.makeNOP(VC(0x13086E), 2);
    arm_tools.makeNOP(VC(0x13088E), 2);
}

void apply_patches()
{
    #ifdef VC_DEBUG
    client_log.push(LOG_CAT_TAG, "Applying patches..");
    #endif

    // fps
    arm_tools.unprotect(VC(0x37BA17));
    *(uint32_t*)(VC(0x37BA17)) = 60; // optimal framerate
    
    // fps debug
    arm_tools.unprotect(VC(0x58A74D));
    *(uint32_t*)(VC(0x58A74D)) = 1;

    // Dynamic shadows
    arm_tools.unprotect(VC(0x37BA05));
    *(uint32_t*)(VC(0x37BA05)) = 0;

    // CAnimManager::AddAnimation ~ from CPed::SetModelIndex
    arm_tools.makeNOP(VC(0x19D0C2), 2);

    // Patch for CPed::SetCurrentWeapon fixes fucking FindPlayerPed shit  
    arm_tools.writeMemory(VC(0x19110C), (uintptr_t)"\x9D\xF8\x10\x36", 4); // orig: 86 F8 10 36

    // Disable game saves
    arm_tools.writeMemory(VC(0x20B664), (uintptr_t)"\x00\x20\xF7\x46", 4);

    // nop calling useless shit
    arm_tools.makeNOP(VC(0x140D8C), 2); // CStreaming::Shutdown
    arm_tools.makeNOP(VC(0x10628A), 2); // CUpsideDownCarCheck::UpdateTimers
    arm_tools.makeNOP(VC(0x141078), 2); // CRoadBlocks::GenerateRoadBlocks
    arm_tools.makeNOP(VC(0x13E5D0), 2); // CFileLoader::LoadPickup
    arm_tools.makeNOP(VC(0x14107C), 2); // CCarCtrl::RemoveDistantCars

    // CPlayerPed::GetPlayerInfoForThisPlayerPed ~ from CPed::RemoveWeaponWhenEnteringVehicle
    arm_tools.makeNOP(VC(0x1914AA), 6); 
    
    // skip gay movies
    arm_tools.writeMemory(VC(0x304B78), (uintptr_t)"\xF7\x46", 2);

    // disable peds/vehicles (single)
    arm_tools.writeMemory(VC(0x1C0860), (uintptr_t)"\x00\x20\xF7\x46", 4); // CPopulation::AddPed
    arm_tools.writeMemory(VC(0x1C34E0), (uintptr_t)"\x4F\xF0\x00\x00\xF7\x46", 6); // CPopulation::AddToPopulation
    arm_tools.makeNOP(VC(0x1410E4), 2); // CCarCtrl::GenerateRandomCars
    arm_tools.makeNOP(VC(0x141096), 2); // CTheCarGenerators::Process 

    // fix radar crash
    arm_tools.writeMemory(VC(0x31847C), (uintptr_t)"\xF7\x46", 2);

    // RQ_Command_rqSetAlphaTest
    arm_tools.writeMemory(VC(0x27CA8C), (uintptr_t)"\xF7\x46", 2);

    // camera fix
    applyCameraPatches();

    // --------------------------------------------------------------------------------------------
    // pools

    // Entry Info Node Pool
    arm_tools.writeMemory(VC(0x163FCA), (uintptr_t)"\x4F\xF6\xFE\x70", 4); // movw r0, #0xFFFE
	arm_tools.writeMemory(VC(0x163FD4), (uintptr_t)"\x4F\xF4\xC8\x50", 4); // mov.w r0, #6400
	arm_tools.writeMemory(VC(0x163FE8), (uintptr_t)"\x4F\xF4\xC8\x50", 4); // mov.w r0, #6400
	arm_tools.writeMemory(VC(0x164006), (uintptr_t)"\xB3\xF5\xC8\x5F", 4); // cmp.w r3, #6400

    // Object Pool
    arm_tools.writeMemory(VC(0x164146), (uintptr_t)"\x45\xF6\x00\x10\xC0\xF2\x06\x00", 8); // movw r0, #0x5900 | movt r0, #6
	arm_tools.writeMemory(VC(0x164154), (uintptr_t)"\x4F\xF4\x3B\x60", 4); // MOV.W R0, #0xBB0
	arm_tools.writeMemory(VC(0x16415C), (uintptr_t)"\x4F\xF4\x3B\x61", 4); // MOV.W R1, #0xBB0
	arm_tools.writeMemory(VC(0x164182), (uintptr_t)"\xB3\xF5\x3B\x6F", 4); // CMP.W R3, #0xBB0

    // todo: pools
}