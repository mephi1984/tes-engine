#ifndef SHADER_MANAGER_H_INCLUDED
#define SHADER_MANAGER_H_INCLUDED

/*
This code contains shader manager
*/

#include <stack>

#ifdef TARGET_WIN32
#include "../OpenGlExt/OpenGlExt.h"
#endif

#include "include/Utils/Utils.h"

namespace SE
{

//==============================================
//=============== CONST AREA ===================
//==============================================


//Global uniforms (must remain same from shader to shader)
//If you add a new one - put it to SetCurrentShader() too
const std::string CONST_STRING_TEXTURE_UNIFORM = "Texture";
const std::string CONST_STRING_NORMALMAP_UNIFORM = "NormalMap";
const std::string CONST_STRING_SHADOWMAPGLOBAL_UNIFORM = "ShadowMapGlobal";
const std::string CONST_STRING_SHADOWMAPLOCAL_UNIFORM = "ShadowMapLocal";
const std::string CONST_STRING_ENV_UNIFORM = "Env";
const std::string CONST_STRING_NORMALMAPEXISTS_UNIFORM = "NormalMapExists";

const std::string CONST_STRING_LIGHT_POS_UNIFORM = "LightPos";
const std::string CONST_STRING_LIGHT_DIRECTION_UNIFORM = "LightDirection";
const std::string CONST_STRING_LIGHT_COLOR_UNIFORM = "LightColor";

const std::string CONST_STRING_CAMPOS_UNIFORM = "CamPos";
const std::string CONST_STRING_SHADOWCLAMPVALUE_UNIFORM = "ShadowClampValue";

const std::string CONST_STRING_FOG_BEGIN_DISTANCE_UNIFORM = "FogBeginDistance";
const std::string CONST_STRING_FOG_END_DISTANCE_UNIFORM = "FogEndDistance";
const std::string CONST_STRING_FOG_COLOR_UNIFORM = "FogColor";

const std::string CONST_STRING_MATERIAL_COLOR_UNIFORM = "MaterialColor";
const std::string CONST_STRING_TRANSPARENCY_UNIFORM = "Transparency";

//For Halibut Render
const std::string CONST_STRING_HALIBUT_PROJECTION_MATRIX_UNIFORM = "ProjectionMatrix";

//Local uniforms (need to be setup before each draw)
const std::string CONST_STRING_MODELROTATEMATRIX_UNIFORM = "ModelRotateMatrix";
const std::string CONST_STRING_MODELTRANSLATEVECTOR_UNIFORM = "ModelTranslateVector";


//Attributes
const std::string CONST_STRING_POSITION_ATTRIB = "vPosition";
const std::string CONST_STRING_TEXCOORD_ATTRIB = "vTexCoord";

const std::string CONST_STRING_NORMAL_ATTRIB = "Normal";
const std::string CONST_STRING_TANGENT_ATTRIB = "Tangent";
const std::string CONST_STRING_BINORMAL_ATTRIB = "Binormal";

//======================================

class TShaderManager; //see below



class TShaderResource
{
protected:
	cardinal ShaderProgram;

	std::map<std::string,cardinal,std::less<std::string> >  UniformList;
	std::map<std::string,cardinal,std::less<std::string> >  AttribList;

public:
	TShaderResource();
	~TShaderResource();

	bool CompileShader(boost::shared_array<char> vertexCode, boost::shared_array<char> fragmentCode);
	void FreeShader();

	int GetAttribIndex(const std::string& attribName);
	int GetUniformIndex(const std::string& uniformName);

	friend void RenderUniform1f(const std::string& uniformName, const float value);
	friend void RenderUniform3fv(const std::string& uniformName, const float* value);
	friend void RenderUniform4fv(const std::string& uniformName, const float* value);
	friend void RenderUniformMatrix3fv(const std::string& uniformName, bool transpose, const float* value);
	friend void RenderUniformMatrix4fv(const std::string& uniformName, bool transpose, const float* value);
	friend void RenderUniform1i(const std::string& uniformName,  const int value);

	friend void VertexAttrib2fv(const std::string& attribName, const float* value);
	friend void VertexAttrib3fv(const std::string& attribName, const float* value);
	friend void VertexAttribPointer2fv(const std::string& attribName,int stride, const char* pointer);
	friend void VertexAttribPointer3fv(const std::string& attribName,int stride, const char* pointer);
	
	friend void EnableVertexAttribArray(const std::string& attribName);
	friend void DisableVertexAttribArray(const std::string& attribName);
	

	#ifdef TARGET_WIN32
	friend void RefreshAttribBuffer2fv(const std::string& attribName, std::map<std::string, std::vector<vec2> >& vec2CoordArr);
	friend void RefreshAttribBuffer3fv(const std::string& attribName, std::map<std::string, std::vector<vec3> >& vec3CoordArr);
	#endif
	
	friend class TShaderManager;

};

class TShaderManager : public TSerializeInterface
{
protected:
	std::map<std::string, std::shared_ptr<TShaderResource>, std::less<std::string> >  ShaderList;
	
	std::stack<std::string> ShaderNameStack;
	
	//To be called only by SalmonRander/HalibutRender!!!
	void PushShader(const std::string& shaderName);
	void PopShader();
public:
	TShaderManager() {}
	~TShaderManager();
	std::shared_ptr<TShaderResource> GetCurrentShader();
	
	virtual void Serialize(boost::property_tree::ptree& propertyTree);

	bool AddShader(const std::string& shaderName, const std::string& vertexFileName, const std::string& fragmentFileName);

	void Clear();

	friend class TRendererInterface;

};

//Dont forget to make them friend to TShaderResource 
void RenderUniform1f(const std::string& uniformName, const float value);
void RenderUniform3fv(const std::string& uniformName, const float* value);
void RenderUniform4fv(const std::string& uniformName, const float* value);
void RenderUniformMatrix3fv(const std::string& uniformName, bool transpose, const float* value);
void RenderUniformMatrix4fv(const std::string& uniformName, bool transpose, const float* value);
void RenderUniform1i(const std::string& uniformName, const int value);

void VertexAttrib2fv(const std::string& attribName, const float* value);
void VertexAttrib3fv(const std::string& attribName, const float* value);
void VertexAttribPointer2fv(const std::string& attribName, int stride, const char* pointer);
void VertexAttribPointer3fv(const std::string& attribName, int stride, const char* pointer);

void EnableVertexAttribArray(const std::string& attribName);
void DisableVertexAttribArray(const std::string& attribName);

#ifdef TARGET_WIN32
void RefreshAttribBuffer2fv(const std::string& attribName, std::map<std::string, std::vector<vec2> >& vec2CoordArr);
void RefreshAttribBuffer3fv(const std::string& attribName, std::map<std::string, std::vector<vec3> >& vec3CoordArr);
#endif

} //namespace SE

#endif
