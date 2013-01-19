#include "include/Engine.h"

namespace SE
{

const float CONST_LAND_Y_LIMIT = 15.0f;
//Limit for y pos of land (edge or trench/dip)

const float CONST_MIN_COS_BETWEEN_CURSOR_AND_VERTEX = 0.99f;
//See CalcCrossing() for explanation


TSimpleLandClass::TSimpleLandClass()
	: Width(CONST_DEFAULT_LAND_WIDTH),
	Height(CONST_DEFAULT_LAND_HEIGHT),
	CellInterval(CONST_DEFAULT_LAND_CELL_INVERVAL),
	TexID(0),
	NormTexID(0)
{
	int k;
	
	VertexMatrix.resize(Width*Height);
	
	for (int i=0; i<Height; i++)
		for (int j=0; j<Width; j++)
		{
			k = j + i*Width;
			VertexMatrix[k].norm = vec3(0.0f, 1.0f, 0.0f);
			VertexMatrix[k].tangent = vec3(0.0f, 0.0f, 1.0f);
			VertexMatrix[k].binorm = vec3(1.0f, 0.0f, 0.0f);

			VertexMatrix[k].pos = vec3(
				j*CellInterval - (CellInterval*Width)/2.0f,
				1.0f,
				i*CellInterval - (CellInterval*Height)/2.0f);

			VertexMatrix[k].landtype = rand() % 2;
		}

	TriangleMatrix.resize(2*(Width-1)*(Height-1));
	
	UpdateTriangleMatrix();

	CreateVBO();
}

TSimpleLandClass::~TSimpleLandClass()
{
	FreeVBO();

	TriangleMatrix.clear();
	VertexMatrix.clear();
	
	if (TexID != 0)
	{
		ResourceManager->TexList.DeleteTexture(TexID);
		TexID = 0;
	}

	if (NormTexID != 0)
	{
		ResourceManager->TexList.DeleteTexture(NormTexID);
		NormTexID = 0;
	}

}

void TSimpleLandClass::CreateVBO()
{
	IndexArr.resize(6*(Width-1)*(Height-1));
	
	//VBOLandVertexArr.resize(Width*Height);
	VBOLandVertexArr.pos.resize(Width * Height);
	VBOLandVertexArr.norm.resize(Width * Height);
	VBOLandVertexArr.tangent.resize(Width * Height);
	VBOLandVertexArr.binorm.resize(Width * Height);
	VBOLandVertexArr.texCoord.resize(Width * Height);

	for (int i = 0; i < Height-1; i++)
		for (int j = 0; j < Width-1; j++)
		{
			
			IndexArr[6*(j + i*(Width-1))] = j + i*(Width);
			IndexArr[6*(j + i*(Width-1)) + 1] = j + 1 + i*(Width);
			IndexArr[6*(j + i*(Width-1)) + 2] = Width + j + i*(Width);

			IndexArr[6*(j + i*(Width-1)) + 3] = j + 1 + i*(Width);
			IndexArr[6*(j + i*(Width-1)) + 4] = Width + 1 + j + i*(Width);
			IndexArr[6*(j + i*(Width-1)) + 5] = Width + j + i*(Width);
			
		}

	for (int i = 0; i < Height-1; i++)
		for (int j = 0; j < Width-1; j++)
		{
			VBOLandVertexArr.pos[j + i*Width] = VertexMatrix[j + i*Width].pos;
			VBOLandVertexArr.norm[j + i*Width] = VertexMatrix[j + i*Width].norm;
			VBOLandVertexArr.tangent[j + i*Width] = VertexMatrix[j + i*Width].tangent;
			VBOLandVertexArr.binorm[j + i*Width] = VertexMatrix[j + i*Width].binorm;
			VBOLandVertexArr.texCoord[j + i*Width].v[0] = float(j) / Width;
			VBOLandVertexArr.texCoord[j + i*Width].v[1] = float(i) / Height;
		}

#ifdef TARGET_WIN32
	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*(Width-1)*(Height-1)*4, &IndexArr[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, Width * Height * sizeof(vec3), &(VBOLandVertexArr.pos[0]), GL_STATIC_DRAW);

	glGenBuffers(1, &NormBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, NormBuffer);
	glBufferData(GL_ARRAY_BUFFER, Width * Height * sizeof(vec3), &(VBOLandVertexArr.norm[0]), GL_STATIC_DRAW);

	glGenBuffers(1, &TangentBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, TangentBuffer);
	glBufferData(GL_ARRAY_BUFFER, Width * Height * sizeof(vec3), &(VBOLandVertexArr.tangent[0]), GL_STATIC_DRAW);

	glGenBuffers(1, &BinormBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, BinormBuffer);
	glBufferData(GL_ARRAY_BUFFER, Width * Height * sizeof(vec3), &(VBOLandVertexArr.binorm[0]), GL_STATIC_DRAW);

	glGenBuffers(1, &TexcoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, TexcoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, Width * Height * sizeof(vec2), &(VBOLandVertexArr.texCoord[0]), GL_STATIC_DRAW);
#endif

}

void TSimpleLandClass::FreeVBO()
{
#ifdef TARGET_WIN32
	glDeleteBuffers(1, &VertexBuffer);
	glDeleteBuffers(1, &NormBuffer);
	glDeleteBuffers(1, &TangentBuffer);
	glDeleteBuffers(1, &BinormBuffer);
	glDeleteBuffers(1, &TexcoordBuffer);
	glDeleteBuffers(1, &IndexBuffer);
#endif

	VBOLandVertexArr.pos.clear();
	VBOLandVertexArr.norm.clear();
	VBOLandVertexArr.tangent.clear();
	VBOLandVertexArr.binorm.clear();
	VBOLandVertexArr.texCoord.clear();
	IndexArr.clear();
}

void TSimpleLandClass::UpdateVBO()
{
	//Index array remains the same - only vertex array is to be changed

	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			VBOLandVertexArr.pos[j + i*Width] = VertexMatrix[j + i*Width].pos;
			VBOLandVertexArr.norm[j + i*Width] = VertexMatrix[j + i*Width].norm;
			VBOLandVertexArr.tangent[j + i*Width] = VertexMatrix[j + i*Width].tangent;
			VBOLandVertexArr.binorm[j + i*Width] = VertexMatrix[j + i*Width].binorm;
			VBOLandVertexArr.texCoord[j + i*Width].v[0] = float(j) / Width;
			VBOLandVertexArr.texCoord[j + i*Width].v[1] = float(i) / Height;
		}
	}

