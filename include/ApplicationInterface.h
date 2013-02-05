#ifndef APPLICATION_INTERFACE_H_INCLUDED
#define APPLICATION_INTERFACE_H_INCLUDED

#include "include/Utils/Utils.h"

namespace SE
{

class TApplicationInterface
{
protected:
	bool IsConsoleOut;
public:


	TApplicationInterface();

	virtual ~TApplicationInterface();

	virtual void OuterInit(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight);

	virtual void InnerInit() { }
	//To do on init

	virtual void OuterDeinit();

	virtual void InnerDeinit() { }
	//To do on deinit

	virtual void OuterDraw() { }

	virtual void InnerDraw() { }
	//What to draw

	virtual void OuterUpdate(cardinal timer) { }

	virtual void InnerUpdate(cardinal timer) { }
};

} //namespace SE


#endif