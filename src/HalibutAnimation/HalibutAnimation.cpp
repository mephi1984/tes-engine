#include "include/Engine.h"

#include "boost/assign.hpp"

namespace SE
{

THalibutAnimObject::THalibutAnimObject(const std::string& texName, vec2 posFrom, vec2 posTo)
	: TexName(texName)
	, TotalAnimTime(0)
	, CurrentAnimTime(0)
	, CurrentAnimFrame(0)
{
	TRenderParams renderParams;

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = texName;
	renderParams.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 1.f;

	TriangleListVector.push_back(std::pair<TRenderParams, TTriangleList>(renderParams, MakeTriangleList(posFrom, posTo)));

	LeftBottomPos = posFrom;
}


void THalibutAnimObject::RefreshTriangleListVector()
{
	vec2 tex1 = AnimShotArr[CurrentAnimFrame].TexCoordFrom;
	vec2 tex2 = vec2(AnimShotArr[CurrentAnimFrame].TexCoordFrom.v[0], AnimShotArr[CurrentAnimFrame].TexCoordTo.v[1]);
	vec2 tex3 = AnimShotArr[CurrentAnimFrame].TexCoordTo;
	vec2 tex4 = vec2(AnimShotArr[CurrentAnimFrame].TexCoordTo.v[0], AnimShotArr[CurrentAnimFrame].TexCoordFrom.v[1]);

	TTriangleList& triangleList = TriangleListVector.begin()->second;

	triangleList.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][0] = tex1;
	triangleList.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][1] = tex2;
	triangleList.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][2] = tex3;

	triangleList.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][3] = tex3;
	triangleList.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][4] = tex4;
	triangleList.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][5] = tex1;

	triangleList.RefreshBuffer();
}

void THalibutAnimObject::AddAnimShot(vec2 texCoordFrom, vec2 texCoordTo, cardinal timePassedFromStart)
{
	AnimShotArr.push_back(TAnimShot(texCoordFrom, texCoordTo, timePassedFromStart));

	CalcTotalAnimTime(); //Not rational but prevents a lot of stupid mistakes :(
}

void THalibutAnimObject::CalcTotalAnimTime()
{
	if (AnimShotArr.size() == 0)
	{
		TotalAnimTime = 0;
	}
	else
	{
		std::vector<TAnimShot>::iterator i = AnimShotArr.end();
		
		i--;

		TotalAnimTime = i->TimePassedFromStart;
	}

}

void THalibutAnimObject::Update(cardinal dt)
{
	if (AnimShotArr.size() == 0)
	{
		return;
	}

	//cardinal prevCurrentAnimTime = CurrentAnimTime;

	CurrentAnimTime += dt;

	cardinal prevCurrentAnimFrame = CurrentAnimFrame;

	while (CurrentAnimTime > AnimShotArr[CurrentAnimFrame].TimePassedFromStart)
	{
		CurrentAnimFrame++;

		if (CurrentAnimFrame == AnimShotArr.size())
		{
			CurrentAnimFrame = 0;
			CurrentAnimTime -= TotalAnimTime;
		}
	}

	if (prevCurrentAnimFrame != CurrentAnimFrame)
	{
		RefreshTriangleListVector();
	}
}

void THalibutAnimObject::Reset()
{
	CurrentAnimFrame = 0;
	CurrentAnimTime = 0;
	RefreshTriangleListVector();
}


