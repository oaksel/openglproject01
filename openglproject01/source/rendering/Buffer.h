#pragma once
#include "prefix.h"

class Buffer
{
private:
	uint bufferID;
	GLenum bufferType;
	GLenum dataType;
	uint size;


public:
	Buffer();
	~Buffer();

	void setProperties(GLenum _bufferType, uint _size, const void *_data);

	inline GLenum getDataType() { return dataType; }
	
	void bind();
	void unBind();

};

