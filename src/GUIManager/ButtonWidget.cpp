#include "include/Engine.h"

namespace SE
{

const size_t CONST_BUTTON_HIGHLIGHT_TIME = 100;

TInstancingWidgetAncestor::~TInstancingWidgetAncestor() 
{
}


void TInstancingWidgetAncestor::Update(size_t dt) 
{
}

bool TInstancingWidgetAncestor::CheckClick(Vector2f mousePos) 
{
	return false;
}


bool TSquareStatic::CheckClick(Vector2f mousePos)
{
	if (mousePos(0) >= LeftBottomPos(0) &&
		mousePos(0) <= LeftBottomPos(0) + Width &&
		mousePos(1) >= LeftBottomPos(1) &&
		mousePos(1) <= LeftBottomPos(1) + Height)
	{
		return true;
	}
	else
	{
		return false;
	}
}

TSquareStatic* TSquareStatic::CreateStatic(Vector2f posFrom, Vector2f posTo)
{
	TSquareStatic* staticWidget = new TSquareStatic();

	staticWidget->LeftBottomPos = posFrom;
	staticWidget->Width = posTo(0) - posFrom(0);
	staticWidget->Height = posTo(1) - posFrom(1);

	return staticWidget;

}


TSquareStatic* TSquareStatic::CreateStaticWithFiller(Vector2f posFrom, Vector2f posTo, boost::function<void(TSquareStatic*)> staticFillerFunc)
{
	TSquareStatic* staticWidget = TSquareStatic::CreateStatic(posFrom, posTo);

	staticFillerFunc(staticWidget);

	return staticWidget;
}


TSquareStatic* TSquareStatic::CreateStaticWithFillers(Vector2f posFrom, Vector2f posTo, std::vector<boost::function<void(TSquareStatic*)>> staticFillerFuncArr)
{
	TSquareStatic* staticWidget = TSquareStatic::CreateStatic(posFrom, posTo);

	BOOST_FOREACH(auto& func, staticFillerFuncArr)
	{
		func(staticWidget);
	}

	return staticWidget;
}


void TSquareStatic::StaticTextureFiller(Vector2f posFrom, Vector2f posTo, const std::string& texName, TSquareStatic* staticToFill)
{
	
	TRenderParams renderParams;

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = texName;
	renderParams.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 1.f;

	TTriangleList triangleList = MakeTriangleList(posFrom, posTo);

	triangleList.RefreshBuffer();

	staticToFill->TriangleListVector.insert(staticToFill->TriangleListVector.end(), TRenderPair(renderParams, triangleList));
}

void TSquareStatic::StaticTextFiller(Vector2f posFrom, Vector2f posTo, TTextParams buttonTextParams, TSquareStatic* staticToFill)
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

TSquareStatic* TSquareStatic::CreateStaticTexture(Vector2f posFrom, Vector2f posTo, const std::string& texName)
{
	return CreateStaticWithFiller(posFrom, posTo, boost::bind(TSquareStatic::StaticTextureFiller, posFrom, posTo, texName, _1));

}

TSquareStatic* TSquareStatic::CreateStaticText(Vector2f posFrom, Vector2f posTo, TTextParams buttonTextParams)
{
	return CreateStaticWithFiller(posFrom, posTo, boost::bind(TSquareStatic::StaticTextFiller, posFrom, posTo, buttonTextParams, _1));
}




TTriangleList CreateTriangleListForText(Vector2f posFrom, Vector2f posTo, TTextParams buttonTextParams)
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

bool TDynamicText::CheckClick(Vector2f mousePos)
{
	if (mousePos(0) >= LeftBottomPos(0) &&
		mousePos(0) <= LeftBottomPos(0) + Width &&
		mousePos(1) >= LeftBottomPos(1) &&
		mousePos(1) <= LeftBottomPos(1) + Height)
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

		Vector2f posFrom = LeftBottomPos;
		Vector2f posTo = LeftBottomPos + Vector2f(Width, Height);

		
		PerformInMainThreadAsync(
		[this, posFrom, posTo]()
		{
			this->TextRenderPairItr->second = CreateTriangleListForText(posFrom, posTo, this->LastTextParams);
		}
		);
	}
}

