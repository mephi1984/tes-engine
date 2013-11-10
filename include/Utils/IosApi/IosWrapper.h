//
//  Header.h
//  doublehitballs
//
//  Created by vvv ооо on 15.07.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

//All of them can be found in SalmonEngineIos.cpp
//This header stores only declarations

namespace SE
{

void AppDeinit();
void AppUpdate(int dt);
void AppDraw();
void AppOnTapDown(int posx, int posy, int touchNumber);
void AppOnTapUp(int posx, int posy, int touchNumber);
void AppOnScroll(int shiftx, int shifty, int touchNumber);
void AppOnScale(float scale);
void AppOnTapUpAfterMove(int posx, int posy, int touchNumber);


}