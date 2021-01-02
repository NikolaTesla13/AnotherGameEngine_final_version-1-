#pragma once

class Shader
{
public:
	Shader(const std::string &vertexFile, const std::string &fragmentFile);
	void start();
	void stop();
	unsigned int getID();
	~Shader();
private:
	unsigned int ID;
	unsigned int vertexShaderID, fragmentShaderID;

	void checkCompileErrors(unsigned int shader, std::string type);
};