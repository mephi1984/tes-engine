#ifndef SIMPLE_LAND_H_INCLUDED
#define SIMPLE_LAND_H_INCLUDED

#include "include/Utils/Utils.h"

namespace SE
{

const int CONST_DEFAULT_LAND_WIDTH = 16;
const int CONST_DEFAULT_LAND_HEIGHT = 16;
const float CONST_DEFAULT_LAND_CELL_INVERVAL = 2.0f;

const float CONST_LAND_MOVE_STEP = 0.3f;


//Struct for one land vertex
struct TSimpleLandVertex 
{
	vec3 pos;		//Vertex position (x,y,z)
	vec3 norm;		//Vertex normal vector
	vec3 tangent;	//Vertex tangent vector
	vec3 binorm;	//Vertex binormal vector
	int landtype;	//Vertex type - not in use right now
};

struct TVBOLandVertex
{
	std::vector<vec3> pos;		//Vertex position (x,y,z)
	std::vector<vec3> norm;		//Vertex normal vector
	std::vector<vec3> tangent;	//Vertex tangent vector
	std::vector<vec3> binorm;	//Vertex binormal vector
	std::vector<vec2> texCoord;  //Vertex texture coords
};

//Land triangle
struct TLandSimpleTriangle
{
	vec3 PointList[3];	//3 vertices
	vec3 NormVec;		//Normal vector (not for draw, but for physics/collision)
};

//Land class
class TSimpleLandClass
{
private:
	
	int Width; //Number of vertices along X
	int Height; //Number of vertices along -Z
	float CellInterval; //Distance between vertices

	std::vector<TSimpleLandVertex> VertexMatrix;
	std::vector<TLandSimpleTriangle> TriangleMatrix;
	
	//Textures:
	cardinal TexID;
	cardinal NormTexID;

	//For VBO:
	std::vector<word> IndexArr;
	TVBOLandVertex VBOLandVertexArr;

	cardinal IndexBuffer;
	cardinal VertexBuffer;
	cardinal NormBuffer;
	cardinal TangentBuffer;
	cardinal BinormBuffer;
	cardinal TexcoordBuffer;

	void CreateVBO();
	void FreeVBO();

	void UpdateVBO();
	void UpdateTriangleMatrix();


	vec3 CalcCrossingForPoint(const vec3& lineFrom, const vec3& lineTo, int i, int j);
	vec3 CalcCrossingForPoint(const vec3& pos, int i, int j);
public:
	
	TSimpleLandClass(); 

	~TSimpleLandClass(); 

	bool LoadFromFile(const std::string& filename);
	//Load land struct from ls1 file

	void SaveToFile(char* filename); //Xperimental. Not in use right now

	void SetTexture(const std::string& texFilename);
	//Set land texture name

	void SetNormTexture(const std::string& normTexFilename);
	//Set land texture name

	void DrawImmediate(); //Very sloooow. Need to speed up!
	//Draw land immediate

	void DrawVBO();

	vec3 CalcCrossing(vec3 LineFrom, vec3 LineTo);
	//Calculate intersection (segment LiteFrom -> LineTo and the land)

	vec3 CalcVerticalCrossing(const vec3& pos);
	//Calculate land height on (pos.x, pos.y)

	float CalcVerticalCrossingFast(const vec3& pos);
	//Calculate approximate land height on (pos.x, pos.y)

	bool IsOnTheLand(const vec3& pos);

	void RecalcNormalVectors();
	//Recalc normal vectors - must be called after land is modified

	void MoveLandUp(vec3 CursorPos, float R);//NIU
	//Move land up under cursor

	void MoveLandDown(vec3 CursorPos, float R);//NIU
	//Move land down under cursor

	void SmoothLand(vec3 CursorPos, float R);//NIU
	//Smooth land up under cursor

	void TextureLand(vec3 CursorPos, float R, int ltype);//NIU
	//Set land type under cursor

	void Scale(float s);

	void Scale(vec3 s);

	void Move(const vec3& v);

	int GetWidth();

	int GetHeight();
	
	float* GetTriangleMatrix();
	//Use it for physics only!
};

} //namespace SE

#endif
