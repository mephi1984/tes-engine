#include "include/ShaderManager/ShaderManager.h"

#include "include/Utils/Utils.h"
#include "include/Engine.h"

namespace SE
{

//===================================================
//============== SHADER RESOURCE ====================
//===================================================



TShaderResource::TShaderResource()
	: ShaderProgram(0)
{
}

TShaderResource::~TShaderResource() 
{ 
	
	*Console<<"ResourceManager::ShaderManager::Shader deleting";
	FreeShader();
	
}

bool TShaderResource::CompileShader(boost::shared_array<char> vertexCode, boost::shared_array<char> fragmentCode)
{

	FreeShader();

	const int CONST_INFOLOG_LENGTH = 256;

	char infoLog[CONST_INFOLOG_LENGTH];
	int infoLogLength;

	int vertexShaderCompiled;
	int fragmentShaderCompiled;
	int programLinked;

	cardinal vertexShader;
	cardinal fragmentShader;

	int vertexCodeLength = strlen(vertexCode.get());
	int fragmentCodeLength = strlen(fragmentCode.get());

	const char* vc = &vertexCode[0];
	const char* fc = &fragmentCode[0];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &(vc), &vertexCodeLength);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &(fc), &fragmentCodeLength);
	
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderCompiled);
	glGetShaderInfoLog(vertexShader, CONST_INFOLOG_LENGTH, &infoLogLength, infoLog);
	*Console<<"ResourceManager::ShaderManager::Shader "+std::string(infoLog);

	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderCompiled);
	glGetShaderInfoLog(fragmentShader, CONST_INFOLOG_LENGTH, &infoLogLength, infoLog);
	*Console<<"ResourceManager::ShaderManager::Shader " + std::string(infoLog);

	if (!vertexShaderCompiled || !fragmentShaderCompiled)
		return false;

	ShaderProgram = glCreateProgram();
	*Console<<"Shader program is - " + tostr(ShaderProgram);
	glAttachShader(ShaderProgram, vertexShader);
	glAttachShader(ShaderProgram, fragmentShader);

	glLinkProgram(ShaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &programLinked);
	glGetProgramInfoLog(ShaderProgram, CONST_INFOLOG_LENGTH, &infoLogLength, infoLog);

	*Console<<"ResourceManager::ShaderManager::Shader "+std::string(infoLog);

	if (!programLinked)
	{
		ShaderProgram = 0;
		return false;
	}
	
	
	int dummySize;			//Dummy
	int dummyLen;			//Dummy
	cardinal dummyType;		//Dummy

	//================= Parsing all uniforms ================

	int activeUniforms;

	const int CONST_UNIFORM_NAME_LENGTH = 256;
	char uniformName[CONST_UNIFORM_NAME_LENGTH];

	glGetProgramiv(ShaderProgram, GL_ACTIVE_UNIFORMS, &activeUniforms);

	for (int i = 0; i < activeUniforms; i++)
	{
		glGetActiveUniform(ShaderProgram, i, CONST_UNIFORM_NAME_LENGTH, &dummyLen, &dummySize, &dummyType, uniformName);
		UniformList[uniformName] = glGetUniformLocation(ShaderProgram, uniformName);
	}

	//================= Parsing all attributes ================
	int activeAttribs;

	const int CONST_ATTRIB_NAME_LENGTH = 256;
	char attribName[CONST_ATTRIB_NAME_LENGTH];

	glGetProgramiv(ShaderProgram, GL_ACTIVE_ATTRIBUTES, &activeAttribs);

	for (int i = 0; i < activeAttribs; i++)
	{
		glGetActiveAttrib(ShaderProgram, i, CONST_ATTRIB_NAME_LENGTH, &dummyLen, &dummySize, &dummyType, attribName);
		AttribList[attribName] = glGetAttribLocation(ShaderProgram, attribName);
	}

	return true;
}

void TShaderResource::FreeShader()
{

	if (ShaderProgram != 0)
	{
#ifdef TARGET_ANDROID
		if (glIsProgram(ShaderProgram))
		{
			glDeleteProgram(ShaderProgram);
		}
#else
		glDeleteProgram(ShaderProgram);
#endif
		ShaderProgram = 0;
	}

	UniformList.clear();
	AttribList.clear();
}

int TShaderResource::GetAttribIndex(const std::string& attribName)
{
	if (AttribList.count(attribName) > 0)
		return AttribList[attribName];
	else
		return -1;
}

int TShaderResource::GetUniformIndex(const std::string& uniformName)
{
	if (UniformList.count(uniformName) > 0)
		return UniformList[uniformName];
	else
		return -1;
}




