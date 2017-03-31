#pragma once

/*
This code combines all headers for Salmon engine into one header file
*/

#include "include/SalmonEngineInterface.h"

	namespace SE
	{

		extern TFileConsole* Console;
		extern TSalmonRendererWindowsUniversal* Renderer;
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

