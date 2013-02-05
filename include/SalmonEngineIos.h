#ifndef SALMON_ENGINE_ANDROID_H_INCLUDED
#define SALMON_ENGINE_ANDROID_H_INCLUDED
/*
This code combines all headers for Salmon engine into one header file
*/

#include "include/SalmonEngineInterface.h"

namespace SE
{

extern TIosConsole* Console;
extern TSalmonRendererIos* Renderer;
extern TResourceManager* ResourceManager;

class TApplication : public TApplicationAncestor
{
protected:
public:
};

void CreateEngine();
void DestroyEngine();

} //namespace SE


#include "include/GUIManager/WidgetTemplatesImpl.h"
#include "include/Utils/ThreadUtilsImpl.h"

#endif
