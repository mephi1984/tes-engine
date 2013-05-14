#include "include/Engine.h"

namespace SE
{

const cardinal CONST_BUTTON_HIGHLIGHT_TIME = 100;

TInstancingWidgetAncestor::~TInstancingWidgetAncestor() 
{
}


void TInstancingWidgetAncestor::Update(cardinal dt) 
{
}

bool TInstancingWidgetAncestor::CheckClick(vec2 mousePos) 
{
	return false;
}


bool TSquareStatic::CheckClick(vec2 mousePos)
{
	if (mousePos.v[0] >= LeftBottomPos.v[0] &&
		mousePos.v[0] <= LeftBottomPos.v[0] + Width &&
		mousePos.v[1] >= LeftBottomPos.v[1] &&
		mousePos.v[1] <= LeftBottomPos.v[1] + Height)
	{
		return true;
	}
	else
	{
		return false;
	}
}

TSquareStatic* TSquareStatic::CreateStatic(vec2 posFrom, vec2 posTo)
{
	TSquareStatic* staticWidget = new TSquareStatic();

	staticWidget->LeftBottomPos = posFrom;
	staticWidget->Width = posTo.v[0] - posFrom.v[0];
	staticWidget->Height = posTo.v[1] - posFrom.v[1];

	return staticWidget;

}


TSquareStatic* TSquareStatic::CreateStaticWithFiller(vec2 posFrom, vec2 posTo, boost::function<void(TSquareStatic*)> staticFillerFunc)
{
	TSquareStatic* staticWidget = TSquareStatic::CreateStatic(posFrom, posTo);

	staticFillerFunc(staticWidget);

	return staticWidget;
}


TSquareStatic* TSquareStatic::CreateStaticWithFillers(vec2 posFrom, vec2 posTo, std::vector<boost::function<void(TSquareStatic*)>> staticFillerFuncArr)
{
	TSquareStatic* staticWidget = TSquareStatic::CreateStatic(posFrom, posTo);

	BOOST_FOREACH(auto& func, staticFillerFuncArr)
	{
		func(staticWidget);
	}

	return staticWidget;
}


void TSquareStatic::StaticTextureFiller(vec2 posFrom, vec2 posTo, const std::string& texName, TSquareStatic* staticToFill)
{
	
	TRenderParams renderParams;

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = texName;
	renderParams.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 1.f;

	TTriangleList triangleList = MakeTriangleList(posFrom, posTo);

	triangleList.RefreshBuffer();

	staticToFill->TriangleListVector.insert(staticToFill->TriangleListVector.end(), TRenderPair(renderParams, triangleList));
}

void TSquareStatic::StaticTextFiller(vec2 posFrom, vec2 posTo, TTextParams buttonTextParams, TSquareStatic* staticToFill)
{
	
	TRenderParams renderParams = buttonTextParams.RenderParams;

	TTriangleList triangleList;
	
	if (buttonTextParams.FontName == "")
	{
		buttonTextParams.FontName = ResourceManager->FontManager.GetCurrentFontName();
	}

	ResourceManager->FontManager.PushFont(buttonTextParams.FontName);

	triangleList = CreateTriangleListForText(posFrom, posTo, buttonTextParams);

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = ResourceManager->FontManager.GetCurrentFontTextureName();
	
	staticToFill->TriangleListVector.insert(staticToFill->TriangleListVector.end(),TRenderPair(renderParams, triangleList));
	triangleList.RefreshBuffer();

	ResourceManager->FontManager.PopFont();
	
}

TSquareStatic* TSquareStatic::CreateStaticTexture(vec2 posFrom, vec2 posTo, const std::string& texName)
{
	return CreateStaticWithFiller(posFrom, posTo, boost::bind(TSquareStatic::StaticTextureFiller, posFrom, posTo, texName, _1));

}

TSquareStatic* TSquareStatic::CreateStaticText(vec2 posFrom, vec2 posTo, TTextParams buttonTextParams)
{
	return CreateStaticWithFiller(posFrom, posTo, boost::bind(TSquareStatic::StaticTextFiller, posFrom, posTo, buttonTextParams, _1));
}




TTriangleList CreateTriangleListForText(vec2 posFrom, vec2 posTo, TTextParams buttonTextParams)
{
	TTriangleList triangleList;

	triangleList = ResourceManager->FontManager.DrawTextInBoxToVBO(posFrom, posTo, buttonTextParams.BasicTextAreaParams, buttonTextParams.Text);

	return triangleList;
}

//====================================================


TDynamicText::TDynamicText()
	: Width(0)
	, Height(0)
{
}

TDynamicText::~TDynamicText()
{
	if (SmartValueConnection.connected())
	{
		SmartValueConnection.disconnect();
	}
}

bool TDynamicText::CheckClick(vec2 mousePos)
{
	if (mousePos.v[0] >= LeftBottomPos.v[0] &&
		mousePos.v[0] <= LeftBottomPos.v[0] + Width &&
		mousePos.v[1] >= LeftBottomPos.v[1] &&
		mousePos.v[1] <= LeftBottomPos.v[1] + Height)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void TDynamicText::UpdateText(std::string text)
{
	if (LastTextParams.Text != text)
	{
		LastTextParams.Text = text;

		vec2 posFrom = LeftBottomPos;
		vec2 posTo = LeftBottomPos + vec2(Width, Height);

		
		PerformInMainThreadAsync(
		[this, posFrom, posTo]()
		{
			this->TextRenderPairItr->second = CreateTriangleListForText(posFrom, posTo, this->LastTextParams);
		}
		);
	}
}

TDynamicText* TDynamicText::CreateDynamicText(vec2 posFrom, vec2 posTo)
{
	TDynamicText* dynamicText = new TDynamicText();

	dynamicText->LeftBottomPos = posFrom;
	dynamicText->Width = posTo.v[0] - posFrom.v[0];
	dynamicText->Height = posTo.v[1] - posFrom.v[1];

	return dynamicText;
}

TDynamicText* TDynamicText::CreateDynamicTextWithFiller(vec2 posFrom, vec2 posTo, boost::function<void(TDynamicText*)> dynamicTextFillerFunc)
{
	TDynamicText* dynamicText = TDynamicText::CreateDynamicText(posFrom, posTo);

	dynamicTextFillerFunc(dynamicText);

	return dynamicText;
}

TDynamicText* TDynamicText::CreateDynamicTextWithFillers(vec2 posFrom, vec2 posTo, std::vector<boost::function<void(TDynamicText*)>> dynamicTextFillerFuncArr)
{
	TDynamicText* dynamicText = TDynamicText::CreateDynamicText(posFrom, posTo);

	BOOST_FOREACH(auto& func, dynamicTextFillerFuncArr)
	{
		func(dynamicText);
	}

	return dynamicText;
}

void TDynamicText::TextParamsFiller(vec2 posFrom, vec2 posTo, TTextParams textParams, TDynamicText* dynamicTextToFill)
{
	TRenderParams renderParams = textParams.RenderParams;

	TTriangleList triangleList;
	
	if (textParams.FontName == "")
	{
		textParams.FontName = ResourceManager->FontManager.GetCurrentFontName();
	}

	ResourceManager->FontManager.PushFont(textParams.FontName);

	triangleList = CreateTriangleListForText(posFrom, posTo, textParams);

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = ResourceManager->FontManager.GetCurrentFontTextureName();

	dynamicTextToFill->TextRenderPairItr = dynamicTextToFill->TriangleListVector.insert(dynamicTextToFill->TriangleListVector.end(),TRenderPair(renderParams, triangleList));
	triangleList.RefreshBuffer();

	ResourceManager->FontManager.PopFont();


	dynamicTextToFill->LastTextParams = textParams;
}

void TDynamicText::SmartValueTextFiller(const std::string& dictionaryName, const std::string& smartValueName, TDynamicText* dynamicTextToFill)
{

	dynamicTextToFill->UpdateText(ResourceManager->SmartValueManager.GetImmediateValue<std::string>(dictionaryName, smartValueName));
	
	dynamicTextToFill->SmartValueConnection = ResourceManager->SmartValueManager.GetSignal<std::string>(dictionaryName, smartValueName).connect(boost::bind(&TDynamicText::UpdateText, dynamicTextToFill, _1));

}


	



//=======================================================


TUniversalButton::TUniversalButton()
	: ButtonState(BS_NORMAL)
	, ButtonStateTimer(0.f)
{
}



void TUniversalButton::Update(cardinal dt)
{
	if (ButtonState == BS_GO_PRESSED)
	{
		ButtonStateTimer += dt;
		if (ButtonStateTimer >= CONST_BUTTON_HIGHLIGHT_TIME)
		{
			ButtonStateTimer = CONST_BUTTON_HIGHLIGHT_TIME;
			ButtonState = BS_PRESSED;
		}
	}
	else if (ButtonState == BS_GO_NORMAL)
	{
		ButtonStateTimer -= dt;
		if (ButtonStateTimer <= 0)
		{
			ButtonStateTimer = 0;
			ButtonState = BS_NORMAL;
		}
	}

	for (std::vector<TRenderPairList::iterator>::iterator i = PressedTextureIteratorArr.begin(); i != PressedTextureIteratorArr.end(); ++i)
	{
		(*i)->first.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = (ButtonStateTimer+0.f) / CONST_BUTTON_HIGHLIGHT_TIME;
	}


}


bool TUniversalButton::CheckClick(vec2 mousePos)
{
	if (mousePos.v[0] >= LeftBottomPos.v[0] &&
		mousePos.v[0] <= LeftBottomPos.v[0] + Width &&
		mousePos.v[1] >= LeftBottomPos.v[1] &&
		mousePos.v[1] <= LeftBottomPos.v[1] + Height)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void TUniversalButton::OnTapDown(vec2 pos)
{
	if (ButtonState == BS_NORMAL || ButtonState == BS_GO_NORMAL)
	{
		ButtonState = BS_GO_PRESSED;
	}

}

void TUniversalButton::OnTapUp(vec2 pos)
{
	if (ButtonState == BS_PRESSED || ButtonState == BS_GO_PRESSED)
	{
		ButtonState = BS_GO_NORMAL;
	}

}

void TUniversalButton::OnMoveOut()
{
	if (ButtonState == BS_PRESSED || ButtonState == BS_GO_PRESSED)
	{
		
		ButtonState = BS_GO_NORMAL;
	}
}

TUniversalButton* TUniversalButton::CreateButton(vec2 posFrom, vec2 posTo)
{
	TUniversalButton* button = new TUniversalButton();

	button->LeftBottomPos = posFrom;

	button->Width = posTo.v[0] - posFrom.v[0];

	button->Height = posTo.v[1] - posFrom.v[1];

	return button;
}

TUniversalButton* TUniversalButton::CreateButtonWithFiller(vec2 posFrom, vec2 posTo, boost::function<void(TUniversalButton*)> buttonFillerFunc)
{
	
	TUniversalButton* button = TUniversalButton::CreateButton(posFrom, posTo);

	buttonFillerFunc(button);

	return button;
}

TUniversalButton* TUniversalButton::CreateButtonWithFillers(vec2 posFrom, vec2 posTo, std::vector<boost::function<void(TUniversalButton*)> > buttonFillerFuncArr)
{
	TUniversalButton* button = TUniversalButton::CreateButton(posFrom, posTo);

	for (std::vector<boost::function<void(TUniversalButton*)> >::iterator funcItr = buttonFillerFuncArr.begin(); funcItr != buttonFillerFuncArr.end(); ++funcItr)
	{
		(*funcItr)(button);
	}

	return button;
}

void TUniversalButton::SquareButtonFiller(vec2 posFrom, vec2 posTo, const std::string& texName, const std::string& texNamePressed, TUniversalButton* buttonToFill)
{
	
	TRenderParams renderParams;

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = texName;
	renderParams.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 1.f;

	TTriangleList triangleList = MakeTriangleList(posFrom, posTo);

	triangleList.RefreshBuffer();

	buttonToFill->NormalTextureIteratorArr.push_back(buttonToFill->TriangleListVector.insert(buttonToFill->TriangleListVector.end(), TRenderPair(renderParams, triangleList)));

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = texNamePressed;
	renderParams.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 0.f;

	buttonToFill->PressedTextureIteratorArr.push_back(buttonToFill->TriangleListVector.insert(buttonToFill->TriangleListVector.end(),TRenderPair(renderParams, triangleList)));

}


void TUniversalButton::TextButtonFiller(vec2 posFrom, vec2 posTo, TTextParams buttonTextParamsNormal, TTextParams buttonTextParamsPressed, TUniversalButton* buttonToFill)
{


	
	TTriangleList triangleList;
	
	if (buttonTextParamsNormal.FontName == "")
	{
		buttonTextParamsNormal.FontName = ResourceManager->FontManager.GetCurrentFontName();
	}

	if (buttonTextParamsPressed.FontName == "")
	{
		buttonTextParamsPressed.FontName = ResourceManager->FontManager.GetCurrentFontName();
	}



	TRenderParams renderParams = buttonTextParamsNormal.RenderParams;

	ResourceManager->FontManager.PushFont(buttonTextParamsNormal.FontName);

	triangleList = CreateTriangleListForText(posFrom, posTo, buttonTextParamsNormal);

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = ResourceManager->FontManager.GetCurrentFontTextureName();
	renderParams.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 1.f;


	buttonToFill->NormalTextureIteratorArr.push_back(buttonToFill->TriangleListVector.insert(buttonToFill->TriangleListVector.end(),TRenderPair(renderParams, triangleList)));
	triangleList.RefreshBuffer();
	ResourceManager->FontManager.PopFont();
	

	renderParams = buttonTextParamsPressed.RenderParams;

	ResourceManager->FontManager.PushFont(buttonTextParamsPressed.FontName);

	triangleList = CreateTriangleListForText(posFrom, posTo, buttonTextParamsPressed);

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = ResourceManager->FontManager.GetCurrentFontTextureName();
	renderParams.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 0.f;


	buttonToFill->PressedTextureIteratorArr.push_back(buttonToFill->TriangleListVector.insert(buttonToFill->TriangleListVector.end(),TRenderPair(renderParams, triangleList)));
	triangleList.RefreshBuffer();
	ResourceManager->FontManager.PopFont();

}

TUniversalButton* TUniversalButton::CreateSquareButton(vec2 posFrom, vec2 posTo, const std::string& texName, const std::string& texNamePressed)
{
	return CreateButtonWithFiller(posFrom, posTo, boost::bind(TUniversalButton::SquareButtonFiller, posFrom, posTo, texName, texNamePressed, _1));
}

TUniversalButton* TUniversalButton::CreateTextOnlyButton(vec2 posFrom, vec2 posTo, TTextParams buttonTextParamsNormal, TTextParams buttonTextParamsPressed)
{
	return CreateButtonWithFiller(posFrom, posTo, boost::bind(TUniversalButton::TextButtonFiller, posFrom, posTo, buttonTextParamsNormal, buttonTextParamsPressed, _1));
}

TUniversalButton* TUniversalButton::CreateSquareAndTextButton(vec2 posFrom, vec2 posTo, const std::string& texName, const std::string& texNamePressed, TTextParams buttonTextParamsNormal, TTextParams buttonTextParamsPressed)
{

	std::vector<boost::function<void(TUniversalButton*)> > funcIter;
	
	funcIter.push_back(boost::bind(TUniversalButton::SquareButtonFiller, posFrom, posTo, texName, texNamePressed, _1));
	funcIter.push_back(boost::bind(TUniversalButton::TextButtonFiller, posFrom, posTo, buttonTextParamsNormal, buttonTextParamsPressed, _1));
	
	TUniversalButton* universalButton = CreateButtonWithFillers(posFrom, posTo, funcIter);
	
	return universalButton;
}


TEdit* TEdit::CreateEdit(vec2 posFrom, vec2 posTo)
{
	TEdit* edit = new TEdit();

	edit->LeftBottomPos = posFrom;

	edit->Width = posTo.v[0] - posFrom.v[0];

	edit->Height = posTo.v[1] - posFrom.v[1];

	return edit;
}

TEdit* TEdit::CreateEditWithFiller(vec2 posFrom, vec2 posTo, boost::function<void(TEdit*)> fillerFunc)
{
	TEdit* edit = CreateEdit(posFrom, posTo);

	fillerFunc(edit);

	return edit;
}

TEdit* TEdit::CreateEditWithFillers(vec2 posFrom, vec2 posTo, std::vector<boost::function<void(TEdit*)> > fillerFuncArr)
{
	TEdit* edit = CreateEdit(posFrom, posTo);

	BOOST_FOREACH(auto& f, fillerFuncArr)
	{
		f(edit);
	}

	return edit;
}

void TEdit::EditFiller(vec2 posFrom, vec2 posTo, const std::string& texName, TTextParams textParams, TEdit* edit)
{

	TRenderParams renderParams;

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = texName;
	renderParams.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 1.f;
	
	TTriangleList triangleList = MakeTriangleList(posFrom, posTo);

	triangleList.RefreshBuffer();

	edit->TriangleListVector.insert(edit->TriangleListVector.end(), TRenderPair(renderParams, triangleList));


	renderParams = textParams.RenderParams;

	if (textParams.FontName == "")
	{
		textParams.FontName = ResourceManager->FontManager.GetCurrentFontName();
	}

	ResourceManager->FontManager.PushFont(textParams.FontName);

	TTriangleList textTriangleList = CreateTriangleListForText(posFrom, posTo, textParams);

	textTriangleList.RefreshBuffer();

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = ResourceManager->FontManager.GetCurrentFontTextureName();

	edit->TextIterator = (edit->TriangleListVector.insert(edit->TriangleListVector.end(),TRenderPair(renderParams, textTriangleList)));
	
	ResourceManager->FontManager.PopFont();

	edit->TextParams = textParams;


}

void TEdit::SmartValueTextFiller(const std::string& dictionaryName, const std::string& smartValueName, TEdit* edit)
{
	edit->SetText(ResourceManager->SmartValueManager.GetImmediateValue<std::string>(dictionaryName, smartValueName));
	
	edit->SetTextSlotConnection = ResourceManager->SmartValueManager.GetSignal<std::string>(dictionaryName, smartValueName).connect(boost::bind(&TEdit::SetText, edit, _1));

	edit->OnTextChanged.connect
	(
		[dictionaryName, smartValueName](std::string text)
		{
			if (ResourceManager->SmartValueManager.GetImmediateValue<std::string>(dictionaryName, smartValueName) != text)
			{
				ResourceManager->SmartValueManager.SetImmediateValue<std::string>(dictionaryName, smartValueName, text);
			}
		}
	);
}

/*
TEdit* TEdit::CreateEdit(vec2 posFrom, vec2 posTo, const std::string& texName, TTextParams textParams)
{
	

	//Filler:

	TRenderParams renderParams = textParams.RenderParams;

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = texName;
	
	TTriangleList triangleList = MakeTriangleList(posFrom, posTo);

	triangleList.RefreshBuffer();

	edit->TriangleListVector.insert(edit->TriangleListVector.end(), TRenderPair(renderParams, triangleList));


	if (textParams.FontName == "")
	{
		textParams.FontName = ResourceManager->FontManager.GetCurrentFontName();
	}

	ResourceManager->FontManager.PushFont(textParams.FontName);

	triangleList = CreateTriangleListForText(posFrom, posTo, textParams);

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = ResourceManager->FontManager.GetCurrentFontTextureName();

	edit->TextIterator = (edit->TriangleListVector.insert(edit->TriangleListVector.end(),TRenderPair(renderParams, triangleList)));
	triangleList.RefreshBuffer();
	ResourceManager->FontManager.PopFont();

	return edit;
}*/

TEdit::~TEdit()
{
	if (InputConnection.connected())
	{
		InputConnection.disconnect();
	}

	if (SetTextSlotConnection.connected())
	{
		SetTextSlotConnection.disconnect();
	}
}

void TEdit::RefreshTextTriangleList()
{
	TTriangleList triangleList = CreateTriangleListForText(LeftBottomPos, LeftBottomPos + vec2(Width, Height), TextParams);

	TextIterator->second = triangleList;

}

bool TEdit::CheckClick(vec2 mousePos)
{
	if (mousePos.v[0] >= LeftBottomPos.v[0] &&
		mousePos.v[0] <= LeftBottomPos.v[0] + Width &&
		mousePos.v[1] >= LeftBottomPos.v[1] &&
		mousePos.v[1] <= LeftBottomPos.v[1] + Height)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void TEdit::OnTapDown(vec2 pos)
{
	if (!InputConnection.connected())
	{
		ResourceManager->GUIManager.KeyPressedSignal.disconnect_all_slots();
		InputConnection = ResourceManager->GUIManager.KeyPressedSignal.connect(boost::bind(&TEdit::OnKeyPress, this, _1));
	}
}

void TEdit::OnKeyPress(int key)
{
	if (key == 8) //Backspace
	{
		if (TextParams.Text.size() > 0)
		{
			TextParams.Text.erase(TextParams.Text.end()-1);
		}
	}
	else if (key >= 32 && key <= 255) //ASCII
	{
		TextParams.Text += static_cast<char>(key);
	}

	OnTextChanged(TextParams.Text);

	RefreshTextTriangleList();
}

void TEdit::SetText(const std::string& newText)
{
	if (newText != TextParams.Text)
	{
		TextParams.Text = newText;

		OnTextChanged(TextParams.Text);

		RefreshTextTriangleList();
	}
}


//============================ Check Box ============================


TCheckBox::TCheckBox()
	: IsChecked(false)
	, Width(0.f)
	, Height(0.f)
{
}

TCheckBox::~TCheckBox()
{

	if (SetCheckedSlotConnection.connected())
	{
		SetCheckedSlotConnection.disconnect();
	}
}

void TCheckBox::SetChecked(bool isChecked)
{

	if (isChecked == IsChecked)
	{
		return; //This prevent recursion
	}


		if (!IsChecked)
		{
			IsChecked = true;

			OnSetChecked(IsChecked);

			RefreshVisibility();
		}
		else
		{
			IsChecked = false;

			OnSetChecked(IsChecked);

			RefreshVisibility();
		}
}

void TCheckBox::RefreshVisibility()
{

	//IsChecked
	if (IsChecked)
	{
		for (std::vector<TRenderPairList::iterator>::iterator i = CheckedTextureIteratorArr.begin(); i != CheckedTextureIteratorArr.end(); ++i)
		{
			(*i)->first.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 1.f;
		}
	}
	else
	{
		for (std::vector<TRenderPairList::iterator>::iterator i = CheckedTextureIteratorArr.begin(); i != CheckedTextureIteratorArr.end(); ++i)
		{
			(*i)->first.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 0.f;
		}
	}
}

bool TCheckBox::CheckClick(vec2 mousePos)
{
	if (mousePos.v[0] >= LeftBottomPos.v[0] &&
		mousePos.v[0] <= LeftBottomPos.v[0] + Width &&
		mousePos.v[1] >= LeftBottomPos.v[1] &&
		mousePos.v[1] <= LeftBottomPos.v[1] + Height)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void TCheckBox::Update(cardinal dt)
{
}

void TCheckBox::OnTapDown(vec2 pos)
{
}

void TCheckBox::OnTapUp(vec2 pos)
{
	SetChecked(!IsChecked);
}

void TCheckBox::OnMoveOut()
{
}


TCheckBox* TCheckBox::CreateCheckBox(vec2 posFrom, vec2 posTo)
{
	TCheckBox* checkBox = new TCheckBox();

	checkBox->LeftBottomPos = posFrom;

	checkBox->Width = posTo.v[0] - posFrom.v[0];

	checkBox->Height = posTo.v[1] - posFrom.v[1];

	return checkBox;
}

TCheckBox* TCheckBox::CreateCheckBoxWithFiller(vec2 posFrom, vec2 posTo, boost::function<void(TCheckBox*)> checkBoxFillerFunc)
{
	TCheckBox* checkBox = TCheckBox::CreateCheckBox(posFrom, posTo);

	checkBoxFillerFunc(checkBox);

	return checkBox;
}

TCheckBox* TCheckBox::CreateCheckBoxWithFillers(vec2 posFrom, vec2 posTo, std::vector<boost::function<void(TCheckBox*)> > checkBoxFillerFuncArr)
{
	TCheckBox* checkBox = TCheckBox::CreateCheckBox(posFrom, posTo);

	BOOST_FOREACH(boost::function<void(TCheckBox*)>& fillerFunc, checkBoxFillerFuncArr)
	{
		fillerFunc(checkBox);
	}

	return checkBox;
}

void TCheckBox::TextureFiller(vec2 posFrom, vec2 posTo, const std::string& texName, const std::string& texNameChecked, TCheckBox* checkBoxToFill)
{

	TRenderParams renderParams;

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = texName;
	renderParams.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 1.f;

	TTriangleList triangleList = MakeTriangleList(posFrom, posTo);

	triangleList.RefreshBuffer();


	checkBoxToFill->StaticTextureIteratorArr.push_back(checkBoxToFill->TriangleListVector.insert(checkBoxToFill->TriangleListVector.end(), TRenderPair(renderParams, triangleList)));

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = texNameChecked;
	renderParams.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 0.f;

	checkBoxToFill->CheckedTextureIteratorArr.push_back(checkBoxToFill->TriangleListVector.insert(checkBoxToFill->TriangleListVector.end(),TRenderPair(renderParams, triangleList)));

}

void TCheckBox::TextFiller(vec2 posFrom, vec2 posTo, TTextParams textParams, TCheckBox* checkBoxToFill)
{
	
	TRenderParams renderParams = textParams.RenderParams;

	TTriangleList triangleList;

	if (textParams.FontName == "")
	{
		textParams.FontName = ResourceManager->FontManager.GetCurrentFontName();
	}

	ResourceManager->FontManager.PushFont(textParams.FontName);

	triangleList = CreateTriangleListForText(posFrom, posTo, textParams);

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = ResourceManager->FontManager.GetCurrentFontTextureName();

	checkBoxToFill->StaticTextureIteratorArr.push_back(checkBoxToFill->TriangleListVector.insert(checkBoxToFill->TriangleListVector.end(),TRenderPair(renderParams, triangleList)));
	triangleList.RefreshBuffer();
	ResourceManager->FontManager.PopFont();

}

void TCheckBox::CheckedFiller(bool isChecked, TCheckBox* checkBoxToFill)
{
	checkBoxToFill->SetChecked(isChecked);
}

void TCheckBox::SmartValueFiller(const std::string& dictionaryName, const std::string& smartValueName, TCheckBox* checkBoxToFill)
{

	checkBoxToFill->SetChecked(ResourceManager->SmartValueManager.GetImmediateValue<bool>(dictionaryName, smartValueName));
	
	checkBoxToFill->SetCheckedSlotConnection = ResourceManager->SmartValueManager.GetSignal<bool>(dictionaryName, smartValueName).connect(boost::bind(&TCheckBox::SetChecked, checkBoxToFill, _1));
    
	checkBoxToFill->OnSetChecked.connect
	(
		[dictionaryName, smartValueName](bool checked)
		{
			if (ResourceManager->SmartValueManager.GetImmediateValue<bool>(dictionaryName, smartValueName) != checked)
			{
				ResourceManager->SmartValueManager.SetImmediateValue<bool>(dictionaryName, smartValueName, checked);
			}
		}
	);
}


TCheckBox* TCheckBox::CreateSimpleCheckBox(vec2 posFrom, vec2 posTo, const std::string& texName, const std::string& texNameChecked)
{
	return CreateCheckBoxWithFiller(posFrom, posTo, boost::bind(&TCheckBox::TextureFiller, posFrom, posTo, texName, texNameChecked, _1));
}

TCheckBox* TCheckBox::CreateCheckBoxWithText(vec2 posFrom, vec2 posTo, const std::string& texName, const std::string& texNameChecked, vec2 textPosFrom, vec2 textPosTo, TTextParams textParams)
{
	TCheckBox* checkBox = CreateCheckBox(posFrom, posTo);

	TCheckBox::TextureFiller(posFrom, posTo, texName, texNameChecked, checkBox);
	TCheckBox::TextFiller(textPosFrom, textPosTo, textParams, checkBox);

	return checkBox;
}


//============================ Radio Button ============================


TRadioButton::TRadioButton()
	: IsChecked(false)
	, Width(0.f)
	, Height(0.f)
{
}

TRadioButton::~TRadioButton()
{

	if (SetCheckedSlotConnection.connected())
	{
		SetCheckedSlotConnection.disconnect();
	}
}

void TRadioButton::SetChecked(bool isChecked)
{

	if (isChecked == IsChecked)
	{
		return; //This prevent recursion
	}

	IsChecked = isChecked;

	OnSetChecked(IsChecked);

	RefreshVisibility();
}

void TRadioButton::RefreshVisibility()
{

	//IsChecked
	if (IsChecked)
	{
		for (std::vector<TRenderPairList::iterator>::iterator i = CheckedTextureIteratorArr.begin(); i != CheckedTextureIteratorArr.end(); ++i)
		{
			(*i)->first.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 1.f;
		}
	}
	else
	{
		for (std::vector<TRenderPairList::iterator>::iterator i = CheckedTextureIteratorArr.begin(); i != CheckedTextureIteratorArr.end(); ++i)
		{
			(*i)->first.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 0.f;
		}
	}
}

bool TRadioButton::CheckClick(vec2 mousePos)
{
	if (mousePos.v[0] >= LeftBottomPos.v[0] &&
		mousePos.v[0] <= LeftBottomPos.v[0] + Width &&
		mousePos.v[1] >= LeftBottomPos.v[1] &&
		mousePos.v[1] <= LeftBottomPos.v[1] + Height)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void TRadioButton::Update(cardinal dt)
{
}

void TRadioButton::OnTapDown(vec2 pos)
{
}

void TRadioButton::OnTapUp(vec2 pos)
{
	SetChecked(true);
}

void TRadioButton::OnMoveOut()
{
}


TRadioButton* TRadioButton::CreateRadioButton(vec2 posFrom, vec2 posTo)
{
	TRadioButton* radioButton = new TRadioButton();

	radioButton->LeftBottomPos = posFrom;

	radioButton->Width = posTo.v[0] - posFrom.v[0];

	radioButton->Height = posTo.v[1] - posFrom.v[1];

	return radioButton;
}

TRadioButton* TRadioButton::CreateRadioButtonWithFiller(vec2 posFrom, vec2 posTo, boost::function<void(TRadioButton*)> radioButtonFillerFunc)
{
	TRadioButton* radioButton = TRadioButton::CreateRadioButton(posFrom, posTo);

	radioButtonFillerFunc(radioButton);

	return radioButton;
}

TRadioButton* TRadioButton::CreateRadioButtonWithFillers(vec2 posFrom, vec2 posTo, std::vector<boost::function<void(TRadioButton*)> > radioButtonFillerFuncArr)
{
	TRadioButton* radioButton = TRadioButton::CreateRadioButton(posFrom, posTo);

	BOOST_FOREACH(boost::function<void(TRadioButton*)>& fillerFunc, radioButtonFillerFuncArr)
	{
		fillerFunc(radioButton);
	}

	return radioButton;
}

void TRadioButton::TextureFiller(vec2 posFrom, vec2 posTo, const std::string& texName, const std::string& texNameChecked, TRadioButton* radioButtonToFill)
{

	TRenderParams renderParams;

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = texName;
	renderParams.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 1.f;

	TTriangleList triangleList = MakeTriangleList(posFrom, posTo);

	triangleList.RefreshBuffer();


	radioButtonToFill->StaticTextureIteratorArr.push_back(radioButtonToFill->TriangleListVector.insert(radioButtonToFill->TriangleListVector.end(), TRenderPair(renderParams, triangleList)));

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = texNameChecked;
	renderParams.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 0.f;

	radioButtonToFill->CheckedTextureIteratorArr.push_back(radioButtonToFill->TriangleListVector.insert(radioButtonToFill->TriangleListVector.end(),TRenderPair(renderParams, triangleList)));

}

void TRadioButton::TextFiller(vec2 posFrom, vec2 posTo, TTextParams textParams, TRadioButton* radioButtonToFill)
{
	
	TRenderParams renderParams = textParams.RenderParams;

	TTriangleList triangleList;

	if (textParams.FontName == "")
	{
		textParams.FontName = ResourceManager->FontManager.GetCurrentFontName();
	}

	ResourceManager->FontManager.PushFont(textParams.FontName);

	triangleList = CreateTriangleListForText(posFrom, posTo, textParams);

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = ResourceManager->FontManager.GetCurrentFontTextureName();
	renderParams.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 1.f;


	radioButtonToFill->StaticTextureIteratorArr.push_back(radioButtonToFill->TriangleListVector.insert(radioButtonToFill->TriangleListVector.end(),TRenderPair(renderParams, triangleList)));
	triangleList.RefreshBuffer();
	ResourceManager->FontManager.PopFont();

}

void TRadioButton::CheckedFiller(bool isChecked, TRadioButton* radioButtonToFill)
{
	radioButtonToFill->SetChecked(isChecked);
}

TRadioButton* TRadioButton::CreateSimpleRadioButton(vec2 posFrom, vec2 posTo, const std::string& texName, const std::string& texNameChecked)
{
	return CreateRadioButtonWithFiller(posFrom, posTo, boost::bind(&TRadioButton::TextureFiller, posFrom, posTo, texName, texNameChecked, _1));
}

TRadioButton* TRadioButton::CreateRadioButtonWithText(vec2 posFrom, vec2 posTo, const std::string& texName, const std::string& texNameChecked, vec2 textPosFrom, vec2 textPosTo, TTextParams textParams)
{
	TRadioButton* radioButton = CreateRadioButton(posFrom, posTo);

	TRadioButton::TextureFiller(posFrom, posTo, texName, texNameChecked, radioButton);
	TRadioButton::TextFiller(textPosFrom, textPosTo, textParams, radioButton);

	return radioButton;
}




//====================== Mover =====================





TMover::TMover()
	: Width(0)
	, Height(0)
	, PosCursor(0)
{
}


TMover::~TMover()
{
}

void TMover::Update(cardinal dt)
{
}

bool TMover::CheckClick(vec2 mousePos)
{
	if (mousePos.v[0] >= LeftBottomPos.v[0] &&
		mousePos.v[0] <= LeftBottomPos.v[0] + Width &&
		mousePos.v[1] >= LeftBottomPos.v[1] &&
		mousePos.v[1] <= LeftBottomPos.v[1] + Height)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void TMover::OnTapDown(vec2 pos)
{
}

void TMover::OnTapUp(vec2 pos)
{
}

void TMover::OnMove(vec2 shift)
{
	float oldPosCursor = PosCursor;
	PosCursor -= shift.v[1];

	if (PosCursor < 0)
	{
		PosCursor = 0;
	}

	if (PosCursor > MaxHeight - Height)
	{
		PosCursor = MaxHeight - Height;
	}

	float shift_y = PosCursor - oldPosCursor;

	if (shift_y != 0)
	{
	ResourceManager->GUIManager.MoveWidgetGroup(GroupName, SelfName, vec2(0, shift_y));
	}
}

void TMover::OnMoveOut()
{
}


TMover* TMover::CreateMover(vec2 posFrom, vec2 posTo, std::string groupName, std::string selfName, float maxHeight)
{
	TMover* mover = new TMover();

	mover->LeftBottomPos = posFrom;

	mover->Width = posTo.v[0] - posFrom.v[0];

	mover->Height = posTo.v[1] - posFrom.v[1];

	mover->GroupName = groupName;

	mover->SelfName = selfName;

	mover->MaxHeight = maxHeight;

	return mover;
}

} //namespace SE