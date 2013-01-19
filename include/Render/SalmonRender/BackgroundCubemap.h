#ifndef BACKGROUND_CUBEMAP_H_INCLUDED
#define BACKGROUND_CUBEMAP_H_INCLUDED

#include "include/Render/RenderMisc.h"
#include "include/Utils/Utils.h"

namespace SE
{

class TBackgroundCubemapClass
{
protected:
	TTriangleList TriangleList;

	
public:
	cardinal BkgTexId;

	TBackgroundCubemapClass();

	void Draw();
};

} //namespace SE


#endif