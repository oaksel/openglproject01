#include "mesh.h"

Mesh::Mesh(std::string _fileLocation)
{
	LoadPLY(_fileLocation);

	uint vertexBufferSize = sizeof(vertex) * vertices.size();
	uint indexBufferSize = sizeof(ushort) * indices.size();

	vb.setProperties(GL_ARRAY_BUFFER, vertexBufferSize, (const void *)vertices.data());
	ib.setProperties(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, (const void *)indices.data());

	vertexAttribute position			 = { 3, GL_FLOAT };
	vertexAttribute normal				 = { 3, GL_FLOAT };
	vertexAttribute textureCoordinates	 = { 2, GL_FLOAT };
	vertexAttribute color				 = { 3, GL_FLOAT };

	VertexAttributeList aList;
	aList.add(position);
	aList.add(normal);
	aList.add(textureCoordinates);
	aList.add(color);

	va.attachBufferAndAttributes(vb, aList);
}


void Mesh::draw(ShaderProgram &_program)
{
	_program.use();
	va.bind();
	ib.bind();

	glDrawElements(GL_TRIANGLES, indices.size(), ib.getDataType(), nullptr);

	_program.disUse();
}
void Mesh::draw(ShaderProgram &_program, GLenum _drawMode)
{
	_program.use();
	va.bind();
	ib.bind();

	glDrawElements(_drawMode, indices.size(), ib.getDataType(), nullptr);

	_program.disUse();
}

void Mesh::LoadPLY(std::string filePath)
{
	std::string fileContent = ReadFile(filePath);

	std::istringstream ss01(fileContent);
	std::string line;

	uint vCount = 0;
	uint faceCount = 0;
	uint iCount = 0;

	std::vector<vertex> temp_vertices;
	std::vector<ushort> temp_indices;

	bool readtemp_vertices = false, readtemp_indices = false, counterEnabled = false;

	int counter01 = 0; // vertex counter
	int counter02 = 0; // face counter

	while (ss01)
	{
		std::getline(ss01, line);

		if (line.find("element vertex") != std::string::npos)
		{
			std::istringstream ss02(line);
			std::string str;
			ss02 >> str >> str; // read it twice because the next value is vCount
			ss02 >> vCount;
		}
		else if (line.find("element face") != std::string::npos)
		{
			std::istringstream ss03(line);
			std::string str;
			ss03 >> str >> str; // read it twice because the next value is faceCount
			ss03 >> faceCount;
		}
		else if (line.find("end_header") != std::string::npos)
		{
			readtemp_vertices = true;
			counterEnabled = true;
		}
		if (counterEnabled && counter01 >= vCount)
		{
			//printf("%d temp_vertices read. Starting temp_indices...\n", temp_vertices.size());
			readtemp_vertices = false;
			counter01 = 0;

			readtemp_indices = true;
		}
		if (counterEnabled && counter02 >= faceCount)
		{
			//printf("%d faces read. %d temp_indices created. Waiting to end of stringstream...\n", counter02, temp_indices.size());

			readtemp_indices = false;

			counter02 = 0;
			counterEnabled = false;

		}

		if (readtemp_vertices)
		{
			counter01++;

			vertex temp;

			ss01 >> temp.position.x >> temp.position.y >> temp.position.z;
			ss01 >> temp.normal.x >> temp.normal.y >> temp.normal.z;
			ss01 >> temp.textureCoordinates.s >> temp.textureCoordinates.t;
			ss01 >> temp.color.r >> temp.color.g >> temp.color.b; /*>> temp.color.a*/

			// reading one for alpha value
			float dummy = 0.0f;
			ss01 >> dummy;

			// RGBA values are between 0-255. They need to be normalized.
			temp.color.r /= 255;
			temp.color.g /= 255;
			temp.color.b /= 255;
			/*temp.color.a /= 255;*/

			temp_vertices.push_back(temp);
		}
		else if (readtemp_indices)
		{
			counter02++;

			int temp_verticesOnTheFace = 0;
			ss01 >> temp_verticesOnTheFace; // first value of the line which indicates how many temp_vertices in the face


			switch (temp_verticesOnTheFace)
			{
			case 3:
				float values3[3];
				for (int i = 0; i < 3; i++) ss01 >> values3[i];

				temp_indices.push_back(values3[0]);
				temp_indices.push_back(values3[1]);
				temp_indices.push_back(values3[2]);
				break;

			case 4:
				float values4[4];
				for (int i = 0; i < 4; i++) ss01 >> values4[i];

				temp_indices.push_back(values4[0]);
				temp_indices.push_back(values4[1]);
				temp_indices.push_back(values4[2]);

				temp_indices.push_back(values4[2]);
				temp_indices.push_back(values4[3]);
				temp_indices.push_back(values4[0]);
				break;

			default: break;
			}
		}
	}

	vertices = temp_vertices;
	indices = temp_indices;

}
