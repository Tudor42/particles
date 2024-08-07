#pragma once

#include <iostream>
#include <stdio.h>
#include "extern/debugging/debugging-traps.h"
#include <GL/glew.h>

#ifdef DEBUG
    #define ASSERT(x, ...) if (!(x)) { psnip_dbg_assert(x); printf("Assert message " __VA_ARGS__); printf("\n"); }
#else 
    #define ASSERT(x, ...) if (!(x)) { printf("Assert message " __VA_ARGS__); printf("\n");}
#endif


#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__), "")

void GLClearError();
  
bool GLLogCall(const char *function, const char *file, int line);
