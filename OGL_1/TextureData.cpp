#include "TextureData.h"
//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

TextureData::TextureData(const char* path, bool flip)
{
	stbi_set_flip_vertically_on_load(flip);
	data = stbi_load(path, &width, &height, &channelsCount, 0);
}

TextureData::~TextureData()
{
	stbi_image_free(data);
}

bool TextureData::isOk() const
{
	return (data != nullptr);
}

int TextureData::getWidth() const
{
	return width;
}

int TextureData::getHeight() const
{
	return height;
}

int TextureData::getChannelsCount() const
{
	return channelsCount;
}

const unsigned char* TextureData::getData() const
{
	return data;
}