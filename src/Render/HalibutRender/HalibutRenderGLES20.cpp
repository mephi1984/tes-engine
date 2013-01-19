#include "include/Engine.h"
#include "include/Render/HalibutRender/HalibutRenderGLES20.h"

namespace SE
{

THalibutRendererGLES20::THalibutRendererGLES20()
{
	ProjectionMatrixStack.push(IdentityMatrix4);
	ModelviewMatrixStack.push(IdentityMatrix4);
	*Console<<"Halibut Render created\n";
}

THalibutRendererGLES20::~THalibutRendererGLES20()
{
	*Console<<"Halibut Render destroying\n";
}

void THalibutRendererGLES20::DrawQuad(const T2DQuad& quad)
{

	VertexAttribPointer3fv(CONST_STRING_POSITION_ATTRIB, 0, reinterpret_cast<const char*>(quad.VertexCoord));
	VertexAttribPointer2fv(CONST_STRING_TEXCOORD_ATTRIB, 0, reinterpret_cast<const char*>(quad.TextureCoord));
   
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  
}

void THalibutRendererGLES20::DrawTriangleList(const TTriangleList& triangleList)
{
  
	for (std::map<std::string, std::vector<vec2> >::iterator i = triangleList.Data.Vec2CoordArr.begin(); i != triangleList.Data.Vec2CoordArr.end(); ++i )
	{
		VertexAttribPointer2fv(i->first, 0, reinterpret_cast<const char*>(&triangleList.Data.Vec2CoordArr[i->first][0]));
	}
	
	for (std::map<std::string, std::vector<vec3> >::iterator i = triangleList.Data.Vec3CoordArr.begin(); i != triangleList.Data.Vec3CoordArr.end(); ++i )
	{
		VertexAttribPointer3fv(i->first, 0, reinterpret_cast<const char*>(&triangleList.Data.Vec3CoordArr[i->first][0]));
	}

	if (!triangleList.Data.Vec3CoordArr.empty())
	{
		glDrawArrays(GL_TRIANGLES,0,triangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].size());
	}

}


} //namespace SE