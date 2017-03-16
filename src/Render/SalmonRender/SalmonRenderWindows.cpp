#include "include/Render/SalmonRender/SalmonRenderWindows.h"

#include "include/OpenGLExt/OpenGlExt.h"
#include "include/Utils/Utils.h"
#include "include/Engine.h"

namespace SE
{

//===================================================
//================ RENDER ===========================
//===================================================

TSalmonRenderer::TSalmonRenderer() : 
	ShadowClampValue(0.0f), 
	FogBeginDistance(CONST_DEFAULT_FOG_BEGIN_DISTANCE), 
	FogEndDistance(CONST_DEFAULT_FOG_END_DISTANCE), 
	FogColor(CONST_DEFAULT_FOG_COLOR), 
	IsCameraTransparentToLand(true)
{ 

}

TSalmonRenderer::~TSalmonRenderer()
{
}



bool TSalmonRenderer::BindOpenGLFunctions()
{
	//AssertIfInMainThread();

	char* extensionList = (char*)glGetString(GL_EXTENSIONS);
	char* glVersion = (char*)glGetString(GL_VERSION);
	bool ok = true;

	*Console<<std::string("Render::OpenGL glVersion = ") + glVersion;
	
	//Requires OpenGL 2.0 or above
	if (glVersion[0]>='2')
	{
		*Console<<"Render::OpenGL glVersion above 2.0 ok";

		glActiveTexture = (PFNGLACTIVETEXTUREPROC) wglGetProcAddress("glActiveTexture");

		glGenBuffers = (PFNGLGENBUFFERSPROC) wglGetProcAddress("glGenBuffers");
		glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) wglGetProcAddress("glDeleteBuffers");
		glBindBuffer = (PFNGLBINDBUFFERPROC) wglGetProcAddress("glBindBuffer");
		glBufferData = (PFNGLBUFFERDATAPROC) wglGetProcAddress("glBufferData");
		glBufferSubData = (PFNGLBUFFERSUBDATAPROC) wglGetProcAddress("glBufferSubData");
		glMapBuffer = (PFNGLMAPBUFFERPROC) wglGetProcAddress("glMapBuffer");
		glUnmapBuffer = (PFNGLUNMAPBUFFERPROC) wglGetProcAddress("glUnmapBuffer");

		glCreateProgram = (PFNGLCREATEPROGRAMPROC) wglGetProcAddress("glCreateProgram");
		glDeleteProgram = (PFNGLDELETEPROGRAMPROC) wglGetProcAddress("glDeleteProgram");
		glLinkProgram = (PFNGLLINKPROGRAMPROC) wglGetProcAddress("glLinkProgram");
		glValidateProgram = (PFNGLVALIDATEPROGRAMPROC) wglGetProcAddress("glValidateProgram");
		glUseProgram = (PFNGLUSEPROGRAMPROC) wglGetProcAddress("glUseProgram");
		glGetProgramiv = (PFNGLGETPROGRAMIVPROC) wglGetProcAddress("glGetProgramiv");
		glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) wglGetProcAddress("glGetProgramInfoLog");
		glCreateShader = (PFNGLCREATESHADERPROC) wglGetProcAddress("glCreateShader");

 
		glDeleteShader = (PFNGLDELETESHADERPROC) wglGetProcAddress("glDeleteShader");
		glShaderSource = (PFNGLSHADERSOURCEPROC) wglGetProcAddress("glShaderSource");
		glCompileShader = (PFNGLCOMPILESHADERPROC) wglGetProcAddress("glCompileShader");
		glAttachShader = (PFNGLATTACHSHADERPROC) wglGetProcAddress("glAttachShader");
		glDetachShader = (PFNGLDETACHSHADERPROC) wglGetProcAddress("glDetachShader");
		glGetShaderiv = (PFNGLGETSHADERIVPROC) wglGetProcAddress("glGetShaderiv");
		glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) wglGetProcAddress("glGetShaderInfoLog");
		glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) wglGetProcAddress("glGetAttribLocation");
		glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) wglGetProcAddress("glVertexAttribPointer");
		glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress("glEnableVertexAttribArray");

		glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress("glDisableVertexAttribArray");
		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) wglGetProcAddress("glGetUniformLocation");
		glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC) wglGetProcAddress("glUniformMatrix3fv");
		glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) wglGetProcAddress("glUniformMatrix4fv");
		glUniform1i = (PFNGLUNIFORM1IPROC) wglGetProcAddress("glUniform1i");
		glUniform1fv = (PFNGLUNIFORM1FVPROC) wglGetProcAddress("glUniform1fv");
		glUniform2fv = (PFNGLUNIFORM2FVPROC) wglGetProcAddress("glUniform2fv");
		glUniform3fv = (PFNGLUNIFORM3FVPROC) wglGetProcAddress("glUniform3fv");
		glUniform4fv = (PFNGLUNIFORM4FVPROC) wglGetProcAddress("glUniform4fv");

		glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC) wglGetProcAddress("glVertexAttrib1f");
		glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC) wglGetProcAddress("glVertexAttrib2f");
		glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC) wglGetProcAddress("glVertexAttrib3f");
		glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC) wglGetProcAddress("glVertexAttrib4f");
		glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC) wglGetProcAddress("glVertexAttrib2fv");
		glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC) wglGetProcAddress("glVertexAttrib3fv");
		glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC) wglGetProcAddress("glVertexAttrib4fv");
		glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC) wglGetProcAddress("glGetActiveAttrib");
		glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC) wglGetProcAddress("glGetActiveUniform");
		

		if (glActiveTexture==NULL ||
			glGenBuffers==NULL ||
			glDeleteBuffers==NULL ||
			glBindBuffer==NULL ||
			glBufferData==NULL ||
			glBufferSubData==NULL ||
			glMapBuffer==NULL ||
			glCreateProgram==NULL ||
			glDeleteProgram==NULL ||
			glLinkProgram==NULL ||
			glValidateProgram==NULL ||
			glUseProgram==NULL ||
			glGetProgramiv==NULL ||
			glGetProgramInfoLog==NULL ||
			glCreateShader==NULL ||
			glDeleteShader==NULL ||
			glShaderSource==NULL ||
			glCompileShader==NULL ||
			glAttachShader==NULL ||
			glDetachShader==NULL ||
			glGetShaderiv==NULL ||
			glGetShaderInfoLog==NULL ||
			glGetAttribLocation==NULL ||
			glVertexAttribPointer==NULL ||
			glEnableVertexAttribArray==NULL ||
			glDisableVertexAttribArray==NULL ||
			glGetUniformLocation==NULL ||
			glUniformMatrix3fv==NULL ||
			glUniformMatrix4fv==NULL ||
			glUniform1i==NULL ||
			glUniform1fv==NULL ||
			glUniform2fv==NULL ||
			glUniform3fv==NULL ||
			glUniform4fv==NULL ||
			glEnableVertexAttribArray==NULL ||
			glVertexAttrib1f==NULL ||
			glVertexAttrib2f==NULL ||
			glVertexAttrib3f==NULL ||
			glVertexAttrib4f==NULL ||
			glVertexAttrib2fv==NULL ||
			glVertexAttrib3fv==NULL ||
			glVertexAttrib4fv==NULL ||
			glGetActiveAttrib==NULL ||
			glGetActiveUniform==NULL)
		{
			*Console<<"Render::OpenGL Some 2.0 version proc not found - starting failed";
			ok = false;
		}


	}

	else
	{
		*Console<<"Render::OpenGL glVersion below 2.0 - starting failed";
		ok = false;
	}
	
	if (findString("GL_ARB_framebuffer_object",extensionList))
	{
		*Console<<"Render::OpenGL GL_ARB_framebuffer_object supported ok";

		glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC) wglGetProcAddress("glIsRenderbuffer");
		glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) wglGetProcAddress("glBindRenderbuffer");
		glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) wglGetProcAddress("glDeleteRenderbuffers");
		glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) wglGetProcAddress("glGenRenderbuffers");
		glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) wglGetProcAddress("glRenderbufferStorage");
		glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC) wglGetProcAddress("glGetRenderbufferParameteriv");
		glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC) wglGetProcAddress("glIsFramebuffer");
		glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) wglGetProcAddress("glBindFramebuffer");
		glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) wglGetProcAddress("glDeleteFramebuffers");
		glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) wglGetProcAddress("glGenFramebuffers");
		glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) wglGetProcAddress("glCheckFramebufferStatus");
		glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC) wglGetProcAddress("glFramebufferTexture1D");
		glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) wglGetProcAddress("glFramebufferTexture2D");
		glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC) wglGetProcAddress("glFramebufferTexture3D");
		glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) wglGetProcAddress("glFramebufferRenderbuffer");
		glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) wglGetProcAddress("glGetFramebufferAttachmentParameteriv");
		glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC) wglGetProcAddress("glBlitFramebuffer");
		glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) wglGetProcAddress("glRenderbufferStorageMultisample");
		glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress("glGenerateMipmap");
		glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC) wglGetProcAddress("glFramebufferTextureLayer");

		if (glIsRenderbuffer == NULL ||
			glBindRenderbuffer == NULL ||
			glDeleteRenderbuffers == NULL ||
			glGenRenderbuffers == NULL ||
			glRenderbufferStorage == NULL ||
			glGetRenderbufferParameteriv == NULL ||
			glIsFramebuffer == NULL ||
			glBindFramebuffer == NULL ||
			glDeleteFramebuffers == NULL ||
			glGenFramebuffers == NULL ||
			glCheckFramebufferStatus == NULL ||
			glFramebufferTexture1D == NULL ||
			glFramebufferTexture2D == NULL ||
			glFramebufferTexture3D == NULL ||
			glFramebufferRenderbuffer == NULL ||
			glGetFramebufferAttachmentParameteriv == NULL ||
			glBlitFramebuffer == NULL ||
			glRenderbufferStorageMultisample == NULL ||
			glGenerateMipmap == NULL ||
			glFramebufferTextureLayer == NULL)
		{
			*Console<<"Render::OpenGL Some GL_ARB_framebuffer_object proc not found - starting failed";
			ok = false;
		}


	}
	else
	{
		*Console<<"Render::OpenGL GL_ARB_framebuffer_object not supported - starting failed";
		ok = false;
	}

	if (findString("GL_ARB_uniform_buffer_object",extensionList))
	{

		glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC) wglGetProcAddress("glGetUniformIndices");
		glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC) wglGetProcAddress("glGetActiveUniformsiv");
		glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC) wglGetProcAddress("glGetActiveUniformName");
		glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC) wglGetProcAddress("glGetUniformBlockIndex");
		glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC) wglGetProcAddress("glGetActiveUniformBlockiv");
		glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC) wglGetProcAddress("glGetActiveUniformBlockName");
		glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC) wglGetProcAddress("glUniformBlockBinding");
		glBindBufferBase = (PFNGLBINDBUFFERBASEPROC) wglGetProcAddress("glBindBufferBase");

		if (glGetUniformIndices == NULL ||
			glGetActiveUniformsiv == NULL ||
			glGetActiveUniformName == NULL ||
			glGetUniformBlockIndex == NULL ||
			glGetActiveUniformBlockiv == NULL ||
			glGetActiveUniformBlockName == NULL ||
			glUniformBlockBinding == NULL ||
			glBindBufferBase == NULL)
		{
			*Console<<"Render::OpenGL some GL_ARB_uniform_buffer_object proc not found - starting failed";
			ok = false;
		}
	
			
		*Console<<"Render::OpenGL GL_ARB_uniform_buffer_object supported ok";
	}
	else
	{
		*Console<<"Render::OpenGL GL_ARB_uniform_buffer_object not supported - starting failed";
		ok = false;
	}

	return ok;
}



