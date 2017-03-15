#ifndef NEW_GUI_MANAGER_H_INCLUDED
#define NEW_GUI_MANAGER_H_INCLUDED

#include "include/Render/RenderMisc.h"
#include "include/Utils/Utils.h"

#include "boost/variant.hpp"
#include "boost/thread.hpp"

namespace SE
{

	class NewGuiManager
	{
	protected:

		TRenderPair BackgroundRenderPair;

	public:

		NewGuiManager();

		~NewGuiManager();

		void Init();
		void Deinit();

		void Update(size_t dt);

		void Draw();

		void OnMouseDown(Vector2f pos, int touchNumber);

		void OnMouseUp(Vector2f pos, int touchNumber);

		void OnMouseUpAfterMove(Vector2f pos, int touchNumber);

		void OnMove(Vector2f shift, int touchNumber);

		void OnKeyPressed(int key);
	};


} //namespace SE




#endif
  //NEW_GUI_MANAGER_H_INCLUDED
