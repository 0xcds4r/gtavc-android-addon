#include "main.h"
#include "javawrapper.h"
#include "scripting.h"
#include "crashhandler.h"
#include "client_handler.h"
#include "gui/gui.h"
#include <dlfcn.h>

JavaVM *javaVM = NULL;

utils::storage  client_storage;
utils::log      client_log;
utils::library  arm_library;
utils::arm      arm_tools;

// graphics
gui::wrapper    gui_wrapper;
gui::drawer     gui_drawer;
gui::events     gui_events;

uintptr_t g_pVCMP = 0;
uintptr_t g_pGTAVC = 0;
uintptr_t g_pImmEmu = 0;

void init_hooks();
void apply_patches();

void onJNI_OnLoad_handle()
{
    #ifdef VC_DEBUG
    client_log.push(LOG_CAT_TAG, "------------------------------------------------------------");
    client_log.push(LOG_CAT_TAG, "VC-MP library loaded! Build time: " __DATE__ " " __TIME__);
    #endif

    g_pVCMP = arm_library.handle("libmultiplayer.so");
    g_pGTAVC = arm_library.handle("libGTAVC.so");
    g_pImmEmu = arm_library.handle("libImmEmulatorJ.so");

    arm_library.push(ID_LIB_HANDLE_CURRENT, g_pVCMP);
    arm_library.push(ID_LIB_HANDLE_GAME, g_pGTAVC);
    arm_library.push(ID_LIB_HANDLE_IMM, g_pImmEmu);

    #ifdef VC_DEBUG
    client_log.push(LOG_CAT_TAG, "Images of libraries:\nlibmultiplayer.so - 0x%X \nlibGTAVC.so - 0x%X \nlibImmEmulatorJ.so - 0x%X", arm_library.handle_by_id(ID_LIB_HANDLE_CURRENT), arm_library.handle_by_id(ID_LIB_HANDLE_GAME), arm_library.handle_by_id(ID_LIB_HANDLE_IMM));
    #endif

    crashdump::initialize();
    
    init_hooks();
    apply_patches();
}

void initVCMP(JNIEnv* pEnv, jobject thiz)
{
    client_log.push(LOG_CAT_TAG, "Initializing VC-MP..");

    client_storage.set(DATA_STORAGE);

    #ifdef VC_DEBUG
    client_log.push(LOG_CAT_TAG, "Storage: %s", client_storage.get());
    #endif

    g_pJavaWrapper = new CJavaWrapper(pEnv, thiz);
    if(!g_pJavaWrapper) 
    {
        #ifdef VC_DEBUG
        client_log.push(LOG_CAT_TAG, "errno: JavaWrapper is not initialized!");
        #endif

        exit(0);
        return;
    }
}  

void updateScreen()
{
    if(!g_pJavaWrapper) {
        return;
    }

    g_pJavaWrapper->hideLayoutCut();
    g_pJavaWrapper->hideNavBar();
} 

extern "C"
{
	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_initVCMP(JNIEnv* pEnv, jobject thiz)
	{
        initVCMP(pEnv, thiz);
    }

    JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_updateScreen(JNIEnv* pEnv, jobject thiz)
	{
        updateScreen();
    }
}

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{ 
    javaVM = vm;

    registerHandler(&onJNI_OnLoad_handle);
    callHandler(1);

    return JNI_VERSION_1_4;
}