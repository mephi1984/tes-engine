#include "include/Engine.h"


namespace SE
{
	
	NewGuiManager::NewGuiManager()
	{

	}

	NewGuiManager::~NewGuiManager()
	{

	}

	void NewGuiManager::Init()
	{
		BackgroundRenderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = "bt_box_yellow.jpg";
		BackgroundRenderPair.second.Data = MakeDataTriangleList(Vector2f(0, 0), Vector2f(Renderer->GetScreenWidth(), Renderer->GetScreenHeight()*0.5f));
		BackgroundRenderPair.second.RefreshBuffer();
	}

	void NewGuiManager::Deinit()
	{

	}

	void NewGuiManager::Update(size_t dt)
	{

		
	}

	void NewGuiManager::Draw()
	{
		TRenderParamsSetter render(BackgroundRenderPair.first);

		Renderer->DrawTriangleList(BackgroundRenderPair.second);
	}

	void NewGuiManager::OnMouseDown(Vector2f pos, int touchNumber)
	{

	}

	void NewGuiManager::OnMouseUp(Vector2f pos, int touchNumber)
	{

	}

	void NewGuiManager::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{

	}

	void NewGuiManager::OnMove(Vector2f shift, int touchNumber)
	{

	}

	void NewGuiManager::OnKeyPressed(int key)
	{

	}





} //namespace SE
