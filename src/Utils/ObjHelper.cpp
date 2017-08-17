#include "include/Utils/ObjHelper.h"

#include "include/Engine.h"

//#include "boost/property_tree/json_parser.hpp"
//#include <chrono>

using namespace std;

namespace SE
{
	const string UNNAMED_MODEL = "unnamed_model";
	const string UNNAMED_GROUP = "unnamed_group";
	const Vector4f NO_COLOR = { 1, 0, 1, 1 };
	const Vector4f DEFAULT_COLOR = { 1, 1, 1, 1 };
	const std::string DEFAULT_TEXTURE = "white.bmp";
	const std::string NO_TEXTURE = "noTexture.bmp";

	vector<vector<TRenderPair>> ObjDataToRenderPairs(const ObjData &data)
	{
		vector<vector<TRenderPair>> result(data.models.size());

		auto srcModel = data.models.begin();
		auto dstModel = result.begin();

		for (; srcModel != data.models.end(); ++srcModel, ++dstModel)
		{
			dstModel->resize(srcModel->groups.size());

			auto srcGroup = srcModel->groups.begin();
			auto dstGroup = dstModel->begin();
			
			for (; srcGroup != srcModel->groups.end(); ++srcGroup, ++dstGroup)
			{
				auto textureRecord = srcGroup->MTL.get_optional<string>("Texture");
				auto ambientRecord = srcGroup->MTL.get_child_optional("ambientLight");
				auto diffuseRecord = srcGroup->MTL.get_child_optional("ambientLight");

				string texture;
				Vector4f color;

				if (textureRecord)
				{
					texture = data.MTLroot + textureRecord.get();

					if (!ambientRecord && !diffuseRecord)
					{
						color = DEFAULT_COLOR;
					}
				}
				
				if (ambientRecord || diffuseRecord)
				{
					Vector3f ambientLight = ambientRecord ? JSONVectorReader::readVector3f(ambientRecord.get()) : Vector3f(0, 0, 0);
					Vector3f diffuseLight = diffuseRecord ? JSONVectorReader::readVector3f(diffuseRecord.get()) : Vector3f(0, 0, 0);

					color << ambientLight + diffuseLight, 1.f;

					if (!textureRecord)
					{
						texture = DEFAULT_TEXTURE;
					}
				}
				
				if (!textureRecord && !ambientRecord && !diffuseRecord)
				{
					color = NO_COLOR;
					texture = NO_TEXTURE;
				}

				dstGroup->first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = GetFileName(texture);
				ResourceManager->TexList.AddTexture(texture);

				auto indexes = srcGroup->indexes.begin();

				auto& dstVertexArr = dstGroup->second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB];
				auto& dstNormalsArr = dstGroup->second.Data.Vec3CoordArr[CONST_STRING_NORMAL_ATTRIB];
				auto& dstTexVertexArr = dstGroup->second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB];
				auto& dstColorArr = dstGroup->second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB];


