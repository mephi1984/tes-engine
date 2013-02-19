#include "include/Engine.h"

namespace SE
{

/*
Here goes all functions that are platform-specific
When I make iOS/Mac/Linux port, I will make same API pair h/cpp 
 */

void GetWindowWidthHeight(HWND hwnd, int& width, int& height)
{
	RECT rc;
	GetClientRect(hwnd, &rc);

	width = rc.right;
	height = rc.bottom;
}

int GetWindowWidth(HWND hwnd)
{
	RECT rc;
	GetClientRect(hwnd, &rc);

	return rc.right;
}

int GetWindowHeight(HWND hwnd)
{
	RECT rc;
	GetClientRect(hwnd, &rc);

	return rc.bottom;
}

} //namespace SE