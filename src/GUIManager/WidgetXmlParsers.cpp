#include "include/Engine.h"


namespace SE
{

void TStaticWidgetParser::Serialize(boost::property_tree::ptree& propertyTree)
{
	std::string widgetType = propertyTree.get<std::string>("<xmlattr>.type");
	std::string name = propertyTree.get<std::string>("<xmlattr>.name");
	std::string group = propertyTree.get<std::string>("<xmlattr>.group");

	float xCenter = propertyTree.get<float>("<xmlattr>.xCenter");
	float yCenter = propertyTree.get<float>("<xmlattr>.yCenter");
	float width = propertyTree.get<float>("<xmlattr>.width");
	float height = propertyTree.get<float>("<xmlattr>.height");
	int order = propertyTree.get<int>("<xmlattr>.order", 0);

	Vector2f posFrom(xCenter - width*0.5f, yCenter - height*0.5f);
	Vector2f posTo(xCenter + width*0.5f, yCenter + height*0.5f);

	//boost::property_tree::ptree& fillerTree = propertyTree.find("Filler")->second;

	std::vector<boost::function<void(TSquareStatic*)>> fillerFuncArr;


	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, propertyTree)
	{
		if (v.first == "Filler")
		{
			boost::property_tree::ptree& fillerTree = v.second;

			if (fillerTree.get<std::string>("<xmlattr>.type") == "StaticTextureFiller")
			{
				std::string normTexName = fillerTree.get<std::string>("<xmlattr>.texName");

				fillerFuncArr.push_back(boost::bind(TSquareStatic::StaticTextureFiller, posFrom, posTo, normTexName, _1));

			}
			else if (fillerTree.get<std::string>("<xmlattr>.type") == "StaticTextFiller")
			{
				TTextParams textParamsNormal;

				boost::property_tree::ptree& textParamsNormalTree = fillerTree.find("TextParams")->second;

				textParamsNormal.Serialize(textParamsNormalTree);

				fillerFuncArr.push_back(boost::bind(TSquareStatic::StaticTextFiller, posFrom, posTo, textParamsNormal, _1));
			}
		}
	}

	ResourceManager->GUIManager.AddWidget(
		std::shared_ptr<TInstancingWidgetAncestor>(TSquareStatic::CreateStaticWithFillers(posFrom, posTo, fillerFuncArr)),
		name,
		group,
		order);

}


void TDynamicTextWidgetParser::Serialize(boost::property_tree::ptree& propertyTree)
{
	std::string widgetType = propertyTree.get<std::string>("<xmlattr>.type");
	std::string name = propertyTree.get<std::string>("<xmlattr>.name");
	std::string group = propertyTree.get<std::string>("<xmlattr>.group");

	float xCenter = propertyTree.get<float>("<xmlattr>.xCenter");
	float yCenter = propertyTree.get<float>("<xmlattr>.yCenter");
	float width = propertyTree.get<float>("<xmlattr>.width");
	float height = propertyTree.get<float>("<xmlattr>.height");
	int order = propertyTree.get<int>("<xmlattr>.order", 0);

	Vector2f posFrom(xCenter - width*0.5f, yCenter - height*0.5f);
	Vector2f posTo(xCenter + width*0.5f, yCenter + height*0.5f);

	//boost::property_tree::ptree& fillerTree = propertyTree.find("Filler")->second;

	std::vector<boost::function<void(TDynamicText*)>> fillerFuncArr;


	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, propertyTree)
	{
		if (v.first == "Filler")
		{
			boost::property_tree::ptree& fillerTree = v.second;

			if (fillerTree.get<std::string>("<xmlattr>.type") == "TextParamsFiller")
			{
				TTextParams textParamsNormal;

				boost::property_tree::ptree& textParamsNormalTree = fillerTree.find("TextParams")->second;

				textParamsNormal.Serialize(textParamsNormalTree);

				fillerFuncArr.push_back(boost::bind(TDynamicText::TextParamsFiller, posFrom, posTo, textParamsNormal, _1));
			}
			else if (fillerTree.get<std::string>("<xmlattr>.type") == "SmartValueTextFiller")
			{
				std::string dictionaryName = fillerTree.get<std::string>("<xmlattr>.dictionaryName");
				std::string smartValueName = fillerTree.get<std::string>("<xmlattr>.smartValueName");
				fillerFuncArr.push_back(boost::bind(TDynamicText::SmartValueTextFiller, dictionaryName, smartValueName, _1));
			}
		}
	}

	ResourceManager->GUIManager.AddWidget(
		std::shared_ptr<TInstancingWidgetAncestor>(TDynamicText::CreateDynamicTextWithFillers(posFrom, posTo, fillerFuncArr)),
		name,
		group,
		order);

}



