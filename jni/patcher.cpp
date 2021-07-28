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

void applyPoolPatches()
{
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

    // ColModel Pool
    arm_tools.writeMemory(VC(0x164234), (uintptr_t)"\x4F\xF4\xFC\x50\xC0\xF2\x17\x00", 8); // mov.w r0, #0x1F80 | movt r0, #0x17
	arm_tools.writeMemory(VC(0x164242), (uintptr_t)"\x47\xF6\xFF\x60", 4); // movw r0, #0x7EFF
	arm_tools.writeMemory(VC(0x16424A), (uintptr_t)"\x47\xF6\xFF\x61", 4); // movw r1, #0x7EFF

    // Dummy Pool
    arm_tools.writeMemory(VC(0x164196), (uintptr_t)"\x4F\xF2\x30\x20\xC0\xF2\x03\x00", 8); // movw r0, #0xF230 | movt r0, #0x3
	arm_tools.writeMemory(VC(0x1641A4), (uintptr_t)"\x40\xF6\xA0\x70", 4); // movw r0, #0xFA0
	arm_tools.writeMemory(VC(0x1641AC), (uintptr_t)"\x40\xF6\xA0\x71", 4); // movw r1, #0xFA0

    // Building Pool
    arm_tools.writeMemory(VC(0x1640B6), (uintptr_t)"\x4F\xF4\x90\x40\xC0\xF2\x19\x00", 8); // mov.w r0, #0x4800 | movt r0, #0x19
	arm_tools.writeMemory(VC(0x1640C4), (uintptr_t)"\x47\xF2\x91\x30", 4); // movw r0, #0x7391
	arm_tools.writeMemory(VC(0x1640CC), (uintptr_t)"\x47\xF2\x91\x31", 4); // movw r1, #0x7391

    // Vehicle Pool
    arm_tools.writeMemory(VC(0x164068), (uintptr_t)"\x4F\xF6\xC0\x50\xC0\xF2\x14\x00", 8); // movw r0, #0xFDC0 | movt r0, #20
    arm_tools.writeMemory(VC(0x164076), (uintptr_t)"\xD2\x20", 2); // movs r0, #0xD2
    arm_tools.writeMemory(VC(0x164088), (uintptr_t)"\xD2\x20", 2); // movs r0, #0xD2
    arm_tools.writeMemory(VC(0x1640A4), (uintptr_t)"\xD2\x2B", 2); // cmp r3, #0xD2

    // Ped Pool
    arm_tools.writeMemory(VC(0x16401A), (uintptr_t)"\x4F\xF6\x40\x70\xC0\xF2\x06\x00", 8); // movw r0, #0xFF40 | movt r0, #6
    arm_tools.writeMemory(VC(0x164028), (uintptr_t)"\xD2\x20", 2); // movs r0, #0xD2
    arm_tools.writeMemory(VC(0x16403A), (uintptr_t)"\xD2\x20", 2); // movs r0, #0xD2
    arm_tools.writeMemory(VC(0x164056), (uintptr_t)"\xD2\x2B", 2); // cmp r3, #0xD2

    // Treadable Pool
    // arm_tools.writeMemory(VC(0x16410E), (uintptr_t)"\xCC\x20", 2); // movs r0, #204
    // arm_tools.writeMemory(VC(0x164116), (uintptr_t)"\x1E\x20", 2); // movs r0, #30
    // arm_tools.writeMemory(VC(0x16411E), (uintptr_t)"\x1E\x22", 2); // movs r2, #30

    // AudioScriptObject Pool
    arm_tools.writeMemory(VC(0x1641EA), (uintptr_t)"\x4F\xF4\x7A\x60", 4); // mov.w r0, #0xFA0
    arm_tools.writeMemory(VC(0x1641F4), (uintptr_t)"\xF0\x20", 2); // movs r0, #0xF0
    arm_tools.writeMemory(VC(0x164206), (uintptr_t)"\xF0\x20", 2); // movs r0, #0xF0
    arm_tools.writeMemory(VC(0x164222), (uintptr_t)"\xF0\x2B", 2); // cmp r3, #0xF0

    // TxdStore Pool
    arm_tools.writeMemory(VC(0x2028CE), (uintptr_t)"\x4F\xF6\xC0\x50", 4); // movw r0, #0xFDC0
    arm_tools.writeMemory(VC(0x2028D8), (uintptr_t)"\x40\xF6\xF0\x10", 4); // movw r0, #0x9F0
    arm_tools.writeMemory(VC(0x2028E0), (uintptr_t)"\x40\xF6\xF0\x11", 4); // movw r1, #0x9F0

    // CRenderer::Init
    arm_tools.writeMemory(VC(0x16C3D6), (uintptr_t)"\x4F\xF4\xA1\x40", 4); // mov.w r0, #0x5080
    arm_tools.writeMemory(VC(0x16C3E0), (uintptr_t)"\x4F\xF4\xA0\x42", 4); // mov.w r2, #0x5000

    // CAnimManager::Initialise
    arm_tools.writeMemory(VC(0xB1486), (uintptr_t)"\x4F\xF4\x1F\x60", 4); // mov.w r0, #0x9F0
    arm_tools.writeMemory(VC(0xB14A2), (uintptr_t)"\x4F\xF4\x1F\x62", 4); // mov.w r2, #0x9F0
}

