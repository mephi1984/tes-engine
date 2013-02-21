#include "include/ModelManager/ModelManager.h"

#include "include/Utils/Utils.h"
#include "include/Engine.h"

namespace SE
{

//===================================================
//=========== LITE MODEL RESOURCE ===================
//===================================================

TLiteModelResource::~TLiteModelResource() 
{
	FreeModel();
	*Console<<"ResourceManager::ModelManager::LiteModelResource deleting";
}

bool TLiteModelResource::LoadModel(boost::shared_array<byte> filePointer, cardinal fileSize)
{

	FreeModel();

	cardinal triangleShift = *reinterpret_cast<cardinal*>(&filePointer[4]);

	TriangleCount = *reinterpret_cast<cardinal*>(&filePointer[0] + triangleShift - 4);

	int texnamelen = *reinterpret_cast<word*>(&filePointer[0] + 76);

	Texname = std::string(reinterpret_cast<char*>(&filePointer[78]), reinterpret_cast<char*>(&filePointer[78]) + texnamelen);

	int normtexnamelen = *reinterpret_cast<word*>(&filePointer[78]+texnamelen);

	Normtexname = std::string(reinterpret_cast<char*>(&filePointer[80]+texnamelen), reinterpret_cast<char*>(&filePointer[80]+texnamelen) + normtexnamelen);

	ResourceManager->TexList.AddTextureDirectly((PathToResource + std::string(Texname)));
	if (Normtexname != "")
	{
		ResourceManager->TexList.AddTextureDirectly((PathToResource + std::string(Normtexname)));
	}

	VertexArr.resize(TriangleCount * 3);
	NormalArr.resize(TriangleCount * 3);
	TangentArr.resize(TriangleCount * 3);
	BinormalArr.resize(TriangleCount * 3);
	TexCoordArr.resize(TriangleCount * 3);

	memcpy(&VertexArr[0], &filePointer[0] + triangleShift, TriangleCount * 36);
	memcpy(&NormalArr[0], &filePointer[0] + triangleShift + TriangleCount * 36, TriangleCount * 36);
	memcpy(&TangentArr[0], &filePointer[0] + triangleShift + TriangleCount * 72, TriangleCount * 36);
	memcpy(&BinormalArr[0], &filePointer[0] + triangleShift + TriangleCount * 108, TriangleCount * 36);
	memcpy(&TexCoordArr[0], &filePointer[0] + triangleShift + TriangleCount * 144, TriangleCount * 24);

	return true;

}

void TLiteModelResource::FreeModel()
{

	if (Texname != "")
	{
		ResourceManager->TexList.DeleteTexture(GetFileName(Texname));
		Texname = "";
	}

	if (Normtexname != "")
	{
		ResourceManager->TexList.DeleteTexture(GetFileName(Normtexname));
		Normtexname = "";
	}

	VertexArr.clear();
	NormalArr.clear();
	TangentArr.clear();
	BinormalArr.clear();
	TexCoordArr.clear();

}

void TLiteModelResource::SetPathToResource(const std::string& pathToResource)
{
	PathToResource = pathToResource;
}



//===================================================
//============= LITE MODEL REF ======================
//===================================================
TLiteModel::~TLiteModel() 
{ 
	FreeModel(); 
	*Console<<"Render::LiteModel model deleting: " + Modelname;
}

bool TLiteModel::LoadModel(const std::string& s)
{

	TLiteModelResource& res = ResourceManager->ModelManager[s];

	FreeModel();

	Modelname = s;

	TriangleCount = res.TriangleCount;

	if (res.Texname != "")
		TexID = ResourceManager->TexList[res.Texname];
	else
		TexID = 0;

	if (res.Normtexname != "")
		NormTexID = ResourceManager->TexList[res.Normtexname];
	else
		NormTexID = 0;


	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB] = res.VertexArr;
	TriangleList.Data.Vec3CoordArr[CONST_STRING_NORMAL_ATTRIB] = res.NormalArr;
	TriangleList.Data.Vec3CoordArr[CONST_STRING_TANGENT_ATTRIB] = res.TangentArr;
	TriangleList.Data.Vec3CoordArr[CONST_STRING_BINORMAL_ATTRIB] = res.BinormalArr;
	TriangleList.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB] = res.TexCoordArr;

	Loaded = true;

	return true;
}

