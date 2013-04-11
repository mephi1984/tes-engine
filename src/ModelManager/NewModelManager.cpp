#include "include/ModelManager/NewModelManager.h"

#include "include/Utils/Utils.h"
#include "include/Engine.h"

namespace SE
{

const int CONST_DATA_STACK_SIZE = 20;

void TFlexModelResource::Serialize(boost::property_tree::ptree& propertyTree)
{

	BOOST_FOREACH(auto& ptree, propertyTree.get_child("Model.Vec3ArrMap"))
	{

		std::string name = ptree.second.get<std::string>("<xmlattr>.name");


		BOOST_FOREACH(auto& vec3tree, ptree.second.get_child(""))
		{
			if (vec3tree.first == "Vec3")
			{
				DataTriangleList.Vec3CoordArr[name].push_back(StringToVec3(vec3tree.second.get<std::string>("")));
			}
		}
	}

	BOOST_FOREACH(auto& ptree, propertyTree.get_child("Model.Vec2ArrMap"))
	{
		std::string name = ptree.second.get<std::string>("<xmlattr>.name");

		BOOST_FOREACH(auto& vec2tree, ptree.second.get_child(""))
		{
			if (vec2tree.first == "Vec2")
			{
				DataTriangleList.Vec2CoordArr[name].push_back(StringToVec2(vec2tree.second.get<std::string>("")));
			}
		}
	}

	if (propertyTree.find("Model")->second.find("SamplerMap") != propertyTree.not_found())
	{
	BOOST_FOREACH(auto& ptree, propertyTree.get_child("Model.SamplerMap")) //Xperimental - fix it!
	{
		if (ptree.first == "Sampler")
		{
			std::string name = ptree.second.get<std::string>("<xmlattr>.name");
			std::string value = ptree.second.get<std::string>("<xmlattr>.value");

			SamplerMap[name] = value;

		}
	}
	}

}



void TFlexModel::PushModelDataTriangleList()
{
	if (DataTriangleListStack.size() == CONST_DATA_STACK_SIZE)
	{
		throw ErrorToLog("Error in TFlexModel::PushModelDataTriangleList - stack overflow!");
	}

	DataTriangleListStack.push(RenderPair.second.Data);
}

void TFlexModel::PopModelDataTriangleList()
{
	if (DataTriangleListStack.size() == 0)
	{
		throw ErrorToLog("Error in TFlexModel::PushModelDataTriangleList - stack underflow!");
	}

	RenderPair.second.Data = DataTriangleListStack.top();

	DataTriangleListStack.pop();
}

void TFlexModel::MoveModel(const vec3& v)
{
	MoveDataTriangleList(RenderPair.second.Data, v);
}

void TFlexModel::RotateModel(const mat3& r)
{
	RotateDataTriangleList(RenderPair.second.Data, r);
}

void TFlexModel::ScaleModel(float s)
{
	ScaleDataTriangleList(RenderPair.second.Data, s);
}

void TFlexModel::ScaleModel(const vec3& s)
{
	ScaleDataTriangleList(RenderPair.second.Data, s);
}

void TFlexModel::RefreshBuffer()
{
	RenderPair.second.RefreshBuffer();
}

void TFlexModel::Draw()
{
	TRenderParamsSetter setter(RenderPair.first);

	Renderer->DrawTriangleList(RenderPair.second);
}

TDataTriangleList& TFlexModel::GetDataTriangleList()
{
	
	return RenderPair.second.Data;
}

void TFlexModelManager::LoadModelFromXml(std::string xmlFileName)
{
	TFlexModelResource model;

	std::string modelName = GetFileName(xmlFileName);
	std::string modelPath = GetFilePath(xmlFileName);

	model.Serialize(*FileToPropertyTree(xmlFileName));

	FlexModelResourceMap[modelName] = model;
}

TFlexModel TFlexModelManager::InstanciateModel(const std::string& modelName)
{
	if (FlexModelResourceMap.count(modelName) == 0)
	{
		throw ErrorToLog("Model not loaded: " + modelName);
	}

	TFlexModel model;

	model.RenderPair.second.Data = FlexModelResourceMap[modelName].DataTriangleList;

	model.RenderPair.second.RefreshBuffer();

	model.RenderPair.first.SamplerMap = FlexModelResourceMap[modelName].SamplerMap;

	return model;
}



} //namespace SE