void TButtonWidgetParser::Serialize(boost::property_tree::ptree& propertyTree)
{

	std::string widgetType = propertyTree.get<std::string>("<xmlattr>.type");
	std::string name = propertyTree.get<std::string>("<xmlattr>.name");
	std::string group = propertyTree.get<std::string>("<xmlattr>.group");

	float xCenter = propertyTree.get<float>("<xmlattr>.xCenter");
	float yCenter = propertyTree.get<float>("<xmlattr>.yCenter");
	float width = propertyTree.get<float>("<xmlattr>.width");
	float height = propertyTree.get<float>("<xmlattr>.height");
	int order = propertyTree.get<int>("<xmlattr>.order", 0);

	Vector2f posFrom(xCenter - width*0.5f, yCenter - height*0.5f);
	Vector2f posTo(xCenter + width*0.5f, yCenter + height*0.5f);

	std::vector<boost::function<void(TUniversalButton*)> > fillerFuncArr;

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, propertyTree)
	{
		if (v.first == "Filler")
		{

			boost::property_tree::ptree& fillerTree = v.second;

			if (fillerTree.get<std::string>("<xmlattr>.type") == "SquareButtonFiller")
			{
				std::string normTexName = fillerTree.get<std::string>("<xmlattr>.normTexName");
				std::string pressedTexName = fillerTree.get<std::string>("<xmlattr>.pressedTexName");


				fillerFuncArr.push_back(boost::bind(TUniversalButton::SquareButtonFiller, posFrom, posTo, normTexName, pressedTexName, _1));

			}
			else if (fillerTree.get<std::string>("<xmlattr>.type") == "TextButtonFiller")
			{
				TTextParams textParamsNormal;
				TTextParams textParamsPressed;

				boost::property_tree::ptree& textParamsNormalTree = fillerTree.find("TextParamsNormal")->second;

				textParamsNormal.Serialize(textParamsNormalTree);

				boost::property_tree::ptree& textParamsPressedTree = fillerTree.find("TextParamsPressed")->second;

				textParamsPressed.Serialize(textParamsPressedTree);

				fillerFuncArr.push_back(boost::bind(TUniversalButton::TextButtonFiller, posFrom, posTo, textParamsNormal, textParamsPressed, _1));

			}
		}

	}

	ResourceManager->GUIManager.AddWidget(
		std::shared_ptr<TInstancingWidgetAncestor>(
		TUniversalButton::CreateButtonWithFillers(posFrom, posTo, fillerFuncArr)
		),
		name,
		group,
		order);

}




