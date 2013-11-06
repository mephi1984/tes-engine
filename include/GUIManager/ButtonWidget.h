#ifndef BUTTON_WIDGET_H_INCLUDED
#define BUTTON_WIDGET_H_INCLUDED


#include "include/Render/RenderMisc.h"
#include "include/Render/RenderParams.h"
#include "include/Utils/Utils.h"
#include "include/FontManager/FontManager.h"

namespace SE
{

class TInstancingWidgetAncestor
{
protected:

public:
	TRenderPairList TriangleListVector;
	vec2 LeftBottomPos;

	virtual void Update(cardinal dt);

	virtual bool CheckClick(vec2 mousePos);

	virtual void OnTapDown(vec2 pos) { }
	virtual void OnTapUp(vec2 pos) { }
    virtual void OnTapUpAfterMove(vec2 pos) { }
	virtual void OnTapUpAfterMoveOut(vec2 pos) { }
	virtual void OnMove(vec2 shift) { }
	virtual void OnMoveOut() { }
	virtual void OnKeyPress(int key) { }

	virtual bool IsTransparentForInput() { return false; }

	virtual ~TInstancingWidgetAncestor();
};


struct TTextParams : public TSerializeInterface
{
	std::string Text;
	std::string FontName;
	TTextBasicAreaParams BasicTextAreaParams;
	TRenderParams RenderParams;

	TTextParams()
	{
	}

	TTextParams(const std::string& text)
		: Text(text)
	{
	}

	TTextParams(const std::string& text, const std::string& fontName, int height, float horizontalPadding, float verticalPadding, TTextHorizontalAlignment textHorizontalAlignment, TTextVerticalAlignment textVerticalAlignment)
		: Text(text)
        , FontName(fontName)
		, BasicTextAreaParams(height, horizontalPadding, verticalPadding, textHorizontalAlignment, textVerticalAlignment)
	{
	}

	virtual void Serialize(boost::property_tree::ptree& propertyTree)
	{
		Text = propertyTree.get<std::string>("Text");
        
        FontName = propertyTree.get<std::string>("Font");

		BasicTextAreaParams.Serialize(propertyTree.find("TextAreaParams")->second);
		
		if (propertyTree.count("RenderParams") != 0)
		{
			RenderParams.Serialize(propertyTree.find("RenderParams")->second);
		}
	}
};


struct TSquareStatic : public TInstancingWidgetAncestor
{
	float Width;

	float Height;

	TSquareStatic()
		: Width(0)
		, Height(0)
	{
	}

	virtual bool CheckClick(vec2 mousePos);

	static TSquareStatic* CreateStatic(vec2 posFrom, vec2 posTo);
	static TSquareStatic* CreateStaticWithFiller(vec2 posFrom, vec2 posTo, boost::function<void(TSquareStatic*)> staticFillerFunc);
	static TSquareStatic* CreateStaticWithFillers(vec2 posFrom, vec2 posTo, std::vector<boost::function<void(TSquareStatic*)>> staticFillerFuncArr);

	static void StaticTextureFiller(vec2 posFrom, vec2 posTo, const std::string& texName, TSquareStatic* staticToFill);
	static void StaticTextFiller(vec2 posFrom, vec2 posTo, TTextParams buttonTextParams, TSquareStatic* staticToFill);

	static TSquareStatic* CreateStaticTexture(vec2 posFrom, vec2 posTo, const std::string& texName);
	static TSquareStatic* CreateStaticText(vec2 posFrom, vec2 posTo, TTextParams buttonTextParams);

};



struct TDynamicText : public TInstancingWidgetAncestor
{
protected:
	boost::signals::connection SmartValueConnection;

	TTextParams LastTextParams;

	TRenderPairList::iterator TextRenderPairItr;

public:
	float Width;

	float Height;

	TDynamicText();
	~TDynamicText();

	virtual bool CheckClick(vec2 mousePos);

	void UpdateText(std::string text);

