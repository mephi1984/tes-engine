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
	Vector2f LeftBottomPos;

	virtual void Update(size_t dt);

	virtual bool CheckClick(Vector2f mousePos);

	virtual void OnTapDown(Vector2f pos) { }
	virtual void OnTapUp(Vector2f pos) { }
    virtual void OnTapUpAfterMove(Vector2f pos) { }
	virtual void OnTapUpAfterMoveOut(Vector2f pos) { }
	virtual void OnMove(Vector2f shift) { }
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

	TTextParams() : BasicTextAreaParams()
	{
	}

	TTextParams(const std::string& text)
		: BasicTextAreaParams()
		, Text(text)
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
		Text = propertyTree.get<std::string>("Text", "");
        
        FontName = propertyTree.get<std::string>("Font", "");

		if (propertyTree.count("TextAreaParams") != 0)
		{
			BasicTextAreaParams.Serialize(propertyTree.find("TextAreaParams")->second);
		}
		else
		{
			BasicTextAreaParams = TTextBasicAreaParams();
		}
		
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

	virtual bool CheckClick(Vector2f mousePos);

	static TSquareStatic* CreateStatic(Vector2f posFrom, Vector2f posTo);
	static TSquareStatic* CreateStaticWithFiller(Vector2f posFrom, Vector2f posTo, boost::function<void(TSquareStatic*)> staticFillerFunc);
	static TSquareStatic* CreateStaticWithFillers(Vector2f posFrom, Vector2f posTo, std::vector<boost::function<void(TSquareStatic*)>> staticFillerFuncArr);

	static void StaticTextureFiller(Vector2f posFrom, Vector2f posTo, const std::string& texName, TSquareStatic* staticToFill);
	static void StaticTextFiller(Vector2f posFrom, Vector2f posTo, TTextParams buttonTextParams, TSquareStatic* staticToFill);

	static TSquareStatic* CreateStaticTexture(Vector2f posFrom, Vector2f posTo, const std::string& texName);
	static TSquareStatic* CreateStaticText(Vector2f posFrom, Vector2f posTo, TTextParams buttonTextParams);

};



struct TDynamicText : public TInstancingWidgetAncestor
{
protected:
	boost::signals2::connection SmartValueConnection;

	TTextParams LastTextParams;

	TRenderPairList::iterator TextRenderPairItr;

public:
	float Width;

	float Height;

	TDynamicText();
	~TDynamicText();

	virtual bool CheckClick(Vector2f mousePos);

	void UpdateText(std::string text);

	static TDynamicText* CreateDynamicText(Vector2f posFrom, Vector2f posTo);
	static TDynamicText* CreateDynamicTextWithFiller(Vector2f posFrom, Vector2f posTo, boost::function<void(TDynamicText*)> dynamicTextFillerFunc);
	static TDynamicText* CreateDynamicTextWithFillers(Vector2f posFrom, Vector2f posTo, std::vector<boost::function<void(TDynamicText*)>> dynamicTextFillerFuncArr);

	static void TextParamsFiller(Vector2f posFrom, Vector2f posTo, TTextParams textParams, TDynamicText* dynamicTextToFill);
	static void SmartValueTextFiller(const std::string& dictionaryName, const std::string& smartValueName, TDynamicText* dynamicTextToFill);

};


TTriangleList CreateTriangleListForText(Vector2f posFrom, Vector2f posTo, TTextParams buttonTextParams);

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

	virtual void Update(size_t dt);

	virtual bool CheckClick(Vector2f mousePos);

	virtual void OnTapDown(Vector2f pos);
	virtual void OnTapUp(Vector2f pos);
    virtual void OnTapUpAfterMove(Vector2f pos);
	virtual void OnMoveOut();

	static TUniversalButton* CreateButton(Vector2f posFrom, Vector2f posTo);
	static TUniversalButton* CreateButtonWithFiller(Vector2f posFrom, Vector2f posTo, boost::function<void(TUniversalButton*)> buttonFillerFunc);
	static TUniversalButton* CreateButtonWithFillers(Vector2f posFrom, Vector2f posTo, std::vector<boost::function<void(TUniversalButton*)> > buttonFillerFuncArr);

	static void SquareButtonFiller(Vector2f posFrom, Vector2f posTo, const std::string& texName, const std::string& texNamePressed, TUniversalButton* buttonToFill);
	static void TextButtonFiller(Vector2f posFrom, Vector2f posTo, TTextParams buttonTextParamsNormal, TTextParams buttonTextParamsPressed, TUniversalButton* buttonToFill);

	static TUniversalButton* CreateSquareButton(Vector2f posFrom, Vector2f posTo, const std::string& texName, const std::string& texNamePressed);
	static TUniversalButton* CreateTextOnlyButton(Vector2f posFrom, Vector2f posTo, TTextParams buttonTextParamsNormal, TTextParams buttonTextParamsPressed);
	static TUniversalButton* CreateSquareAndTextButton(Vector2f posFrom, Vector2f posTo, const std::string& texName, const std::string& texNamePressed, TTextParams buttonTextParamsNormal, TTextParams buttonTextParamsPressed);
};


