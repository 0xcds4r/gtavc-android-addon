#include "main.h"
#include "client_handler.h"

capture_callback *m_pHandler;

void registerHandler(capture_callback *handler)
{
    if(handler == nullptr) return;
    m_pHandler = handler;
}

void* handlerThread(void* p)
{
    if(m_pHandler == nullptr) pthread_exit(0);
    m_pHandler();
    pthread_exit(0);
}

void* handlerThreadRepeating(void* p)
{
    if(m_pHandler == nullptr) pthread_exit(0);

    while(true) {
        m_pHandler();
    }

    pthread_exit(0);
}

void callHandler(uint8_t async)
{
    if(async == 1) 
    {
        pthread_t asyncThread;
        pthread_create(&asyncThread, 0, handlerThread, 0);
        return;
    }

    if(async == 2) 
    {
        pthread_t asyncThreadRep;
        pthread_create(&asyncThreadRep, 0, handlerThreadRepeating, 0);
        return;
    }

    if(m_pHandler == nullptr) return;
    m_pHandler();
}