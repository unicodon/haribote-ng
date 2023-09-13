#include "DrawMesh.h"

#include <GLES2/gl2.h>
#include <math.h>
#include <mutex>

static void calculateNormal(float* normal /* vec3 */, const float* vert0, const float* vert1, const float* vert2)
{
	float v1[3] = {
		vert0[0] - vert2[0],
		vert0[1] - vert2[1],
		vert0[2] - vert2[2]
	};
	float v2[3] = {
		vert1[0] - vert2[0],
		vert1[1] - vert2[1],
		vert1[2] - vert2[2]
	};

	float prod[3] = {
		v1[1] * v2[2] - v1[2] * v2[1],
		v1[2] * v2[0] - v1[0] * v2[2],
		v1[0] * v2[1] - v1[1] * v2[0]
	};

	float len = sqrt(prod[0] * prod[0] + prod[1] * prod[1] + prod[2] * prod[2]);
	if (len > 0) {
		normal[0] = prod[0] / len;
		normal[1] = prod[1] / len;
		normal[2] = prod[2] / len;
	}
	else {
		normal[0] = 1;
		normal[1] = 0;
		normal[2] = 0;
	}
}

DrawMesh::DrawMesh(Value& source)
{
	auto& mesh = source.asObject();

	auto& vertices = mesh["vertices"].asArray();
	auto& faceVertices = mesh["faceVertices"].asArray();

	auto numTriangles = faceVertices.size() / 3;
	auto numVertices = vertices.size() / 3;

	m_vertices.assign(numVertices * 3, 0);
	m_indices.assign(numTriangles * 3, 0);
	m_normals.assign(numTriangles * 3, 0);

	for (size_t i = 0; i < numVertices * 3; i++) {
		m_vertices[i] = vertices[i].asNumber();
	}
	for (size_t i = 0; i < numTriangles * 3; i++) {
		m_indices[i] = faceVertices[i].asNumber();
	}
	for (size_t i = 0; i < numTriangles; i++) {
		float* vert0 = m_vertices.data() + m_indices[i * 3 + 0] * 3;
		float* vert1 = m_vertices.data() + m_indices[i * 3 + 1] * 3;
		float* vert2 = m_vertices.data() + m_indices[i * 3 + 2] * 3;
		calculateNormal(m_normals.data() + i * 3, vert0, vert1, vert2);
	}
}

GLuint basicProgram()
{
	static GLuint prog;
	static std::once_flag once;
	std::call_once(once, [] {
		const GLchar* vertSource =
			"attribute mediump vec3 aPos;"
			"attribute mediump vec3 aNormal;"
			"uniform mediump mat4 uProjection;"
			"uniform mediump mat4 uView;"
			"uniform mediump mat4 uModel;"
			"varying mediump vec3 norm;"
			"void main() {"
			"	vec4 tmp = uProjection * uView * uModel * vec4(aPos, 1.0);"
			"   norm = aNormal + vec3(0.5,0.5,0.5);"
			"	gl_Position = tmp;"
			"}";

		const GLchar* fragSource =
			"uniform mediump vec4 uColor;"
			"varying mediump vec3 norm;"
			"void main() {"
			"	gl_FragColor = vec4(norm, 0.8);"
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


void DrawMesh::draw(unsigned drawFlags, const Matrix& transform, const Camera& camera, const LightInfo& lights)
{
	auto prog = basicProgram();
	glUseProgram(prog);

	static GLint aPos = glGetAttribLocation(prog, "aPos");
	static GLint aNormal = glGetAttribLocation(prog, "aNormal");
	static GLint uProjection = glGetUniformLocation(prog, "uProjection");
	static GLint uView = glGetUniformLocation(prog, "uView");
	static GLint uModel = glGetUniformLocation(prog, "uModel");
	static GLint uColor = glGetUniformLocation(prog, "uColor");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, camera.projection().data());
	glUniformMatrix4fv(uView, 1, GL_FALSE, camera.view().data());
	glUniformMatrix4fv(uModel, 1, GL_FALSE, transform.data());

	GLfloat white[4] = { 1, 1, 1, 1 };
	glUniform4fv(uColor, 1, white);

	glEnableVertexAttribArray(aPos);

	size_t numTriangles = m_indices.size() / 3;
	std::vector<GLfloat> vertices(numTriangles * 3 * 3);
	auto ptr = vertices.data();
	for (size_t i = 0; i < numTriangles; i++) {
		int idx[3] = {
			m_indices[3 * i + 0],
			m_indices[3 * i + 1],
			m_indices[3 * i + 2]
		};

		*ptr++ = m_vertices[3 * idx[0] + 0];
		*ptr++ = m_vertices[3 * idx[0] + 1];
		*ptr++ = m_vertices[3 * idx[0] + 2];
		*ptr++ = m_vertices[3 * idx[1] + 0];
		*ptr++ = m_vertices[3 * idx[1] + 1];
		*ptr++ = m_vertices[3 * idx[1] + 2];
		*ptr++ = m_vertices[3 * idx[2] + 0];
		*ptr++ = m_vertices[3 * idx[2] + 1];
		*ptr++ = m_vertices[3 * idx[2] + 2];
	}

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 0, vertices.data());

	glEnableVertexAttribArray(aNormal);
	std::vector<GLfloat> normals(numTriangles * 3 * 3);
	ptr = normals.data();
	for (size_t i = 0; i < numTriangles; i++) {
		auto normal = m_normals.data() +  i * 3;

		for (size_t j = 0; j < 3; j++) {
			*ptr++ = normal[0];
			*ptr++ = normal[1];
			*ptr++ = normal[2];
		}
	}

	glVertexAttribPointer(aNormal, 3, GL_FLOAT, GL_FALSE, 0, normals.data());

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLES, 0, numTriangles * 3);

	drawAxes(camera, transform);
}