void THalibutAnimObject::Serialize(boost::property_tree::ptree& propertyTree)
{
	Clear();

	TexName = propertyTree.get<std::string>("Animation.TexName");

	vec2 posFrom, posTo;

	posFrom.v[0] = propertyTree.get<float>("Animation.PosFrom.<xmlattr>.x");
	posFrom.v[1] = propertyTree.get<float>("Animation.PosFrom.<xmlattr>.y");
	posTo.v[0] = propertyTree.get<float>("Animation.PosTo.<xmlattr>.x");
	posTo.v[1] = propertyTree.get<float>("Animation.PosTo.<xmlattr>.y");

	TRenderParams renderParams;

	renderParams.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = TexName;
	renderParams.FloatMap[CONST_STRING_TRANSPARENCY_UNIFORM] = 1.f;


	TriangleListVector.push_back(std::pair<TRenderParams, TTriangleList>(renderParams, MakeTriangleList(posFrom, posTo)));

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, propertyTree.get_child("Animation.Frames"))
	{
		vec2 texCoordFrom, texCoordTo;
			
		texCoordFrom.v[0] = v.second.get<float>("TexCoordFrom.<xmlattr>.x");
		texCoordFrom.v[1] = v.second.get<float>("TexCoordFrom.<xmlattr>.y");
		texCoordTo.v[0] = v.second.get<float>("TexCoordTo.<xmlattr>.x");
		texCoordTo.v[1] = v.second.get<float>("TexCoordTo.<xmlattr>.y");

		cardinal timePassedFromStart = v.second.get<cardinal>("TimePassed");

		AddAnimShot(texCoordFrom, texCoordTo, timePassedFromStart);
		
	}

}


void THalibutAnimObject::Clear()
{
	AnimShotArr.clear();

	TriangleListVector.clear();

	TotalAnimTime = 0;
	CurrentAnimTime = 0;
	CurrentAnimFrame = 0;
}



//==================================================



THalibutExternalAnimObject::THalibutExternalAnimObject(TRenderPairList::iterator renderPairIter, cardinal rectFrom, cardinal rectCount)
	: RectCount(rectCount)
	, RectFrom(rectFrom)
	, TotalAnimTime(0)
	, CurrentAnimTime(0)
	, CurrentAnimFrame(0)
	, RenderPairIter(renderPairIter)
{
}

THalibutExternalAnimObject::THalibutExternalAnimObject(const THalibutExternalAnimObject& copyFrom)
	: CurrentAnimTime(copyFrom.CurrentAnimTime)
	, TotalAnimTime(copyFrom.TotalAnimTime)
	, CurrentAnimFrame(copyFrom.CurrentAnimFrame)
	, AnimShotArr(copyFrom.AnimShotArr)
	, RenderPairIter(copyFrom.RenderPairIter)
	, RectCount(copyFrom.RectCount)
	, RectFrom(copyFrom.RectFrom)
{
	
}

THalibutExternalAnimObject& THalibutExternalAnimObject::operator=(const THalibutExternalAnimObject& copyFrom)
{
	//Self-assignment is ok
	
	CurrentAnimTime = copyFrom.CurrentAnimTime;
	TotalAnimTime = copyFrom.TotalAnimTime;

	CurrentAnimFrame = copyFrom.CurrentAnimFrame;
	
	AnimShotArr = copyFrom.AnimShotArr;

	RenderPairIter = copyFrom.RenderPairIter;

	RectCount = copyFrom.RectCount;
	RectFrom = copyFrom.RectFrom;
	
	return *this;

}

THalibutExternalAnimObject::~THalibutExternalAnimObject()
{
	
}

void THalibutExternalAnimObject::ReplaceAnimObject(TRenderPairList::iterator renderPairIter, cardinal rectFrom)
{
	
	RectFrom = rectFrom;
	RenderPairIter = renderPairIter;
}

void THalibutExternalAnimObject::RefreshTriangleListVector()
{

	vec2 tex1 = AnimShotArr[CurrentAnimFrame].TexCoordFrom;
	vec2 tex2 = vec2(AnimShotArr[CurrentAnimFrame].TexCoordFrom.v[0], AnimShotArr[CurrentAnimFrame].TexCoordTo.v[1]);
	vec2 tex3 = AnimShotArr[CurrentAnimFrame].TexCoordTo;
	vec2 tex4 = vec2(AnimShotArr[CurrentAnimFrame].TexCoordTo.v[0], AnimShotArr[CurrentAnimFrame].TexCoordFrom.v[1]);



	for (cardinal i = RectFrom; i<RectFrom+RectCount; i++)
	{
		RenderPairIter->second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][i*6+0] = tex1;
		RenderPairIter->second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][i*6+1] = tex2;
		RenderPairIter->second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][i*6+2] = tex3;

		RenderPairIter->second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][i*6+3] = tex3;
		RenderPairIter->second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][i*6+4] = tex4;
		RenderPairIter->second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB][i*6+5] = tex1;
	}

	RenderPairIter->second.RefreshBuffer();

}

