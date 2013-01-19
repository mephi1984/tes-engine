//
//  HalibutEngineIos.h
//  Halibut Engine
//
//  Created by vvv ооо on 13.07.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef HALIBUT_ENGINE_IOS_H_INCLUDED
#define HALIBUT_ENGINE_IOS_H_INCLUDED

#include "include/HalibutEngineInterface.h"

#include "include/Render/HalibutRender/HalibutRenderIos.h"

#include "include/Utils/Utils.h"

namespace SE
{

extern TIosConsole* Console;

extern THalibutResourceManager* ResourceManager;

extern THalibutRenderIos* Renderer;

class TApplication : public TApplicationAncestor
{
public:
    TApplication();
    
	virtual ~TApplication();
	
	virtual void OuterInit(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight);
	
	virtual void OuterDeinit();
	
	virtual void OnKeyPress(cardinal key);
};



} //namespace SE


//This file includes templates that call any of three singletones: Console, ResourceManager or Renderer

#include "include/GUIManager/WidgetTemplatesImpl.h"



#endif
