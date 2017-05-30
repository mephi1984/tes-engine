#include "include/Engine.h"
#include "boost/lexical_cast.hpp"

namespace SE
{

TRenderParamsSetter::TRenderParamsSetter(const TRenderParams& renderParams)
	: RenderParams(renderParams)
{
	AssertIfInMainThread();

	if (RenderParams.ShaderName != "")
	{
		Renderer->PushShader(RenderParams.ShaderName);
	
	}
	else
	{
		Renderer->SetUniforms();
	}

	//Xperimental -- NEED TO DO SOMETHING WITH THIS, later!
	glActiveTexture(GL_TEXTURE1);

	if (RenderParams.SamplerMap.count(CONST_STRING_NORMALMAP_UNIFORM))
	{
		glBindTexture(GL_TEXTURE_2D, ResourceManager->TexList[RenderParams.SamplerMap[CONST_STRING_NORMALMAP_UNIFORM]]);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glActiveTexture(GL_TEXTURE0);

	if (RenderParams.SamplerMap.count(CONST_STRING_TEXTURE_UNIFORM))
	{
		glBindTexture(GL_TEXTURE_2D, ResourceManager->TexList[RenderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM]]);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	for (std::map<std::string, float>::iterator i = RenderParams.FloatMap.begin(); i != RenderParams.FloatMap.end(); ++i)
	{
		RenderUniform1f(i->first, i->second);
	}

	for (std::map<std::string, Vector4f>::iterator i = RenderParams.Vec4Map.begin(); i != RenderParams.Vec4Map.end(); ++i)
	{
		RenderUniform4fv(i->first, i->second.data());
	}
}

TRenderParamsSetter::~TRenderParamsSetter()
{
	if (RenderParams.ShaderName != "")
	{
		Renderer->PopShader();
	}
}


void TRenderParams::Serialize(boost::property_tree::ptree& propertyTree)
{
	if (propertyTree.count("Shader") != 0)
	{
		ShaderName = propertyTree.get<std::string>("Shader");
	}

	if (propertyTree.count("SamplerMap") != 0)
	{
		SamplerMap.clear();

		TMapParser<std::string, std::string> mapParser;

		mapParser.Serialize(propertyTree.get_child("SamplerMap"));

		SamplerMap = mapParser.Map;
	}

	if (propertyTree.count("FloatMap") != 0)
	{
		SamplerMap.clear();

		TMapParser<std::string, float> mapParser;

		mapParser.Serialize(propertyTree.get_child("FloatMap"));

		FloatMap = mapParser.Map;
	}

	if (propertyTree.count("Vec4Map") != 0)
	{
		SamplerMap.clear();

		TMapParser<std::string, std::string> mapParser;

		mapParser.Serialize(propertyTree.get_child("Vec4Map"));

		for (std::map<std::string, std::string>::iterator i = mapParser.Map.begin(); i != mapParser.Map.end(); ++i)
		{
			Vec4Map[i->first] = StringToVec4(i->second);
		}

	}
}

} //namespace SE