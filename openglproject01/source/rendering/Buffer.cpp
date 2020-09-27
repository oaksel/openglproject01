#include "buffer.h"

Buffer::Buffer()
{
	glGenBuffers(1, &bufferID);
	// Don't unBind() before vertexAttribPointer is set.
	//unBind();
}

Buffer::~Buffer()
{
	unBind();
	glDeleteBuffers(1, &bufferID);
}

void Buffer::setProperties(GLenum _bufferType, uint _size, const void *_data)
{
	bufferType = _bufferType;
	size = _size;
	if (bufferType == GL_ARRAY_BUFFER) dataType = GL_FLOAT;
	else dataType = GL_UNSIGNED_SHORT;


	bind();
	glBindBuffer(bufferType, bufferID);
	glBufferData(bufferType, _size, _data, GL_DYNAMIC_DRAW);
	unBind();
}

void Buffer::bind()
{
	glBindBuffer(bufferType, bufferID);
}

void Buffer::unBind()
{
	glBindBuffer(bufferType, GL_NONE);
}
