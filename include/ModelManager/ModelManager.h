#ifndef MODEL_MANAGER_H_INCLUDED
#define MODEL_MANAGER_H_INCLUDED

/*
This code contains manager to operate static Lite Models

Use global variable ResourceManager to get access to model manager
*/


#include "include/Utils/Utils.h"

namespace SE
{

class TLiteModel;

class TAnimModel;

/* Class to work with Lite Model Resource from model file. 
Lite Model Resource can be applied to a number of in-game Lite Models */
class TLiteModelResource
{
protected:
	
	std::vector<vec3> VertexArr;
	std::vector<vec3> NormalArr;
	std::vector<vec3> TangentArr;
	std::vector<vec3> BinormalArr;
	std::vector<vec2> TexCoordArr;

	int TriangleCount;
	std::string Texname;
	std::string Normtexname;
	std::string PathToResource;
public:
	TLiteModelResource() : TriangleCount(0), PathToResource("") { }
	~TLiteModelResource();

	bool LoadModel(boost::shared_array<byte> filePointer, cardinal fileSize);

	void FreeModel();

	void SetPathToResource(const std::string& pathToResource);

	friend class TLiteModel; 
};

typedef std::map<std::string, TLiteModelResource> TLiteModelResourceMap;

/* Class to work with Lite Model in game. 
Lite Model could be translated/scaled/rotated, also Lite Model have its own model matrix */
class TLiteModel
{
protected:
	std::string Modelname;
	
	cardinal TriangleCount;
	cardinal TexID;
	cardinal NormTexID;

	TTriangleList TriangleList;

	mat3 ShaderRotateMatrix;
	vec3 ShaderTranslateVector;
	bool Loaded;

public:
	TLiteModel() : Modelname(""), TriangleCount(0), TexID(0), NormTexID(0),
		Loaded(false), ShaderRotateMatrix(IdentityMatrix), ShaderTranslateVector(ZeroVec3) {}
	
	~TLiteModel();
	
	bool LoadModel(const std::string& s);
	//Load model from Resource Manager with name s
	//Model must already be loaded as a Lite Model Resource
	
	void FreeModel();
	//Clear all data from model

	//Long-term model processing methods
	//To rotate/translate model quickly use SetShaderRotateMatrix/SetShaderTranslateVector
	//If you use VBO to draw model, you must call UpdateVBO() after you finished model translation/rotation/scaling
	void MoveModel(const vec3& v);
	void RotateModel(const mat3& r);
	void ScaleModel(float s);
	void ScaleModel(const vec3& s);

	//Sets translate vector to do fast model translation. No need to call UpdateVBO after
	void SetShaderTranslateVector(const vec3& v);

	//Sets rotate matrix to do fast model rotation. No need to call UpdateVBO after
	void SetShaderRotateMatrix(const mat3& m);

	//Method to update VBO after translation / rotation / scaling 
	void UpdateVBO();

	//Method to draw model in immediate mode. Slow method
	void DrawImmediate();

	//Method to draw model using VBO. Faster than previous but must be called between SalmonRender->BeginDrawVBO() and SalmonRender->EndDrawVBO()
	void DrawVBO();

	friend class TAnimModel; //For animation;
};

class TModelManager
{
protected:
	TLiteModelResourceMap LiteModelMap;
public:
	TModelManager() {}
	~TModelManager();

	//Clean up
	void Clear();

	//Get access to Lite Model resource
	TLiteModelResource& operator[](const std::string& s);

	//Add lite model resource from file
	bool AddLiteModel(const std::string& filename, const std::string& pathToResources = "");

	//Delete lite model resource 
	void DeleteLiteModel(const std::string& modelname);
};

} //namespace SE

#endif