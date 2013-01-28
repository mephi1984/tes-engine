//
//  Halibut_Engine.h
//  Halibut Engine
//
//  Created by vvv ооо on 13.07.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include "include/Utils/ErrorTypes/ErrorTypes.h"

namespace SE
{

std::string IosGetFileReadPath(const std::string& filename);

std::string IosGetFilePathUserData(const std::string& filename);

void IosSwitchToScreen();

} //namespace SE
