#ifndef WIDGET_XML_PARSERS_H_INCLUDED
#define WIDGET_XML_PARSERS_H_INCLUDED

#include "ButtonWidget.h"

namespace SE
{

struct TStaticWidgetParser : public TSerializeInterface
{
	virtual void Serialize(boost::property_tree::ptree& propertyTree);
};

struct TDynamicTextWidgetParser : public TSerializeInterface
{
	virtual void Serialize(boost::property_tree::ptree& propertyTree);
};


struct TButtonWidgetParser : public TSerializeInterface
{
	virtual void Serialize(boost::property_tree::ptree& propertyTree);
};


struct TEditWidgetParser : public TSerializeInterface
{
	virtual void Serialize(boost::property_tree::ptree& propertyTree);
};

struct TCheckBoxWidgetParser : public TSerializeInterface
{
	virtual void Serialize(boost::property_tree::ptree& propertyTree);
};

struct TRadioButtonWidgetParser : public TSerializeInterface
{
	virtual void Serialize(boost::property_tree::ptree& propertyTree);
};


struct TWidgetParser : public TSerializeInterface
{
	virtual void Serialize(boost::property_tree::ptree& propertyTree);
};


struct TWidgetListParser : public TSerializeInterface
{
	virtual void Serialize(boost::property_tree::ptree& propertyTree);
};

} //namespace SE

#endif