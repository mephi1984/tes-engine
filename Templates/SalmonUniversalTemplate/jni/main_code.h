#ifndef MAIN_CODE_H_INCLUDED
#define MAIN_CODE_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef TARGET_ANDROID

#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include "boost/shared_ptr.hpp"
#include "boost/thread/thread.hpp"
#include "boost/assign.hpp"
#include "boost/bind.hpp"
#include "boost/asio.hpp"
#include "boost/signal.hpp"

#include "include/Engine.h"

using namespace SE;


class TMyApplication : public TApplication
{
protected:
	
public:
	bool Inited;

	TFlexModel FlexModel;

	TMyApplication() : TApplication(), Inited(false) { }

	
     virtual void InnerInit();
    
     virtual void InnerDeinit();
	
	 virtual void InnerDraw();
	
	 virtual void InnerUpdate(cardinal dt);
	 
	bool IsInited() { return Inited; }
	

};


#endif
