#ifndef HALIBUT_ANIMATION_H_INCLUDED
#define HALIBUT_ANIMATION_H_INCLUDED

#include "include/Render/RenderMisc.h"
#include "include/Render/RenderParams.h"
#include "include/ScriptManager/ScriptManager.h"
#include <map>
#include <vector>

namespace SE
{

struct TAnimShot
{
	vec2 TexCoordFrom;
	vec2 TexCoordTo;
	cardinal TimePassedFromStart;
	TAnimShot(vec2 texCoordFrom, vec2 texCoordTo, cardinal timePassedFromStart)
		: TexCoordFrom(texCoordFrom)
		, TexCoordTo(texCoordTo)
		, TimePassedFromStart(timePassedFromStart)
	{
	}
};


class THalibutAnimObject : public TSerializeInterface
{
protected:
	std::string TexName;
	std::vector<TAnimShot> AnimShotArr;

	cardinal CurrentAnimTime;
	cardinal TotalAnimTime;

	cardinal CurrentAnimFrame;

	void RefreshTriangleListVector();

public:

	TRenderPairList TriangleListVector;

	vec2 LeftBottomPos;

	THalibutAnimObject(const std::string& texName, vec2 posFrom, vec2 posTo);

	void AddAnimShot(vec2 texCoordFrom, vec2 texCoordTo, cardinal timePassedFromStart);

	void CalcTotalAnimTime();

	void Update(cardinal dt);

	void Reset();

	void Clear();

	virtual void Serialize(boost::property_tree::ptree& propertyTree);


};


class THalibutExternalAnimObject : public TSerializeInterface
{
protected:

	cardinal CurrentAnimTime;
	cardinal TotalAnimTime;

	cardinal CurrentAnimFrame;
	
	std::vector<TAnimShot> AnimShotArr;

	TRenderPairList::iterator RenderPairIter;

	cardinal RectCount;
	cardinal RectFrom;

	void RefreshTriangleListVector();

public:

	THalibutExternalAnimObject()
		: RectCount(1)
	{
	}

	THalibutExternalAnimObject(TRenderPairList::iterator renderPairIter, cardinal rectFrom, cardinal rectCount);

	THalibutExternalAnimObject(const THalibutExternalAnimObject& copyFrom);

	THalibutExternalAnimObject& operator=(const THalibutExternalAnimObject& copyFrom);

	~THalibutExternalAnimObject();

	void ReplaceAnimObject(TRenderPairList::iterator renderPairIter, cardinal rectFrom);

	void AddAnimShot(vec2 texCoordFrom, vec2 texCoordTo, cardinal timePassedFromStart);

	void CalcTotalAnimTime();

	void Update(cardinal dt);

	void Reset();

	void Clear();

	virtual void Serialize(boost::property_tree::ptree& propertyTree);
};



struct THalibutAnimObjectRecord
{
	std::string Name;

	THalibutExternalAnimObject HalibutAnimObject;

	enum TAnimState
	{
		AS_PLAY,
		AS_STOP
	} AnimState;

	bool Visible;

	THalibutAnimObjectRecord(const std::string& name, THalibutExternalAnimObject halibutAnimObject, TAnimState animState)
		: Name(name)
		, HalibutAnimObject(halibutAnimObject)
		, AnimState(animState)
		, Visible(true)
	{
	}

};



class THalibutAnimationManager : public TFunctionBinderInterface
{
protected:
	std::vector<THalibutAnimObjectRecord> HalibutAnimObjectArr;
	std::vector<THalibutAnimObjectRecord>::iterator FindAnimObjectByName(const std::string& name);

public:

	~THalibutAnimationManager();

	void AddAnimationObject(const std::string& animName, THalibutExternalAnimObject halibutExternalAnimObject);
	void DeleteAnimationObject(const std::string& animName);

	bool AnimationExists(const std::string& animName);
	void ReplaceAnimationObject(const std::string& animName, TRenderPairList::iterator renderPairIter, cardinal rectFrom);

	void Update(cardinal dt);

	void StartAnimation(const std::string& animName);
	void StopAnimation(const std::string& animName);
	void PauseAnimation(const std::string& animName);
	//void HideAnimation(const std::string& animName);
	//void ShowAnimation(const std::string& animName);

	void PrintAnimationList();
	virtual void BindFunctions();

};

} //namespace SE

#endif
