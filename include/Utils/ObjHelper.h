#ifndef OBJ_HELPER_INCLUDED
#define OBJ_HELPER_INCLUDED

#include <Eigen/Geometry>
#include <Eigen/Dense>

#include "boost/property_tree/ptree.hpp"
//#include "boost/regex.hpp"

#include "include/Utils/FileUtils/FileUtils.h"
#include "include/Render/RenderParams.h"
#include "include/Utils/JSONVectorReader.h"

namespace SE
{
	class TResourceManager;

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

		boost::property_tree::ptree MTL;

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

		std::string OBJfolder;
	};

	// loads an OBJ file with the given full name relativly SE::PathToResources,
	// then swaps .obj to .mtl, reads MTL, then truncs file name & loads textures from MTL, adding textures paths to folder's
	ObjData loadObjFile(const std::string &OBJfile);

	boost::property_tree::ptree loadMtlFile(const std::string &filename);

	std::vector<std::vector<TRenderPair>> ObjDataToRenderPairs(const ObjData &data);

} //SE

#endif