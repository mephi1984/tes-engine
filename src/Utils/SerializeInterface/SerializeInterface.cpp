#include "include/Utils/SerializeInterface/SerializeInterface.h"
#include "include/Engine.h"

#include <boost/algorithm/string.hpp>


namespace SE
{



template <>
vec2 GetValueFromSubtree(const boost::property_tree::ptree::value_type& subTree, const std::string& path)
{
	return StringToVec2(subTree.second.get<std::string>(path));
}

template <>
vec3 GetValueFromSubtree(const boost::property_tree::ptree::value_type& subTree, const std::string& path)
{
	return StringToVec3(subTree.second.get<std::string>(path));
}

template <>
vec4 GetValueFromSubtree(const boost::property_tree::ptree::value_type& subTree, const std::string& path)
{
	return StringToVec4(subTree.second.get<std::string>(path));
}




void ReplaceText(boost::property_tree::ptree& propertyTree, std::map<std::string, std::string>& replaceMap)
{
	for (boost::property_tree::ptree::iterator i = propertyTree.begin(); i != propertyTree.end(); )
	{
		BOOST_FOREACH(auto& itr, replaceMap)
		{
			std::string param = i->second.get<std::string>("");
			if (param != "")
			{
				boost::algorithm::replace_all(param, itr.first, itr.second);
				i->second.put("", param);
			}
		}
		

		if (i != propertyTree.end())
		{
			i++;
		}
	}
}

boost::property_tree::ptree::iterator ReplaceIncludeFile(boost::property_tree::ptree& propertyTree, boost::property_tree::ptree::iterator propertyTreeItr, std::map<std::string, std::string> replaceMap)
{
	boost::shared_array<char> xmlFileArr;
	cardinal xmlFileSize;

	std::string fileName = propertyTreeItr->second.get<std::string>("<xmlattr>.file");

	for (boost::property_tree::ptree::iterator i = propertyTreeItr->second.begin(); i != propertyTreeItr->second.end(); ++i)
	{
		if (i->first == "Param")
		{
			std::string paramName = i->second.get<std::string>("<xmlattr>.name");
			std::string paramValue = i->second.get<std::string>("<xmlattr>.value");

			//If value already consist template that can be expanded:
			while (replaceMap.count(paramValue) != 0)
			{
				paramValue = replaceMap[paramValue];
			}

			replaceMap[paramName] = paramValue;
		}
	}

#ifdef UTILS_ENGINE
	xmlFileArr = CreateMemFromFile<char>(fileName, xmlFileSize);
#else
	xmlFileArr = CreateMemFromFile<char>(ST::PathToResources + fileName, xmlFileSize);
#endif

	std::string xmlString = std::string(&xmlFileArr[0], &xmlFileArr[xmlFileSize]);

	std::shared_ptr<boost::property_tree::ptree> includeFileTree = StringToPropertyTree(xmlString, replaceMap);

	boost::property_tree::ptree::iterator newItr = propertyTree.erase(propertyTreeItr);
	
	propertyTree.insert(newItr, includeFileTree->begin(), includeFileTree->end());

	if (newItr == propertyTree.begin())
	{
		throw ErrorToLog("Property tree logic error - don't know how to handle :(");
	}

	newItr--;
	
	return newItr;
}

void GetReplaceMapRecoursively(boost::property_tree::ptree& propertyTree, std::map<std::string, std::string> replaceMap)
{
	ReplaceText(propertyTree, replaceMap);

	for (boost::property_tree::ptree::iterator i = propertyTree.begin(); i != propertyTree.end(); )
	{
		std::map<std::string, std::string> localMap = replaceMap;

		if (i->first == "Include")
		{
			i = ReplaceIncludeFile(propertyTree, i, localMap);
		}
		else
		{
			GetReplaceMapRecoursively(i->second, localMap);
		}

		BOOST_FOREACH(auto& itr, localMap)
		{
			if (localMap.count(itr.second) != 0)
			{
				localMap[itr.first] = localMap[itr.second];
			}
		}

		if (i != propertyTree.end())
		{
			i++;
		}
	}

	
}


std::shared_ptr<boost::property_tree::ptree> StringToPropertyTree(std::string xmlCode, std::map<std::string, std::string> replaceMap)
{
	std::shared_ptr<boost::property_tree::ptree> propertyTree(new boost::property_tree::ptree);

	std::stringstream stream(xmlCode);
	
	try
	{
		boost::property_tree::read_xml(stream, *propertyTree);

		//Parsing <include/> tag
		GetReplaceMapRecoursively(*propertyTree, replaceMap);


	}
	catch(boost::property_tree::xml_parser_error)
	{
		throw ErrorToLog("ERROR: xml parse error");
	}

	return propertyTree;

}

std::shared_ptr<boost::property_tree::ptree> FileToPropertyTree(boost::shared_array<char> xmlFileArr, cardinal xmlFileSize, std::map<std::string, std::string> replaceMap)
{
	std::string xmlCode = std::string(&xmlFileArr[0], &xmlFileArr[xmlFileSize]);
	
	return StringToPropertyTree(xmlCode, replaceMap);
}

std::shared_ptr<boost::property_tree::ptree> FileToPropertyTree(const std::string& fileName, std::map<std::string, std::string> replaceMap)
{
	cardinal byteCount;

#ifdef UTILS_ENGINE
	boost::shared_array<char> file = CreateMemFromFile<char>(fileName, byteCount);
#else
	boost::shared_array<char> file = CreateMemFromFile<char>(ST::PathToResources + fileName, byteCount);
#endif
	
	return FileToPropertyTree(file, byteCount, replaceMap);
}



} //namespace SE