void TEditWidgetParser::Serialize(boost::property_tree::ptree& propertyTree)
{
	std::string widgetType = propertyTree.get<std::string>("<xmlattr>.type");
	std::string name = propertyTree.get<std::string>("<xmlattr>.name");
	std::string group = propertyTree.get<std::string>("<xmlattr>.group");

	float xCenter = propertyTree.get<float>("<xmlattr>.xCenter");
	float yCenter = propertyTree.get<float>("<xmlattr>.yCenter");
	float width = propertyTree.get<float>("<xmlattr>.width");
	float height = propertyTree.get<float>("<xmlattr>.height");
	int order = propertyTree.get<int>("<xmlattr>.order", 0);

	Vector2f posFrom(xCenter - width*0.5f, yCenter - height*0.5f);
	Vector2f posTo(xCenter + width*0.5f, yCenter + height*0.5f);
	/*
	//The only one filler
	std::string texName = propertyTree.get<std::string>("Filler.<xmlattr>.texName");

	TTextParams textParams;

	boost::property_tree::ptree& textParamsTree = propertyTree.find("Filler")->second.find("TextParams")->second;

	textParams.Serialize(textParamsTree);

	ResourceManager->GUIManager.AddWidget(
		std::shared_ptr<TInstancingWidgetAncestor>(TEdit::CreateEdit(posFrom, posTo, texName, textParams)),
		name,
		group,
		order);*/

	std::vector<boost::function<void(TEdit*)>> fillerFuncArr;

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, propertyTree)
	{
		if (v.first == "Filler")
		{

			boost::property_tree::ptree& fillerTree = v.second;

			if (fillerTree.get<std::string>("<xmlattr>.type") == "EditFiller")
			{

				std::string texName = fillerTree.get<std::string>("<xmlattr>.texName");

				TTextParams textParams;

				boost::property_tree::ptree& textParamsTree = fillerTree.find("TextParams")->second;

				textParams.Serialize(textParamsTree);

				fillerFuncArr.push_back(boost::bind(TEdit::EditFiller, posFrom, posTo, texName, textParams, _1));


			}
			else if (fillerTree.get<std::string>("<xmlattr>.type") == "SmartValueTextFiller")
			{
				
				std::string dictionaryName = fillerTree.get<std::string>("<xmlattr>.dictionaryName");
				std::string smartValueName = fillerTree.get<std::string>("<xmlattr>.smartValueName");
				fillerFuncArr.push_back(boost::bind(TEdit::SmartValueTextFiller, dictionaryName, smartValueName, _1));

			}
		}

	}

	ResourceManager->GUIManager.AddWidget(
		std::shared_ptr<TInstancingWidgetAncestor>(
		TEdit::CreateEditWithFillers(posFrom, posTo, fillerFuncArr)
		),
		name,
		group,
		order);
}





