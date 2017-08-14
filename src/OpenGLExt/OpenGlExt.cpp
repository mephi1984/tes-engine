#include "include/OpenGlExt/OpenGlExt.h"

namespace SE
{

//====================================================
//===================== GLSL Shaders =================  
//====================================================

//Requires GL_VERSION_2_0
PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;  
PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;  
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;  
PFNGLVALIDATEPROGRAMPROC glValidateProgram = NULL;  
PFNGLUSEPROGRAMPROC glUseProgram = NULL;  
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;  
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;  
PFNGLCREATESHADERPROC glCreateShader = NULL;  
PFNGLDELETESHADERPROC glDeleteShader = NULL;  
PFNGLSHADERSOURCEPROC glShaderSource = NULL;  
PFNGLCOMPILESHADERPROC glCompileShader = NULL;  
PFNGLATTACHSHADERPROC glAttachShader = NULL;  
PFNGLDETACHSHADERPROC glDetachShader = NULL;  
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;  
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;  
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = NULL;  
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;  
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;  
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = NULL;  
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;  
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = NULL;  
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;  
PFNGLUNIFORM1IPROC glUniform1i = NULL;  
PFNGLUNIFORM1FVPROC glUniform1fv = NULL;  
PFNGLUNIFORM3FVPROC glUniform2fv = NULL;  
PFNGLUNIFORM3FVPROC glUniform3fv = NULL;  
PFNGLUNIFORM4FVPROC glUniform4fv = NULL;  
PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f = NULL;
PFNGLVERTEXATTRIB2FPROC glVertexAttrib2f = NULL;
PFNGLVERTEXATTRIB3FPROC glVertexAttrib3f = NULL;
PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f = NULL;
PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv = NULL;
PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv = NULL;
PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv = NULL;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib = NULL;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = NULL;


//=======================================
//=========== Multitexture ==============
//=======================================

//Requires GL version 1.3
PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;

//=======================================
//========== Vertex buffer ==============
//=======================================

//Requires GL_VERSION_1_5
PFNGLGENBUFFERSPROC glGenBuffers = NULL;  
PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;  
PFNGLBINDBUFFERPROC glBindBuffer = NULL;  
PFNGLBUFFERDATAPROC glBufferData = NULL;  
PFNGLBUFFERSUBDATAPROC glBufferSubData = NULL;  
PFNGLMAPBUFFERPROC glMapBuffer = NULL;  
PFNGLUNMAPBUFFERPROC glUnmapBuffer = NULL;  

//=========================================
//============ Frame buffer ===============
//=========================================

//Requires GL_ARB_framebuffer_object
PFNGLISRENDERBUFFERPROC glIsRenderbuffer = NULL;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = NULL;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = NULL;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers = NULL;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = NULL;
PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv = NULL;
PFNGLISFRAMEBUFFERPROC glIsFramebuffer = NULL;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = NULL;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = NULL;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = NULL;
PFNGLFRAMEBUFFERTEXTURE1DPROC glFramebufferTexture1D = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = NULL;
PFNGLFRAMEBUFFERTEXTURE3DPROC glFramebufferTexture3D = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = NULL;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv = NULL;
PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer = NULL;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample = NULL;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = NULL;
PFNGLFRAMEBUFFERTEXTURELAYERPROC glFramebufferTextureLayer = NULL;



//===========================================
//============ Uniform buffer ===============
//===========================================

//Requires GL_ARB_uniform_buffer_object
PFNGLGETUNIFORMINDICESPROC glGetUniformIndices = NULL;
PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv = NULL;
PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName = NULL;
PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex = NULL;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv = NULL;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName = NULL;
PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding = NULL;
PFNGLBINDBUFFERBASEPROC glBindBufferBase = NULL;




} //namespace SE