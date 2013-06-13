#include "include/Engine.h"

namespace SE
{


	TKeyboardWidget::TKeyboardWidget()
		: Shifted(false)
	{
		std::vector<std::pair<ivec2, int>> LineChars;

		LineChars.push_back(std::pair<ivec2, int>(ivec2(16, 185), static_cast<int>('q')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(48, 185), static_cast<int>('w')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(80, 185), static_cast<int>('e')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(112, 185), static_cast<int>('r')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(144, 185), static_cast<int>('t')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(176, 185), static_cast<int>('y')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(208, 185), static_cast<int>('u')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(240, 185), static_cast<int>('i')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(272, 185), static_cast<int>('o')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(304, 185), static_cast<int>('p')));


		LineChars.push_back(std::pair<ivec2, int>(ivec2(32, 131), static_cast<int>('a')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(64, 131), static_cast<int>('s')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(96, 131), static_cast<int>('d')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(128, 131), static_cast<int>('f')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(160, 131), static_cast<int>('g')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(192, 131), static_cast<int>('h')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(224, 131), static_cast<int>('j')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(256, 131), static_cast<int>('k')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(288, 131), static_cast<int>('l')));


		LineChars.push_back(std::pair<ivec2, int>(ivec2(62, 78), static_cast<int>('z')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(94, 78), static_cast<int>('x')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(126, 78), static_cast<int>('c')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(158, 78), static_cast<int>('v')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(190, 78), static_cast<int>('b')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(222, 78), static_cast<int>('n')));
		LineChars.push_back(std::pair<ivec2, int>(ivec2(254, 78), static_cast<int>('m')));

		
		
		VisualKeyArr.clear();

		BOOST_FOREACH(auto& linePair, LineChars)
		{
			std::tuple<TVisualKey, int, int> line_tuple = std::tuple<TVisualKey, int, int>(TVisualKey(linePair.first, ivec2(26, 38)), linePair.second, linePair.second);
		
			VisualKeyArr.push_back(line_tuple);
		}

		//Backspace
		std::tuple<TVisualKey, int, int> backspace_tuple = std::tuple<TVisualKey, int, int>(TVisualKey(ivec2(299, 77), ivec2(36, 38)), 8, 8);
		
		VisualKeyArr.push_back(backspace_tuple);

		//Space
		std::tuple<TVisualKey, int, int> space_tuple = std::tuple<TVisualKey, int, int>(TVisualKey(ivec2(160, 23), ivec2(154, 38)), static_cast<int>(' '), static_cast<int>(' '));
		
		VisualKeyArr.push_back(space_tuple);


		LeftBottomPos = vec2(0,0);

		TRenderParams renderParams;

		renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = "keyboard.png";

		TTriangleList triangleList = MakeTriangleList(vec2(0,0), vec2(320.f, 216.f));

		triangleList.RefreshBuffer();

		TriangleListVector.push_back(TRenderPair(renderParams, triangleList));

		//Button width and height = 26, 38
		//Button half-width and half-height = 13, 19

		//Upper line:
		//3, 204 q
		//35, 204 w
		//67, 204 e
		//99, 204 r
		//131, 204 t
		//163, 204 y
		//195, 204 u
		//227, 204 i
		//259, 204 o
		//291, 204 p
		

		//Middle line:
		//19, 150 a 
		//51, 150 s
		//83, 150 d
		//115, 150, f
		//147, 150, g
		//179, 150, h
		//211, 150, j
		//243, 150, k
		//275, 150, l

		//Lower line
		//49, 97 z
		//81, 97 x
		//113, 97 c
		//145, 97 v
		//177, 97 b
		//209, 97 n
		//241, 97 m

		//All keyboard w=320 h=216
		//return w=74 h = 38 pos = 280 23
		//shift w=36 h = 38 pos = 21 77

		//backspace w=36 h = 38 pos = 299 77

		//x=83 y=174 w=154 h =38

		//space w = 154 h = 38 x = 160 y = 23

		//numbers x=20 y=23 w= 34 h=38

		//lang x=26 y=23 w= 34 h=38



		
	}

	bool TKeyboardWidget::CheckClick(vec2 mousePos)
	{
		return (mousePos.v[0] >= 0.f && mousePos.v[1] >= 0.f && mousePos.v[0] <= 320.f && mousePos.v[1] <= 216.f);
	}

	void TKeyboardWidget::OnTapDown(vec2 pos)
	{
	}

	void TKeyboardWidget::OnTapUp(vec2 pos)
	{
		BOOST_FOREACH(auto& visualKeyElement, VisualKeyArr)
		{
			if (std::get<0>(visualKeyElement).CheckClicked(ivec2(pos.v[0], pos.v[1])))
			{
				ResourceManager->GUIManager.KeyPressedSignal(std::get<1>(visualKeyElement));
			}
		}

		if (pos.v[0] >= 280 - 74/2 && pos.v[0] <= 280 + 74/2 &&
			pos.v[1] >= 23 - 38/2 && pos.v[1] <= 23 + 38/2)
		{
			ResourceManager->GUIManager.HideKeyboard();
		}
	}


}