void THalibutExternalAnimObject::AddAnimShot(vec2 texCoordFrom, vec2 texCoordTo, cardinal timePassedFromStart)
{

	AnimShotArr.push_back(TAnimShot(texCoordFrom, texCoordTo, timePassedFromStart));

	CalcTotalAnimTime(); //Not rational but prevents a lot of stupid mistakes :(
}

void THalibutExternalAnimObject::CalcTotalAnimTime()
{

	if (AnimShotArr.size() == 0)
	{
		TotalAnimTime = 0;
	}
	else
	{
		std::vector<TAnimShot>::iterator i = AnimShotArr.end();
		
		i--;

		TotalAnimTime = i->TimePassedFromStart;
	}

}

void THalibutExternalAnimObject::Update(cardinal dt)
{

	if (AnimShotArr.size() == 0)
	{
		return;
	}

	//cardinal prevCurrentAnimTime = CurrentAnimTime;

	CurrentAnimTime += dt;

	cardinal prevCurrentAnimFrame = CurrentAnimFrame;

	while (CurrentAnimTime > AnimShotArr[CurrentAnimFrame].TimePassedFromStart)
	{
		CurrentAnimFrame++;

		if (CurrentAnimFrame == AnimShotArr.size())
		{
			CurrentAnimFrame = 0;
			CurrentAnimTime -= TotalAnimTime;
		}
	}

	if (prevCurrentAnimFrame != CurrentAnimFrame)
	{
		RefreshTriangleListVector();
	}
}

void THalibutExternalAnimObject::Reset()
{
	
	CurrentAnimFrame = 0;
	CurrentAnimTime = 0;
	RefreshTriangleListVector();
}


void THalibutExternalAnimObject::Serialize(boost::property_tree::ptree& propertyTree)
{
	
	//*Console<<"Halibut external anim object serialize\n";
	Clear();

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, propertyTree.get_child("Animation.Frames"))
	{
		vec2 texCoordFrom, texCoordTo;
			
		texCoordFrom.v[0] = v.second.get<float>("TexCoordFrom.<xmlattr>.x");
		texCoordFrom.v[1] = v.second.get<float>("TexCoordFrom.<xmlattr>.y");
		texCoordTo.v[0] = v.second.get<float>("TexCoordTo.<xmlattr>.x");
		texCoordTo.v[1] = v.second.get<float>("TexCoordTo.<xmlattr>.y");

		cardinal timePassedFromStart = v.second.get<cardinal>("TimePassed");

		AddAnimShot(texCoordFrom, texCoordTo, timePassedFromStart);
		
	}

}


void THalibutExternalAnimObject::Clear()
{
	
	//*Console<<"Halibut external anim object clear\n";

	AnimShotArr.clear();

	TotalAnimTime = 0;
	CurrentAnimTime = 0;
	CurrentAnimFrame = 0;
}


//==================================================


THalibutAnimationManager::~THalibutAnimationManager()
{
	*Console<<"HalibutAnimationManager deleting...";
}

std::vector<THalibutAnimObjectRecord>::iterator THalibutAnimationManager::FindAnimObjectByName(const std::string& name)
{
	std::vector<THalibutAnimObjectRecord>::iterator i;

	for (i = HalibutAnimObjectArr.begin(); i != HalibutAnimObjectArr.end(); ++i)
	{
		if (i->Name == name)
		{
			return i;
		}
	}

	//throw ErrorToLog("ERROR: Animation not found: " + name + "\n");

	return HalibutAnimObjectArr.end();
}


