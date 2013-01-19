#ifndef GL_CODE_H_INCLUDED
#define GL_CODE_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "boost/shared_ptr.hpp"
#include "boost/thread/thread.hpp"
#include "boost/asio.hpp"
#include "boost/signal.hpp"
#include "boost/assign.hpp"
#include "boost/bind.hpp"

#include "include/Engine.h"

using namespace SE;


class TMyApplication : public TApplication
{
protected:
	
public:
	bool Inited;

	TMyApplication() : TApplication(), Inited(false) { }

	
     virtual void InnerInit();
    
     virtual void InnerDeinit();
	
	 virtual void InnerDraw();
	
	 virtual void InnerUpdate(cardinal dt);
	 
	bool IsInited() { return Inited; }

};


static void checkGlError(const std::string& op);


#endif
