#pragma once

class Texture
{
public:
	Texture(const std::string& path);
	unsigned int getID();
private:
	unsigned int ID;
	int width, height, bits;
};