#ifndef RENDER_PARAMS_H_INCLUDED
#define RENDER_PARAMS_H_INCLUDED


#include "include/Utils/DataTypes/DataTypes.h"
#include "include/Utils/SerializeInterface/SerializeInterface.h"
#include "boost/shared_ptr.hpp"
#include <vector>
#include <list>
#include <map>



#ifdef TARGET_WIN32

#include "include/OpenGLExt/OpenGlExt.h"

#endif

namespace SE
{

struct TRenderParams : public TSerializeInterface
{
	std::string ShaderName;

	mutable std::map<std::string, std::string> SamplerMap;
	mutable std::map<std::string, float> FloatMap;
	mutable std::map<std::string, Vector4f> Vec4Map;
	
	virtual void Serialize(boost::property_tree::ptree& propertyTree);

};

typedef std::pair<TRenderParams, TTriangleList> TRenderPair;

typedef std::list<TRenderPair> TRenderPairList;


struct TRenderParamsSetter
{
protected:
	const TRenderParams& RenderParams;
public:
	TRenderParamsSetter(const TRenderParams& renderParams);
	~TRenderParamsSetter();
};

} //namespace SE

#endif