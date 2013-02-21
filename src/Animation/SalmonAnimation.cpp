#include "include/Animation/SalmonAnimation.h"

#include "include/Utils/Utils.h"
#include "include/Engine.h"

namespace SE
{

TAnimSequence::TAnimSequence(const TAnimSequence& animSeq)
{

	BoneCount = animSeq.BoneCount;
	FrameCount = animSeq.FrameCount;

	BoneInfoMatrix = animSeq.BoneInfoMatrix;
}

TAnimSequence::~TAnimSequence() 
{ 
	Clear();
}

TAnimSequence& TAnimSequence::operator=(const TAnimSequence& animSeq)
{
	if (this == &animSeq)
		return *this;

	Clear();

	BoneCount = animSeq.BoneCount;
	FrameCount = animSeq.FrameCount;
	BoneInfoMatrix = animSeq.BoneInfoMatrix;

	

	return *this;

}

void TAnimSequence::Clear()
{
	
	BoneInfoMatrix.clear();
	FrameCount = 0;
	BoneCount = 0;
	

}


bool TAnimSequence::LoadFromFileAn1(const std::string& fileName)
{
	Clear();

	int i,j;

	cardinal fSize;

	boost::shared_array<cardinal> fileData = CreateMemFromFile<cardinal>(fileName, fSize);


	if ((((char*)&fileData[0])[0] != 'A')||
		(((char*)&fileData[0])[1] != 'N')||
		(((char*)&fileData[0])[2] != 0)||
		(((char*)&fileData[0])[3] != 1))
		throw ErrorFileNotCorrect(fileName);


	if (fSize < 3)
		throw ErrorFileNotCorrect(fileName);

	BoneCount = fileData[1];
	FrameCount = fileData[2];


	BoneInfoMatrix.resize(FrameCount);

	for (i = 0; i < FrameCount; i++)
		BoneInfoMatrix[i].resize(BoneCount);

	//Shift from file pointer
	cardinal filePointer = 3;

	for (i=0; i<FrameCount; i++)
		for (j=0; j<BoneCount; j++)
		{
			if (filePointer+8>fSize)
			{
				throw ErrorFileTooShort(fileName);
			}

			BoneInfoMatrix[i][j].Pos.v[0] = ((float*)&fileData[0])[filePointer++];
			BoneInfoMatrix[i][j].Pos.v[1] = ((float*)&fileData[0])[filePointer++];
			BoneInfoMatrix[i][j].Pos.v[2] = ((float*)&fileData[0])[filePointer++];
			
			BoneInfoMatrix[i][j].Quat.v[0] = ((float*)&fileData[0])[filePointer++];
			BoneInfoMatrix[i][j].Quat.v[1] = ((float*)&fileData[0])[filePointer++];
			BoneInfoMatrix[i][j].Quat.v[2] = ((float*)&fileData[0])[filePointer++];
			BoneInfoMatrix[i][j].Quat.v[3] = ((float*)&fileData[0])[filePointer++];

			BoneInfoMatrix[i][j].Len = ((float*)&fileData[0])[filePointer++];

			BoneInfoMatrix[i][j].Ancient = 0; //dummy
			}

	
	return true;
}

void TAnimSequence::ScaleAnimSequence(float s)
{


	for (int i=0; i<FrameCount; i++)
		for (int j=0; j<BoneCount; j++)
		{
			BoneInfoMatrix[i][j].Pos *= s;
			BoneInfoMatrix[i][j].Len *= s;
		}
}


void TAnimSequence::MoveAnimSequence(const vec3& v)
{
	for (int i=0; i<FrameCount; i++)
		for (int j=0; j<BoneCount; j++)
		{
			BoneInfoMatrix[i][j].Pos += v;
		}
}

void TAnimSequence::RotateAnimSequence(const mat3& m)
{
	vec4 q(m);

	for (int i=0; i<FrameCount; i++)
		for (int j=0; j<BoneCount; j++)
		{
			BoneInfoMatrix[i][j].Pos = m * BoneInfoMatrix[i][j].Pos;
			BoneInfoMatrix[i][j].Quat = BoneInfoMatrix[i][j].Quat * q;
		}
}


//======================================================
//================= Anim List ==========================
//======================================================



bool TAnimList::LoadBoneSystemFromFileBn1(const std::string& fileName)
{
	Clear();

	int i;

	cardinal fSize;
	boost::shared_array<cardinal> fileData = CreateMemFromFile<cardinal>(ST::PathToResources+fileName,fSize);



	if (fileData == NULL)
		throw ErrorFileNotLoaded(fileName);

	if ((((char*)&fileData[0])[0]!='B')||
		(((char*)&fileData[0])[1]!='N')||
		(((char*)&fileData[0])[2]!=0)||
		(((char*)&fileData[0])[3]!=1))
		throw ErrorFileNotCorrect(fileName);

	if (fSize < 3)
		throw ErrorFileNotCorrect(fileName);

	int boneCount = fileData[1];
	int vertexCount = fileData[2];
	

	//Shift from file pointer
	cardinal filePointer = 3;

	//temporary data storage
	TBoneStruct boneStruct;
	TVertexWeightStruct vertexWeightStruct;


	for (i=0; i<boneCount; i++)
	{
		if (filePointer+9 > fSize)
			throw ErrorFileTooShort(fileName);

		boneStruct.Pos.v[0] = ((float*)fileData[0])[filePointer++];
		boneStruct.Pos.v[1] = ((float*)&fileData[0])[filePointer++];
		boneStruct.Pos.v[2] = ((float*)&fileData[0])[filePointer++];
			
		boneStruct.Quat.v[0] = ((float*)&fileData[0])[filePointer++];
		boneStruct.Quat.v[1] = ((float*)&fileData[0])[filePointer++];
		boneStruct.Quat.v[2] = ((float*)&fileData[0])[filePointer++];
		boneStruct.Quat.v[3] = ((float*)&fileData[0])[filePointer++];

		boneStruct.Len = ((float*)&fileData[0])[filePointer++];

		boneStruct.Ancient = fileData[filePointer++];

		BoneList.push_back(boneStruct);
	}

	for (i=0; i<vertexCount; i++)
	{
		if (filePointer + 7 > fSize)
			throw ErrorFileTooShort(fileName);

		vertexWeightStruct.WeightCount = fileData[filePointer++];

		vertexWeightStruct.VertexBoneInfo.resize(3);

		vertexWeightStruct.VertexBoneInfo[0].BoneNum = fileData[filePointer++];
		vertexWeightStruct.VertexBoneInfo[0].BoneWeight = ((float*)&fileData[0])[filePointer++];
		vertexWeightStruct.VertexBoneInfo[1].BoneNum = fileData[filePointer++];
		vertexWeightStruct.VertexBoneInfo[1].BoneWeight = ((float*)&fileData[0])[filePointer++];
		vertexWeightStruct.VertexBoneInfo[2].BoneNum = fileData[filePointer++];
		vertexWeightStruct.VertexBoneInfo[2].BoneWeight = ((float*)&fileData[0])[filePointer++];

		VertexWeightList.push_back(vertexWeightStruct);
	}
	
	
	Ready = true;


	return true;
}


bool TAnimList::LoadBoneSystemFromFileBn2(const std::string& fileName)
{
	Clear();

	int i;

	cardinal fSize;
	boost::shared_array<cardinal> fileData = CreateMemFromFile<cardinal>(ST::PathToResources+fileName,fSize);

	if (fileData == NULL)
		throw ErrorFileNotLoaded(fileName);

	if (((reinterpret_cast<char*>(&fileData[0]))[0]!='B')||
		((reinterpret_cast<char*>(&fileData[0]))[1]!='N')||
		((reinterpret_cast<char*>(&fileData[0]))[2]!=0)||
		((reinterpret_cast<char*>(&fileData[0]))[3]!=2))
		throw ErrorFileNotCorrect(fileName);

	if (fSize < 3)
		throw ErrorFileNotCorrect(fileName);

	int boneCount = fileData[1];
	int vertexCount = fileData[2];
	

	//Shift from file pointer
	cardinal filePointer = 3;

	//temporary data storage
	TBoneStruct boneStruct;
	TVertexWeightStruct vertexWeightStruct;


	for (i=0; i<boneCount; i++)
	{
		if (filePointer+9 > fSize)
			throw ErrorFileTooShort(fileName);

		boneStruct.Pos.v[0] = (reinterpret_cast<float*>(&fileData[0]))[filePointer++];
		boneStruct.Pos.v[1] = (reinterpret_cast<float*>(&fileData[0]))[filePointer++];
		boneStruct.Pos.v[2] = (reinterpret_cast<float*>(&fileData[0]))[filePointer++];
			
		boneStruct.Quat.v[0] = (reinterpret_cast<float*>(&fileData[0]))[filePointer++];
		boneStruct.Quat.v[1] = (reinterpret_cast<float*>(&fileData[0]))[filePointer++];
		boneStruct.Quat.v[2] = (reinterpret_cast<float*>(&fileData[0]))[filePointer++];
		boneStruct.Quat.v[3] = (reinterpret_cast<float*>(&fileData[0]))[filePointer++];

		boneStruct.Len = (reinterpret_cast<float*>(&fileData[0]))[filePointer++];

		boneStruct.Ancient = (reinterpret_cast<int*>(&fileData[0]))[filePointer++];

		BoneList.push_back(boneStruct);
	}

	for (i=0; i<vertexCount; i++)
	{
		if (filePointer + 7 > fSize)
			throw ErrorFileTooShort(fileName);

		vertexWeightStruct.WeightCount = fileData[filePointer++];

		vertexWeightStruct.VertexBoneInfo.resize(vertexWeightStruct.WeightCount);

		TBoneWeightStruct boneWeightStructZero;
		boneWeightStructZero.BoneNum = -1;
		boneWeightStructZero.BoneWeight = -1;

		for (cardinal j=0; j<vertexWeightStruct.VertexBoneInfo.size(); j++)
		{
			vertexWeightStruct.VertexBoneInfo[j] = boneWeightStructZero;
		}
		
		
		for (int j=0; j<vertexWeightStruct.WeightCount; j++)
		{ 
			TBoneWeightStruct boneWeightStruct;
			boneWeightStruct.BoneNum = fileData[filePointer++];
			boneWeightStruct.BoneWeight =  (reinterpret_cast<float*>(&fileData[0]))[filePointer++];
			vertexWeightStruct.VertexBoneInfo[j] = boneWeightStruct;
		}
		
		VertexWeightList.push_back(vertexWeightStruct);
	}
	
	
	Ready = true;

	//delete [] fileData;

	return true;
}

bool TAnimList::LoadAnimSequenceFromFileAn1(const std::string& fileName)
{
	std::string shortFileName(GetFileName((char*)fileName.c_str()));

	if (AnimSequenceMap.count(shortFileName) == 0)
	{
		TAnimSequence animSeqience;
		if (animSeqience.LoadFromFileAn1(ST::PathToResources+fileName))
		{
			AnimSequenceMap[shortFileName] = animSeqience;
			return true;
		}
		else
			return false;
	}

	return false;
}

void TAnimList::Clear()
{
	BoneList.clear();
	VertexWeightList.clear();
	AnimSequenceMap.clear();
	Ready = false;
}

void TAnimList::ScaleAnimList(float s)
{
	for (TBoneList::iterator i = BoneList.begin(); i != BoneList.end(); ++i)
	{
		i->Pos *= s;
		i->Len *= s;
	}

	for (TAnimSequenceMap::iterator j = AnimSequenceMap.begin(); j != AnimSequenceMap.end(); ++j)
	{
		j->second.ScaleAnimSequence(s);
	}
}

void TAnimList::MoveAnimList(const vec3& v)
{
	for (TBoneList::iterator i = BoneList.begin(); i != BoneList.end(); ++i)
	{
		i->Pos += v;
	}

	for (TAnimSequenceMap::iterator j = AnimSequenceMap.begin(); j != AnimSequenceMap.end(); ++j)
	{
		j->second.MoveAnimSequence(v);
	}
}

void TAnimList::RotateAnimList(const mat3& m)
{
	vec4 q(m);

	for (TBoneList::iterator i = BoneList.begin(); i != BoneList.end(); ++i)
	{
		i->Pos = m * i->Pos;
		i->Quat = i->Quat * q;
	}

	for (TAnimSequenceMap::iterator j = AnimSequenceMap.begin(); j != AnimSequenceMap.end(); ++j)
	{
		j->second.RotateAnimSequence(m);
	}
}


void TAnimList::DrawBoneSystem()
{
#ifdef TARGET_WIN32
	vec3 startpos, endpos;
	vec4 vx, q, inv_q;

	TBoneList::iterator i;

	for (TBoneList::iterator i = BoneList.begin(); i != BoneList.end(); ++i)
	{
		
		startpos = i->Pos;
		q = i->Quat;

		vx = vec4(i->Len, 0.0f, 0.0f, 0.0f);
	
		inv_q.v[0] = - q.v[0];
		inv_q.v[1] = - q.v[1];
		inv_q.v[2] = - q.v[2];
		inv_q.v[3] = q.v[3];

		vx = q * vx * inv_q;
		
		endpos = startpos + vec3(vx);

		glBegin(GL_LINES);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glVertex3fv(startpos.v);
			glVertex3fv(endpos.v);
		glEnd();
	}
#endif
}

void TAnimList::SaveBoneSystemToBuf()
{
	PrevBoneList.assign(BoneList.begin(),BoneList.end());
}

void TAnimList::LoadBoneSystemFromBuf()
{
	BoneList.assign(PrevBoneList.begin(),PrevBoneList.end());
}

//=====================================================
//=========== TModelAnimManager =======================
//=====================================================

TAnimList* TModelAnimManager::operator [] (const std::string& s)
{
	if (AnimListMap.count(s) > 0)
	{
		return &AnimListMap[s];
	}

	return NULL;
}

//NOT SAFE!!! Need to check if model exists in Lite Model Manager and need to check if bone list belongs to this model
bool TModelAnimManager::AddAnimationToModel(const std::string& modelName, const std::string& boneListFileName)
{
	if (AnimListMap.count(modelName) == 0)
	{
		//TODO: Make switch
		//if (!AnimListMap[modelName].LoadBoneSystemFromFileBn1(boneListFileName))
		if (!AnimListMap[modelName].LoadBoneSystemFromFileBn2(boneListFileName))
		{
			AnimListMap.erase(modelName);
			return false;
		}

		return true;
	}
	return false;
}

void TModelAnimManager::Clear()
{
	
	AnimListMap.clear();
}
	

//==================================================
//=============== ANIM MODEL =======================
//==================================================




void TAnimModel::RecalcVertexPos()
{
	
	int i,j,nBone;
	mat3 R, R0, dR;
	vec4 dq,inv_q;
	vec4 q0;
	vec4 q;
	vec4 inv_q0;
	vec4 inv_dq;
	//vec3 endbone0;
	vec3 endbone;
	vec3 p[20];//TODO: MAKE THIS DYNAMIC!!!!
	vec3 PrevPos,dbp,px;

	//float n;

	int VertexCount = LiteModel.TriangleCount * 3;

	
	
	for (i=0; i<VertexCount; i++)
	{
		PrevPos = CopyVertexArr[i];

		int weightCount = AnimList->VertexWeightList[i].WeightCount;

		for (j=0; j<weightCount; j++)
		{
			
			nBone = AnimList->VertexWeightList[i].VertexBoneInfo[j].BoneNum;

			q0 = AnimList->PrevBoneList[nBone].Quat;
			
			inv_q0 = q0;

			inv_q0.v[0] = - inv_q0.v[0];
			inv_q0.v[1] = - inv_q0.v[1];
			inv_q0.v[2] = - inv_q0.v[2];
			inv_q0.v[3] = inv_q0.v[3];

			q = AnimList->BoneList[nBone].Quat;

			inv_q = q;

			inv_q.v[0] = - inv_q.v[0];
			inv_q.v[1] = - inv_q.v[1];
			inv_q.v[2] = - inv_q.v[2];
			inv_q.v[3] = inv_q.v[3];

			vec3 prevBonePos = AnimList->PrevBoneList[nBone].Pos;

			px = PrevPos - prevBonePos;

			px = inv_q * (q0 * vec4(px) * inv_q0) * q;
			
			vec3 bonePos = AnimList->BoneList[nBone].Pos;

			p[j] = bonePos + px;
		
		}

		px = vec3(0.0f, 0.0f, 0.0f);

		for (j=0; j<weightCount; j++)
		{
			px += p[j] * AnimList->VertexWeightList[i].VertexBoneInfo[j].BoneWeight;
		}

		//LiteModel.VertexArr[i] = px;
		LiteModel.TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][i] = px;

		/*
		//Normal
		px = ((vec3*)(LiteModel.TrianglePointer))[i+VertexCount]; 
		px = dR * px;
		((vec3*)(LiteModel.TrianglePointer))[i+VertexCount] = px;

		//Tangent
		px = ((vec3*)(LiteModel.TrianglePointer))[i+VertexCount+VertexCount]; 
		px = dR * px;
		((vec3*)(LiteModel.TrianglePointer))[i+VertexCount+VertexCount] = px;

		//Binormal
		px = ((vec3*)(LiteModel.TrianglePointer))[i+VertexCount+VertexCount+VertexCount]; 
		px = dR * px;
		((vec3*)(LiteModel.TrianglePointer))[i+VertexCount+VertexCount+VertexCount] = px;*/
	}
	
}