TDynamicText* TDynamicText::CreateDynamicText(Vector2f posFrom, Vector2f posTo)
{
	TDynamicText* dynamicText = new TDynamicText();

	dynamicText->LeftBottomPos = posFrom;
	dynamicText->Width = posTo(0) - posFrom(0);
	dynamicText->Height = posTo(1) - posFrom(1);

	return dynamicText;
}

TDynamicText* TDynamicText::CreateDynamicTextWithFiller(Vector2f posFrom, Vector2f posTo, boost::function<void(TDynamicText*)> dynamicTextFillerFunc)
{
	TDynamicText* dynamicText = TDynamicText::CreateDynamicText(posFrom, posTo);

	dynamicTextFillerFunc(dynamicText);

	return dynamicText;
}

TDynamicText* TDynamicText::CreateDynamicTextWithFillers(Vector2f posFrom, Vector2f posTo, std::vector<boost::function<void(TDynamicText*)>> dynamicTextFillerFuncArr)
{
	TDynamicText* dynamicText = TDynamicText::CreateDynamicText(posFrom, posTo);

	BOOST_FOREACH(auto& func, dynamicTextFillerFuncArr)
	{
		func(dynamicText);
	}

	return dynamicText;
}

void TDynamicText::TextParamsFiller(Vector2f posFrom, Vector2f posTo, TTextParams textParams, TDynamicText* dynamicTextToFill)
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



void TUniversalButton::Update(size_t dt)
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