	static TDynamicText* CreateDynamicText(vec2 posFrom, vec2 posTo);
	static TDynamicText* CreateDynamicTextWithFiller(vec2 posFrom, vec2 posTo, boost::function<void(TDynamicText*)> dynamicTextFillerFunc);
	static TDynamicText* CreateDynamicTextWithFillers(vec2 posFrom, vec2 posTo, std::vector<boost::function<void(TDynamicText*)>> dynamicTextFillerFuncArr);

	static void TextParamsFiller(vec2 posFrom, vec2 posTo, TTextParams textParams, TDynamicText* dynamicTextToFill);
	static void SmartValueTextFiller(const std::string& dictionaryName, const std::string& smartValueName, TDynamicText* dynamicTextToFill);

};


TTriangleList CreateTriangleListForText(vec2 posFrom, vec2 posTo, TTextParams buttonTextParams);

class TUniversalButton : public TInstancingWidgetAncestor
{
protected:
	
	enum 
	{
		BS_NORMAL,
		BS_GO_PRESSED,
		BS_PRESSED,
		BS_GO_NORMAL
	} ButtonState;

	float ButtonStateTimer;

public:
	std::vector<TRenderPairList::iterator> NormalTextureIteratorArr;
	std::vector<TRenderPairList::iterator> PressedTextureIteratorArr;

	float Width;

	float Height;

	TUniversalButton();

	virtual void Update(cardinal dt);

	virtual bool CheckClick(vec2 mousePos);

	virtual void OnTapDown(vec2 pos);
	virtual void OnTapUp(vec2 pos);
    virtual void OnTapUpAfterMove(vec2 pos);
	virtual void OnMoveOut();

	static TUniversalButton* CreateButton(vec2 posFrom, vec2 posTo);
	static TUniversalButton* CreateButtonWithFiller(vec2 posFrom, vec2 posTo, boost::function<void(TUniversalButton*)> buttonFillerFunc);
	static TUniversalButton* CreateButtonWithFillers(vec2 posFrom, vec2 posTo, std::vector<boost::function<void(TUniversalButton*)> > buttonFillerFuncArr);

	static void SquareButtonFiller(vec2 posFrom, vec2 posTo, const std::string& texName, const std::string& texNamePressed, TUniversalButton* buttonToFill);
	static void TextButtonFiller(vec2 posFrom, vec2 posTo, TTextParams buttonTextParamsNormal, TTextParams buttonTextParamsPressed, TUniversalButton* buttonToFill);

	static TUniversalButton* CreateSquareButton(vec2 posFrom, vec2 posTo, const std::string& texName, const std::string& texNamePressed);
	static TUniversalButton* CreateTextOnlyButton(vec2 posFrom, vec2 posTo, TTextParams buttonTextParamsNormal, TTextParams buttonTextParamsPressed);
	static TUniversalButton* CreateSquareAndTextButton(vec2 posFrom, vec2 posTo, const std::string& texName, const std::string& texNamePressed, TTextParams buttonTextParamsNormal, TTextParams buttonTextParamsPressed);
};


class TEdit : public TInstancingWidgetAncestor
{
protected:

	float Width;

	float Height;

	TTextParams TextParams;

	void RefreshTextTriangleList();

	boost::signals::connection InputConnection;

	boost::signals::connection SetTextSlotConnection;

public:
	~TEdit();
	TRenderPairList::iterator TextIterator;

	
	boost::signal<void(std::string)> OnTextChanged;

	virtual bool CheckClick(vec2 mousePos);

	virtual void OnKeyPress(int key);
	virtual void OnTapDown(vec2 pos);

	void SetText(const std::string& newText);
    void OnSetAllText(std::string newText); //For iOS only
    std::string GetText();

	static TEdit* CreateEdit(vec2 posFrom, vec2 posTo);
	static TEdit* CreateEditWithFiller(vec2 posFrom, vec2 posTo, boost::function<void(TEdit*)> fillerFunc);
	static TEdit* CreateEditWithFillers(vec2 posFrom, vec2 posTo, std::vector<boost::function<void(TEdit*)> > fillerFuncArr);
	