void TCheckBoxWidgetParser::Serialize(boost::property_tree::ptree& propertyTree)
{
	std::string widgetType = propertyTree.get<std::string>("<xmlattr>.type");
	std::string name = propertyTree.get<std::string>("<xmlattr>.name");
	std::string group = propertyTree.get<std::string>("<xmlattr>.group");

	float xCenter = propertyTree.get<float>("<xmlattr>.xCenter");
	float yCenter = propertyTree.get<float>("<xmlattr>.yCenter");
	float width = propertyTree.get<float>("<xmlattr>.width");
	float height = propertyTree.get<float>("<xmlattr>.height");
	int order = propertyTree.get<int>("<xmlattr>.order", 0);

	Vector2f posFrom(xCenter - width*0.5f, yCenter - height*0.5f);
	Vector2f posTo(xCenter + width*0.5f, yCenter + height*0.5f);

	std::vector<boost::function<void(TCheckBox*)> > fillerFuncArr;

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, propertyTree)
	{
		if (v.first == "Filler")
		{

			boost::property_tree::ptree& fillerTree = v.second;

			if (fillerTree.get<std::string>("<xmlattr>.type") == "TextureFiller")
			{
				std::string texName = fillerTree.get<std::string>("<xmlattr>.texName");
				std::string checkedTexName = fillerTree.get<std::string>("<xmlattr>.checkedTexName");

				fillerFuncArr.push_back(boost::bind(TCheckBox::TextureFiller, posFrom, posTo, texName, checkedTexName, _1));

			}
			else if (fillerTree.get<std::string>("<xmlattr>.type") == "TextFiller")
			{
				TTextParams textParams;

				boost::property_tree::ptree& textParamsTree = fillerTree.find("TextParams")->second;

				textParams.Serialize(textParamsTree);

				float textXCenter = fillerTree.get<float>("<xmlattr>.xCenter");
				float textYCenter = fillerTree.get<float>("<xmlattr>.yCenter");
				float textWidth = fillerTree.get<float>("<xmlattr>.width");
				float textHeight = fillerTree.get<float>("<xmlattr>.height");

				Vector2f textPosFrom(textXCenter - textWidth*0.5f, textYCenter - textHeight*0.5f);
				Vector2f textPosTo(textXCenter + textWidth*0.5f, textYCenter + textHeight*0.5f);

				fillerFuncArr.push_back(boost::bind(TCheckBox::TextFiller, textPosFrom, textPosTo, textParams, _1));

			}
			else if (fillerTree.get<std::string>("<xmlattr>.type") == "RadioButtonGroupFiller")
			{
				//std::string radioGroupName = fillerTree.get<std::string>("");
				//fillerFuncArr.push_back(boost::bind(TCheckBox::RadioButtonGroupFiller, radioGroupName, _1));
				throw ErrorToLog("RadioButtonGroupFiller is not allowed for checkboxes!");
			}
			else if (fillerTree.get<std::string>("<xmlattr>.type") == "SmartValueFiller")
			{
				std::string dictionaryName = fillerTree.get<std::string>("<xmlattr>.dictionaryName");
				std::string smartValueName = fillerTree.get<std::string>("<xmlattr>.smartValueName");
				fillerFuncArr.push_back(boost::bind(TCheckBox::SmartValueFiller, dictionaryName, smartValueName, _1));
			}
		}

	}


	ResourceManager->GUIManager.AddWidget(
		std::shared_ptr<TInstancingWidgetAncestor>(
		TCheckBox::CreateCheckBoxWithFillers(posFrom, posTo, fillerFuncArr)
		),
		name,
		group,
		order);

}