bool TUniversalButton::CheckClick(Vector2f mousePos)
{
	if (mousePos(0) >= LeftBottomPos(0) &&
		mousePos(0) <= LeftBottomPos(0) + Width &&
		mousePos(1) >= LeftBottomPos(1) &&
		mousePos(1) <= LeftBottomPos(1) + Height)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void TUniversalButton::OnTapDown(Vector2f pos)
{
	if (ButtonState == BS_NORMAL || ButtonState == BS_GO_NORMAL)
	{
		ButtonState = BS_GO_PRESSED;
	}

}

void TUniversalButton::OnTapUp(Vector2f pos)
{
	if (ButtonState == BS_PRESSED || ButtonState == BS_GO_PRESSED)
	{
		ButtonState = BS_GO_NORMAL;
	}

}
    
void TUniversalButton::OnTapUpAfterMove(Vector2f pos)
{
    OnTapUp(pos);
}

void TUniversalButton::OnMoveOut()
{
	if (ButtonState == BS_PRESSED || ButtonState == BS_GO_PRESSED)
	{
		
		ButtonState = BS_GO_NORMAL;
	}
}

TUniversalButton* TUniversalButton::CreateButton(Vector2f posFrom, Vector2f posTo)
{
	TUniversalButton* button = new TUniversalButton();

	button->LeftBottomPos = posFrom;

	button->Width = posTo(0) - posFrom(0);

	button->Height = posTo(1) - posFrom(1);

	return button;
}

TUniversalButton* TUniversalButton::CreateButtonWithFiller(Vector2f posFrom, Vector2f posTo, boost::function<void(TUniversalButton*)> buttonFillerFunc)
{
	
	TUniversalButton* button = TUniversalButton::CreateButton(posFrom, posTo);

	buttonFillerFunc(button);

	return button;
}

TUniversalButton* TUniversalButton::CreateButtonWithFillers(Vector2f posFrom, Vector2f posTo, std::vector<boost::function<void(TUniversalButton*)> > buttonFillerFuncArr)
{
	TUniversalButton* button = TUniversalButton::CreateButton(posFrom, posTo);

	for (std::vector<boost::function<void(TUniversalButton*)> >::iterator funcItr = buttonFillerFuncArr.begin(); funcItr != buttonFillerFuncArr.end(); ++funcItr)
	{
		(*funcItr)(button);
	}

	return button;
}

void TUniversalButton::SquareButtonFiller(Vector2f posFrom, Vector2f posTo, const std::string& texName, const std::string& texNamePressed, TUniversalButton* buttonToFill)
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


void TUniversalButton::TextButtonFiller(Vector2f posFrom, Vector2f posTo, TTextParams buttonTextParamsNormal, TTextParams buttonTextParamsPressed, TUniversalButton* buttonToFill)
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

TUniversalButton* TUniversalButton::CreateSquareButton(Vector2f posFrom, Vector2f posTo, const std::string& texName, const std::string& texNamePressed)
{
	return CreateButtonWithFiller(posFrom, posTo, boost::bind(TUniversalButton::SquareButtonFiller, posFrom, posTo, texName, texNamePressed, _1));
}

TUniversalButton* TUniversalButton::CreateTextOnlyButton(Vector2f posFrom, Vector2f posTo, TTextParams buttonTextParamsNormal, TTextParams buttonTextParamsPressed)
{
	return CreateButtonWithFiller(posFrom, posTo, boost::bind(TUniversalButton::TextButtonFiller, posFrom, posTo, buttonTextParamsNormal, buttonTextParamsPressed, _1));
}

TUniversalButton* TUniversalButton::CreateSquareAndTextButton(Vector2f posFrom, Vector2f posTo, const std::string& texName, const std::string& texNamePressed, TTextParams buttonTextParamsNormal, TTextParams buttonTextParamsPressed)
{

	std::vector<boost::function<void(TUniversalButton*)> > funcIter;
	
	funcIter.push_back(boost::bind(TUniversalButton::SquareButtonFiller, posFrom, posTo, texName, texNamePressed, _1));
	funcIter.push_back(boost::bind(TUniversalButton::TextButtonFiller, posFrom, posTo, buttonTextParamsNormal, buttonTextParamsPressed, _1));
	
	TUniversalButton* universalButton = CreateButtonWithFillers(posFrom, posTo, funcIter);
	
	return universalButton;
}


TEdit* TEdit::CreateEdit(Vector2f posFrom, Vector2f posTo)
{
	TEdit* edit = new TEdit();

	edit->LeftBottomPos = posFrom;

	edit->Width = posTo(0) - posFrom(0);

	edit->Height = posTo(1) - posFrom(1);

	return edit;
}

TEdit* TEdit::CreateEditWithFiller(Vector2f posFrom, Vector2f posTo, boost::function<void(TEdit*)> fillerFunc)
{
	TEdit* edit = CreateEdit(posFrom, posTo);

	fillerFunc(edit);

	return edit;
}

TEdit* TEdit::CreateEditWithFillers(Vector2f posFrom, Vector2f posTo, std::vector<boost::function<void(TEdit*)> > fillerFuncArr)
{
	TEdit* edit = CreateEdit(posFrom, posTo);

	BOOST_FOREACH(auto& f, fillerFuncArr)
	{
		f(edit);
	}

	return edit;
}

void TEdit::EditFiller(Vector2f posFrom, Vector2f posTo, const std::string& texName, TTextParams textParams, TEdit* edit)
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
TEdit* TEdit::CreateEdit(Vector2f posFrom, Vector2f posTo, const std::string& texName, TTextParams textParams)
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
	TTriangleList triangleList = CreateTriangleListForText(LeftBottomPos, LeftBottomPos + Vector2f(Width, Height), TextParams);

	TextIterator->second = triangleList;

}

