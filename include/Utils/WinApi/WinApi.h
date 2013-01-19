#pragma once

#define NOMINMAX 1
#include "windows.h"
#undef NOMINMAX
#include "gl/gl.h"
#include "gl/glu.h"
#include "gl/glext.h"

namespace SE
{

/*
Here goes all functions that are platform-specific
When I make iOS/Mac/Linux port, I will make same API pair h/cpp 
 */

void GetWindowWidthHeight(int& width, int& height);

int GetWindowWidth();

int GetWindowHeight();

} //namespace SE