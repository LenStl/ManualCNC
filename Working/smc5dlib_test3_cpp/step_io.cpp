#include "pch.h"
#include "step_io.h"

static int reverseEndian(int value);

static int reverseEndian(int value)
{
    byte* p = (byte*)&value;
    return (*p << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}