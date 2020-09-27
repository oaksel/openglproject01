#pragma once
#include "prefix.h"
#include "rendering/Buffer.h"
#include "rendering/ShaderProgram.h"
#include "rendering/VertexArray.h"

class Mesh
{
private:
	std::vector<vertex> vertices;
	std::vector<ushort> indices;

	VertexArray va;
	Buffer vb, ib;

public:
	Mesh(std::string fileLocation);
	~Mesh(){}

	void draw(ShaderProgram &_program);
	void draw(ShaderProgram &_program, GLenum drawMode);

	void LoadPLY(std::string filePath);
};

