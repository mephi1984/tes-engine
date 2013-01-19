#ifndef SERIALIZE_INTERFACE_H_INCLUDED
#define SERIALIZE_INTERFACE_H_INCLUDED

#include "include/Utils/DataTypes/DataTypes.h"
#include "include/Utils/ErrorTypes/ErrorTypes.h"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "boost/shared_array.hpp"

#include "boost/foreach.hpp"
#include <string>

namespace SE
{

std::shared_ptr<boost::property_tree::ptree> StringToPropertyTree(std::string xmlCode, std::map<std::string, std::string> replaceMap = std::map<std::string, std::string>());

std::shared_ptr<boost::property_tree::ptree> FileToPropertyTree(boost::shared_array<char> xmlFileArr, cardinal xmlFileSize, std::map<std::string, std::string> replaceMap = std::map<std::string, std::string>());

std::shared_ptr<boost::property_tree::ptree> FileToPropertyTree(const std::string& fileName, std::map<std::string, std::string> replaceMap = std::map<std::string, std::string>());


class TSerializeInterface
{
public:
	virtual void Serialize(boost::property_tree::ptree& propertyTree)
	{
	}

};


template <typename TKEY, typename TVALUE>
class TMapParser : public TSerializeInterface
{
public:
	std::map<TKEY, TVALUE> Map;

	virtual void Serialize(boost::property_tree::ptree& propertyTree)
	{

		BOOST_FOREACH(boost::property_tree::ptree::value_type& subTree, propertyTree)
		{
			TKEY key = subTree.second.get<TKEY>("<xmlattr>.key");
			TVALUE value = subTree.second.get<TVALUE>("<xmlattr>.value");

			Map[key] = value;
		}
	}
};

template <typename TKEY, typename TVALUE>
std::map<TKEY, TVALUE> SerializeToMap(boost::property_tree::ptree& propertyTree)
{
	TMapParser<TKEY, TVALUE> mapParser;
	mapParser.Serialize(propertyTree);
	return mapParser.Map;
}

} //namespace SE

#endif