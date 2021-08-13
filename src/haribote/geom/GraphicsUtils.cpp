#include "GraphicsUtils.h"

#include "Camera.h"
#include <GLES2/gl2.h>
#include <math.h>
#include <ode/common.h>
#include <mutex>

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
	static std::once_flag once;
	std::call_once(once, [] {
		const GLchar* vertSource =
			"attribute mediump vec4 pos;"
			"uniform mediump mat4 mvp;"
			"void main() {"
			"	vec4 tmp = mvp * pos;"
			"	gl_Position = tmp;"
			"}";

		const GLchar* fragSource =
			"uniform mediump vec4 color;"
			"void main() {"
			"	gl_FragColor = color;"
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

void drawAxes(const Camera& camera, const Matrix& transform)
{
	Matrix mat = camera.projection();
	mat *= camera.view();
	mat *= transform;
	mat.scaleX(0.1);
	mat.scaleY(0.1);
	mat.scaleZ(0.1);

	auto prog = commonProgram();
	glUseProgram(prog);

	static GLint aPos = glGetAttribLocation(prog, "pos");
	static GLint uMVP = glGetUniformLocation(prog, "mvp");
	static GLint uColor = glGetUniformLocation(prog, "color");

	GLfloat red[4] = { 1, 0, 0, 1 };
	GLfloat green[4] = { 0, 1, 0, 1 };
	GLfloat blue[4] = { 0, 0, 1, 1 };

	glUniformMatrix4fv(uMVP, 1, GL_FALSE, mat.data());

	glEnableVertexAttribArray(aPos);

	{
		const GLfloat vertex[] = {
		   0,  0,  0,
		   1,  0,  0,
		};
		glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 0, vertex);
		glUniform4fv(uColor, 1, red);
		glDrawArrays(GL_LINES, 0, 2);
	}
	{
		const GLfloat vertex[] = {
		   0,  0,  0,
		   0,  1,  0,
		};
		glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 0, vertex);
		glUniform4fv(uColor, 1, green);
		glDrawArrays(GL_LINES, 0, 2);
	}
	{
		const GLfloat vertex[] = {
		   0,  0,  0,
		   0,  0,  1,
		};
		glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 0, vertex);
		glUniform4fv(uColor, 1, blue);
		glDrawArrays(GL_LINES, 0, 2);
	}
}

void drawBoxWireframe(const Camera& camera, const Matrix& transform)
{
	Matrix mat = camera.projection();
	mat *= camera.view();
	mat *= transform;

	auto prog = commonProgram();
	glUseProgram(prog);

	static GLint aPos = glGetAttribLocation(prog, "pos");
	static GLint uMVP = glGetUniformLocation(prog, "mvp");
	static GLint uColor = glGetUniformLocation(prog, "color");

	GLfloat white[4] = { 1, 1, 1, 1 };
	glUniform4fv(uColor, 1, white);

	glUniformMatrix4fv(uMVP, 1, GL_FALSE, mat.data());

	glEnableVertexAttribArray(aPos);
	const GLfloat vertex[] = {
	   0.5,  0.5,  0.5,
	   0.5, -0.5,  0.5,
	   0.5, -0.5,  0.5,
	  -0.5, -0.5,  0.5,
	  -0.5, -0.5,  0.5,
	  -0.5,  0.5,  0.5,
	  -0.5,  0.5,  0.5,
	   0.5,  0.5,  0.5,

	   0.5,  0.5, -0.5,
	   0.5, -0.5, -0.5,
	   0.5, -0.5, -0.5,
	  -0.5, -0.5, -0.5,
	  -0.5, -0.5, -0.5,
	  -0.5,  0.5, -0.5,
	  -0.5,  0.5, -0.5,
	   0.5,  0.5, -0.5,

	   0.5,  0.5,  0.5,
	   0.5,  0.5, -0.5,
	   0.5, -0.5,  0.5,
	   0.5, -0.5, -0.5,
	  -0.5, -0.5,  0.5,
	  -0.5, -0.5, -0.5,
	  -0.5,  0.5,  0.5,
	  -0.5,  0.5, -0.5,
	};
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 0, vertex);
	glDrawArrays(GL_LINES, 0, 24);
}

void drawSphereWireframe(const Camera& camera, const Matrix& transform)
{
	Matrix mat = camera.projection();
	mat *= camera.view();
	mat *= transform;

	auto prog = commonProgram();
	glUseProgram(prog);

	static GLint aPos = glGetAttribLocation(prog, "pos");
	static GLint uMVP = glGetUniformLocation(prog, "mvp");
	static GLint uColor = glGetUniformLocation(prog, "color");

	GLfloat white[4] = { 1, 1, 1, 1 };
	glUniform4fv(uColor, 1, white);

	glUniformMatrix4fv(uMVP, 1, GL_FALSE, mat.data());

	glEnableVertexAttribArray(aPos);

	const int NDIV = 16;
	for (int i = 0; i < NDIV; i++) {
		GLfloat vertex[(NDIV / 2 + 1) * 3];
		float phi = 2 * M_PI * i / NDIV;
		float cosphi = cos(phi);
		float sinphi = sin(phi);
		for (int j = 0; j < (NDIV / 2 + 1); j++) {
			float theta = 2 * M_PI * j / NDIV;
			vertex[j * 3 + 0] = cosphi * cos(theta);
			vertex[j * 3 + 1] = cosphi * sin(theta);
			vertex[j * 3 + 2] = sinphi;
		};
		glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 0, vertex);
		glDrawArrays(GL_LINE_LOOP, 0, (NDIV / 2 + 1));
	}

	for (int i = 0; i < NDIV; i++) {
		GLfloat vertex[NDIV * 3];
		float theta = 2 * M_PI * i / NDIV;
		float costheta = cos(theta);
		float sintheta = sin(theta);
		for (int j = 0; j < NDIV; j++) {
			float phi = 2 * M_PI * j / NDIV;
			vertex[j * 3 + 0] = cos(phi) * costheta;
			vertex[j * 3 + 1] = cos(phi) * sintheta;
			vertex[j * 3 + 2] = sin(phi);
		};
		glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 0, vertex);
		glDrawArrays(GL_LINE_LOOP, 0, NDIV);
	}

}
