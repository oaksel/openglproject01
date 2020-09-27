#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &vertexArrayID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &vertexArrayID);
}

void VertexArray::attachBufferAndAttributes(Buffer vertexBuffer, VertexAttributeList list)
{
	bind();
	vertexBuffer.bind();

	std::vector<vertexAttribute> attributes = list.getList();
	for (uint i = 0; i < attributes.size(); i++)
	{
		vertexAttribute attribute = attributes[i];
		glVertexAttribPointer(i, attribute.elementCount, attribute.dataType, attribute.normalized, sizeof(vertex), (const void *)nextAttributeOffset);
		list.enableAttribute(i);

		nextAttributeOffset += attribute.elementCount * GLenumSize(attribute.dataType);
	}
	unBind();
}

void VertexArray::bind()
{
	glBindVertexArray(vertexArrayID);
}

void VertexArray::unBind()
{
	glBindVertexArray(GL_NONE);
}
