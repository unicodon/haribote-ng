#include "MeshLoader.h"
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <fstream>

#include "../graphics/GraphicsUtils.h"

#include "../util/Json.h"

namespace
{
	enum STATE
	{
		NOP = 0,
		AN,
		CANMA,
		SEMICOLON,
		BEGIN_BRACE,
		END_BRACE
	};

	const char table[128] = {
		//0
		NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,
		NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,

		//1
		NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,
		NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,

		//2
		NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,	NOP,
		NOP,	NOP,	NOP,	NOP,	CANMA,	AN,		AN,		NOP,

		//3
		AN,		AN,		AN,		AN,		AN,		AN,		AN,		AN,
		AN,		AN,		NOP,	SEMICOLON,NOP,	NOP,	NOP,	NOP,

		//4
		NOP,	AN,		AN,		AN,		AN,		AN,		AN,		AN,
		AN,		AN,		AN,		AN,		AN,		AN,		AN,		AN,

		//5
		AN,		AN,		AN,		AN,		AN,		AN,		AN,		AN,
		AN,		AN, 	AN, 	NOP,	NOP,	NOP,	NOP,	AN,

		//6
		NOP,	AN,		AN,		AN,		AN,		AN,		AN,		AN,
		AN,		AN,		AN,		AN,		AN,		AN,		AN,		AN,

		//7
		AN,		AN,		AN,		AN,		AN,		AN,		AN,		AN,
		AN,		AN, 	AN, 	BEGIN_BRACE,	NOP,	END_BRACE,	NOP,	NOP

	};

	void SkipBlock(const char*& str, int& index) {
		int depth = 0;
		while (1) {
			if (str[index] == 0)
				break;

			switch (table[str[index++]]) {
			case BEGIN_BRACE:
				depth++;
				break;
			case END_BRACE:
				depth--;
				if (depth == 0)
					return;
				if (depth < 0) {
					index--;
					return;
				}
				break;
			}
		}
	}

	void SkipTemplate(const char*& str, int& index)
	{
		while (table[str[index++]] != END_BRACE) {
		}
	}


	struct XHeader {
		char magicNumber[4];
		char versionNumber[4];
		char formatType[4];
		char floatSize[4];
	};

	struct Header {
		void Parse(const char*& pointer, int& index);
		uint16_t major;
		uint16_t minor;
		uint32_t flags;
	};

	struct Vector {
		void Parse(const char*& pointer, int& index);
		float x;
		float y;
		float z;
	};

	struct Coords2d {
		void Parse(const char*& pointer, int& index);
		float u;
		float v;
	};

	struct Matrix4x4 {
		void Parse(const char*& pointer, int& index);
		float matrix[16];
	};

	struct ColorRGBA {
		void Parse(const char*& pointer, int& index);
		float red;
		float green;
		float blue;
		float alpha;
	};

	struct ColorRGB {
		void Parse(const char*& pointer, int& index);
		float red;
		float green;
		float blue;
	};

	struct IndexedColor {
		void Parse(const char*& pointer, int& index);
		uint32_t index;
		ColorRGBA indexColor;
	};

	struct bool2d {
		void Parse(const char*& pointer, int& index);
		bool u;
		bool v;
	};

	struct MaterialWrap {
		void Parse(const char*& pointer, int& index);
		bool u;
		bool v;
	};

	struct TextureFilename {
		void Parse(const char*& pointer, int& index);
		std::string filename;
	};

	struct Material {
		void Parse(const char*& pointer, int& index);
		ColorRGBA faceColor;
		float power;
		ColorRGB specularColor;
		ColorRGB emissiveColor;
	};

	struct MeshFace {
		void Parse(const char*& pointer, int& index);
		uint32_t nFaceVertexIndices;
		std::vector<uint32_t> faceVertexIndices;
	};

	struct MeshFaceWraps {
		void Parse(const char*& pointer, int& index);
		uint32_t nFaceWrapValues;
		bool2d faceWrapValues;
	};

	struct MeshTextureCoords {
		void Parse(const char*& pointer, int& index);
		uint32_t nTextureCoords;
		std::vector<Coords2d> textureCoords;
	};