//===================================================
//=============== SHADER MANAGER ====================
//===================================================


TShaderManager::~TShaderManager()
{
	Clear();
	*Console<<"ResourceManager::ShaderManager deleting"; 
}

std::shared_ptr<TShaderResource> TShaderManager::GetCurrentShader() 
{
	if (ShaderNameStack.size() == 0)
	{
		throw ErrorToLog("GetCurrentShader: Shader is not selected!");
	}
	
	return ShaderList[ShaderNameStack.top()]; 
}

void TShaderManager::Serialize(boost::property_tree::ptree& propertyTree)
{
	
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, propertyTree.get_child("Shaders"))
	{
		std::string shaderName = v.second.get<std::string>("Name");
		std::string vertexFileName = v.second.get<std::string>("VertexFileName");
		std::string fragmentFileName = v.second.get<std::string>("FragmentFileName");

		AddShader(shaderName, vertexFileName, fragmentFileName);

	}
}

bool TShaderManager::AddShader(const std::string& shaderName, const std::string& vertexFileName, const std::string& fragmentFileName)
{

	std::string pathToResources = ST::PathToResources;

	if (ShaderList.count(shaderName) > 0)
	{
		*Console<<"ResourceManager::ShaderManager ERROR shader already exists: "+shaderName;
		return false;
	}

	*Console<<"Prepare to add shader "+shaderName;
	cardinal fSize;
	boost::shared_array<char> fPointer;
	boost::shared_array<char> vertexCode;
	boost::shared_array<char> fragmentCode;

	fPointer = CreateMemFromFile<char>(pathToResources + vertexFileName, fSize);
	
	vertexCode = boost::shared_array<char>(new char [fSize + 1]);

	memcpy(vertexCode.get(), fPointer.get(), fSize);

	vertexCode[fSize] = 0;
	
	fPointer = CreateMemFromFile<char>(pathToResources + fragmentFileName, fSize);

	fragmentCode = boost::shared_array<char>(new char [fSize + 1]);
	
	memcpy(fragmentCode.get(), fPointer.get(), fSize);
	
	fragmentCode[fSize] = 0;

	std::shared_ptr<TShaderResource> shaderResource(new TShaderResource);

	ShaderList[shaderName] = shaderResource;
	
	if (!ShaderList[shaderName]->CompileShader(vertexCode, fragmentCode))
	{
		*Console<<"ResourceManager::ShaderManager ERROR shader could not be compiled or linked: "+shaderName;
		return false;
	}

	*Console<<"ResourceManager::ShaderManager shader loaded: "+shaderName;

	return true;
}

void TShaderManager::Clear() 
{
	glUseProgram(0);
	ShaderList.clear();
	*Console<<"ResourceManager::ShaderManager cleared";
}


void TShaderManager::PushShader(const std::string& shaderName)
{

	if (ShaderList.count(shaderName) == 0) 
		throw ErrorShaderNotExists(shaderName);

	if (ShaderNameStack.size() >= CONST_MAX_STACK_SIZE)
		throw ErrorShaderStackOverflow();

	
	if (ShaderNameStack.size() == 0 || ShaderNameStack.top() != shaderName)
	{
		glUseProgram(ShaderList[shaderName]->ShaderProgram);
	}

	ShaderNameStack.push(shaderName);
}


void TShaderManager::PopShader()
{
	if (ShaderNameStack.size() <= 1)
	{
		throw ErrorToLog("Shader stack underflow!!");
	}
	std::string oldShaderName = ShaderNameStack.top();

	ShaderNameStack.pop();
	std::string shaderName = ShaderNameStack.top();
	if (oldShaderName != shaderName)
	{
		glUseProgram(ShaderList[shaderName]->ShaderProgram);
	}
}

//========================================================
//======================== Utilities =====================
//========================================================

void RenderUniform1f(const std::string& uniformName, const float value)
{
	if (ResourceManager != NULL)
			if (ResourceManager->ShaderManager.GetCurrentShader()->UniformList.count(uniformName) > 0)
				glUniform1fv(ResourceManager->ShaderManager.GetCurrentShader()->UniformList[uniformName],1,&value);
}

void RenderUniform3fv(const std::string& uniformName, const float* value)
{
	if (ResourceManager != NULL)
			if (ResourceManager->ShaderManager.GetCurrentShader()->UniformList.count(uniformName) > 0)
				glUniform3fv(ResourceManager->ShaderManager.GetCurrentShader()->UniformList[uniformName],1,value);
}

void RenderUniform4fv(const std::string& uniformName, const float* value)
{
	if (ResourceManager != NULL)
			if (ResourceManager->ShaderManager.GetCurrentShader()->UniformList.count(uniformName) > 0)
				glUniform4fv(ResourceManager->ShaderManager.GetCurrentShader()->UniformList[uniformName],1,value);
}


