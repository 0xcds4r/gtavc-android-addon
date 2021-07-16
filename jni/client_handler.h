#pragma once

#include <unistd.h>

typedef void capture_callback();

void registerHandler(capture_callback *handler);
void callHandler(uint8_t async);