void THalibutAnimationManager::ReplaceAnimationObject(const std::string& animName, TRenderPairList::iterator renderPairIter, cardinal rectFrom)
{
	
	std::vector<THalibutAnimObjectRecord>::iterator i = FindAnimObjectByName(animName);

	if (i == HalibutAnimObjectArr.end())
	{
		throw ErrorToLog("ERROR: Animation not found: " + animName);
	}

	i->HalibutAnimObject.ReplaceAnimObject(renderPairIter, rectFrom);
}

void THalibutAnimationManager::AddAnimationObject(const std::string& animName, THalibutExternalAnimObject halibutExternalAnimObject)
{
	
	if (FindAnimObjectByName(animName) == HalibutAnimObjectArr.end())
	{
		HalibutAnimObjectArr.push_back(THalibutAnimObjectRecord(animName, halibutExternalAnimObject, THalibutAnimObjectRecord::AS_STOP));
	}
}


void THalibutAnimationManager::DeleteAnimationObject(const std::string& animName)
{
	
	std::vector<THalibutAnimObjectRecord>::iterator i = FindAnimObjectByName(animName);
	if (i != HalibutAnimObjectArr.end())
	{
		HalibutAnimObjectArr.erase(i);
	}

}


bool THalibutAnimationManager::AnimationExists(const std::string& animName)
{
	return FindAnimObjectByName(animName) != HalibutAnimObjectArr.end();
}

void THalibutAnimationManager::Update(cardinal dt)
{
	
	std::vector<THalibutAnimObjectRecord>::iterator i;

	for (i = HalibutAnimObjectArr.begin(); i != HalibutAnimObjectArr.end(); ++i)
	{
		if (i->AnimState == THalibutAnimObjectRecord::AS_PLAY)
		{
			i->HalibutAnimObject.Update(dt);
		}
	}
}



void THalibutAnimationManager::StartAnimation(const std::string& animName)
{
	
	std::vector<THalibutAnimObjectRecord>::iterator i = FindAnimObjectByName(animName);

	if (FindAnimObjectByName(animName) == HalibutAnimObjectArr.end())
	{
		return;
	}

	i->AnimState = THalibutAnimObjectRecord::AS_PLAY;
}

void THalibutAnimationManager::StopAnimation(const std::string& animName)
{
	
	std::vector<THalibutAnimObjectRecord>::iterator i = FindAnimObjectByName(animName);

	if (FindAnimObjectByName(animName) == HalibutAnimObjectArr.end())
	{
		return;
	}

	i->AnimState = THalibutAnimObjectRecord::AS_STOP;

	i->HalibutAnimObject.Reset();
}

void THalibutAnimationManager::PauseAnimation(const std::string& animName)
{
	
	std::vector<THalibutAnimObjectRecord>::iterator i = FindAnimObjectByName(animName);

	i->AnimState = THalibutAnimObjectRecord::AS_STOP;
}

/*
void THalibutAnimationManager::HideAnimation(const std::string& animName)
{
	std::vector<THalibutAnimObjectRecord>::iterator i = FindAnimObjectByName(animName);

	i->Visible = false;
}


void THalibutAnimationManager::ShowAnimation(const std::string& animName)
{
	std::vector<THalibutAnimObjectRecord>::iterator i = FindAnimObjectByName(animName);

	i->Visible = true;
}*/

void THalibutAnimationManager::PrintAnimationList()
{

	static std::map<THalibutAnimObjectRecord::TAnimState, std::string> mapAnimState = boost::assign::map_list_of(THalibutAnimObjectRecord::AS_PLAY, "AS_PLAY")(THalibutAnimObjectRecord::AS_STOP, "AS_STOP");

	std::vector<THalibutAnimObjectRecord>::iterator i;

	Console->PrintImmediate("==========> ANIMATION LIST <==========");

	for (i = HalibutAnimObjectArr.begin(); i != HalibutAnimObjectArr.end(); ++i)
	{
		Console->PrintImmediate(i->Name + " " + mapAnimState[i->AnimState]);
	}
}


void THalibutAnimationManager::BindFunctions()
{
	//*Console<<"Bind functions in THalibutAnimationManager\n";
	
}

} //namespace SE