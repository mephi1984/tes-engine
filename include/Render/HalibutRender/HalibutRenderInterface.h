#ifndef HALIBUT_RENDER_INTERFACE_H_INCLUDED 
#define HALIBUT_RENDER_INTERFACE_H_INCLUDED 

#include "include/Utils/Utils.h"
#include "include/Render/RenderInterface.h"

namespace SE
{

class THalibutRendererInterface : public TRendererInterface
{
protected:



public:

	virtual void SetUniforms();

	virtual void InitOpenGL(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight);

	virtual void SwitchToScreen();
	virtual void SwitchToFrameBuffer(const std::string& frameName);

	virtual void PrintMatrices();

	
};

} //namespace SE

#endif