void RenderUniformMatrix3fv(const std::string& uniformName,bool transpose, const float* value)
{
	if (ResourceManager != NULL)	
			if (ResourceManager->ShaderManager.GetCurrentShader()->UniformList.count(uniformName) > 0)
				glUniformMatrix3fv(ResourceManager->ShaderManager.GetCurrentShader()->UniformList[uniformName],1,transpose,value);
			
}

void RenderUniformMatrix4fv(const std::string& uniformName,bool transpose, const float* value)
{
	if (ResourceManager != NULL)	
			if (ResourceManager->ShaderManager.GetCurrentShader()->UniformList.count(uniformName) > 0)
				glUniformMatrix4fv(ResourceManager->ShaderManager.GetCurrentShader()->UniformList[uniformName],1,transpose,value);
			
}


void RenderUniform1i(const std::string& uniformName, const int value)
{
	if (ResourceManager != NULL)
			if (ResourceManager->ShaderManager.GetCurrentShader()->UniformList.count(uniformName) > 0)
				glUniform1i(ResourceManager->ShaderManager.GetCurrentShader()->UniformList[uniformName],value);
}

void VertexAttrib2fv(const std::string& attribName, const float* value)
{
	if (ResourceManager != NULL)
			if (ResourceManager->ShaderManager.GetCurrentShader()->AttribList.count(attribName) > 0)
				glVertexAttrib2fv(ResourceManager->ShaderManager.GetCurrentShader()->AttribList[attribName], value);
}

void VertexAttrib3fv(const std::string& attribName, const float* value)
{
	if (ResourceManager != NULL)
			if (ResourceManager->ShaderManager.GetCurrentShader()->AttribList.count(attribName) > 0)
				glVertexAttrib3fv(ResourceManager->ShaderManager.GetCurrentShader()->AttribList[attribName], value);
}

void VertexAttribPointer2fv(const std::string& attribName,int stride, const char* pointer)
{
	if (ResourceManager != NULL)
			if (ResourceManager->ShaderManager.GetCurrentShader()->AttribList.count(attribName) > 0)
				glVertexAttribPointer(ResourceManager->ShaderManager.GetCurrentShader()->AttribList[attribName], 2, GL_FLOAT, GL_FALSE, stride, pointer);
}

void VertexAttribPointer3fv(const std::string& attribName,int stride, const char* pointer)
{
	if (ResourceManager != NULL)
			if (ResourceManager->ShaderManager.GetCurrentShader()->AttribList.count(attribName) > 0)
				glVertexAttribPointer(ResourceManager->ShaderManager.GetCurrentShader()->AttribList[attribName], 3, GL_FLOAT, GL_FALSE, stride, pointer);
}

void EnableVertexAttribArray(const std::string& attribName)
{
	if (ResourceManager != NULL)
			if (ResourceManager->ShaderManager.GetCurrentShader()->AttribList.count(attribName) > 0)
				glEnableVertexAttribArray(ResourceManager->ShaderManager.GetCurrentShader()->AttribList[attribName]);
}

void DisableVertexAttribArray(const std::string& attribName)
{
	if (ResourceManager != NULL)
			if (ResourceManager->ShaderManager.GetCurrentShader()->AttribList.count(attribName) > 0)
				glDisableVertexAttribArray(ResourceManager->ShaderManager.GetCurrentShader()->AttribList[attribName]);
}


#ifdef TARGET_WIN32
void RefreshAttribBuffer2fv(const std::string& attribName, std::map<std::string, std::vector<vec2> >& vec2CoordArr)
{
	if (ResourceManager != NULL)
			if (ResourceManager->ShaderManager.GetCurrentShader()->AttribList.count(attribName) > 0)
				if (vec2CoordArr[attribName].size() > 0)
					glBufferData(GL_ARRAY_BUFFER, vec2CoordArr[attribName].size()*8, &(*vec2CoordArr[attribName].begin()), GL_STATIC_DRAW);
	
}

void RefreshAttribBuffer3fv(const std::string& attribName, std::map<std::string, std::vector<vec3> >& vec3CoordArr)
{
	if (ResourceManager != NULL)
			if (ResourceManager->ShaderManager.GetCurrentShader()->AttribList.count(attribName) > 0)
				if (vec3CoordArr[attribName].size() > 0)
					glBufferData(GL_ARRAY_BUFFER, vec3CoordArr[attribName].size()*12, &(*vec3CoordArr[attribName].begin()), GL_STATIC_DRAW);
}
#endif

} //namespace SE