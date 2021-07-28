#include "../main.h"
#include "gui.h"

extern utils::storage  client_storage;
extern utils::log      client_log;
extern utils::library  arm_library;
extern utils::arm      arm_tools;

void gui::events::initialise()
{
    // todo
}

bool gui::events::touch(eTouchType type, int x, int y)
{
    // 32CDDC 46 50 53 3A 20 25 2E 32 66 20 20+    aFps2fFootzoomD DCB "FPS: %.2f    FootZoom: %d%%",0
    arm_tools.unprotect(VC(0x32CDDC));

    std::string str;
    str = std::string("\t\t\t\t\t\tFPS: %.2f") + std::string(" FootZoom: %d%%") + std::string(" x: ") + std::to_string(x) + std::string(" y: ") + std::to_string(y); 
    strcpy((char*)(VC(0x32CDDC)), str.c_str());

    switch (type)
    {
        case TOUCH_POP:
        
        break;
    
        case TOUCH_PUSH:
        
        break;

        case TOUCH_MOVE:
        
        break;
    
        default:
            break;
    }

    return true;
}

void gui::wrapper::initialise()
{
    // todo
}

void gui::drawer::initialise()
{
    // todo
}