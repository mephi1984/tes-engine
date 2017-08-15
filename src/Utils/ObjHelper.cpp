#include "include/Utils/ObjHelper.h"

#include <chrono>

using namespace std;

namespace SE
{
	const string UNNAMED_MODEL = "unnamed_model";
	const string UNNAMED_GROUP = "unnamed_group";
	const string DEFAULT_TEXTURE = "white.bmp";
	const Vector4f DEFAULT_COLOR = { 1, 1, 1, 1 };

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
				dstGroup->first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = srcGroup->texture;

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

							dstColorArr.push_back(srcGroup->color);
							dstColorArr.push_back(srcGroup->color);
							dstColorArr.push_back(srcGroup->color);
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

							dstColorArr.push_back(srcGroup->color);
							dstColorArr.push_back(srcGroup->color);
							dstColorArr.push_back(srcGroup->color);
							dstColorArr.push_back(srcGroup->color);
							dstColorArr.push_back(srcGroup->color);
							dstColorArr.push_back(srcGroup->color);
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

	Vector3f loadVertex(string &line);
	Vector2f loadTexCoord(string &line);
	Vector3f loadNormal(string &line);
	TriangleIndexes loadIndexes(string &line);
	Vector4f loadColor(std::string &value);

	ObjData loadObjFile(const string &filename)
	{
		string test = "v 1 2 3";
		auto arr = loadVertex(test);

		ObjData data;

		data.vertexes.push_back(Vector3f(0, 0, 0));
		data.texCoords.push_back(Vector2f(0, 0));
		data.normals.push_back(Vector3f(0, 0, 0));

		bool dataLoadedToGroup = false;
		bool dataLoadedToModel = false;

		ObjModel curModel;
		curModel.name = UNNAMED_MODEL;

		ObjGroup curGroup;
		curGroup.name = UNNAMED_GROUP;

		Vector4f color = DEFAULT_COLOR;
		string texture = DEFAULT_TEXTURE;

		string line;
		line.reserve(255);

// 		float currentLine = 0;
// 		float totalLines = 0;
// 		float progress = 0;

 		ifstream ifs;
 		ifs.open(ST::PathToResources + filename);
// 
// 		while (ifs.good())
// 		{
// 			ifs.ignore(numeric_limits<streamsize>::max(), '\n');
// 			totalLines += 1.f;
// 		}
// 
// 		ifs.close();
// 		ifs.open(ST::PathToResources + filename);
//		
// 		auto lastMoment = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
// 		float lastProgress = 0;
// 
// 		FILE *f;
// 		f = fopen("timelog.txt", "w");

		while (ifs.good())
		{
			std::getline(ifs, line);

			switch (line[0])
			{
			case 'o':
				if (dataLoadedToModel)
				{
					curGroup.color = color;
					curGroup.texture = texture;
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
					curGroup.color = color;
					curGroup.texture = texture;
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
					data.vertexes.push_back(loadVertex(line));
					break;

				case 't':
					data.texCoords.push_back(loadTexCoord(line));
					break;

				case 'n':
					data.normals.push_back(loadNormal(line));
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
				string keyword = line.substr(0, 6);
				if (keyword == "usemtl")
				{
					color = loadColor(line.substr(7));
				}
				else if (keyword == "usemap")
				{
					texture = line.substr(7);
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
			curGroup.color = color;
			curGroup.texture = texture;
			curModel.groups.push_back(curGroup);
			data.models.push_back(curModel);
		}

		//fclose(f);

		ifs.close();
		return data;
	}

	Vector3f loadVertex(string &line)
	{
		Vector3f result;
		char
			*begin,
			*end = line._Myptr() + 1;

		for (int i = 0; i < 3; ++i)
		{
			++end;
			begin = end;
			while (*end != ' ' && *end != 0) ++end;
			try
			{
				result[i] = stof(string(begin, end));
			}
			catch (const invalid_argument &e)
			{
				throw ErrorFileNotCorrect("Invalid OBJ file line: " + line);
			}
		}
		return result;
	}

	Vector2f loadTexCoord(string &line)
	{
		Vector2f result;
		char
			*begin,
			*end = line._Myptr() + 2;

		for (int i = 0; i < 2; ++i)
		{
			++end;
			begin = end;
			while (*end != ' ' && *end != 0) ++end;
			try
			{
				result[i] = stof(string(begin, end));
			}
			catch (const invalid_argument &e)
			{
				throw ErrorFileNotCorrect("Invalid OBJ file line: " + line);
			}
		}
		return result;
	}

	Vector3f loadNormal(string &line)
	{
		Vector3f result;
		char
			*begin,
			*end = line._Myptr() + 2;

		for (int i = 0; i < 3; ++i)
		{
			++end;
			begin = end;
			while (*end != ' ' && *end != 0) ++end;
			try
			{
				result[i] = stof(string(begin, end));
			}
			catch (const invalid_argument &e)
			{
				throw ErrorFileNotCorrect("Invalid OBJ file line: " + line);
			}
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

	Vector4f loadColor(std::string &value)
	{
		unsigned int color;
		stringstream ss;
		ss << hex << value;
		ss >> color;

		Vector4f result;

		result(3) = (color % 256) / 255.f;
		result(2) = ((color >> 8) % 256) / 255.f;
		result(1) = ((color >> 16) % 256) / 255.f;
		result(0) = ((color >> 24) % 256) / 255.f;

		return result;
	}

} // SE