#ifdef TARGET_WIN32
	
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0,  Width * Height * sizeof(vec3), &(VBOLandVertexArr.pos[0]));

	glBindBuffer(GL_ARRAY_BUFFER, NormBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0,  Width * Height * sizeof(vec3), &(VBOLandVertexArr.norm[0]));

	glBindBuffer(GL_ARRAY_BUFFER, TangentBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0,  Width * Height * sizeof(vec3), &(VBOLandVertexArr.tangent[0]));

	glBindBuffer(GL_ARRAY_BUFFER, BinormBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0,  Width * Height * sizeof(vec3), &(VBOLandVertexArr.binorm[0]));

	glBindBuffer(GL_ARRAY_BUFFER, TexcoordBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0,  Width * Height * sizeof(vec2), &(VBOLandVertexArr.texCoord[0]));
#endif
}

vec3 TSimpleLandClass::CalcCrossingForPoint(const vec3& pos, int i, int j)
{
	vec3 lineFrom = pos;
	vec3 lineTo = pos;
	
	lineFrom.v[1] = CONST_LAND_Y_LIMIT;
	lineTo.v[1] = -CONST_LAND_Y_LIMIT;

	vec3 r = CalcCrossingForPoint(lineFrom, lineTo, i, j);

	if (r == lineFrom)
		return pos;
	
	return r;
}