void TAnimModel::SaveVertexInfoToBuf() 
{

	CopyVertexArr = LiteModel.TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB];

	AnimList->SaveBoneSystemToBuf(); 
}
void TAnimModel::LoadVertexInfoFromBuf() 
{  

	LiteModel.TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB] = CopyVertexArr;

	AnimList->LoadBoneSystemFromBuf(); 
}


bool TAnimModel::LoadModel(const std::string& modelName)
{
	if (!LiteModel.LoadModel(modelName))
		return false;


	CopyVertexArr.resize(LiteModel.TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].size());
	CopyVertexArr = LiteModel.TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB];

	AnimList = ResourceManager->ModelAnimManager[modelName];

	if (AnimList == NULL)
		return false;

	Ready = true;

	SaveVertexInfoToBuf();
	
	return true;
}

void TAnimModel::Clear()
{
	
	CopyVertexArr.clear();

	LiteModel.FreeModel();
	AnimList = NULL;

	Ready = false;
}

void TAnimModel::ScaleModel(float s)
{
	LiteModel.ScaleModel(s);
	AnimList->ScaleAnimList(s);

	SaveVertexInfoToBuf();
}

void TAnimModel::MoveModel(const vec3& v)
{
	LiteModel.MoveModel(v);
	AnimList->MoveAnimList(v);

	SaveVertexInfoToBuf();
}

void TAnimModel::RotateModel(const mat3& r)
{
	LiteModel.RotateModel(r);
	AnimList->RotateAnimList(r);

	SaveVertexInfoToBuf();
}

void TAnimModel::DrawImmediate()
{
	LiteModel.DrawImmediate();
}

void TAnimModel::ApplySequence(const std::string& seq, int n)
{
	
	int boneCount = AnimList->BoneList.size();

	for (int i=0; i<boneCount; i++)
	{
		AnimList->BoneList[i].Pos = AnimList->AnimSequenceMap[seq].BoneInfoMatrix[n][i].Pos;
		AnimList->BoneList[i].Quat = AnimList->AnimSequenceMap[seq].BoneInfoMatrix[n][i].Quat;
		AnimList->BoneList[i].Len  = AnimList->AnimSequenceMap[seq].BoneInfoMatrix[n][i].Len;
	}

	RecalcVertexPos();
}

} //namespace SE