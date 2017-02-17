#pragma once
#include <string>
#include <Texture.h>

class Shader
{
public:
	static unsigned int CompileShaders(const char* vsFile, const char* fsFile);
private:
	static bool LoadShaderFromFile(const char* filePath, std::string& code);
};