void TSalmonRenderer::DrawQuad(const T2DQuad& quad)
{
	AssertIfInMainThread();

	const float CONST_DEFAULT_NORM_VEC[3] = { 0.0f, 0.0f, 1.0f};

	glBegin(GL_QUADS);
		VertexAttrib3fv(CONST_STRING_NORMAL_ATTRIB, CONST_DEFAULT_NORM_VEC);
		VertexAttrib2fv(CONST_STRING_TEXCOORD_ATTRIB, quad.TextureCoord[0].data());
		VertexAttrib3fv(CONST_STRING_POSITION_ATTRIB, quad.VertexCoord[0].data());
		VertexAttrib2fv(CONST_STRING_TEXCOORD_ATTRIB, quad.TextureCoord[1].data());
		VertexAttrib3fv(CONST_STRING_POSITION_ATTRIB, quad.VertexCoord[1].data());
		VertexAttrib2fv(CONST_STRING_TEXCOORD_ATTRIB, quad.TextureCoord[3].data());
		VertexAttrib3fv(CONST_STRING_POSITION_ATTRIB, quad.VertexCoord[3].data());
		VertexAttrib2fv(CONST_STRING_TEXCOORD_ATTRIB, quad.TextureCoord[2].data());
		VertexAttrib3fv(CONST_STRING_POSITION_ATTRIB, quad.VertexCoord[2].data());
	glEnd();
}


float TSalmonRenderer::GetShadowClampValue() 
{ 
	return ShadowClampValue; 
}

void TSalmonRenderer::SetShadowClampValue(float shadowClampValue)
{
	ShadowClampValue = shadowClampValue;

	if (ShadowClampValue < 0.0f)
		ShadowClampValue = 0.0f;

	if (ShadowClampValue > 1.0f)
		ShadowClampValue = 1.0f;

	RenderUniform1f(CONST_STRING_SHADOWCLAMPVALUE_UNIFORM, ShadowClampValue);
}


float TSalmonRenderer::GetFogBeginDistance()
{
	return FogBeginDistance;
}

float TSalmonRenderer::GetFogEndDistance()
{
	return FogEndDistance;
}

Vector4f TSalmonRenderer::GetFogColor()
{
	return FogColor;
}
	



void TSalmonRenderer::DrawTriangleList(const TTriangleList& triangleList)
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
		glDrawArrays(GL_TRIANGLES,0,triangleList.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].size());
	}

}

} //namespace SE
