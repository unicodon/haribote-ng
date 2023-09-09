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
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(shader); // Don't leak the shader.
		return 0;
	}

	return shader;
}

GLuint wireframeProgram()
{
	static GLuint prog;
	static std::once_flag once;
	std::call_once(once, [] {
		const GLchar* vertSource =
			"attribute mediump vec3 pos;"
			"uniform mediump mat4 projection;"
			"uniform mediump mat4 view;"
			"uniform mediump mat4 model;"
			"void main() {"
			"	vec4 tmp = projection * view * model * vec4(pos, 1.0);"
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
	Matrix model = transform;
	model.scaleX(0.1);
	model.scaleY(0.1);
	model.scaleZ(0.1);

	auto prog = wireframeProgram();
	glUseProgram(prog);

	static GLint aPos = glGetAttribLocation(prog, "pos");
	static GLint uProjection = glGetUniformLocation(prog, "projection");
	static GLint uView = glGetUniformLocation(prog, "view");
	static GLint uModel = glGetUniformLocation(prog, "model");
	static GLint uColor = glGetUniformLocation(prog, "color");

	GLfloat red[4] = { 1, 0, 0, 1 };
	GLfloat green[4] = { 0, 1, 0, 1 };
	GLfloat blue[4] = { 0, 0, 1, 1 };

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, camera.projection().data());
	glUniformMatrix4fv(uView, 1, GL_FALSE, camera.view().data());
	glUniformMatrix4fv(uModel, 1, GL_FALSE, model.data());

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
	auto prog = wireframeProgram();
	glUseProgram(prog);

	static GLint aPos = glGetAttribLocation(prog, "pos");
	static GLint uProjection = glGetUniformLocation(prog, "projection");
	static GLint uView = glGetUniformLocation(prog, "view");
	static GLint uModel = glGetUniformLocation(prog, "model");
	static GLint uColor = glGetUniformLocation(prog, "color");

	GLfloat white[4] = { 1, 1, 1, 1 };
	glUniform4fv(uColor, 1, white);

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, camera.projection().data());
	glUniformMatrix4fv(uView, 1, GL_FALSE, camera.view().data());
	glUniformMatrix4fv(uModel, 1, GL_FALSE, transform.data());

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
	auto prog = wireframeProgram();
	glUseProgram(prog);

	static GLint aPos = glGetAttribLocation(prog, "pos");
	static GLint uProjection = glGetUniformLocation(prog, "projection");
	static GLint uView = glGetUniformLocation(prog, "view");
	static GLint uModel = glGetUniformLocation(prog, "model");
	static GLint uColor = glGetUniformLocation(prog, "color");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, camera.projection().data());
	glUniformMatrix4fv(uView, 1, GL_FALSE, camera.view().data());
	glUniformMatrix4fv(uModel, 1, GL_FALSE, transform.data());

	GLfloat white[4] = { 1, 1, 1, 1 };
	glUniform4fv(uColor, 1, white);

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

void drawTriMeshWireframe(const Camera& camera, const Matrix& transform, const TriMeshData& data)
{
	auto prog = wireframeProgram();
	glUseProgram(prog);

	static GLint aPos = glGetAttribLocation(prog, "pos");
	static GLint uProjection = glGetUniformLocation(prog, "projection");
	static GLint uView = glGetUniformLocation(prog, "view");
	static GLint uModel = glGetUniformLocation(prog, "model");
	static GLint uColor = glGetUniformLocation(prog, "color");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, camera.projection().data());
	glUniformMatrix4fv(uView, 1, GL_FALSE, camera.view().data());
	glUniformMatrix4fv(uModel, 1, GL_FALSE, transform.data());

	GLfloat white[4] = { 1, 1, 1, 1 };
	glUniform4fv(uColor, 1, white);


	glEnableVertexAttribArray(aPos);

	size_t numTriangles = data.indices.size() / 3;
	std::vector<GLfloat> vertices(numTriangles * 6 * 3);
	auto ptr = vertices.data();
	for (size_t i = 0; i < numTriangles; i++) {
		int idx[3] = {
			data.indices[3 * i + 0],
			data.indices[3 * i + 1],
			data.indices[3 * i + 2]
		};

		*ptr++ = data.vertices[3 * idx[0] + 0];
		*ptr++ = data.vertices[3 * idx[0] + 1];
		*ptr++ = data.vertices[3 * idx[0] + 2];
		*ptr++ = data.vertices[3 * idx[1] + 0];
		*ptr++ = data.vertices[3 * idx[1] + 1];
		*ptr++ = data.vertices[3 * idx[1] + 2];

		*ptr++ = data.vertices[3 * idx[1] + 0];
		*ptr++ = data.vertices[3 * idx[1] + 1];
		*ptr++ = data.vertices[3 * idx[1] + 2];
		*ptr++ = data.vertices[3 * idx[2] + 0];
		*ptr++ = data.vertices[3 * idx[2] + 1];
		*ptr++ = data.vertices[3 * idx[2] + 2];

		*ptr++ = data.vertices[3 * idx[2] + 0];
		*ptr++ = data.vertices[3 * idx[2] + 1];
		*ptr++ = data.vertices[3 * idx[2] + 2];
		*ptr++ = data.vertices[3 * idx[0] + 0];
		*ptr++ = data.vertices[3 * idx[0] + 1];
		*ptr++ = data.vertices[3 * idx[0] + 2];
	}

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 0, vertices.data());
	glDrawArrays(GL_LINES, 0, vertices.size() / 3);
}

