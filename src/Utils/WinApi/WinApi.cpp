#include "include/Engine.h"

namespace SE
{

/*
Here goes all functions that are platform-specific
When I make iOS/Mac/Linux port, I will make same API pair h/cpp 
 */

void GetWindowWidthHeight(int& width, int& height)
{
	RECT rc;
	GetClientRect(Hwnd, &rc);

	width = rc.right;
	height = rc.bottom;
}

int GetWindowWidth()
{
	RECT rc;
	GetClientRect(Hwnd, &rc);

	return rc.right;
}

int GetWindowHeight()
{
	RECT rc;
	GetClientRect(Hwnd, &rc);

	return rc.bottom;
}

} //namespace SE