#pragma once

#define ASSERT(x) if(!(x)) __debugbreak();
#define glCall(x) glClearErrors(); \
	x;\
	ASSERT(glLogCall())

static void glClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool glLogCall()
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")\n";
		return false;
	}
	return true;
}