vec3 TSimpleLandClass::CalcCrossingForPoint(const vec3& lineFrom,const  vec3& lineTo, int i, int j)
{
	if (i >= Height-1 || i < 0 || j >= Width-1 || j < 0)
		return lineFrom;

	TSmpTriangle tr;
	vec3 r;


	tr.p[0] = VertexMatrix[j + i*Width].pos;
	tr.p[1] = VertexMatrix[(j+1) + i*Width].pos;
	tr.p[2] = VertexMatrix[(j+1) + (i+1)*Width].pos;

	tr.v = tr.p[1] - tr.p[0];
	tr.w = tr.p[2] - tr.p[1];
	tr.n = CrossProduct(tr.v, tr.w);

	if (LineCrossTriangle(lineFrom, lineTo, tr, r) == 1)
		return r;

	tr.p[0] = VertexMatrix[j + i*Width].pos;
	tr.p[1] = VertexMatrix[j + (i+1)*Width].pos;
	tr.p[2] = VertexMatrix[(j+1) + (i+1)*Width].pos;

	tr.v = tr.p[1] - tr.p[0];
	tr.w = tr.p[2] - tr.p[1];
	tr.n = CrossProduct(tr.v, tr.w);

	if (LineCrossTriangle(lineFrom, lineTo, tr, r) == 1)
		return r;

	return lineFrom;
}

bool TSimpleLandClass::LoadFromFile(const std::string& filename)
{

	cardinal dwordCount;
	boost::shared_array<cardinal> fileArr = CreateMemFromFile<cardinal>((ResourceManager->PathToResources + filename).c_str(), dwordCount);

	if ((((char*)&fileArr[0])[0]!='L')||
		(((char*)&fileArr[0])[1]!='S')||
		(((char*)&fileArr[0])[2]!=0)||
		(((char*)&fileArr[0])[3]!=1))
		throw ErrorFileNotCorrect(filename);

	Width = fileArr[1];
	Height = fileArr[2];
	CellInterval = ((float*)&fileArr[0])[3];

	std::vector<TSimpleLandVertex> tempVertexMatrix; 
	tempVertexMatrix.resize(Width*Height);


	for (int i = 0; i < Height; i++)
		for (int j = 0; j < Width; j++)
		{
			int k = j + i * Width;
			tempVertexMatrix[k] = ((TSimpleLandVertex*)(&fileArr[0] + 4))[k];
		}

	//Replace old matrices with new

	TriangleMatrix.resize(2*(Width-1)*(Height-1));
	VertexMatrix = tempVertexMatrix;
	
	UpdateTriangleMatrix();

	RecalcNormalVectors();

	FreeVBO();
	CreateVBO();

	return true;
}



void TSimpleLandClass::SaveToFile(char* filename)
{
	//NIU

	//Need to clean up
#ifdef TARGET_WIN32

	OFSTRUCT ofstruct;
	DWORD FSize;
	DWORD BytesWriten;
	char* FileArr;

	//Открываем файл
	HANDLE h=(HANDLE)OpenFile(filename,&ofstruct,OF_CREATE);

	FSize = 16 + Width*Height*sizeof(TSimpleLandVertex);

	FileArr = new char [FSize];

	FileArr[0] = 'L';
	FileArr[1] = 'S';
	FileArr[2] = 0;
	FileArr[3] = 1;

	((int*)FileArr)[1] = Width;
	((int*)FileArr)[2] = Height;
	((float*)FileArr)[3] = CellInterval;

	int k;

	for (int i=0;i<Height;i++)
		for (int j=0;j<Width;j++)
		{
			k = j+i*Width;
			((TSimpleLandVertex*)(FileArr + 16))[k] = VertexMatrix[k];
		}

	WriteFile(h,FileArr,FSize,&BytesWriten,NULL);

	delete [] FileArr;
	CloseHandle(h);
#endif
}