	static void EditFiller(vec2 posFrom, vec2 posTo, const std::string& texName, TTextParams textParams, TEdit* edit);
	static void SmartValueTextFiller(const std::string& dictionaryName, const std::string& smartValueName, TEdit* edit);

	//static TEdit* CreateEdit(vec2 posFrom, vec2 posTo, const std::string& texName, TTextParams textParams);
};


class TCheckBox : public TInstancingWidgetAncestor
{
protected:
	bool IsChecked; //To be set immediately on tap up

	boost::signals::connection SetCheckedSlotConnection;
	void RefreshVisibility();
public:
	std::vector<TRenderPairList::iterator> StaticTextureIteratorArr;
	std::vector<TRenderPairList::iterator> CheckedTextureIteratorArr;

	float Width;

	float Height;

	TCheckBox();
	~TCheckBox();

	virtual bool CheckClick(vec2 mousePos);
	virtual void Update(cardinal dt);

	virtual void OnTapDown(vec2 pos);
	virtual void OnTapUp(vec2 pos);
	virtual void OnMoveOut();

	//Logic:
	void SetChecked(bool isChecked);
	boost::signal<void(bool)> OnSetChecked;

	static TCheckBox* CreateCheckBox(vec2 posFrom, vec2 posTo);
	static TCheckBox* CreateCheckBoxWithFiller(vec2 posFrom, vec2 posTo, boost::function<void(TCheckBox*)> checkBoxFillerFunc);
	static TCheckBox* CreateCheckBoxWithFillers(vec2 posFrom, vec2 posTo, std::vector<boost::function<void(TCheckBox*)> > checkBoxFillerFuncArr);

	static void TextureFiller(vec2 posFrom, vec2 posTo, const std::string& texName, const std::string& texNameChecked, TCheckBox* checkBoxToFill);
	static void TextFiller(vec2 posFrom, vec2 posTo, TTextParams textParams, TCheckBox* checkBoxToFill);
	static void CheckedFiller(bool isChecked, TCheckBox* checkBoxToFill);
	static void SmartValueFiller(const std::string& dictionaryName, const std::string& smartValueName, TCheckBox* checkBoxToFill);

	static TCheckBox* CreateSimpleCheckBox(vec2 posFrom, vec2 posTo, const std::string& texName, const std::string& texNameChecked);
	static TCheckBox* CreateCheckBoxWithText(vec2 posFrom, vec2 posTo, const std::string& texName, const std::string& texNameChecked, vec2 textPosFrom, vec2 textPosTo, TTextParams textParams);
};



class TRadioButton : public TInstancingWidgetAncestor
{
protected:
	bool IsChecked; //To be set immediately on tap up

	boost::signals::connection SetCheckedSlotConnection;
	void RefreshVisibility();
public:
	std::vector<TRenderPairList::iterator> StaticTextureIteratorArr;
	std::vector<TRenderPairList::iterator> CheckedTextureIteratorArr;

	float Width;

	float Height;

	TRadioButton();
	~TRadioButton();

	virtual bool CheckClick(vec2 mousePos);
	virtual void Update(cardinal dt);

	virtual void OnTapDown(vec2 pos);
	virtual void OnTapUp(vec2 pos);
	virtual void OnMoveOut();

	//Logic:
	void SetChecked(bool isChecked);
	boost::signal<void(bool)> OnSetChecked;

	static TRadioButton* CreateRadioButton(vec2 posFrom, vec2 posTo);
	static TRadioButton* CreateRadioButtonWithFiller(vec2 posFrom, vec2 posTo, boost::function<void(TRadioButton*)> radioButtonFillerFunc);
	static TRadioButton* CreateRadioButtonWithFillers(vec2 posFrom, vec2 posTo, std::vector<boost::function<void(TRadioButton*)> > radioButtonFillerFuncArr);

