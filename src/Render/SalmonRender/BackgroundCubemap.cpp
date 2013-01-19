#include "include/Engine.h"

namespace SE
{

const float CONST_CUBEMAP_SIDE = 50.0f;

TBackgroundCubemapClass::TBackgroundCubemapClass()
{


	vec3 pos1 = vec3(-CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE);
	vec3 pos2 = vec3(-CONST_CUBEMAP_SIDE,  CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE);
	vec3 pos3 = vec3(-CONST_CUBEMAP_SIDE,  CONST_CUBEMAP_SIDE,  CONST_CUBEMAP_SIDE);
	vec3 pos4 = vec3(-CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE,  CONST_CUBEMAP_SIDE);


	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos1);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos2);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos3);

	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos3);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos4);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos1);

	pos1 = vec3(CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE);
	pos2 = vec3(CONST_CUBEMAP_SIDE,  CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE);
	pos3 = vec3(CONST_CUBEMAP_SIDE,  CONST_CUBEMAP_SIDE,  CONST_CUBEMAP_SIDE);
	pos4 = vec3(CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE,  CONST_CUBEMAP_SIDE);

	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos1);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos2);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos3);

	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos3);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos4);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos1);

	pos1 = vec3(-CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE);
	pos2 = vec3(CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE);
	pos3 = vec3(CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE,  CONST_CUBEMAP_SIDE);
	pos4 = vec3(-CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE,  CONST_CUBEMAP_SIDE);

	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos1);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos2);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos3);

	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos3);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos4);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos1);

	pos1 = vec3(-CONST_CUBEMAP_SIDE, CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE);
	pos2 = vec3(CONST_CUBEMAP_SIDE, CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE);
	pos3 = vec3(CONST_CUBEMAP_SIDE, CONST_CUBEMAP_SIDE,  CONST_CUBEMAP_SIDE);
	pos4 = vec3(-CONST_CUBEMAP_SIDE, CONST_CUBEMAP_SIDE,  CONST_CUBEMAP_SIDE);

	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos1);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos2);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos3);

	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos3);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos4);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos1);

	pos1 = vec3(-CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE);
	pos2 = vec3(CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE);
	pos3 = vec3(CONST_CUBEMAP_SIDE, CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE);
	pos4 = vec3(-CONST_CUBEMAP_SIDE,  CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE);

	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos1);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos2);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos3);

	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos3);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos4);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos1);

	pos1 = vec3(-CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE, CONST_CUBEMAP_SIDE);
	pos2 = vec3(CONST_CUBEMAP_SIDE, -CONST_CUBEMAP_SIDE, CONST_CUBEMAP_SIDE);
	pos3 = vec3(CONST_CUBEMAP_SIDE, CONST_CUBEMAP_SIDE, CONST_CUBEMAP_SIDE);
	pos4 = vec3(-CONST_CUBEMAP_SIDE,  CONST_CUBEMAP_SIDE, CONST_CUBEMAP_SIDE);

	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos1);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos2);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos3);

	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos3);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos4);
	TriangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].push_back(pos1);

	TriangleList.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].resize(36); //Texture coords actually not in use


	TriangleList.RefreshBuffer();
}


void TBackgroundCubemapClass::Draw()
{
	
	mat4 m = Renderer->GetModelviewMatrix();

	m.m[12] = 0.f;
	m.m[13] = 0.f;
	m.m[14] = 0.f;

	Renderer->PushSpecialMatrix(m);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, BkgTexId);

	Renderer->DrawTriangleList(TriangleList);
	Renderer->PopMatrix();

}

} //namespace SE