class TEdit : public TInstancingWidgetAncestor
{
protected:

	float Width;

	float Height;

	TTextParams TextParams;

	void RefreshTextTriangleList();

	boost::signals2::connection InputConnection;

	boost::signals2::connection SetTextSlotConnection;

public:
	~TEdit();
	TRenderPairList::iterator TextIterator;

	
	boost::signals2::signal<void(std::string)> OnTextChanged;

	virtual bool CheckClick(Vector2f mousePos);

	virtual void OnKeyPress(int key);
	virtual void OnTapDown(Vector2f pos);

	void SetText(const std::string& newText);
    void OnSetAllText(std::string newText); //For iOS only
    std::string GetText();

	static TEdit* CreateEdit(Vector2f posFrom, Vector2f posTo);
	static TEdit* CreateEditWithFiller(Vector2f posFrom, Vector2f posTo, boost::function<void(TEdit*)> fillerFunc);
	static TEdit* CreateEditWithFillers(Vector2f posFrom, Vector2f posTo, std::vector<boost::function<void(TEdit*)> > fillerFuncArr);
	
	static void EditFiller(Vector2f posFrom, Vector2f posTo, const std::string& texName, TTextParams textParams, TEdit* edit);
	static void SmartValueTextFiller(const std::string& dictionaryName, const std::string& smartValueName, TEdit* edit);

	//static TEdit* CreateEdit(Vector2f posFrom, Vector2f posTo, const std::string& texName, TTextParams textParams);
};


class TCheckBox : public TInstancingWidgetAncestor
{
protected:
	bool IsChecked; //To be set immediately on tap up

	boost::signals2::connection SetCheckedSlotConnection;
	void RefreshVisibility();
public:
	std::vector<TRenderPairList::iterator> StaticTextureIteratorArr;
	std::vector<TRenderPairList::iterator> CheckedTextureIteratorArr;

	float Width;

	float Height;

	TCheckBox();
	~TCheckBox();

	virtual bool CheckClick(Vector2f mousePos);
	virtual void Update(size_t dt);

	virtual void OnTapDown(Vector2f pos);
	virtual void OnTapUp(Vector2f pos);
	virtual void OnMoveOut();

	//Logic:
	void SetChecked(bool isChecked);
	boost::signals2::signal<void(bool)> OnSetChecked;

	static TCheckBox* CreateCheckBox(Vector2f posFrom, Vector2f posTo);
	static TCheckBox* CreateCheckBoxWithFiller(Vector2f posFrom, Vector2f posTo, boost::function<void(TCheckBox*)> checkBoxFillerFunc);
	static TCheckBox* CreateCheckBoxWithFillers(Vector2f posFrom, Vector2f posTo, std::vector<boost::function<void(TCheckBox*)> > checkBoxFillerFuncArr);

	static void TextureFiller(Vector2f posFrom, Vector2f posTo, const std::string& texName, const std::string& texNameChecked, TCheckBox* checkBoxToFill);
	static void TextFiller(Vector2f posFrom, Vector2f posTo, TTextParams textParams, TCheckBox* checkBoxToFill);
	static void CheckedFiller(bool isChecked, TCheckBox* checkBoxToFill);
	static void SmartValueFiller(const std::string& dictionaryName, const std::string& smartValueName, TCheckBox* checkBoxToFill);

	static TCheckBox* CreateSimpleCheckBox(Vector2f posFrom, Vector2f posTo, const std::string& texName, const std::string& texNameChecked);
	static TCheckBox* CreateCheckBoxWithText(Vector2f posFrom, Vector2f posTo, const std::string& texName, const std::string& texNameChecked, Vector2f textPosFrom, Vector2f textPosTo, TTextParams textParams);
};