void TSimpleLandClass::SetTexture(const std::string& texFilename)
{
	if (TexID != 0)
	{
		ResourceManager->TexList.DeleteTexture(TexID);
	}
	TexID = ResourceManager->TexList.AddTexture(texFilename);
}

void TSimpleLandClass::SetNormTexture(const std::string& normTexFilename)
{
	if (NormTexID != 0)
	{
		ResourceManager->TexList.DeleteTexture(NormTexID);
	}
	NormTexID = ResourceManager->TexList.AddTexture(normTexFilename);
}




void TSimpleLandClass::DrawImmediate()
{
	//Need to speed up!
	
#ifdef TARGET_WIN32

	int i, j, k, k_up, k_right, k_right_up;

	if (NormTexID == 0)
	{
		RenderUniform1i(CONST_STRING_NORMALMAPEXISTS_UNIFORM, 0);
	}

	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,WhiteColor);

	RenderUniformMatrix3fv(CONST_STRING_MODELROTATEMATRIX_UNIFORM, false, (float*)IdentityMatrix.m);
	RenderUniform3fv(CONST_STRING_MODELTRANSLATEVECTOR_UNIFORM, (float*)ZeroVec3.v);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, NormTexID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TexID);

	for (i = 0; i < Height-1; i++)
		for (j = 0; j < Width-1; j++)
		{
			k = j + i*Width;
			k_right = (j+1) + i*Width;
			k_up = j + (i+1)*Width;
			k_right_up = (j+1) + (i+1)*Width;

			float tx0 = float(j)/(Width-1);
			float ty0 = float(i)/(Height-1);

			float tx1 = float(j+1)/(Width-1);
			float ty1 = float(i+1)/(Height-1);

			glBegin(GL_TRIANGLES);
				glNormal3fv(VertexMatrix[k].norm.v);
				VertexAttrib3fv(CONST_STRING_TANGENT_ATTRIB, VertexMatrix[k].tangent.v);
				VertexAttrib3fv(CONST_STRING_BINORMAL_ATTRIB, VertexMatrix[k].binorm.v);
				glTexCoord2f(tx0,ty0);
				glVertex3fv(VertexMatrix[k].pos.v);

				glNormal3fv(VertexMatrix[k_right].norm.v);
				VertexAttrib3fv(CONST_STRING_TANGENT_ATTRIB, VertexMatrix[k_right].tangent.v);
				VertexAttrib3fv(CONST_STRING_BINORMAL_ATTRIB, VertexMatrix[k_right].binorm.v);
				glTexCoord2f(tx1,ty0);
				glVertex3fv(VertexMatrix[k_right].pos.v);

				glNormal3fv(VertexMatrix[k_up].norm.v);
				VertexAttrib3fv(CONST_STRING_TANGENT_ATTRIB, VertexMatrix[k_up].tangent.v);
				VertexAttrib3fv(CONST_STRING_BINORMAL_ATTRIB, VertexMatrix[k_up].binorm.v);
				glTexCoord2f(tx0,ty1);
				glVertex3fv(VertexMatrix[k_up].pos.v);
			glEnd();
			
			glBegin(GL_TRIANGLES);
				glNormal3fv(VertexMatrix[k_right_up].norm.v);
				VertexAttrib3fv(CONST_STRING_TANGENT_ATTRIB, VertexMatrix[k_right_up].tangent.v);
				VertexAttrib3fv(CONST_STRING_BINORMAL_ATTRIB, VertexMatrix[k_right_up].binorm.v);
				glTexCoord2f(tx1,ty1);
				glVertex3fv(VertexMatrix[k_right_up].pos.v);

				glNormal3fv(VertexMatrix[k_up].norm.v);
				VertexAttrib3fv(CONST_STRING_TANGENT_ATTRIB, VertexMatrix[k_up].tangent.v);
				VertexAttrib3fv(CONST_STRING_BINORMAL_ATTRIB, VertexMatrix[k_up].binorm.v);
				glTexCoord2f(tx0,ty1);
				glVertex3fv(VertexMatrix[k_up].pos.v);

				glNormal3fv(VertexMatrix[k_right].norm.v);
				VertexAttrib3fv(CONST_STRING_TANGENT_ATTRIB, VertexMatrix[k_right].tangent.v);
				VertexAttrib3fv(CONST_STRING_BINORMAL_ATTRIB, VertexMatrix[k_right].binorm.v);
				glTexCoord2f(tx1,ty0);
				glVertex3fv(VertexMatrix[k_right].pos.v);
			glEnd();

		}

		RenderUniform1i(CONST_STRING_NORMALMAPEXISTS_UNIFORM, 1);

