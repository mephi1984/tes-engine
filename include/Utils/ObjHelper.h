#ifndef OBJ_HELPER_INCLUDED
#define OBJ_HELPER_INCLUDED

#include "include/Render/RenderParams.h"
#include "include/Render/RenderMisc.h"
#include "include/ShaderManager/ShaderManager.h"
#include <Eigen/Geometry>
#include <Eigen/Dense>

#include "boost/regex.hpp"

namespace SE
{
	extern const std::string UNNAMED_MODEL;
	extern const std::string UNNAMED_GROUP;
	extern const std::string DEFAULT_TEXTURE;
	extern const Vector4f DEFAULT_MATERIAL;

	struct TriangleIndexes
	{
		std::vector<int> vertexes;
		std::vector<int> texCoords;
		std::vector<int> normals;

		TriangleIndexes()
		{
			vertexes.reserve(4);
			texCoords.reserve(4);
			normals.reserve(4);
		}
	};

	struct ObjGroup
	{
		std::string name;

		Vector4f color; // from usemtl
		std::string texture; // from usemap

		std::vector<TriangleIndexes> indexes;
	};

	struct ObjModel
	{
		std::string name;
		std::vector<ObjGroup> groups;
	};

	struct ObjData
	{
		std::vector<Vector3f> vertexes;
		std::vector<Vector2f> texCoords;
		std::vector<Vector3f> normals;

		std::vector<ObjModel> models;
	};

	ObjData loadObjFile(const std::string &filename);

	std::vector<std::vector<TRenderPair>> ObjDataToRenderPairs(const ObjData &data);

} //SE

#endif