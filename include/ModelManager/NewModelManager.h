#ifndef NEW_MODEL_MANAGER_H_INCLUDED
#define NEW_MODEL_MANAGER_H_INCLUDED

#include "include/Utils/Utils.h"
#include "include/Render/RenderMisc.h"
#include "include/Render/RenderParams.h"

namespace SE
{


class TFlexModelManager;

class TFlexModelResource : public TSerializeInterface
{
protected:
	TDataTriangleList DataTriangleList;

	std::map<std::string, std::string> SamplerMap;

	int TriangleCount;
public:

	TFlexModelResource()
		: TriangleCount(0)
	{
	}

	virtual void Serialize(boost::property_tree::ptree& propertyTree);

	friend class TFlexModelManager;
};


class TFlexModel
{
protected:
	TRenderPair RenderPair;

	std::stack<TDataTriangleList> DataTriangleListStack;

	bool Loaded;

public:

	void PushModelDataTriangleList();
	void PopModelDataTriangleList();

	void MoveModel(const Vector3f& v);
	void RotateModel(const Matrix3f& r);
	void ScaleModel(float s);
	void ScaleModel(const Vector3f& s);

	void RefreshBuffer();

	void Draw();

	TDataTriangleList& GetDataTriangleList();

	friend class TFlexModelManager;
};


class TFlexModelManager
{
protected:

	std::map<std::string, TFlexModelResource> FlexModelResourceMap;

public:

	void LoadModelFromXml(std::string xmlFileName);

	TFlexModel InstanciateModel(const std::string& modelName);

};

} //namespace SE

#endif