class TRadioButton : public TInstancingWidgetAncestor
{
protected:
	bool IsChecked; //To be set immediately on tap up

	boost::signals2::connection SetCheckedSlotConnection;
	void RefreshVisibility();
public:
	std::vector<TRenderPairList::iterator> StaticTextureIteratorArr;
	std::vector<TRenderPairList::iterator> CheckedTextureIteratorArr;

	float Width;

	float Height;

	TRadioButton();
	~TRadioButton();

	virtual bool CheckClick(Vector2f mousePos);
	virtual void Update(size_t dt);

	virtual void OnTapDown(Vector2f pos);
	virtual void OnTapUp(Vector2f pos);
	virtual void OnMoveOut();

	//Logic:
	void SetChecked(bool isChecked);
	boost::signals2::signal<void(bool)> OnSetChecked;

	static TRadioButton* CreateRadioButton(Vector2f posFrom, Vector2f posTo);
	static TRadioButton* CreateRadioButtonWithFiller(Vector2f posFrom, Vector2f posTo, boost::function<void(TRadioButton*)> radioButtonFillerFunc);
	static TRadioButton* CreateRadioButtonWithFillers(Vector2f posFrom, Vector2f posTo, std::vector<boost::function<void(TRadioButton*)> > radioButtonFillerFuncArr);

	static void TextureFiller(Vector2f posFrom, Vector2f posTo, const std::string& texName, const std::string& texNameChecked, TRadioButton* radioButtonToFill);
	static void TextFiller(Vector2f posFrom, Vector2f posTo, TTextParams textParams, TRadioButton* radioButtonToFill);
	static void CheckedFiller(bool isChecked, TRadioButton* radioButtonToFill);
	
	template <typename TYPENAME>
	static void SmartValueFiller(const std::string& dictionaryName, const std::string& smartValueName, TYPENAME value, TRadioButton* radioButtonToFill);

	static TRadioButton* CreateSimpleRadioButton(Vector2f posFrom, Vector2f posTo, const std::string& texName, const std::string& texNameChecked);
	static TRadioButton* CreateRadioButtonWithText(Vector2f posFrom, Vector2f posTo, const std::string& texName, const std::string& texNameChecked, Vector2f textPosFrom, Vector2f textPosTo, TTextParams textParams);
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

	virtual void Update(size_t dt);

	virtual bool CheckClick(Vector2f mousePos);

	virtual void OnTapDown(Vector2f pos);
	virtual void OnTapUp(Vector2f pos);
	virtual void OnMove(Vector2f shift);
	virtual void OnMoveOut();

	virtual bool IsTransparentForInput() { return true; }

	static TMover* CreateMover(Vector2f posFrom, Vector2f posTo, std::string groupName, std::string selfName, float maxHeight);
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

	virtual void Update(size_t dt);

	virtual bool CheckClick(Vector2f mousePos);

	virtual void OnTapDown(Vector2f pos);
	virtual void OnTapUp(Vector2f pos);
    virtual void OnTapUpAfterMove(Vector2f pos);
	virtual void OnTapUpAfterMoveOut(Vector2f pos);
	virtual void OnMoveOut();
	virtual void OnMove(Vector2f shift);

	float GetSelectorPos();
	bool CheckSelectorClicked(Vector2f mousePos);

	static THorizontalJoystick* CreateJoystick(Vector2f posFrom, Vector2f posTo, float selectorWidth);
	static THorizontalJoystick* CreateJoystickWithFiller(Vector2f posFrom, Vector2f posTo, float selectorWidth, boost::function<void(THorizontalJoystick*)> fillerFunc);
	static THorizontalJoystick* CreateJoystickWithFillers(Vector2f posFrom, Vector2f posTo, float selectorWidth, std::vector<boost::function<void(THorizontalJoystick*)> > fillerFuncArr);

	static void SquareJoystickFiller(Vector2f posFrom, Vector2f posTo, float selectorWidth, const std::string& texNameField, const std::string& texName, const std::string& texNamePressed, THorizontalJoystick* joystickToFill);
	
	static THorizontalJoystick* CreateSquareJoystick(Vector2f posFrom, Vector2f posTo, float selectorWidth, const std::string& texNameField, const std::string& texName, const std::string& texNamePressed);
};

} //namespace SE

#endif
