#pragma once

// api & sdk
#include <jni.h>
#include <android/log.h>
#include <ucontext.h>
#include <pthread.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <unistd.h>

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

// storage
#ifndef DATA_STORAGE
#define DATA_STORAGE "/storage/emulated/0/Android/data/com.rockstargames.gtavc/files/"
#endif 

typedef void capture_callback();

#include "utils.h"