#pragma once

/*
This code contains openGL extensions (for them to be called from everywhere)

Windows only!
*/

#ifdef TARGET_WIN32
#include "include/Utils/WinApi/WinApi.h"
#endif

namespace SE
{

//====================================================
//===================== GLSL Shaders =================  
//====================================================

//Requires GL_VERSION_2_0
extern PFNGLCREATEPROGRAMPROC glCreateProgram;  
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;  
extern PFNGLLINKPROGRAMPROC glLinkProgram;  
extern PFNGLVALIDATEPROGRAMPROC glValidateProgram;  
extern PFNGLUSEPROGRAMPROC glUseProgram;  
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;  
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;  
extern PFNGLCREATESHADERPROC glCreateShader;  
extern PFNGLDELETESHADERPROC glDeleteShader;  
extern PFNGLSHADERSOURCEPROC glShaderSource;  
extern PFNGLCOMPILESHADERPROC glCompileShader;  
extern PFNGLATTACHSHADERPROC glAttachShader;  
extern PFNGLDETACHSHADERPROC glDetachShader;  
extern PFNGLGETSHADERIVPROC glGetShaderiv;  
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;  
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;  
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;  
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;  
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;  
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;  
extern PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;  
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;  
extern PFNGLUNIFORM1IPROC glUniform1i;  
extern PFNGLUNIFORM1FVPROC glUniform1fv;  
extern PFNGLUNIFORM3FVPROC glUniform2fv;  
extern PFNGLUNIFORM3FVPROC glUniform3fv;  
extern PFNGLUNIFORM4FVPROC glUniform4fv;  
extern PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f;
extern PFNGLVERTEXATTRIB2FPROC glVertexAttrib2f;
extern PFNGLVERTEXATTRIB3FPROC glVertexAttrib3f;
extern PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f;
extern PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv;
extern PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv;
extern PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv;
extern PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib;
extern PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;


//=======================================
//=========== Multitexture ==============
//=======================================

//Requires GL version 1.3
extern PFNGLACTIVETEXTUREPROC glActiveTexture;

//=======================================
//========== Vertex buffer ==============
//=======================================

//Requires GL_VERSION_1_5
extern PFNGLGENBUFFERSPROC glGenBuffers;  
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;  
extern PFNGLBINDBUFFERPROC glBindBuffer;  
extern PFNGLBUFFERDATAPROC glBufferData;  
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;  
extern PFNGLMAPBUFFERPROC glMapBuffer;  
extern PFNGLUNMAPBUFFERPROC glUnmapBuffer;  

//=========================================
//============ Frame buffer ===============
//=========================================

//Requires GL_ARB_framebuffer_object
extern PFNGLISRENDERBUFFERPROC glIsRenderbuffer;
extern PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
extern PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
extern PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
extern PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
extern PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv;
extern PFNGLISFRAMEBUFFERPROC glIsFramebuffer;
extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
extern PFNGLFRAMEBUFFERTEXTURE1DPROC glFramebufferTexture1D;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
extern PFNGLFRAMEBUFFERTEXTURE3DPROC glFramebufferTexture3D;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv;
extern PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer;
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
extern PFNGLFRAMEBUFFERTEXTURELAYERPROC glFramebufferTextureLayer;



//===========================================
//============ Uniform buffer ===============
//===========================================

//Requires GL_ARB_uniform_buffer_object
extern PFNGLGETUNIFORMINDICESPROC glGetUniformIndices;
extern PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv;
extern PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName;
extern PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;
extern PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv;
extern PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName;
extern PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;
extern PFNGLBINDBUFFERBASEPROC glBindBufferBase;

} //namespace SE