void TLiteModel::FreeModel()
{
	Loaded = false;

	TriangleList.Data.Vec3CoordArr.clear();
	TriangleList.Data.Vec2CoordArr.clear();
#ifdef TARGET_WIN32
	TriangleList.VertBufferArr.clear();
#endif
	TriangleCount = 0;
	TexID = 0;
	NormTexID = 0;

	Modelname = "";
}


void TLiteModel::UpdateVBO()
{
	
	CheckGlError();

	TriangleList.RefreshBuffer();
	
	CheckGlError();

}

void TLiteModel::DrawImmediate()
{
	
#ifdef TARGET_WIN32
	
	if (Loaded)
	{
		
		int triangleCount = TriangleCount;

		if (NormTexID == 0)
		{
			RenderUniform1i(CONST_STRING_NORMALMAPEXISTS_UNIFORM, 0);
		}

		RenderUniformMatrix3fv(CONST_STRING_MODELROTATEMATRIX_UNIFORM, false, ShaderRotateMatrix.m);
		RenderUniform3fv(CONST_STRING_MODELTRANSLATEVECTOR_UNIFORM, ShaderTranslateVector.v);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, NormTexID);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexID);
		
		for (int i=0; i<triangleCount; i++)
		{

			

			glBegin(GL_TRIANGLES);
				VertexAttrib3fv(CONST_STRING_NORMAL_ATTRIB, (float*)(&TriangleList.Data.Vec3CoordArr[CONST_STRING_NORMAL_ATTRIB][3*i]));
				VertexAttrib3fv(CONST_STRING_TANGENT_ATTRIB, (float*)(&TriangleList.Data.Vec3CoordArr[CONST_STRING_TANGENT_ATTRIB][3*i]));
				VertexAttrib3fv(CONST_STRING_BINORMAL_ATTRIB, (float*)(&TriangleList.Data.Vec3CoordArr[CONST_STRING_BINORMAL_ATTRIB][3*i]));
				VertexAttrib2fv(CONST_STRING_TEXCOORD_ATTRIB, (float*)(&TriangleList.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][3*i]));
				VertexAttrib3fv(CONST_STRING_POSITION_ATTRIB, (float*)(&TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][3*i]));

				VertexAttrib3fv(CONST_STRING_NORMAL_ATTRIB, (float*)(&TriangleList.Data.Vec3CoordArr[CONST_STRING_NORMAL_ATTRIB][3*i + 1]));
				VertexAttrib3fv(CONST_STRING_TANGENT_ATTRIB, (float*)(&TriangleList.Data.Vec3CoordArr[CONST_STRING_TANGENT_ATTRIB][3*i + 1]));
				VertexAttrib3fv(CONST_STRING_BINORMAL_ATTRIB, (float*)(&TriangleList.Data.Vec3CoordArr[CONST_STRING_BINORMAL_ATTRIB][3*i + 1]));
				VertexAttrib2fv(CONST_STRING_TEXCOORD_ATTRIB, (float*)(&TriangleList.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][3*i + 1]));
				VertexAttrib3fv(CONST_STRING_POSITION_ATTRIB, (float*)(&TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][3*i + 1]));

				VertexAttrib3fv(CONST_STRING_NORMAL_ATTRIB, (float*)(&TriangleList.Data.Vec3CoordArr[CONST_STRING_NORMAL_ATTRIB][3*i + 2]));
				VertexAttrib3fv(CONST_STRING_TANGENT_ATTRIB, (float*)(&TriangleList.Data.Vec3CoordArr[CONST_STRING_TANGENT_ATTRIB][3*i + 2]));
				VertexAttrib3fv(CONST_STRING_BINORMAL_ATTRIB, (float*)(&TriangleList.Data.Vec3CoordArr[CONST_STRING_BINORMAL_ATTRIB][3*i + 2]));
				VertexAttrib2fv(CONST_STRING_TEXCOORD_ATTRIB, (float*)(&TriangleList.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][3*i + 2]));
				VertexAttrib3fv(CONST_STRING_POSITION_ATTRIB, (float*)(&TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][3*i + 2]));
			glEnd();
		}
		
	
		RenderUniform1i(CONST_STRING_NORMALMAPEXISTS_UNIFORM, 1);

	}
