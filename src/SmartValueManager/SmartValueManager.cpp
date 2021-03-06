#include "include/Engine.h"

namespace SE
{

TSmartValue& TSmartValueManager::GetSmartValue(const std::string& dictName, const std::string& valueName)
{
	return DisctionariesMap[dictName][valueName];
}


void TSmartValueManager::Serialize(boost::property_tree::ptree& propertyTree)
{
	BOOST_FOREACH(auto& subTree, propertyTree.begin()->second)
	{
		std::string dictName = subTree.second.get<std::string>("<xmlattr>.name");

		BOOST_FOREACH(auto& subSubTree, subTree.second)
		{
			if (subSubTree.first == "Record")
			{
				std::string key = subSubTree.second.get<std::string>("<xmlattr>.key");
	
				std::string type = subSubTree.second.get<std::string>("<xmlattr>.type", "");

				if (type == "int")
				{
					DisctionariesMap[dictName][key] = std::shared_ptr<TBindableVar<int>>(new TBindableVar<int>(subSubTree.second.get<int>("<xmlattr>.value")));
				}
				else if (type == "bool")
				{
					DisctionariesMap[dictName][key] = std::shared_ptr<TBindableVar<bool>>(new TBindableVar<bool>(subSubTree.second.get<bool>("<xmlattr>.value")));
				}
				else if (type == "float")
				{
					DisctionariesMap[dictName][key] = std::shared_ptr<TBindableVar<float>>(new TBindableVar<float>(subSubTree.second.get<float>("<xmlattr>.value")));
				}
				else if (type == "Vector2f")
				{
					DisctionariesMap[dictName][key] = std::shared_ptr<TBindableVar<Vector2f>>(new TBindableVar<Vector2f>(StringToVec2(subSubTree.second.get<std::string>("<xmlattr>.value"))));
				}
				else if (type == "Vector3f")
				{
					DisctionariesMap[dictName][key] = std::shared_ptr<TBindableVar<Vector3f>>(new TBindableVar<Vector3f>(StringToVec3(subSubTree.second.get<std::string>("<xmlattr>.value"))));
				}
				else if (type == "Vector4f")
				{
					DisctionariesMap[dictName][key] = std::shared_ptr<TBindableVar<Vector4f>>(new TBindableVar<Vector4f>(StringToVec4(subSubTree.second.get<std::string>("<xmlattr>.value"))));
				}
				else
				{
					DisctionariesMap[dictName][key] = std::shared_ptr<TBindableVar<std::string>>(new TBindableVar<std::string>(subSubTree.second.get<std::string>("<xmlattr>.value")));
				}
			}
		}
	}
}

} //namespace SE