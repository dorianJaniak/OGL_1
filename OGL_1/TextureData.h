#pragma once

class TextureData
{
	int width;
	int height;
	int channelsCount;
	unsigned char* data;
	
public:
	TextureData(TextureData&) = delete;
	TextureData(TextureData&&) = delete;
	TextureData(const char* path, bool flip = false);
	~TextureData();

	bool isOk() const;
	int getWidth() const;
	int getHeight() const;
	int getChannelsCount() const;
	const unsigned char* getData() const;
};