#endif
}

void TSimpleLandClass::DrawVBO()
{


	//RenderUniform1i(CONST_STRING_NORMALMAPEXISTS_UNIFORM, min<cardinal>(NormTexID, 1));
	
	//RenderUniformMatrix3fv(CONST_STRING_MODELROTATEMATRIX_UNIFORM, false, (float*)IdentityMatrix.m);
	//RenderUniform3fv(CONST_STRING_MODELTRANSLATEVECTOR_UNIFORM, (float*)ZeroVec3.v);

	if (NormTexID != 0)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D,NormTexID);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,TexID);

	
#ifdef TARGET_WIN32
	
	
	glBindBuffer(GL_ARRAY_BUFFER, NormBuffer);
	VertexAttribPointer3fv(CONST_STRING_NORMAL_ATTRIB, 0, (char*)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, TangentBuffer);
	VertexAttribPointer3fv(CONST_STRING_TANGENT_ATTRIB, 0, (char*)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, BinormBuffer);
	VertexAttribPointer3fv(CONST_STRING_BINORMAL_ATTRIB, 0, (char*)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, TexcoordBuffer);
	VertexAttribPointer2fv(CONST_STRING_TEXCOORD_ATTRIB, 0, (char*)NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	VertexAttribPointer3fv(CONST_STRING_POSITION_ATTRIB, 0, (char*)NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

	glDrawElements(GL_TRIANGLES, 6*(Width-1)*(Height-1), GL_UNSIGNED_INT, NULL);

	
#endif
#ifdef TARGET_ANDROID
	
	
	VertexAttribPointer3fv(CONST_STRING_NORMAL_ATTRIB, 0, reinterpret_cast<const char*>(&(VBOLandVertexArr.norm[0])));

	VertexAttribPointer3fv(CONST_STRING_TANGENT_ATTRIB, 0, reinterpret_cast<const char*>(&(VBOLandVertexArr.tangent[0])));

	VertexAttribPointer3fv(CONST_STRING_BINORMAL_ATTRIB, 0, reinterpret_cast<const char*>(&(VBOLandVertexArr.binorm[0])));
	
	VertexAttribPointer2fv(CONST_STRING_TEXCOORD_ATTRIB, 0, reinterpret_cast<const char*>(&(VBOLandVertexArr.texCoord[0])));
	
	VertexAttribPointer3fv(CONST_STRING_POSITION_ATTRIB, 0, reinterpret_cast<const char*>(&(VBOLandVertexArr.pos[0])));
	
	glDrawElements(GL_TRIANGLES, 6*(Width-1)*(Height-1), GL_UNSIGNED_SHORT, &IndexArr[0]);

#endif

	RenderUniform1i(CONST_STRING_NORMALMAPEXISTS_UNIFORM,1);

}

vec3 TSimpleLandClass::CalcCrossing(vec3 LineFrom,vec3 LineTo)
{
	int i, j;
	vec3 CursorVec = Normalize(LineTo - LineFrom);
	vec3 CamPos = Renderer->GetCamPos();
	
	for (i = 0; i < Height-1; i++)
		for (j = 0; j < Width-1; j++)
		{
			vec3 CamToVertex = Normalize(VertexMatrix[j + i*Width].pos - CamPos);
			float cosA = DotProduct(CamToVertex, CursorVec);
			
			if (cosA > CONST_MIN_COS_BETWEEN_CURSOR_AND_VERTEX)
			{
				vec3 r = CalcCrossingForPoint(LineFrom, LineTo, i, j);

				if (!(r == LineFrom))
					return r;
			}
		}

	return LineFrom;
}

vec3 TSimpleLandClass::CalcVerticalCrossing(const vec3& pos)
{
	int ir = -1;
	int jr = -1;
	
	float xMin = VertexMatrix[0].pos.v[0];
	float xMax = VertexMatrix[Width-1].pos.v[0];

	float yMin = VertexMatrix[Height*Width-1].pos.v[2];
	float yMax = VertexMatrix[0].pos.v[2];

	jr = static_cast<int>(floor((Width-1)*(pos.v[0]-xMin)/(xMax - xMin)));
	ir = static_cast<int>(floor((Height-1)*(pos.v[2]-yMax)/(yMin - yMax))); //Must be reversed
	

	if (ir < 0 || ir >= Height-1 || jr < 0 || jr >= Width-1)
	{
		return pos;
	}
	else
	{
		vec3 r;
		r = CalcCrossingForPoint(pos, ir, jr);
		//r = CalcCrossingForPoint(r, ir+1, jr);
		//r = CalcCrossingForPoint(r, ir+2, jr);
		//r = CalcCrossingForPoint(r, ir-1, jr);
		//r = CalcCrossingForPoint(r, ir-2, jr);
		return r;
	}

}

float TSimpleLandClass::CalcVerticalCrossingFast(const vec3& pos)
{
	int ir = -1;
	int jr = -1;
	
	float xMin = VertexMatrix[0].pos.v[0];
	float xMax = VertexMatrix[Width-1].pos.v[0];

	float zMin = VertexMatrix[Height*Width-1].pos.v[2];
	float zMax = VertexMatrix[0].pos.v[2];

	float w = (xMax - xMin) / (Width-1);
	float h = (zMax - zMin) / (Height-1);

	jr = int((pos.v[0] - xMin)/w);
	ir = (Height-1) - int(ceilf((pos.v[2] - zMin)/h));

	if (ir < 0 || ir >= Height-1 || jr < 0 || jr >= Width-1)
	{
		return pos.v[1];
	}
	else
	{
		return max(VertexMatrix[jr + ir*Width].pos.v[1], 
			max(VertexMatrix[(jr+1) + ir*Width].pos.v[1], 
			max(VertexMatrix[jr + (ir+1)*Width].pos.v[1],
			VertexMatrix[(jr+1) + (ir+1)*Width].pos.v[1])));
	}


}

bool TSimpleLandClass::IsOnTheLand(const vec3& pos)
{

	float xMin = VertexMatrix[0].pos.v[0];
	float xMax = VertexMatrix[Width-1].pos.v[0];

	float yMin = VertexMatrix[Height*Width-1].pos.v[2];
	float yMax = VertexMatrix[0].pos.v[2];
	
	return (pos.v[0] >= xMin) && (pos.v[0] <= xMax) && (pos.v[2] >= yMin) && (pos.v[2] <= yMax);

}

void TSimpleLandClass::MoveLandUp(vec3 CursorPos,float R)
{
	//NIU
	int i,j;
	vec3 pointpos;
	float r,k;

	for (i=0;i<Height;i++)
		for (j=0;j<Width;j++)
		{
			pointpos = VertexMatrix[j+i*Width].pos;
			r = sqrtf((pointpos.v[0]-CursorPos.v[0])*(pointpos.v[0]-CursorPos.v[0])+(pointpos.v[2]-CursorPos.v[2])*(pointpos.v[2]-CursorPos.v[2]));
			if (r<R)
			{
				k=sqrtf((R-r)/R);
				VertexMatrix[j+i*Width].pos.v[1] += k*CONST_LAND_MOVE_STEP;
			}
		}
	RecalcNormalVectors();
	UpdateTriangleMatrix();

	UpdateVBO();
}

void TSimpleLandClass::MoveLandDown(vec3 CursorPos,float R)
{
	//NIU
	int i,j;
	vec3 pointpos;
	float r,k;

	for (i=0;i<Height;i++)
		for (j=0;j<Width;j++)
		{
			pointpos = VertexMatrix[j+i*Width].pos;
			r = sqrtf((pointpos.v[0]-CursorPos.v[0])*(pointpos.v[0]-CursorPos.v[0])+(pointpos.v[2]-CursorPos.v[2])*(pointpos.v[2]-CursorPos.v[2]));
			if (r<R)
			{
				k=sqrtf((R-r)/R);
				VertexMatrix[j+i*Width].pos.v[1] -= k*CONST_LAND_MOVE_STEP;
			}
		}
	RecalcNormalVectors();
	UpdateTriangleMatrix();

	UpdateVBO();
}
void TSimpleLandClass::SmoothLand(vec3 CursorPos,float R)
{
	//NIU
	int i,j;
	vec3 pointpos;
	float r,k;
	float h1,h2,h3,h4;

	for (i=0;i<Height;i++)
		for (j=0;j<Width;j++)
		{
			pointpos = VertexMatrix[j+i*Width].pos;
			r = sqrtf((pointpos.v[0]-CursorPos.v[0])*(pointpos.v[0]-CursorPos.v[0])+(pointpos.v[2]-CursorPos.v[2])*(pointpos.v[2]-CursorPos.v[2]));
			if (r<R)
			{
				k = sqrtf((R-r)/R);

				h1 = (i>0) ? VertexMatrix[j+(i-1)*Width].pos.v[1] : pointpos.v[1];
				h2 = (j>0) ? VertexMatrix[j-1+i*Width].pos.v[1] : pointpos.v[1];

				h3 = (i<Height-1) ? VertexMatrix[j+(i+1)*Width].pos.v[1] : pointpos.v[1];
				h4 = (j<Width-1) ? VertexMatrix[j+1+i*Width].pos.v[1] : pointpos.v[1];

				h1 = (h1+h2+h3+h4)/4.0f - pointpos.v[1];

				VertexMatrix[j+i*Width].pos.v[1] += h1;
			}
		}
	RecalcNormalVectors();
	UpdateTriangleMatrix();

	UpdateVBO();
}

void TSimpleLandClass::TextureLand(vec3 CursorPos,float R,int ltype)
{
	//NIU
	int i,j;
	vec3 pointpos;
	float r;

	for (i=0;i<Height;i++)
		for (j=0;j<Width;j++)
		{
			pointpos = VertexMatrix[j+i*Width].pos;
			r = sqrtf((pointpos.v[0]-CursorPos.v[0])*(pointpos.v[0]-CursorPos.v[0])+(pointpos.v[2]-CursorPos.v[2])*(pointpos.v[2]-CursorPos.v[2]));
			if (r<R)
			{
				VertexMatrix[j+i*Width].landtype = ltype;
			}
		}
}

void TSimpleLandClass::Scale(float s)
{
	for (int i = 0; i < Height; i++)
		for (int j = 0; j < Width; j++)
		{
			VertexMatrix[j + i*Width].pos *= s;
		}

	RecalcNormalVectors();
	UpdateTriangleMatrix();

	UpdateVBO();
}

void TSimpleLandClass::Scale(vec3 s)
{
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			VertexMatrix[j + i*Width].pos.v[0] *= s.v[0];
			VertexMatrix[j + i*Width].pos.v[1] *= s.v[1];
			VertexMatrix[j + i*Width].pos.v[2] *= s.v[2];
		}
	}

	RecalcNormalVectors();
	UpdateTriangleMatrix();

	UpdateVBO();
}

