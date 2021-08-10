#include "Box.h"

#include "GLES2/gl2.h"
#include <mutex>

#define Renderer GLRenderer

namespace
{

GLuint compileShader(GLuint shaderType, const GLchar* source)
{
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	return shader;
}

GLuint commonProgram()
{
	static GLuint prog;
	std::once_flag once;
	std::call_once(once, [] {
		const GLchar* vertSource =
			"attribute mediump vec4 pos;"
			"void main() {"
			"	gl_Position = pos;"
			"}";

		const GLchar* fragSource =
			"void main() {"
			"	gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);"
			"}";

		prog = glCreateProgram();
		auto vert = compileShader(GL_VERTEX_SHADER, vertSource);
		auto frag = compileShader(GL_FRAGMENT_SHADER, fragSource);
		glAttachShader(prog, vert);
		glAttachShader(prog, frag);
		glLinkProgram(prog);
		glDeleteShader(vert);
		glDeleteShader(frag);
	});
	return prog;
}

}

void Box::drawWireframe(Camera& camera)
{
	auto prog = commonProgram();
	glUseProgram(prog);

	static GLint aPos = glGetAttribLocation(prog, "pos");
	glEnableVertexAttribArray(aPos);
	const GLfloat vertex[] = {
	  0.0f, 0.5f,
	  -0.5f, -0.5f,
	  0.5f, -0.5f,
	};
	glVertexAttribPointer(aPos, 2, GL_FLOAT, GL_FALSE, 0, vertex);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
