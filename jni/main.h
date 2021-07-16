#pragma once

// api & sdk
#include <jni.h>
#include <android/log.h>
#include <ucontext.h>
#include <pthread.h>

#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <unistd.h>

#include "utils.h"

// debug
#define VC_DEBUG

// libs
extern uintptr_t g_pVCMP;
extern uintptr_t g_pGTAVC;
extern uintptr_t g_pImmEmu;

// macro
#define MP(dest) \
    (g_pVCMP + dest)
#define VC(dest) \
    (g_pGTAVC + dest)
#define IMM(dest) \
    (g_pImmEmu + dest)

// logcat
#ifndef LOG_CAT_TAG
#define LOG_CAT_TAG "vcmp" // logcat -s vcmp
#endif

#ifndef LOG_CAT_TAG_TOUCH
#define LOG_CAT_TAG_TOUCH "vcmp-touch" // logcat -s vcmp-touch
#endif

// storage
#ifndef DATA_STORAGE
#define DATA_STORAGE "/storage/emulated/0/Android/data/com.rockstargames.gtavc/files/"
#endif 



