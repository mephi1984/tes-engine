#include "include/Engine.h"

namespace SE
{

void THalibutRendererInterface::SetUniforms()
{

	if (ProjectionMatrixStack.size() <=0)
	{
		throw ErrorToLog("Projection matrix stack out!");
	}
	
	if (ModelviewMatrixStack.size() <=0)
	{
		throw ErrorToLog("Modelview matrix stack out!");
	}
	
	ProjectionModelviewMatrix = MultMatrixMatrix(ProjectionMatrixStack.top(), ModelviewMatrixStack.top());
	RenderUniformMatrix4fv(CONST_STRING_HALIBUT_PROJECTION_MATRIX_UNIFORM, false, ProjectionModelviewMatrix.m);

	//Refactoring!
	
	RenderUniform1i(CONST_STRING_TEXTURE_UNIFORM, 0);
	RenderUniform1i(CONST_STRING_NORMALMAP_UNIFORM, 1);
	
	RenderUniform1f(CONST_STRING_TRANSPARENCY_UNIFORM, 1.f);
	
	RenderUniform4fv(CONST_STRING_MATERIAL_COLOR_UNIFORM, WhiteColor);
	

	/*
	RenderUniform1i(CONST_STRING_SHADOWMAPGLOBAL_UNIFORM,2);
	RenderUniform1i(CONST_STRING_SHADOWMAPLOCAL_UNIFORM,3);

	RenderUniform1i(CONST_STRING_NORMALMAPEXISTS_UNIFORM,1);
	RenderUniform1i(CONST_STRING_ENV_UNIFORM,0);*/
}

void THalibutRendererInterface::InitOpenGL(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight)
{

	*Console<<"Halibut Render OpenGL init\n";
	
	#ifdef TARGET_WIN32
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);
	#endif
	
	glDisable(GL_DEPTH_TEST); 

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	ScreenWidth = screenWidth;
	ScreenHeight = screenHeight;
	
	MatrixWidth = matrixWidth;
	MatrixHeight = matrixHeight;
	
	while (!ProjectionMatrixStack.empty())
	{
	   ProjectionMatrixStack.pop();
    }
	ProjectionMatrixStack.push(MakeOrthoMatrix(matrixWidth, matrixHeight));
	
    while (!ModelviewMatrixStack.empty())
	{
	   ModelviewMatrixStack.pop();
    }
	ModelviewMatrixStack.push(IdentityMatrix4);
	
	glViewport(0, 0, ScreenWidth, ScreenHeight);
	
	SetUniforms();
	#ifndef TARGET_IOS
	SwitchToScreen();
	#endif

	CheckGlError();

}

void THalibutRendererInterface::SwitchToScreen()
{
	//TODO: fix this stupid bug
#ifdef TARGET_IOS
    glBindFramebuffer(GL_FRAMEBUFFER, 2);
#else
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
	glViewport(0, 0, ScreenWidth, ScreenHeight);
	SetProjectionMatrix(MatrixWidth, MatrixHeight);
	SetUniforms();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void THalibutRendererInterface::SwitchToFrameBuffer(const std::string& frameName)
{
	if (ResourceManager->FrameManager.FrameMap.count(frameName) > 0)
	{
		TFrame& Frame = ResourceManager->FrameManager.FrameMap[frameName];

		glBindFramebuffer(GL_FRAMEBUFFER, Frame.FrameBuffer);
		glViewport(0, 0, Frame.Width, Frame.Height);
		SetProjectionMatrix(static_cast<float>(Frame.Width), static_cast<float>(Frame.Height));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void THalibutRendererInterface::PrintMatrices()
{
	mat4 proj = ProjectionMatrixStack.top();
	mat4 modv = ModelviewMatrixStack.top();
	*Console<<"Projection:\n ";
	for (int i=0; i<16; i++)
	{
		*Console<<tostr(proj.m[i])<<" ";
	}

	*Console<<endl;
	*Console<<"Modelview:\n ";
	for (int i=0; i<16; i++)
	{
		*Console<<tostr(modv.m[i])<<" ";
	}

	modv = MultMatrixMatrix(proj, modv);

	*Console<<endl;
	*Console<<"ProjectonModelview:\n ";
	for (int i=0; i<16; i++)
	{
		*Console<<tostr(modv.m[i])<<" ";
	}

	*Console<<endl;
}

} //namespace SE