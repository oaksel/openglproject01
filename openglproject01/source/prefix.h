#pragma once

// Main includes

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

// Standard includes.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// Sloth.

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
#define _LENGTH_OF_ARRAY(x) sizeof(x)/sizeof(*x)


// Global functions.

struct vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoordinates;
	glm::vec3 color;
};

struct material {

	float ambient;
	float diffuse;
	float specular;
	float shininess;
};

struct light
{
	glm::vec3 position;
	glm::vec3 color;

	float linearAttenuation;
	float quadraticAttenuation;
};

std::string ReadFile(std::string filePath);
uint GLenumSize(GLenum e);


void enableBlend();