void TSimpleLandClass::Move(const vec3& v)
{
	for (int i = 0; i < Height; i++)
		for (int j = 0; j < Width; j++)
		{
			VertexMatrix[j + i*Width].pos = VertexMatrix[j + i*Width].pos + v;
		}

	RecalcNormalVectors();
	UpdateTriangleMatrix();

	UpdateVBO();
}

int TSimpleLandClass::GetWidth() 
{ 
	return Width; 
};

int TSimpleLandClass::GetHeight() 
{ 
	return Height; 
};
	
float* TSimpleLandClass::GetTriangleMatrix() 
{
	return (float*)&TriangleMatrix[0]; 
};

void TSimpleLandClass::RecalcNormalVectors()
{
	int i,j,ifrom,ito,jfrom,jto;
	vec3 v1,v2;

	for (i=0;i<Height;i++)
		for (j=0;j<Width;j++)
		{
			ifrom = (i==0) ? i : i-1;
			jfrom = (j==0) ? j : j-1;
			ito = (i==Height-1) ? Height-1 : i+1;
			jto = (j==Width-1) ? Width-1 : j+1;

			
			v1 = VertexMatrix[j+ito*Width].pos - VertexMatrix[j+ifrom*Width].pos;

			v2 = VertexMatrix[jto+i*Width].pos - VertexMatrix[jfrom+i*Width].pos;
	
			VertexMatrix[j+i*Width].binorm = v1;
			VertexMatrix[j+i*Width].tangent = v2;
			VertexMatrix[j+i*Width].norm = CrossProduct(v2, v1);

		}
}