	static void TextureFiller(vec2 posFrom, vec2 posTo, const std::string& texName, const std::string& texNameChecked, TRadioButton* radioButtonToFill);
	static void TextFiller(vec2 posFrom, vec2 posTo, TTextParams textParams, TRadioButton* radioButtonToFill);
	static void CheckedFiller(bool isChecked, TRadioButton* radioButtonToFill);
	
	template <typename TYPENAME>
	static void SmartValueFiller(const std::string& dictionaryName, const std::string& smartValueName, TYPENAME value, TRadioButton* radioButtonToFill);

	static TRadioButton* CreateSimpleRadioButton(vec2 posFrom, vec2 posTo, const std::string& texName, const std::string& texNameChecked);
	static TRadioButton* CreateRadioButtonWithText(vec2 posFrom, vec2 posTo, const std::string& texName, const std::string& texNameChecked, vec2 textPosFrom, vec2 textPosTo, TTextParams textParams);
};


class TMover : public TInstancingWidgetAncestor
{
protected:

	float PosCursor;
public:
	float Width;
	float Height;
	float MaxHeight;

	std::string GroupName;
	std::string SelfName; //For exclusion

	TMover();
	~TMover();

	virtual void Update(cardinal dt);

	virtual bool CheckClick(vec2 mousePos);

	virtual void OnTapDown(vec2 pos);
	virtual void OnTapUp(vec2 pos);
	virtual void OnMove(vec2 shift);
	virtual void OnMoveOut();

	virtual bool IsTransparentForInput() { return true; }

	static TMover* CreateMover(vec2 posFrom, vec2 posTo, std::string groupName, std::string selfName, float maxHeight);
};


class THorizontalJoystick : public TInstancingWidgetAncestor
{
protected:
	
	enum 
	{
		BS_NORMAL,
		BS_GO_PRESSED,
		BS_PRESSED,
		BS_GO_NORMAL
	} ButtonState;

	float ButtonStateTimer;

	float SelectorWidth;
	float SelectorPos; //from -1 to 1

	bool SelectorTouched;

	void ReturnSelectorBack();

public:
	std::vector<TRenderPairList::iterator> FieldTextureIteratorArr;
	std::vector<TRenderPairList::iterator> NormalTextureIteratorArr;
	std::vector<TRenderPairList::iterator> PressedTextureIteratorArr;

	float Width;

	float Height;

	THorizontalJoystick();

	virtual void Update(cardinal dt);

	virtual bool CheckClick(vec2 mousePos);

	virtual void OnTapDown(vec2 pos);
	virtual void OnTapUp(vec2 pos);
    virtual void OnTapUpAfterMove(vec2 pos);
	virtual void OnTapUpAfterMoveOut(vec2 pos);
	virtual void OnMoveOut();
	virtual void OnMove(vec2 shift);

	float GetSelectorPos();
	bool CheckSelectorClicked(vec2 mousePos);

	static THorizontalJoystick* CreateJoystick(vec2 posFrom, vec2 posTo, float selectorWidth);
	static THorizontalJoystick* CreateJoystickWithFiller(vec2 posFrom, vec2 posTo, float selectorWidth, boost::function<void(THorizontalJoystick*)> fillerFunc);
	static THorizontalJoystick* CreateJoystickWithFillers(vec2 posFrom, vec2 posTo, float selectorWidth, std::vector<boost::function<void(THorizontalJoystick*)> > fillerFuncArr);

	static void SquareJoystickFiller(vec2 posFrom, vec2 posTo, float selectorWidth, const std::string& texNameField, const std::string& texName, const std::string& texNamePressed, THorizontalJoystick* joystickToFill);
	
	static THorizontalJoystick* CreateSquareJoystick(vec2 posFrom, vec2 posTo, float selectorWidth, const std::string& texNameField, const std::string& texName, const std::string& texNamePressed);
};

} //namespace SE

#endif