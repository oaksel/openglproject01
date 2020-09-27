#pragma once
#include "prefix.h"
#include "Buffer.h"


struct vertexAttribute
{
	uint elementCount;
	GLenum dataType;
	GLenum normalized = GL_FALSE;

};

class VertexAttributeList
{
private:
	std::vector<vertexAttribute> list;

public:
	VertexAttributeList()/* : nextAttributeOffset(0)*/ {}
	~VertexAttributeList() {}

	inline void enableAttribute(uint attributeIndex) { glEnableVertexAttribArray(attributeIndex); }
	inline std::vector<vertexAttribute> getList() { return list; }
	inline void add(vertexAttribute &attribute) { list.push_back(attribute); }


};

class VertexArray
{
private:
	uint vertexArrayID;
	uint nextAttributeOffset = 0;
public:
	VertexArray();
	~VertexArray();

	void attachBufferAndAttributes(Buffer vertexBuffer, VertexAttributeList list);

	void bind();
	void unBind();
};