				try
				{
					for (; indexes != srcGroup->indexes.end(); ++indexes)
					{
						if (indexes->vertexes.size() == 3)
						{
							dstVertexArr.push_back(data.vertexes[indexes->vertexes[0] >= 0 ? indexes->vertexes[0] : data.vertexes.size() + indexes->vertexes[0]]);
							dstVertexArr.push_back(data.vertexes[indexes->vertexes[1] >= 0 ? indexes->vertexes[1] : data.vertexes.size() + indexes->vertexes[1]]);
							dstVertexArr.push_back(data.vertexes[indexes->vertexes[2] >= 0 ? indexes->vertexes[2] : data.vertexes.size() + indexes->vertexes[2]]);

							dstNormalsArr.push_back(data.normals[indexes->normals[0] >= 0 ? indexes->normals[0] : data.normals.size() + indexes->normals[0]]);
							dstNormalsArr.push_back(data.normals[indexes->normals[1] >= 0 ? indexes->normals[1] : data.normals.size() + indexes->normals[1]]);
							dstNormalsArr.push_back(data.normals[indexes->normals[2] >= 0 ? indexes->normals[2] : data.normals.size() + indexes->normals[2]]);

							dstTexVertexArr.push_back(data.texCoords[indexes->texCoords[0] >= 0 ? indexes->texCoords[0] : data.texCoords.size() + indexes->texCoords[0]]);
							dstTexVertexArr.push_back(data.texCoords[indexes->texCoords[1] >= 0 ? indexes->texCoords[1] : data.texCoords.size() + indexes->texCoords[1]]);
							dstTexVertexArr.push_back(data.texCoords[indexes->texCoords[2] >= 0 ? indexes->texCoords[2] : data.texCoords.size() + indexes->texCoords[2]]);

							dstColorArr.push_back(color);
							dstColorArr.push_back(color);
							dstColorArr.push_back(color);
						}
						else if (indexes->vertexes.size() == 4)
						{
							dstVertexArr.push_back(data.vertexes[indexes->vertexes[0] >= 0 ? indexes->vertexes[0] : data.vertexes.size() + indexes->vertexes[0]]);
							dstVertexArr.push_back(data.vertexes[indexes->vertexes[1] >= 0 ? indexes->vertexes[1] : data.vertexes.size() + indexes->vertexes[1]]);
							dstVertexArr.push_back(data.vertexes[indexes->vertexes[2] >= 0 ? indexes->vertexes[2] : data.vertexes.size() + indexes->vertexes[2]]);
							dstVertexArr.push_back(data.vertexes[indexes->vertexes[2] >= 0 ? indexes->vertexes[2] : data.vertexes.size() + indexes->vertexes[2]]);
							dstVertexArr.push_back(data.vertexes[indexes->vertexes[3] >= 0 ? indexes->vertexes[3] : data.vertexes.size() + indexes->vertexes[3]]);
							dstVertexArr.push_back(data.vertexes[indexes->vertexes[0] >= 0 ? indexes->vertexes[0] : data.vertexes.size() + indexes->vertexes[0]]);

							dstNormalsArr.push_back(data.normals[indexes->normals[0] >= 0 ? indexes->normals[0] : data.normals.size() + indexes->normals[0]]);
							dstNormalsArr.push_back(data.normals[indexes->normals[1] >= 0 ? indexes->normals[1] : data.normals.size() + indexes->normals[1]]);
							dstNormalsArr.push_back(data.normals[indexes->normals[2] >= 0 ? indexes->normals[2] : data.normals.size() + indexes->normals[2]]);
							dstNormalsArr.push_back(data.normals[indexes->normals[2] >= 0 ? indexes->normals[2] : data.normals.size() + indexes->normals[2]]);
							dstNormalsArr.push_back(data.normals[indexes->normals[3] >= 0 ? indexes->normals[3] : data.normals.size() + indexes->normals[3]]);
							dstNormalsArr.push_back(data.normals[indexes->normals[0] >= 0 ? indexes->normals[0] : data.normals.size() + indexes->normals[0]]);

							dstTexVertexArr.push_back(data.texCoords[indexes->texCoords[0] >= 0 ? indexes->texCoords[0] : data.texCoords.size() + indexes->texCoords[0]]);
							dstTexVertexArr.push_back(data.texCoords[indexes->texCoords[1] >= 0 ? indexes->texCoords[1] : data.texCoords.size() + indexes->texCoords[1]]);
							dstTexVertexArr.push_back(data.texCoords[indexes->texCoords[2] >= 0 ? indexes->texCoords[2] : data.texCoords.size() + indexes->texCoords[2]]);
							dstTexVertexArr.push_back(data.texCoords[indexes->texCoords[2] >= 0 ? indexes->texCoords[2] : data.texCoords.size() + indexes->texCoords[2]]);
							dstTexVertexArr.push_back(data.texCoords[indexes->texCoords[3] >= 0 ? indexes->texCoords[3] : data.texCoords.size() + indexes->texCoords[3]]);
							dstTexVertexArr.push_back(data.texCoords[indexes->texCoords[0] >= 0 ? indexes->texCoords[0] : data.texCoords.size() + indexes->texCoords[0]]);

							dstColorArr.push_back(color);
							dstColorArr.push_back(color);
							dstColorArr.push_back(color);
							dstColorArr.push_back(color);
							dstColorArr.push_back(color);
							dstColorArr.push_back(color);
						}
					}
				}
				catch (const out_of_range &e)
				{
					throw ErrorFileNotCorrect("Out of range index in OBJ file.");
				}