bool TEdit::CheckClick(Vector2f mousePos)
{
	if (mousePos(0) >= LeftBottomPos(0) &&
		mousePos(0) <= LeftBottomPos(0) + Width &&
		mousePos(1) >= LeftBottomPos(1) &&
		mousePos(1) <= LeftBottomPos(1) + Height)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void TEdit::OnTapDown(Vector2f pos)
{
	if (!InputConnection.connected())
	{
		ResourceManager->GUIManager.KeyPressedSignal.disconnect_all_slots();
		InputConnection = ResourceManager->GUIManager.KeyPressedSignal.connect(boost::bind(&TEdit::OnKeyPress, this, _1));

	}

	ResourceManager->GUIManager.ShowKeyboard(GetText());
    ResourceManager->GUIManager.SetTextSignal.connect(boost::bind(&TEdit::OnSetAllText, this, _1));
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
    
void TEdit::OnSetAllText(std::string newText)
{
    SetText(newText);
}
    
    
std::string TEdit::GetText()
{
    return TextParams.Text;
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

bool TCheckBox::CheckClick(Vector2f mousePos)
{
	if (mousePos(0) >= LeftBottomPos(0) &&
		mousePos(0) <= LeftBottomPos(0) + Width &&
		mousePos(1) >= LeftBottomPos(1) &&
		mousePos(1) <= LeftBottomPos(1) + Height)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void TCheckBox::Update(size_t dt)
{
}

void TCheckBox::OnTapDown(Vector2f pos)
{
}

void TCheckBox::OnTapUp(Vector2f pos)
{
	SetChecked(!IsChecked);
}

void TCheckBox::OnMoveOut()
{
}


TCheckBox* TCheckBox::CreateCheckBox(Vector2f posFrom, Vector2f posTo)
{
	TCheckBox* checkBox = new TCheckBox();

	checkBox->LeftBottomPos = posFrom;

	checkBox->Width = posTo(0) - posFrom(0);

	checkBox->Height = posTo(1) - posFrom(1);

	return checkBox;
}

TCheckBox* TCheckBox::CreateCheckBoxWithFiller(Vector2f posFrom, Vector2f posTo, boost::function<void(TCheckBox*)> checkBoxFillerFunc)
{
	TCheckBox* checkBox = TCheckBox::CreateCheckBox(posFrom, posTo);

	checkBoxFillerFunc(checkBox);

	return checkBox;
}

TCheckBox* TCheckBox::CreateCheckBoxWithFillers(Vector2f posFrom, Vector2f posTo, std::vector<boost::function<void(TCheckBox*)> > checkBoxFillerFuncArr)
{
	TCheckBox* checkBox = TCheckBox::CreateCheckBox(posFrom, posTo);

	BOOST_FOREACH(boost::function<void(TCheckBox*)>& fillerFunc, checkBoxFillerFuncArr)
	{
		fillerFunc(checkBox);
	}

	return checkBox;
}

void TCheckBox::TextureFiller(Vector2f posFrom, Vector2f posTo, const std::string& texName, const std::string& texNameChecked, TCheckBox* checkBoxToFill)
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

void TCheckBox::TextFiller(Vector2f posFrom, Vector2f posTo, TTextParams textParams, TCheckBox* checkBoxToFill)
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


TCheckBox* TCheckBox::CreateSimpleCheckBox(Vector2f posFrom, Vector2f posTo, const std::string& texName, const std::string& texNameChecked)
{
	return CreateCheckBoxWithFiller(posFrom, posTo, boost::bind(&TCheckBox::TextureFiller, posFrom, posTo, texName, texNameChecked, _1));
}

TCheckBox* TCheckBox::CreateCheckBoxWithText(Vector2f posFrom, Vector2f posTo, const std::string& texName, const std::string& texNameChecked, Vector2f textPosFrom, Vector2f textPosTo, TTextParams textParams)
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

bool TRadioButton::CheckClick(Vector2f mousePos)
{
	if (mousePos(0) >= LeftBottomPos(0) &&
		mousePos(0) <= LeftBottomPos(0) + Width &&
		mousePos(1) >= LeftBottomPos(1) &&
		mousePos(1) <= LeftBottomPos(1) + Height)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void TRadioButton::Update(size_t dt)
{
}

void TRadioButton::OnTapDown(Vector2f pos)
{
}

void TRadioButton::OnTapUp(Vector2f pos)
{
	SetChecked(true);
}

void TRadioButton::OnMoveOut()
{
}


TRadioButton* TRadioButton::CreateRadioButton(Vector2f posFrom, Vector2f posTo)
{
	TRadioButton* radioButton = new TRadioButton();

	radioButton->LeftBottomPos = posFrom;

	radioButton->Width = posTo(0) - posFrom(0);

	radioButton->Height = posTo(1) - posFrom(1);

	return radioButton;
}

TRadioButton* TRadioButton::CreateRadioButtonWithFiller(Vector2f posFrom, Vector2f posTo, boost::function<void(TRadioButton*)> radioButtonFillerFunc)
{
	TRadioButton* radioButton = TRadioButton::CreateRadioButton(posFrom, posTo);

	radioButtonFillerFunc(radioButton);

	return radioButton;
}

TRadioButton* TRadioButton::CreateRadioButtonWithFillers(Vector2f posFrom, Vector2f posTo, std::vector<boost::function<void(TRadioButton*)> > radioButtonFillerFuncArr)
{
	TRadioButton* radioButton = TRadioButton::CreateRadioButton(posFrom, posTo);

	BOOST_FOREACH(boost::function<void(TRadioButton*)>& fillerFunc, radioButtonFillerFuncArr)
	{
		fillerFunc(radioButton);
	}

	return radioButton;
}

void TRadioButton::TextureFiller(Vector2f posFrom, Vector2f posTo, const std::string& texName, const std::string& texNameChecked, TRadioButton* radioButtonToFill)
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

void TRadioButton::TextFiller(Vector2f posFrom, Vector2f posTo, TTextParams textParams, TRadioButton* radioButtonToFill)
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

TRadioButton* TRadioButton::CreateSimpleRadioButton(Vector2f posFrom, Vector2f posTo, const std::string& texName, const std::string& texNameChecked)
{
	return CreateRadioButtonWithFiller(posFrom, posTo, boost::bind(&TRadioButton::TextureFiller, posFrom, posTo, texName, texNameChecked, _1));
}

TRadioButton* TRadioButton::CreateRadioButtonWithText(Vector2f posFrom, Vector2f posTo, const std::string& texName, const std::string& texNameChecked, Vector2f textPosFrom, Vector2f textPosTo, TTextParams textParams)
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

void TMover::Update(size_t dt)
{
}

bool TMover::CheckClick(Vector2f mousePos)
{
	if (mousePos(0) >= LeftBottomPos(0) &&
		mousePos(0) <= LeftBottomPos(0) + Width &&
		mousePos(1) >= LeftBottomPos(1) &&
		mousePos(1) <= LeftBottomPos(1) + Height)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void TMover::OnTapDown(Vector2f pos)
{
}

void TMover::OnTapUp(Vector2f pos)
{
}

void TMover::OnMove(Vector2f shift)
{
	float oldPosCursor = PosCursor;
	PosCursor -= shift(1);

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
	ResourceManager->GUIManager.MoveWidgetGroup(GroupName, SelfName, Vector2f(0, shift_y));
	}
}

void TMover::OnMoveOut()
{
}


TMover* TMover::CreateMover(Vector2f posFrom, Vector2f posTo, std::string groupName, std::string selfName, float maxHeight)
{
	TMover* mover = new TMover();

	mover->LeftBottomPos = posFrom;

	mover->Width = posTo(0) - posFrom(0);

	mover->Height = posTo(1) - posFrom(1);

	mover->GroupName = groupName;

	mover->SelfName = selfName;

	mover->MaxHeight = maxHeight;

	return mover;
}


THorizontalJoystick::THorizontalJoystick()
	: ButtonState(BS_NORMAL)
	, ButtonStateTimer(0.f)
	, SelectorWidth(0)
	, SelectorPos(0)
	, SelectorTouched(false)
{
}


void THorizontalJoystick::ReturnSelectorBack()
{
	SelectorPos = 0;

	float selectorCenterPos = LeftBottomPos(0) + 0.5f * Width;

	Vector2f selectorPosFrom = Vector2f(selectorCenterPos - SelectorWidth * 0.5f, LeftBottomPos(1));
	Vector2f selectorPosTo = Vector2f(selectorCenterPos + SelectorWidth * 0.5f, LeftBottomPos(1) + Height);


	for (std::vector<TRenderPairList::iterator>::iterator i = PressedTextureIteratorArr.begin(); i != PressedTextureIteratorArr.end(); ++i)
	{
		Replace6PointsInTriangleList((*i)->second.Data, 0, selectorPosFrom, selectorPosTo);
		(*i)->second.RefreshBuffer();
	}

	for (std::vector<TRenderPairList::iterator>::iterator i = NormalTextureIteratorArr.begin(); i != NormalTextureIteratorArr.end(); ++i)
	{
		Replace6PointsInTriangleList((*i)->second.Data, 0, selectorPosFrom, selectorPosTo);
		(*i)->second.RefreshBuffer();
	}
}

void THorizontalJoystick::Update(size_t dt)
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


bool THorizontalJoystick::CheckClick(Vector2f mousePos)
{
	if (mousePos(0) >= LeftBottomPos(0) &&
		mousePos(0) <= LeftBottomPos(0) + Width &&
		mousePos(1) >= LeftBottomPos(1) &&
		mousePos(1) <= LeftBottomPos(1) + Height)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool THorizontalJoystick::CheckSelectorClicked(Vector2f mousePos)
{
	float selectorCenterPos = LeftBottomPos(0) + (SelectorPos + 1) * 0.5f * Width;
	float selectorLeftPos = selectorCenterPos - SelectorWidth * 0.5f;
	float selectorRightPos = selectorCenterPos + SelectorWidth * 0.5f;

	if (mousePos(0) >= selectorLeftPos &&
		mousePos(0) <= selectorRightPos &&
		mousePos(1) >= LeftBottomPos(1) &&
		mousePos(1) <= LeftBottomPos(1) + Height)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void THorizontalJoystick::OnTapDown(Vector2f pos)
{
	if (ButtonState == BS_NORMAL || ButtonState == BS_GO_NORMAL)
	{
		ButtonState = BS_GO_PRESSED;
	}

	if (CheckSelectorClicked(pos))
	{
		SelectorTouched = true;
	}
}

void THorizontalJoystick::OnTapUp(Vector2f pos)
{
	if (ButtonState == BS_PRESSED || ButtonState == BS_GO_PRESSED)
	{
		ButtonState = BS_GO_NORMAL;
	}

	SelectorTouched = false;
	ReturnSelectorBack();
}
    
void THorizontalJoystick::OnTapUpAfterMove(Vector2f pos)
{
    OnTapUp(pos);
	SelectorPos = 0.f;
}

void THorizontalJoystick::OnTapUpAfterMoveOut(Vector2f pos)
{
    OnTapUp(pos);
	SelectorPos = 0.f;
}

void THorizontalJoystick::OnMove(Vector2f shift)
{
	if (SelectorTouched)
	{
		SelectorPos = SE::clamp(SelectorPos - 2.f * shift(0) / (Width - SelectorWidth), -1.f, 1.f);
		
		float selectorCenterPos = LeftBottomPos(0) + SelectorWidth * 0.5f + (SelectorPos + 1) * 0.5f * (Width - SelectorWidth);

		Vector2f selectorPosFrom = Vector2f(selectorCenterPos - SelectorWidth * 0.5f, LeftBottomPos(1));
		Vector2f selectorPosTo = Vector2f(selectorCenterPos + SelectorWidth * 0.5f, LeftBottomPos(1) + Height);


		for (std::vector<TRenderPairList::iterator>::iterator i = PressedTextureIteratorArr.begin(); i != PressedTextureIteratorArr.end(); ++i)
		{
			Replace6PointsInTriangleList((*i)->second.Data, 0, selectorPosFrom, selectorPosTo);
			(*i)->second.RefreshBuffer();
		}

		for (std::vector<TRenderPairList::iterator>::iterator i = NormalTextureIteratorArr.begin(); i != NormalTextureIteratorArr.end(); ++i)
		{
			Replace6PointsInTriangleList((*i)->second.Data, 0, selectorPosFrom, selectorPosTo);
			(*i)->second.RefreshBuffer();
		}
	}
}

void THorizontalJoystick::OnMoveOut()
{
	if (ButtonState == BS_PRESSED || ButtonState == BS_GO_PRESSED)
	{
		
		ButtonState = BS_GO_NORMAL;
	}
}

float THorizontalJoystick::GetSelectorPos()
{
	return SelectorPos;
}

THorizontalJoystick* THorizontalJoystick::CreateJoystick(Vector2f posFrom, Vector2f posTo, float selectorWidth)
{
	
	THorizontalJoystick* widget = new THorizontalJoystick();

	widget->LeftBottomPos = posFrom;

	widget->Width = posTo(0) - posFrom(0);

	widget->Height = posTo(1) - posFrom(1);

	widget->SelectorWidth = selectorWidth;

	return widget;
}

THorizontalJoystick* THorizontalJoystick::CreateJoystickWithFiller(Vector2f posFrom, Vector2f posTo, float selectorWidth, boost::function<void(THorizontalJoystick*)> fillerFunc)
{
	
	THorizontalJoystick* joystick = THorizontalJoystick::CreateJoystick(posFrom, posTo, selectorWidth);

	fillerFunc(joystick);

	return joystick;
}

THorizontalJoystick* THorizontalJoystick::CreateJoystickWithFillers(Vector2f posFrom, Vector2f posTo, float selectorWidth, std::vector<boost::function<void(THorizontalJoystick*)> > fillerFuncArr)
{
	THorizontalJoystick* joystick = THorizontalJoystick::CreateJoystick(posFrom, posTo, selectorWidth);

	for (std::vector<boost::function<void(THorizontalJoystick*)> >::iterator funcItr = fillerFuncArr.begin(); funcItr != fillerFuncArr.end(); ++funcItr)
	{
		(*funcItr)(joystick);
	}

	return joystick;
}


void THorizontalJoystick::SquareJoystickFiller(Vector2f posFrom, Vector2f posTo, float selectorWidth, const std::string& texNameField, const std::string& texName, const std::string& texNamePressed, THorizontalJoystick* joystickToFill)
{
	
	float selectorCenterPos = 0.5f * (posFrom(0) + posTo(0));

	Vector2f selectorPosFrom = Vector2f(selectorCenterPos - selectorWidth * 0.5f, posFrom(1));
	Vector2f selectorPosTo = Vector2f(selectorCenterPos + selectorWidth * 0.5f, posTo(1));

	TRenderParams renderParams;

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = texName;
	renderParams.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 1.f;

	TTriangleList triangleList = MakeTriangleList(selectorPosFrom, selectorPosTo);
		
	triangleList.RefreshBuffer();

	joystickToFill->NormalTextureIteratorArr.push_back(joystickToFill->TriangleListVector.insert(joystickToFill->TriangleListVector.end(), TRenderPair(renderParams, triangleList)));

	TTriangleList triangleList2 = MakeTriangleList(selectorPosFrom, selectorPosTo);
	triangleList2.RefreshBuffer();

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = texNamePressed;
	renderParams.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 0.f;

	joystickToFill->PressedTextureIteratorArr.push_back(joystickToFill->TriangleListVector.insert(joystickToFill->TriangleListVector.end(),TRenderPair(renderParams, triangleList2)));
	

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = texNameField;
	renderParams.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 1.f;

	TTriangleList fieldTriangleList = MakeTriangleList(posFrom, posTo);
		
	fieldTriangleList.RefreshBuffer();

	joystickToFill->FieldTextureIteratorArr.push_back(joystickToFill->TriangleListVector.insert(joystickToFill->TriangleListVector.end(), TRenderPair(renderParams, fieldTriangleList)));

}

THorizontalJoystick* THorizontalJoystick::CreateSquareJoystick(Vector2f posFrom, Vector2f posTo, float selectorWidth, const std::string& texNameField, const std::string& texName, const std::string& texNamePressed)
{
	return CreateJoystickWithFiller(posFrom, posTo, selectorWidth, boost::bind(THorizontalJoystick::SquareJoystickFiller, posFrom, posTo, selectorWidth, texNameField, texName, texNamePressed, _1));
}

} //namespace SE