GLuint basicProgram()
{
	static GLuint prog;
	static std::once_flag once;
	std::call_once(once, [] {
		const GLchar* vertSource =
			"attribute mediump vec3 pos;"
			"attribute mediump vec3 normal;"
			"uniform mediump mat4 projection;"
			"uniform mediump mat4 view;"
			"uniform mediump mat4 model;"
			"varying mediump vec4 norm;"
			"void main() {"
			"	vec4 tmp = projection * view * model * vec4(pos, 1.0);"
			"   norm = vec4(normal, 1.0);"
			"	gl_Position = tmp;"
			"}";

		const GLchar* fragSource =
			"uniform mediump vec4 color;"
			"varying mediump vec4 norm;"
			"void main() {"
			"	gl_FragColor = vec4(1.0, 1.0, 1.0, 0.1 );/*norm;*/"
			"}";

		prog = glCreateProgram();
		auto vert = compileShader(GL_VERTEX_SHADER, vertSource);
		auto frag = compileShader(GL_FRAGMENT_SHADER, fragSource);
		glAttachShader(prog, vert);
		glAttachShader(prog, frag);
		glLinkProgram(prog);
		GLint isLinked = 0;
		glGetProgramiv(prog, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(prog, maxLength, &maxLength, &infoLog[0]);

		}
		glDeleteShader(vert);
		glDeleteShader(frag);
		});
	return prog;
}


void drawTriMesh(const Camera& camera, const LightInfo& lights, const Matrix& transform, const TriMeshData& data)
{
	auto prog = basicProgram();
	glUseProgram(prog);

	static GLint aPos = glGetAttribLocation(prog, "pos");
	static GLint aNormal = glGetAttribLocation(prog, "normal");
	static GLint uProjection = glGetUniformLocation(prog, "projection");
	static GLint uView = glGetUniformLocation(prog, "view");
	static GLint uModel = glGetUniformLocation(prog, "model");
	static GLint uColor = glGetUniformLocation(prog, "color");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, camera.projection().data());
	glUniformMatrix4fv(uView, 1, GL_FALSE, camera.view().data());
	glUniformMatrix4fv(uModel, 1, GL_FALSE, transform.data());

	GLfloat white[4] = { 1, 1, 1, 1 };
	glUniform4fv(uColor, 1, white);


	glEnableVertexAttribArray(aPos);

	size_t numTriangles = data.indices.size() / 3;
	std::vector<GLfloat> vertices(numTriangles * 3 * 3);
	auto ptr = vertices.data();
	for (size_t i = 0; i < numTriangles; i++) {
		int idx[3] = {
			data.indices[3 * i + 0],
			data.indices[3 * i + 1],
			data.indices[3 * i + 2]
		};

		*ptr++ = data.vertices[3 * idx[0] + 0];
		*ptr++ = data.vertices[3 * idx[0] + 1];
		*ptr++ = data.vertices[3 * idx[0] + 2];
		*ptr++ = data.vertices[3 * idx[1] + 0];
		*ptr++ = data.vertices[3 * idx[1] + 1];
		*ptr++ = data.vertices[3 * idx[1] + 2];
		*ptr++ = data.vertices[3 * idx[2] + 0];
		*ptr++ = data.vertices[3 * idx[2] + 1];
		*ptr++ = data.vertices[3 * idx[2] + 2];
	}

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 0, vertices.data());

	glEnableVertexAttribArray(aNormal);
	std::vector<GLfloat> normals(numTriangles * 3 * 3);
	ptr = normals.data();
	for (size_t i = 0; i < numTriangles; i++) {
		auto normal = data.normals.data() + 3 * i;

		for (size_t j = 0; j < 3; j++) {
			*ptr++ = normal[0];
			*ptr++ = normal[1];
			*ptr++ = normal[2];
		}
	}

	glVertexAttribPointer(aNormal, 3, GL_FLOAT, GL_FALSE, 0, normals.data());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 9);
}