				dstGroup->second.RefreshBuffer();
			}
		}
		return result;
	}

	Vector3f loadVector3f(char* dateBegin);
	Vector2f loadVector2f(char* dateBegin);
	TriangleIndexes loadIndexes(string &line);

	ObjData loadObjFile(const string &OBJfile, const std::string &MTLfile)
	{
		ifstream ifs;
		ifs.open(ST::PathToResources + OBJfile);
		if (!ifs.good())
		{
			throw ErrorFileNotFound(ST::PathToResources + OBJfile);
		}

		ObjData data;

		data.MTLroot = GetFilePath(MTLfile);

		boost::property_tree::ptree MTLtree;
		if (!MTLfile.empty())
		{
			MTLtree = loadMtlFile(MTLfile);
		}

		data.vertexes.push_back(Vector3f(0, 0, 0));
		data.texCoords.push_back(Vector2f(0, 0));
		data.normals.push_back(Vector3f(0, 0, 0));

		bool dataLoadedToGroup = false;
		bool dataLoadedToModel = false;

		ObjModel curModel;
		curModel.name = UNNAMED_MODEL;

		ObjGroup curGroup;
		curGroup.name = UNNAMED_GROUP;

		string line;
		line.reserve(255);

// 		float currentLine = 0;
// 		float totalLines = 0;
// 		float progress = 0;
// 
//		ifstream quickReader;
//		quickReader.open(ST::PathToResources + OBJpath);
// 		while (quickReader.good())
// 		{
// 			quickReader.ignore(numeric_limits<streamsize>::max(), '\n');
// 			totalLines += 1.f;
// 		}
// 		quickReader.close();
//		
// 		auto lastMoment = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
// 		float lastProgress = 0;
// 
// 		FILE *f;
// 		f = fopen("timelog.txt", "w");

		while (ifs.good())
		{
			getline(ifs, line);

			switch (line[0])
			{
			case 'o':
				if (dataLoadedToModel)
				{
					curModel.groups.push_back(curGroup);
					data.models.push_back(curModel);
					curModel.groups.clear();
					curGroup.indexes.clear();
					dataLoadedToGroup = false;
					dataLoadedToModel = false;
				}
				curModel.name = line.substr(2);
				break;

			case 'g':
				if (dataLoadedToGroup)
				{
					curModel.groups.push_back(curGroup);
					curGroup.indexes.clear();
					dataLoadedToGroup = false;
				}
				curGroup.name = line.substr(2);
				break;

			case 'v':
				switch (line[1])
				{
				case ' ':
					data.vertexes.push_back(loadVector3f(line._Myptr() + 2));
					break;

				case 't':
					data.texCoords.push_back(loadVector2f(line._Myptr() + 3));
					break;

				case 'n':
					data.normals.push_back(loadVector3f(line._Myptr() + 3));
					break;
				}
				break;

			case 'f':
			{
				dataLoadedToGroup = true;
				dataLoadedToModel = true;
				curGroup.indexes.push_back(loadIndexes(line));
				break;
			}

			case 'u':
			{
				if (line.substr(0, 6) == "usemtl")
				{
					auto MTL = MTLtree.get_child_optional(line.substr(7));
					if (MTL)
					{
						curGroup.MTL = MTL.get();
					}
				}
				break;
			}
			}

// 			currentLine += 1.f;
// 			progress = currentLine / totalLines;
// 
// 			if (progress - lastProgress > 0.001f)
// 			{
// 				auto newMoment = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
// 				double dt = (double)(newMoment - lastMoment);
// 
// 				fprintf(f, "%0.000f : line %f , time %f\n", progress, currentLine, dt);
// 
// 				lastProgress = progress;
// 				lastMoment = newMoment;
// 			}
		}

		if (dataLoadedToGroup)
		{
			curModel.groups.push_back(curGroup);
			data.models.push_back(curModel);
		}

		//fclose(f);

		ifs.close();
		return data;
	}

	boost::property_tree::ptree loadMtlFile(const string &MTLpath)
	{
		ifstream ifs(ST::PathToResources + MTLpath);

		if (!ifs.good())
		{
			throw ErrorFileNotFound(ST::PathToResources + MTLpath);
		}

		boost::property_tree::ptree MTLs;
		boost::property_tree::ptree newMTL;

		string name;
		name.reserve(32);

		string line;
		line.reserve(255);

		bool isEmptyMTL = true;

		while (ifs.good())
		{
			getline(ifs, line);

			switch (line[0])
			{
			case 'n':
			{
				if (line.substr(0, 6) == "newmtl")
				{
					if (!isEmptyMTL)
					{
						MTLs.put_child(name, newMTL);
					}
					name = line.substr(7);
					newMTL.clear();
					isEmptyMTL = true;
				}
				break;
			}

			case 'K':
			{
				boost::property_tree::ptree vector3fTree;

				Vector3f vector3f = loadVector3f(line._Myptr() + 3);

				for (int i = 0; i < 3; ++i)
				{
					boost::property_tree::ptree vector3fTreeChild;
					vector3fTreeChild.put("", vector3f[i]);
					vector3fTree.push_back(make_pair("", vector3fTreeChild));
				}

				switch (line[1])
				{
				case 'a':
				{
					newMTL.put_child("ambientLight", vector3fTree);
					isEmptyMTL = false;
					break;
				}
				case 'd':
				{
					newMTL.put_child("diffuseLight", vector3fTree);
					isEmptyMTL = false;
					break;
				}
				case 's':
				{
					newMTL.put_child("specularLight", vector3fTree);
					isEmptyMTL = false;
					break;
				}
				}
				break;
			}

			case 'N':
			{
				if (line[1] == 's')
				{
					newMTL.put("specularPower", line.substr(3));
					isEmptyMTL = false;
				}
				break;
			}

			case 'm':
			{
				if (line.substr(0, 5) == "map_K")
				{
					switch (line[5])
					{
					case 'a':
					case 'd':
					{
						newMTL.put("Texture", line.substr(line[7] == '/' || line[7] == '\\' ? 8 : 7));
						isEmptyMTL = false;
						break;
					}
					case 's':
					{
						newMTL.put("NormalMap", line.substr(line[7] == '/' || line[7] == '\\' ? 8 : 7));
						isEmptyMTL = false;
						break;
					}
					}
				}
				break;
			}

			}
		}

		ifs.close();

		if (!isEmptyMTL)
		{
			MTLs.put_child(name, newMTL);
		}

		//boost::property_tree::json_parser::write_json("MTLtree.json", MTLs);

		return MTLs;
	}

	Vector3f loadVector3f(char *dataBegin)
	{
		Vector3f result;
		char *begin = dataBegin;
		char *end = dataBegin;

		for (int i = 0; i < 3; ++i)
		{
			while (*end != ' ' && *end != 0) ++end;
			try
			{
				result[i] = stof(string(begin, end));
			}
			catch (const invalid_argument &e)
			{
				throw ErrorFileNotCorrect("Invalid OBJ file line: " + string(dataBegin));
			}

			begin = ++end; // skip only one whitespace
		}
		return result;
	}

	Vector2f loadVector2f(char *dataBegin)
	{
		Vector2f result;
		char *begin = dataBegin;
		char *end = dataBegin;

		for (int i = 0; i < 2; ++i)
		{
			while (*end != ' ' && *end != 0) ++end;
			try
			{
				result[i] = stof(string(begin, end));
			}
			catch (const invalid_argument &e)
			{
				throw ErrorFileNotCorrect("Invalid OBJ file line: " + string(dataBegin));
			}
			begin = ++end;  // skip only one whitespace
		}
		return result;
	}

	TriangleIndexes loadIndexes(string &line)
	{
// 		TriangleIndexes result;
// 		boost::regex xRegEx("(\\d+)\/(\\d+)\\/(\\d+)+");
// 		boost::smatch xResults;
// 		boost::sregex_token_iterator xIt1(line.begin(), line.end(), xRegEx, 1);
// 		boost::sregex_token_iterator xIt2(line.begin(), line.end(), xRegEx, 2);
// 		boost::sregex_token_iterator xIt3(line.begin(), line.end(), xRegEx, 3);
// 		boost::sregex_token_iterator xInvalidIt;
// 		while (xIt1 != xInvalidIt)
// 		{
// 			result.vertexes.push_back(boost::lexical_cast<int>(*xIt1));
// 			xIt1++;
// 		}
// 		while (xIt2 != xInvalidIt)
// 		{
// 			result.texCoords.push_back(boost::lexical_cast<int>(*xIt2));
// 			xIt2++;
// 		}
// 		while (xIt3 != xInvalidIt)
// 		{
// 			result.normals.push_back(boost::lexical_cast<int>(*xIt3));
// 			xIt3++;
// 		}
// 		return result;

 		TriangleIndexes result;
 		char
 			*begin,
 			*end = line._Myptr() + 1;
		
 		while (*end != 0)
 		{
 			int property = 0;
 
 			do
 			{
 				++end;
 				begin = end;
 				while (*end != ' ' && *end != '/' && *end != 0)
 					++end;
 
 				int value;
 				if (begin < end)
 				{
 					try
 					{
 						value = stoi(string(begin, end));
 					}
 					catch (const invalid_argument &e)
 					{
 						throw ErrorFileNotCorrect("Invalid OBJ file line: " + line);
 					}
 				}
 				else
 				{
 					value = 0;
 				}
 
 				if (property == 0)
 				{
 					result.vertexes.push_back(value);
 				}
 				else if (property == 1)
 				{
 					result.texCoords.push_back(value);
 				}
 				else
 				{
 					result.normals.push_back(value);
 				}
 
 			} while (++property < 3 && *end == '/');
 
 			if (property < 2)
 			{
 				result.texCoords.push_back(0);
 			}
 			if (property < 3)
 			{
 				result.normals.push_back(0);
 			}
 		}
 		return result;
	}

} // SE