void TRadioButtonWidgetParser::Serialize(boost::property_tree::ptree& propertyTree)
{
	std::string widgetType = propertyTree.get<std::string>("<xmlattr>.type");
	std::string name = propertyTree.get<std::string>("<xmlattr>.name");
	std::string group = propertyTree.get<std::string>("<xmlattr>.group");

	float xCenter = propertyTree.get<float>("<xmlattr>.xCenter");
	float yCenter = propertyTree.get<float>("<xmlattr>.yCenter");
	float width = propertyTree.get<float>("<xmlattr>.width");
	float height = propertyTree.get<float>("<xmlattr>.height");
	int order = propertyTree.get<int>("<xmlattr>.order", 0);

	Vector2f posFrom(xCenter - width*0.5f, yCenter - height*0.5f);
	Vector2f posTo(xCenter + width*0.5f, yCenter + height*0.5f);

	std::vector<boost::function<void(TRadioButton*)> > fillerFuncArr;

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, propertyTree)
	{
		if (v.first == "Filler")
		{

			boost::property_tree::ptree& fillerTree = v.second;

			if (fillerTree.get<std::string>("<xmlattr>.type") == "TextureFiller")
			{
				std::string texName = fillerTree.get<std::string>("<xmlattr>.texName");
				std::string checkedTexName = fillerTree.get<std::string>("<xmlattr>.checkedTexName");

				fillerFuncArr.push_back(boost::bind(TRadioButton::TextureFiller, posFrom, posTo, texName, checkedTexName, _1));

			}
			else if (fillerTree.get<std::string>("<xmlattr>.type") == "TextFiller")
			{
				TTextParams textParams;

				boost::property_tree::ptree& textParamsTree = fillerTree.find("TextParams")->second;

				textParams.Serialize(textParamsTree);

				float textXCenter = fillerTree.get<float>("<xmlattr>.xCenter");
				float textYCenter = fillerTree.get<float>("<xmlattr>.yCenter");
				float textWidth = fillerTree.get<float>("<xmlattr>.width");
				float textHeight = fillerTree.get<float>("<xmlattr>.height");

				Vector2f textPosFrom(textXCenter - textWidth*0.5f, textYCenter - textHeight*0.5f);
				Vector2f textPosTo(textXCenter + textWidth*0.5f, textYCenter + textHeight*0.5f);

				fillerFuncArr.push_back(boost::bind(TRadioButton::TextFiller, textPosFrom, textPosTo, textParams, _1));

			}
			else if (fillerTree.get<std::string>("<xmlattr>.type") == "SmartValueFiller")
			{
				std::string dictionaryName = fillerTree.get<std::string>("<xmlattr>.dictionaryName");
				std::string smartValueName = fillerTree.get<std::string>("<xmlattr>.smartValueName");

				std::string smartValueType = fillerTree.get<std::string>("type");

				if (smartValueType == "int")
				{
					int value = fillerTree.get<int>("value");
					fillerFuncArr.push_back(boost::bind(TRadioButton::SmartValueFiller<int>, dictionaryName, smartValueName, value, _1));
				}
				else if (smartValueType == "float")
				{
					float value = fillerTree.get<float>("value");
					fillerFuncArr.push_back(boost::bind(TRadioButton::SmartValueFiller<float>, dictionaryName, smartValueName, value, _1));
				}
				else if (smartValueType == "bool")
				{
					bool value = fillerTree.get<bool>("value");
					fillerFuncArr.push_back(boost::bind(TRadioButton::SmartValueFiller<bool>, dictionaryName, smartValueName, value, _1));
				}
				else if (smartValueType == "string")
				{
					std::string value = fillerTree.get<std::string>("value");
					fillerFuncArr.push_back(boost::bind(TRadioButton::SmartValueFiller<std::string>, dictionaryName, smartValueName, value, _1));
				}
				else
				{
					throw ErrorToLog("Unknown type: "+smartValueType);
				}

			}
		}

	}


	ResourceManager->GUIManager.AddWidget(
		std::shared_ptr<TInstancingWidgetAncestor>(
		TRadioButton::CreateRadioButtonWithFillers(posFrom, posTo, fillerFuncArr)
		),
		name,
		group,
		order);
}



/*
void TWidgetParser::Serialize(boost::property_tree::ptree& propertyTree)
{

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, propertyTree.begin()->second)
	{
		std::string widgetType = v.second.get<std::string>("<xmlattr>.type");


		if (widgetType == "Button")
		{
			TButtonWidgetParser().Serialize(v.second);
		}
		if (widgetType == "Static")
		{
			TStaticWidgetParser().Serialize(v.second);
		}
		if (widgetType == "Edit")
		{
			TEditWidgetParser().Serialize(v.second);
		}
		if (widgetType == "CheckBox")
		{
			TCheckBoxWidgetParser().Serialize(v.second);
		}
	}
}
*/

void TWidgetParser::Serialize(boost::property_tree::ptree& propertyTree)
{

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, propertyTree)
	{
		std::string widgetType = v.second.get<std::string>("<xmlattr>.type");


		if (widgetType == "Button")
		{
			TButtonWidgetParser().Serialize(v.second);
		}
		if (widgetType == "Static")
		{
			TStaticWidgetParser().Serialize(v.second);
		}
		if (widgetType == "DynamicText")
		{
			TDynamicTextWidgetParser().Serialize(v.second);
		}
		if (widgetType == "Edit")
		{
			TEditWidgetParser().Serialize(v.second);
		}
		if (widgetType == "CheckBox")
		{
			TCheckBoxWidgetParser().Serialize(v.second);
		}
		if (widgetType == "RadioButton")
		{
			TRadioButtonWidgetParser().Serialize(v.second);
		}
	}
}


void TWidgetListParser::Serialize(boost::property_tree::ptree& propertyTree)
{
	TWidgetParser().Serialize(propertyTree.begin()->second);
}


} //namespace SE