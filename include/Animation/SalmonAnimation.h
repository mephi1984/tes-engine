#ifndef SALMON_ANIMATION_H_INCLUDED
#define SALMON_ANIMATION_H_INCLUDED
/*
This code contains Model Animation Manager and code to operate Animation
*/

#include "include/ModelManager/ModelManager.h"

namespace SE
{

//We have to declare it in advance
class TAnimModel;

//This struct contains bone information
struct TBoneStruct
{
	vec3 Pos;
	vec4 Quat;
	float Len;
	int Ancient;
};

struct TBoneWeightStruct
{
	int BoneNum;
	float BoneWeight;
};

//This struct contains additional info for every model vertex - weight of 3 bones per vertex
struct TVertexWeightStruct
{
	int WeightCount;
	std::vector<TBoneWeightStruct> VertexBoneInfo;
};

//One animation sequence (i.e. one move for one model)
class TAnimSequence
{
protected:
	std::vector<std::vector<TBoneStruct> > BoneInfoMatrix;
	int BoneCount;
	int FrameCount;
public:
	TAnimSequence() : BoneCount(0), FrameCount(0) { }

	//copy constructor
	TAnimSequence(const TAnimSequence& animSeq);

	~TAnimSequence();

	TAnimSequence& operator=(const TAnimSequence& animSeq);

	bool LoadFromFileAn1(const std::string& fileName);  //not safe

	void Clear();


	void ScaleAnimSequence(float s);
	void MoveAnimSequence(const vec3& v);
	void RotateAnimSequence(const mat3& m);

	friend class TAnimModel;
};

typedef std::vector<TBoneStruct> TBoneList;
typedef std::vector<TVertexWeightStruct> TVertexWeightList;
typedef std::map<std::string, TAnimSequence, std::less<std::string> > TAnimSequenceMap;

/* Class for list of animations
List of animations contains bone system + vertex weight table + a list of avaiable animations
*/
class TAnimList
{
protected:
	TBoneList BoneList;
	TBoneList PrevBoneList;
	TVertexWeightList VertexWeightList;
	TAnimSequenceMap AnimSequenceMap;
	
	//Means bonelist and vertex weight list are loaded (AnimSequence may remain empty)
	bool Ready;
public:
	TAnimList() : Ready(false) { }
	~TAnimList() { Clear(); }

	//Load bone system + vertex weight from file
	bool LoadBoneSystemFromFileBn1(const std::string& fileName);
	bool LoadBoneSystemFromFileBn2(const std::string& fileName);

	//Add one more anim sequence to sequence map
	bool LoadAnimSequenceFromFileAn1(const std::string& fileName);

	//Process animations. Must be processed after you process model
	void ScaleAnimList(float s);
	void MoveAnimList(const vec3& v);
	void RotateAnimList(const mat3& m);

	void Clear();

	void DrawBoneSystem();

	//this method copies bone system to reserved memory
	void SaveBoneSystemToBuf();

	//this method reloads bone system form reserved memory
	void LoadBoneSystemFromBuf();

	friend class TAnimModel;
};

typedef std::map<std::string, TAnimList, std::less<std::string> > TAnimListMap;

//Class that contains list of animations for each model
class TModelAnimManager
{
protected:
	TAnimListMap AnimListMap;

public:
	TModelAnimManager() { };
	~TModelAnimManager() { Clear(); };

	TAnimList* operator [] (const std::string& s);

	//NOT SAFE!!!
	bool AddAnimationToModel(const std::string& modelName, const std::string& boneListFileName);

	void Clear();
	
};


class TAnimModel
{
protected:
	TLiteModel LiteModel;

	std::vector<vec3> CopyVertexArr;

	TAnimList* AnimList;

	bool Ready;

	void RecalcVertexPos(); 

	//This is used for animation only - to save original t=0 position
	void SaveVertexInfoToBuf();
	void LoadVertexInfoFromBuf();

public:
	TAnimModel() : Ready(false), AnimList(NULL) {}

	~TAnimModel() { Clear(); }

	bool LoadModel(const std::string& modelName);

	void Clear();

	void ScaleModel(float s);
	void MoveModel(const vec3& v);
	void RotateModel(const mat3& r);

	inline void SetShaderTranslateVector(const vec3& v) { LiteModel.SetShaderTranslateVector(v); }
	inline void SetShaderRotateMatrix(const mat3& m) { LiteModel.SetShaderRotateMatrix(m); }

	void DrawImmediate();
	void DrawVBO() { LiteModel.DrawVBO(); }
	void UpdateVBO() { LiteModel.UpdateVBO(); }

	//n must be in 0..99; ???
	void ApplySequence(const std::string& seq, int n);

};

} //namespace SE

#endif