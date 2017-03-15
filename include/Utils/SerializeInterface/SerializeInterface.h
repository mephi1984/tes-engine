#ifndef SERIALIZE_INTERFACE_H_INCLUDED
#define SERIALIZE_INTERFACE_H_INCLUDED

#include "include/Utils/DataTypes/DataTypes.h"
#include "include/Utils/ErrorTypes/ErrorTypes.h"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "boost/shared_array.hpp"

#include "boost/foreach.hpp"
#include <string>
#include <map>

namespace SE
{

std::shared_ptr<boost::property_tree::ptree> StringToPropertyTree(std::string xmlCode, std::map<std::string, std::string> replaceMap = std::map<std::string, std::string>());

std::shared_ptr<boost::property_tree::ptree> FileToPropertyTree(boost::shared_array<char> xmlFileArr, size_t xmlFileSize, std::map<std::string, std::string> replaceMap = std::map<std::string, std::string>());

std::shared_ptr<boost::property_tree::ptree> FileToPropertyTree(const std::string& fileName, std::map<std::string, std::string> replaceMap = std::map<std::string, std::string>());


class TSerializeInterface
{
public:
	virtual void Serialize(boost::property_tree::ptree& propertyTree)
	{
	}

};

template <typename TVALUE>
TVALUE GetValueFromSubtree(const boost::property_tree::ptree::value_type& subTree, const std::string& path)
{
	return subTree.second.get<TVALUE>(path);
}

template <>
vec2 GetValueFromSubtree(const boost::property_tree::ptree::value_type& subTree, const std::string& path);

template <>
vec3 GetValueFromSubtree(const boost::property_tree::ptree::value_type& subTree, const std::string& path);

template <>
vec4 GetValueFromSubtree(const boost::property_tree::ptree::value_type& subTree, const std::string& path);


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
			TVALUE value = GetValueFromSubtree<TVALUE>(subTree, "<xmlattr>.value");

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