#pragma once

#define ID_LIB_HANDLE_CURRENT   0
#define ID_LIB_HANDLE_GAME      1
#define ID_LIB_HANDLE_IMM       2

namespace utils
{
    class storage
    {
    private:
        char g_szStorage[0xFF] = { '\0' };
    public:
        void set(const char* storage) { strcpy(g_szStorage, storage); };
        const char* get() { return g_szStorage; };
    };

    class log
    {
    private:
        /* data */
    public:
        void push(const char*, const char*, ...);
    };

    class library
    {
    private:
        uintptr_t g_libHandle[0xFF+1] = { 0 };
    public:
        uintptr_t handle(const char*);

        void push(uint8_t id, uintptr_t img) {
            g_libHandle[id] = img;
        };

        void pop(uint8_t id) {
            g_libHandle[id] = 0;
        };

        uintptr_t handle_by_id(uint8_t id) { return g_libHandle[id]; };
    };

    class arm
    {
    private:
        /* data */
    public:
        void initialise(uintptr_t lib, uintptr_t start, uintptr_t end);
        void unprotect(uintptr_t ptr, size_t dwSize = 100);
        void makeNOP(uintptr_t addr, unsigned int count);
        void writeMemory(uintptr_t dest, uintptr_t src, size_t size);
        void readMemory(uintptr_t dest, uintptr_t src, size_t size);
        void jump(uintptr_t func, uintptr_t addr);
        void writeHookProc(uintptr_t addr, uintptr_t func);
        void installHook(uintptr_t addr, uintptr_t func, uintptr_t *orig);
        void redirectCall(uintptr_t addr, uintptr_t func);
        void installMethodHook(uintptr_t addr, uintptr_t func);
        void injectCode(uintptr_t addr, uintptr_t func, int reg);
        void installPLTHook(uintptr_t addr, uintptr_t func, uintptr_t *orig);
    };
};