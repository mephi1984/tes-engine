#include "include/Render/SalmonRender/SalmonRenderGLES20.h"

#include "include/Utils/Utils.h"
#include "include/Engine.h"

namespace SE
{

void TSalmonRendererGLES20::DrawQuad(const T2DQuad& quad)
{
	AssertIfInMainThread();
	
	//EnableVertexAttribArray("vPosition");
	//EnableVertexAttribArray("vTexCoord");
	
	VertexAttribPointer3fv("vPosition", 0, reinterpret_cast<const char*>(quad.VertexCoord));
	VertexAttribPointer2fv("vTexCoord", 0, reinterpret_cast<const char*>(quad.TextureCoord));
   
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  
	//DisableVertexAttribArray("vPosition");
	//DisableVertexAttribArray("vTexCoord");
}



void TSalmonRendererGLES20::DrawTriangleList(const TTriangleList& triangleList)
{
	AssertIfInMainThread();
	
	for (std::map<std::string, std::vector<Vector2f> >::iterator i = triangleList.Data.Vec2CoordArr.begin(); i != triangleList.Data.Vec2CoordArr.end(); ++i )
	{
		VertexAttribPointer2fv(i->first, 0, reinterpret_cast<const char*>(&triangleList.Data.Vec2CoordArr[i->first][0]));
	}
	
	for (std::map<std::string, std::vector<Vector3f> >::iterator i = triangleList.Data.Vec3CoordArr.begin(); i != triangleList.Data.Vec3CoordArr.end(); ++i )
	{
		VertexAttribPointer3fv(i->first, 0, reinterpret_cast<const char*>(&triangleList.Data.Vec3CoordArr[i->first][0]));
	}

	for (std::map<std::string, std::vector<Vector4f> >::iterator i = triangleList.Data.Vec4CoordArr.begin(); i != triangleList.Data.Vec4CoordArr.end(); ++i)
	{
		VertexAttribPointer4fv(i->first, 0, reinterpret_cast<const char*>(&triangleList.Data.Vec4CoordArr[i->first][0]));
	}

	if (!triangleList.Data.Vec3CoordArr.empty())
	{
		glDrawArrays(GL_TRIANGLES,0,triangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].size());
	}

}

} //namespace SE