#endif
}

void TLiteModel::DrawVBO()
{
	CheckGlError();
	
	if (NormTexID == 0)
		RenderUniform1i(CONST_STRING_NORMALMAPEXISTS_UNIFORM,0);

	RenderUniformMatrix3fv(CONST_STRING_MODELROTATEMATRIX_UNIFORM,false,ShaderRotateMatrix.m);
	RenderUniform3fv(CONST_STRING_MODELTRANSLATEVECTOR_UNIFORM,ShaderTranslateVector.v);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,NormTexID);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,TexID);

	Renderer->DrawTriangleList(TriangleList);

	RenderUniform1i(CONST_STRING_NORMALMAPEXISTS_UNIFORM, 1);

	CheckGlError();
}




void TLiteModel::MoveModel(const vec3& v)
{
	for (cardinal i = 0; i < TriangleCount * 3; i++)
	{
		TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][i] += v;
	}
}

void TLiteModel::RotateModel(const mat3& r)
{

	for (cardinal i = 0; i < TriangleCount * 3; i++)
	{
		TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][i] = r * TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][i];
		TriangleList.Data.Vec3CoordArr[CONST_STRING_NORMAL_ATTRIB][i] = r * TriangleList.Data.Vec3CoordArr[CONST_STRING_NORMAL_ATTRIB][i];
		TriangleList.Data.Vec3CoordArr[CONST_STRING_TANGENT_ATTRIB][i] = r * TriangleList.Data.Vec3CoordArr[CONST_STRING_TANGENT_ATTRIB][i];
		TriangleList.Data.Vec3CoordArr[CONST_STRING_BINORMAL_ATTRIB][i] = r * TriangleList.Data.Vec3CoordArr[CONST_STRING_BINORMAL_ATTRIB][i];
	}
}

void TLiteModel::ScaleModel(float s)
{

	for (cardinal i = 0; i < TriangleCount * 3; i++)
	{
		TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][i] *= s;
	}

}


void TLiteModel::ScaleModel(const vec3& s)
{

	for (cardinal i = 0; i < TriangleCount * 3; i++)
	{
		TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][i].v[0] *= s.v[0];
		TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][i].v[1] *= s.v[1];
		TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][i].v[2] *= s.v[2];
	}

}

void TLiteModel::SetShaderTranslateVector(const vec3& v)
{
	ShaderTranslateVector = v;
}

void TLiteModel::SetShaderRotateMatrix(const mat3& m)
{
	ShaderRotateMatrix = m;
}


//===================================================
//=============== MODEL MANAGER =====================
//===================================================

TModelManager::~TModelManager() 
{ 

	Clear(); 

	*Console<<"ResourceManager::ModelManager deleting"; 
}

void TModelManager::Clear()
{
	if (LiteModelMap.size() != 0)
	{
		LiteModelMap.clear();
		*Console<<"ResourceManager::ModelManager cleared";
	}
}

TLiteModelResource& TModelManager::operator[](const std::string& s)
{
	if (LiteModelMap.count(s) == 0)
	{
		throw ErrorToLog("Lite model with name " + s + " not loaded!");
	}

	return LiteModelMap[s];
}

bool TModelManager::AddLiteModel(const std::string& filename, const std::string& pathToResources)
{
	cardinal fileSize;
	
	std::string modelName = GetFileName(filename);

	//If model already exists
	if (LiteModelMap.count(modelName) != 0)
		return true;

	boost::shared_array<byte> fileArr = CreateMemFromFile<byte>((ST::PathToResources + filename), fileSize);

	if (!(fileArr[0] == 'L' && fileArr[1] == 'M' && fileArr[2] == 0 && fileArr[3] == 1))
		throw ErrorFileNotCorrect(filename);
		

	LiteModelMap[modelName].SetPathToResource(GetFilePath(ST::PathToResources + filename));

	bool result = LiteModelMap[modelName].LoadModel(fileArr, fileSize);

	if (!result)
		throw ErrorFileNotLoaded(filename);

	*Console<<"Lite model added successfully: "+filename;
		
	return true;
}

void TModelManager::DeleteLiteModel(const std::string& modelname)
{
	LiteModelMap.erase(modelname);
	*Console<<"ResourceManager::ModelManager model deleted: "+modelname;
}


} //namespace SE