void apply_patches()
{
    #ifdef VC_DEBUG
    client_log.push(LOG_CAT_TAG, "Applying patches..");
    #endif
    
    // fps debug
    arm_tools.unprotect(VC(0x58A74D));
    *(uint32_t*)(VC(0x58A74D)) = 1;

    // Dynamic shadows
    arm_tools.unprotect(VC(0x37BA05));
    *(uint32_t*)(VC(0x37BA05)) = 0;

    // CAnimManager::AddAnimation ~ from CPed::SetModelIndex
    arm_tools.makeNOP(VC(0x19D0C2), 2);

    // Patch for CPed::SetCurrentWeapon fixes fucking FindPlayerPed shit  
    arm_tools.writeMemory(VC(0x19110C), (uintptr_t)"\x9D\xF8\x10\x36", 4);

    // Disable game saves
    arm_tools.writeMemory(VC(0x20B664), (uintptr_t)"\x00\x20\xF7\x46", 4);

    // nop calling useless shit
    arm_tools.makeNOP(VC(0x140D8C), 2); // CStreaming::Shutdown
    arm_tools.makeNOP(VC(0x10628A), 2); // CUpsideDownCarCheck::UpdateTimers
    arm_tools.makeNOP(VC(0x141078), 2); // CRoadBlocks::GenerateRoadBlocks
    arm_tools.makeNOP(VC(0x13E5D0), 2); // CFileLoader::LoadPickup
    arm_tools.makeNOP(VC(0x14107C), 2); // CCarCtrl::RemoveDistantCars
    arm_tools.makeNOP(VC(0x1753DC), 2); // CPlaceName::Process
    arm_tools.makeNOP(VC(0x1BF854), 2); // CPlayerPed::ProcessAnimGroups

    // Patch for cAudioManager::GetPedCommentSfx()
    arm_tools.writeMemory(VC(0xC953A), (uintptr_t)"\x58\xD0", 2);

    // CPlayerPed::GetPlayerInfoForThisPlayerPed ~ from CPed::RemoveWeaponWhenEnteringVehicle
    arm_tools.makeNOP(VC(0x1914AA), 6); 
    
    // skip gay movies
    arm_tools.unprotect(VC(0x6F5A18));
    *(uint8_t*)(VC(0x6F5A18)) = 1;

    /* Replay mode disable */
    arm_tools.writeMemory(VC(0xFB9E0), (uintptr_t)"\xF7\x46", 2);
    arm_tools.writeMemory(VC(0xFD5F8), (uintptr_t)"\x00\x20\xF7\x46", 4);
    arm_tools.writeMemory(VC(0xFD488), (uintptr_t)"\x00\x20\xF7\x46", 4);
    
    // disable peds/vehicles (single)
    arm_tools.writeMemory(VC(0x1C0860), (uintptr_t)"\x00\x20\xF7\x46", 4); // CPopulation::AddPed
    arm_tools.writeMemory(VC(0x1C34E0), (uintptr_t)"\x4F\xF0\x00\x00\xF7\x46", 6); // CPopulation::AddToPopulation
    arm_tools.makeNOP(VC(0x1410E4), 2); // CCarCtrl::GenerateRandomCars
    arm_tools.makeNOP(VC(0x141096), 2); // CTheCarGenerators::Process 

    // fix radar crash
    arm_tools.writeMemory(VC(0x31847C), (uintptr_t)"\xF7\x46", 2);

    // RQ_Command_rqSetAlphaTest
    arm_tools.writeMemory(VC(0x27CA8C), (uintptr_t)"\xF7\x46", 2);

    // sprint fix
    arm_tools.writeMemory(VC(0x2138EE), (uintptr_t)"\x01\x22", 2);
    arm_tools.writeMemory(VC(0x1BAA98), (uintptr_t)"\xF7\x46", 2);
    
    // CHud::SetHelpMessage
    arm_tools.writeMemory(VC(0x1D7BA8), (uintptr_t)"\x00\x20\xF7\x46", 4);

    // CFire::Extinguish nopped
    arm_tools.writeMemory(VC(0x1BB432), (uintptr_t)"\x00\x46\x00\x46", 4); 

    // camera fix
    applyCameraPatches();

    // pools
    applyPoolPatches();
}