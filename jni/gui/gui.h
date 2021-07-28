#pragma once

enum eTouchType
{
	TOUCH_POP = 1,
	TOUCH_PUSH = 2,
	TOUCH_MOVE = 3
};

namespace gui
{
    class wrapper
    {
    private:
        /* data */
    public:
        void initialise();
    };
    
    class drawer
    {
    private:
        /* data */
    public:
        void initialise();
    };

    class events 
    {
    private:
        /* data */
    public:
        void initialise();
        bool touch(eTouchType type, int x, int y);
    };
}