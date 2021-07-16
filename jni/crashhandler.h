#ifndef CRASHDUMP_H
#define CRASHDUMP_H

#include <ucontext.h>

class crashdump
{
private:
    /* data */
public:
     static void initialize();
     static void report(ucontext* context, siginfo_t *info, const char* typeName);
     static void printSymbols(void* pc, void* lr);
     static void OnEnterCrashZone(uint);
     static void OnCrashAverted(uint);
};

#endif // CRASHDUMP_H