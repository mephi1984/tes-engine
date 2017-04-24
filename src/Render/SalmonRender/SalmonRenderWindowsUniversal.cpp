#include "include/Render/SalmonRender/SalmonRenderWindowsUniversal.h"

#include "include/Utils/Utils.h"
#include "include/Engine.h"

namespace SE
{

	void TSalmonRendererWindowsUniversal::DrawTriangleList(const TTriangleList& triangleList)
	{

		DrawTriangleList(triangleList, GL_TRIANGLES);
		
	}

	void TSalmonRendererWindowsUniversal::DrawTriangleList(const TTriangleList& triangleList, GLenum mode)
	{
		AssertIfInMainThread();

		BOOST_FOREACH(auto& i, triangleList.Data.Vec2CoordArr)
		{
			glBindBuffer(GL_ARRAY_BUFFER, triangleList.VertBufferArr[i.first]->Buffer);
			VertexAttribPointer2fv(i.first, 0, NULL);
		}

		BOOST_FOREACH(auto& i, triangleList.Data.Vec3CoordArr)
		{
			glBindBuffer(GL_ARRAY_BUFFER, triangleList.VertBufferArr[i.first]->Buffer);
			VertexAttribPointer3fv(i.first, 0, NULL);
		}

		BOOST_FOREACH(auto& i, triangleList.Data.Vec4CoordArr)
		{
			glBindBuffer(GL_ARRAY_BUFFER, triangleList.VertBufferArr[i.first]->Buffer);
			VertexAttribPointer4fv(i.first, 0, NULL);
		}

		if (!triangleList.Data.Vec3CoordArr.empty())
		{
			glDrawArrays(mode, 0, triangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].size());
		}

	}


} //namespace SE