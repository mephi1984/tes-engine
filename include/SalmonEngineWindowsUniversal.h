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
		public:
			TApplication() { };

			~TApplication() { };

			virtual void UpdateQuick() { };
			//To process input - this method is called more frequently than Update()  

			virtual void OnMouseWheel(short int delta) { }
			//To do on mouse wheel move

			virtual void OnKeyPress(size_t key); //Try not to override this. But if you need to override, call ancestor!

			virtual void InnerOnKeyPress(size_t key) { }
		};

		void CreateEngine();
		void DestroyEngine();

	} //namespace SE

#include "include/GUIManager/WidgetTemplatesImpl.h"
#include "include/Utils/ThreadUtilsImpl.h"