void TSimpleLandClass::UpdateTriangleMatrix()
{
	int k;
	vec3 v,w;
	for (int i=0;i<Height-1;i++)
		for (int j=0;j<Width-1;j++)
		{
			k = 2*(j + i*(Width-1));
			TriangleMatrix[k].PointList[0] = VertexMatrix[j + i*(Width)].pos;
			TriangleMatrix[k].PointList[1] = VertexMatrix[j+1 + i*(Width)].pos;
			TriangleMatrix[k].PointList[2] = VertexMatrix[j + (i+1)*(Width)].pos;
			v =  TriangleMatrix[k].PointList[1] - TriangleMatrix[k].PointList[0];
			w =  TriangleMatrix[k].PointList[2] - TriangleMatrix[k].PointList[1];

			TriangleMatrix[k].NormVec = CrossProduct(v,w);
			if (TriangleMatrix[k].NormVec.v[1] < 0.0f)
			{
				//Xperimental
				TriangleMatrix[k].NormVec.v[0] = - TriangleMatrix[k].NormVec.v[0];
				TriangleMatrix[k].NormVec.v[1] = - TriangleMatrix[k].NormVec.v[1];
				TriangleMatrix[k].NormVec.v[2] = - TriangleMatrix[k].NormVec.v[2];
			}

			k++;

			TriangleMatrix[k].PointList[0] = VertexMatrix[j+1 + (i+1)*(Width)].pos;
			TriangleMatrix[k].PointList[1] = VertexMatrix[j + (i+1)*(Width)].pos;
			TriangleMatrix[k].PointList[2] = VertexMatrix[j+1 + i*(Width)].pos;
			v =  TriangleMatrix[k].PointList[1] - TriangleMatrix[k].PointList[0];
			w =  TriangleMatrix[k].PointList[2] - TriangleMatrix[k].PointList[1];

			TriangleMatrix[k].NormVec = CrossProduct(v, w);
			if (TriangleMatrix[k].NormVec.v[1] < 0.0f)
			{
				//Xperimental
				TriangleMatrix[k].NormVec.v[0] = - TriangleMatrix[k].NormVec.v[0];
				TriangleMatrix[k].NormVec.v[1] = - TriangleMatrix[k].NormVec.v[1];
				TriangleMatrix[k].NormVec.v[2] = - TriangleMatrix[k].NormVec.v[2];
			}
			
		}
}

} //namespace SE