	struct MeshMaterialList {
		void Parse(const char*& pointer, int& index);
		uint32_t nMaterials;
		uint32_t nFaceIndexes;
		std::vector<uint32_t> faceIndexes;
		std::vector<Material> materials;
	};

	struct MeshNormals {
		void Parse(const char*& pointer, int& index);
		uint32_t nNormals;
		std::vector<Vector> normals;
		uint32_t nFaceNormals;
		std::vector<MeshFace> faceNormals;
	};

	struct MeshVertexColors {
		void Parse(const char*& pointer, int& index);
		uint32_t nVertexColors;
		std::vector<IndexedColor> vertexColors;
	};

	struct Mesh {
		void Parse(const char*& pointer, int& index);
		uint32_t nVertices;
		std::vector<Vector> vertices;
		uint32_t nFaces;
		std::vector<MeshFace> faces;
		MeshMaterialList materialList;
		MeshNormals normals;
	};


	void bool2d::Parse(const char*& str, int& index)
	{
	}

	void ColorRGB::Parse(const char*& str, int& index)
	{
		char token[256];
		int tokind = 0;

		//red
		while (table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		red = atof(token);

		//green
		while (table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		green = atof(token);

		//blue
		while (table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		blue = atof(token);
	}

	void ColorRGBA::Parse(const char*& str, int& index)
	{
		char token[256];
		int tokind = 0;

		//red
		while (table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		red = atof(token);

		//green
		while (table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		green = atof(token);

		//blue
		while (table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		blue = atof(token);

		//alpha
		while (table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		alpha = atof(token);
	}

	void Coords2d::Parse(const char*& str, int& index)
	{
	}

	void Header::Parse(const char*& str, int& index)
	{
		char token[256];
		int tokind = 0;
		while (table[str[index++]] != BEGIN_BRACE) {
		}

		//major
		while (table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		major = atoi(token);

		//minor
		while (table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		minor = atoi(token);

		//flags
		while (table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		flags = atoi(token);

		while (table[str[index++]] != END_BRACE) {
		}
	}

	void IndexedColor::Parse(const char*& str, int& index)
	{
	}

	void Material::Parse(const char*& str, int& index)
	{
		char token[256];
		int tokind = 0;

		while (table[str[index++]] != BEGIN_BRACE) {
		}

		//faceColor
		while (table[str[index]] == NOP) {
			index++;
		}
		faceColor.Parse(str, index);
		while (table[str[index]] == SEMICOLON) {
			index++;
		}

		//power
		while (table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		power = atof(token);
		while (table[str[index]] == SEMICOLON) {
			index++;
		}

		//specular
		while (table[str[index]] == NOP) {
			index++;
		}
		specularColor.Parse(str, index);
		while (table[str[index]] == SEMICOLON) {
			index++;
		}

		//emissive
		while (table[str[index]] == NOP) {
			index++;
		}
		emissiveColor.Parse(str, index);
		while (table[str[index]] == SEMICOLON) {
			index++;
		}


		while (table[str[index++]] != END_BRACE) {
		}
	}

	void MaterialWrap::Parse(const char*& str, int& index)
	{
	}

	void Matrix4x4::Parse(const char*& str, int& index)
	{
	}

	void Mesh::Parse(const char*& str, int& index)
	{
		char token[256];
		int tokind = 0;
		while (str[index] && table[str[index++]] != BEGIN_BRACE) {
		}

		//nVertices
		while (str[index] && table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (str[index] && table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		nVertices = atoi(token);

		//vertices
		for (int i = 0; i < nVertices; i++) {
			Vector vec;
			vec.Parse(str, index);
			vertices.push_back(vec);
			while (str[index] && (table[str[index]] == NOP || table[str[index]] == CANMA)) {
				index++;
			}
		}
		while (str[index] && (table[str[index]] == NOP || table[str[index]] == SEMICOLON)) {
			index++;
		}

		//nFaces
		while (str[index] && table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (str[index] && (table[str[index]] != SEMICOLON && table[str[index]] != NOP)) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		nFaces = atoi(token);

		//faces
		for (int i = 0; i < nFaces; i++) {
			MeshFace face;
			face.Parse(str, index);
			faces.push_back(face);
			while (str[index] && (table[str[index]] == NOP || table[str[index]] == CANMA)) {
				index++;
			}
		}

		while (str[index] && table[str[index]] != AN) {
			index++;
		}

		while (str[index] && table[str[index]] != END_BRACE) {
			while (str[index] && table[str[index]] == NOP) {
				index++;
			}

			tokind = 0;
			while (str[index] && table[str[index]] == AN) {
				token[tokind++] = str[index++];
			}
			token[tokind] = 0; index++;

			if (strcmp(token, "MeshMaterialList") == 0) {
				materialList.Parse(str, index);
				continue;
			}

			if (strcmp(token, "MeshNormals") == 0) {
				normals.Parse(str, index);
				continue;
			}

			//skip
			SkipBlock(str, index);
			while (str[index] && table[str[index]] == NOP) {
				index++;
			}
		}
	}

	void MeshFace::Parse(const char*& str, int& index)
	{
		char token[256];
		int tokind = 0;

		//nFaceVertexIndices
		while (table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		nFaceVertexIndices = atoi(token);

		//faceVertexIndices
		while (table[str[index]] == NOP) {
			index++;
		}
		for (int i = 0; i < nFaceVertexIndices; i++) {
			uint32_t ver;
			tokind = 0;
			while (table[str[index]] != CANMA && table[str[index]] != NOP && table[str[index]] != SEMICOLON) {
				token[tokind++] = str[index++];
			}
			token[tokind] = 0; index++;
			ver = atoi(token);
			faceVertexIndices.push_back(ver);
		}
	}

	void MeshFaceWraps::Parse(const char*& str, int& index)
	{
	}

	void MeshMaterialList::Parse(const char*& str, int& index)
	{
		char token[256];
		int tokind = 0;

		while (table[str[index++]] != BEGIN_BRACE) {
		}


		//nMaterials
		while (table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		nMaterials = atoi(token);

		//nFaceIndexes
		while (table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		nFaceIndexes = atoi(token);

		//faceIndexes
		for (int i = 0; i < nFaceIndexes; i++) {
			uint32_t ind;
			while (table[str[index]] == NOP) {
				index++;
			}
			tokind = 0;
			while (table[str[index]] != SEMICOLON && table[str[index]] != NOP && table[str[index]] != CANMA) {
				token[tokind++] = str[index++];
			}
			token[tokind] = 0; index++;
			ind = atoi(token);
			faceIndexes.push_back(ind);

			while (table[str[index]] == CANMA || table[str[index]] == SEMICOLON) {
				index++;
			}

		}
		while (table[str[index]] == NOP) {
			index++;
		}

		for (int i = 0; i < nMaterials; i++) {
			tokind = 0;
			while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
				token[tokind++] = str[index++];
			}
			token[tokind] = 0; index++;

			if (strcmp(token, "Material") == 0) {
				Material mat;
				mat.Parse(str, index);
				materials.push_back(mat);

				while (table[str[index]] == NOP || table[str[index]] == CANMA) {
					index++;
				}
			}

		}

		while (table[str[index++]] != END_BRACE) {
		}
	}

	void MeshNormals::Parse(const char*& str, int& index)
	{
		char token[256];
		int tokind = 0;

		while (table[str[index++]] != BEGIN_BRACE) {
		}

		//nNormals
		while (table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		nNormals = atoi(token);

		//normals
		for (int i = 0; i < nNormals; i++) {
			Vector vec;
			vec.Parse(str, index);
			normals.push_back(vec);
			while (table[str[index]] == NOP || table[str[index]] == CANMA) {
				index++;
			}
		}
		while (table[str[index]] == NOP || table[str[index]] == SEMICOLON) {
			index++;
		}

		//nFaceNormals
		while (table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		nFaceNormals = atoi(token);

		//faceNormals
		for (int i = 0; i < nFaceNormals; i++) {
			MeshFace face;
			face.Parse(str, index);
			faceNormals.push_back(face);
			while (table[str[index]] == NOP || table[str[index]] == CANMA) {
				index++;
			}
		}

		while (table[str[index++]] != END_BRACE) {
		}

	}

	void MeshTextureCoords::Parse(const char*& str, int& index)
	{
	}

	void MeshVertexColors::Parse(const char*& str, int& index)
	{
	}

	void TextureFilename::Parse(const char*& str, int& index)
	{
	}

	void Vector::Parse(const char*& str, int& index)
	{
		char token[256];
		int tokind = 0;

		//x
		while (table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		x = atof(token);

		//y
		while (table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		y = atof(token);

		//z
		while (table[str[index]] == NOP) {
			index++;
		}
		tokind = 0;
		while (table[str[index]] != SEMICOLON && table[str[index]] != NOP) {
			token[tokind++] = str[index++];
		}
		token[tokind] = 0; index++;
		z = atof(token);

	}

}


MeshData parseMeshData(const std::vector<char>& data)
{
	const char* str = data.data();
	int size = data.size();
	int index = 0;
	XHeader xheader;
	memcpy(&xheader, str, sizeof(xheader));
	index += sizeof(xheader);

	if (memcmp(xheader.magicNumber, "xof", 3)) return nullptr;
	if (memcmp(xheader.formatType, "txt", 3)) return nullptr;

	Header header;
	Mesh* mesh = new Mesh;

	char tag[256];
	while (index < size) {
		while (table[str[index]] == NOP) {
			index++;
			if (index < size)
				break;

		}

		int tagind = 0;
		while (table[str[index]] == AN) {
			tag[tagind++] = str[index];
			index++;
		}
		tag[tagind] = 0;

		if (strcmp(tag, "template") == 0) {
			SkipTemplate(str, index);
			continue;
		}

		if (strcmp(tag, "Header") == 0) {
			header.Parse(str, index);
			continue;
		}

		if (strcmp(tag, "Mesh") == 0) {
			mesh->Parse(str, index);
			continue;
		}
		index++;
	}

	return mesh;
}

Value conv(ColorRGBA& in)
{
	Value::Array a(4);
	a[0] = (double)in.red;
	a[1] = (double)in.green;
	a[2] = (double)in.blue;
	a[3] = (double)in.alpha;
	return std::move(a);
}

Value conv(ColorRGB& in)
{
	Value::Array a(4);
	a[0] = (double)in.red;
	a[1] = (double)in.green;
	a[2] = (double)in.blue;
	a[3] = (double)1;
	return std::move(a);
}

void exportMeshData(const char* path, MeshData opaque)
{
	std::string p = path;
//	p += ".json";
	Value::Object obj;

	Mesh* mesh = (Mesh*)opaque;

	{
		Value::Array a;
		for (auto& vert : mesh->vertices) {
			Value::Array v(3);
			v[0] = vert.x;
			v[1] = vert.y;
			v[2] = vert.z;
			a.push_back(std::move(v));
		}
		obj["vertices"] = std::move(a);
	}

	{
		Value::Array a;
		for (auto& face : mesh->faces) {
			Value::Array v;
			if (face.nFaceVertexIndices == 4) {
				v.push_back(face.faceVertexIndices[0]);
				v.push_back(face.faceVertexIndices[1]);
				v.push_back(face.faceVertexIndices[2]);

				v.push_back(face.faceVertexIndices[2]);
				v.push_back(face.faceVertexIndices[3]);
				v.push_back(face.faceVertexIndices[0]);
			}
			else {
				for (auto& idx : face.faceVertexIndices) {
					v.push_back(idx);
				}
			}
			a.push_back(std::move(v));
		}
		obj["faceVertices"] = std::move(a);
	}

	{
		Value::Array a;
		for (auto& face : mesh->normals.faceNormals) {
			Value::Array v;
			if (face.nFaceVertexIndices == 4) {
				v.push_back(face.faceVertexIndices[0]);
				v.push_back(face.faceVertexIndices[1]);
				v.push_back(face.faceVertexIndices[2]);

				v.push_back(face.faceVertexIndices[2]);
				v.push_back(face.faceVertexIndices[3]);
				v.push_back(face.faceVertexIndices[0]);
			}
			else {
				for (auto& idx : face.faceVertexIndices) {
					v.push_back(idx);
				}
			}
			a.push_back(std::move(v));
		}
		obj["faceNormals"] = std::move(a);
	}

	{
		Value::Array a;
		for (auto& norm : mesh->normals.normals) {
			Value::Array v(3);
			v[0] = norm.x;
			v[1] = norm.y;
			v[2] = norm.z;
			a.push_back(std::move(v));
		}
		obj["normals"] = std::move(a);
	}

	std::map<int, std::vector<int>> mat_to_face;
	{
		int idx = 0;
		for (auto& mat : mesh->materialList.faceIndexes) {
			mat_to_face[mat].push_back(idx++);
		}
	}

	{
		Value::Array a;
		int idx = 0;
		for (auto& mat : mesh->materialList.materials) {
			Value::Object o;
			o["diffuse"] = conv(mat.faceColor);
			o["specular"] = conv(mat.specularColor);

			Value::Array vert;

			Value::Array fa;
			auto& faces = mat_to_face[idx];
			for (auto& i : faces) {
				fa.push_back(i);
				auto& face = mesh->faces[i];
				auto& norm = mesh->normals.faceNormals[i];
				if (face.nFaceVertexIndices != norm.nFaceVertexIndices) {
					__debugbreak();
				}

				switch (face.nFaceVertexIndices) {
				case 3:
				{
					vert.push_back(face.faceVertexIndices[0]);
					vert.push_back(face.faceVertexIndices[1]);
					vert.push_back(face.faceVertexIndices[2]);
					vert.push_back(norm.faceVertexIndices[0]);
					vert.push_back(norm.faceVertexIndices[1]);
					vert.push_back(norm.faceVertexIndices[2]);
				}
				break;
				case 4:
				{
					vert.push_back(face.faceVertexIndices[0]);
					vert.push_back(face.faceVertexIndices[1]);
					vert.push_back(face.faceVertexIndices[2]);
					vert.push_back(norm.faceVertexIndices[0]);
					vert.push_back(norm.faceVertexIndices[1]);
					vert.push_back(norm.faceVertexIndices[2]);

					vert.push_back(face.faceVertexIndices[2]);
					vert.push_back(face.faceVertexIndices[3]);
					vert.push_back(face.faceVertexIndices[0]);
					vert.push_back(norm.faceVertexIndices[2]);
					vert.push_back(norm.faceVertexIndices[3]);
					vert.push_back(norm.faceVertexIndices[0]);
				}
				break;
				default:
					__debugbreak();
					break;
				}
			}
			o["triangles"] = std::move(vert);
			o["faces"] = std::move(fa);

			a.push_back(std::move(o));
			idx++;
		}
		obj["materials"] = std::move(a);
	}

	std::ofstream ofs(p);
	ofs << Value(std::move(obj));
}

Value loadMeshData(const char* path)
{
	FILE* f;
	if (fopen_s(&f, path, "rb") == 0) {
		std::vector<char> data;

		const size_t BufSize = 16 * 1024;
		std::vector<char> buf(BufSize);

		while (true) {
			auto read = fread(buf.data(), 1, buf.size(), f);
			if (read < 0) {
				fclose(f);
				return Value();
			}
			if (read == 0) {
				break;
			}

			data.insert(data.end(), buf.begin(), buf.begin() + read);
		}

		fclose(f);
		auto mesh = parseMeshData(data);
		std::string jsonPath = path;
		jsonPath += ".json";
		exportMeshData(jsonPath.c_str(), mesh);
		return loadJsonMeshData(jsonPath.c_str());
	}

	return Value();
}


#if 0
void drawMesh(const Camera& camera, MeshData data, const Matrix& transform)
{
	if (!data) {
		return;
	}

	Mesh* mesh = (Mesh*)data;

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

	mesh->vertices.data();
	for (const auto& face : mesh->faces) {
		GLfloat vertex[12];
		auto n = face.faceVertexIndices.size();
		auto* ptr = vertex;
		for (const auto& idx : face.faceVertexIndices) {
			auto v = mesh->vertices[idx];
			*ptr++ = v.x;
			*ptr++ = v.y;
			*ptr++ = v.z;
		}
		glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 0, vertex);
		glDrawArrays(GL_LINE_LOOP, 0, n);
	}
}
#endif

void drawMesh(const Camera& camera, Value& data, const Matrix& transform)
{
	if (!data.isObject()) {
		return;
	}

	auto& mesh = data.asObject();

	Matrix mat = camera.projection();
	mat *= camera.view();
	mat *= transform;

	auto prog = wireframeProgram();
	glUseProgram(prog);

	static GLint aPos = glGetAttribLocation(prog, "pos");
	static GLint uMVP = glGetUniformLocation(prog, "mvp");
	static GLint uColor = glGetUniformLocation(prog, "color");


	GLfloat white[4] = { 1, 1, 1, 1 };
	glUniform4fv(uColor, 1, white);

	glUniformMatrix4fv(uMVP, 1, GL_FALSE, mat.data());

	glEnableVertexAttribArray(aPos);

	auto& materials = mesh["materials"].asArray();
	auto& vertices = mesh["vertices"].asArray();
	auto& faces = mesh["faceVertices"].asArray();

	for (auto& elem : materials) {
		auto& material = elem.asObject();
		auto& triangles = material["triangles"].asArray();
		auto& matfaces = material["faces"].asArray();
		auto& diffuse = material["diffuse"].asArray();
		GLfloat color[4] = {
			diffuse[0].asNumber(),
			diffuse[1].asNumber(),
			diffuse[2].asNumber(),
			1
		};
		glUniform4fv(uColor, 1, color);

		int n = matfaces.size();
		std::vector<GLfloat> vert(n * 18);
		auto ptr = vert.data();
		for (int i = 0; i < n; i++) {
			int faceIdx = matfaces[i].asInt();
			auto faceVerts = faces[faceIdx].asArray();
			int idx[3] = {
				faceVerts[0].asInt(),
				faceVerts[1].asInt(),
				faceVerts[2].asInt()
			};
			auto arr0 = vertices[idx[0]].asArray();
			auto arr1 = vertices[idx[1]].asArray();
			auto arr2 = vertices[idx[2]].asArray();

			*ptr++ = arr0[0].asNumber();
			*ptr++ = arr0[1].asNumber();
			*ptr++ = arr0[2].asNumber();
			*ptr++ = arr1[0].asNumber();
			*ptr++ = arr1[1].asNumber();
			*ptr++ = arr1[2].asNumber();
			*ptr++ = arr1[0].asNumber();
			*ptr++ = arr1[1].asNumber();
			*ptr++ = arr1[2].asNumber();
			*ptr++ = arr2[0].asNumber();
			*ptr++ = arr2[1].asNumber();
			*ptr++ = arr2[2].asNumber();
			*ptr++ = arr2[0].asNumber();
			*ptr++ = arr2[1].asNumber();
			*ptr++ = arr2[2].asNumber();
			*ptr++ = arr0[0].asNumber();
			*ptr++ = arr0[1].asNumber();
			*ptr++ = arr0[2].asNumber();
		}
		glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 0, vert.data());
		glDrawArrays(GL_LINES, 0, vert.size() / 3);
	}
}

Value loadJsonMeshData(const char* path)
{
	FILE* f;
	if (fopen_s(&f, path, "rb") == 0) {
		std::vector<char> data;

		const size_t BufSize = 16 * 1024;
		std::vector<char> buf(BufSize);

		while (true) {
			auto read = fread(buf.data(), 1, buf.size(), f);
			if (read < 0) {
				fclose(f);
				return Value{};
			}
			if (read == 0) {
				break;
			}

			data.insert(data.end(), buf.begin(), buf.begin() + read);
		}

		fclose(f);
		return parseJson(data.data(), data.size());
	}

	return Value{};
}
