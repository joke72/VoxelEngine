#pragma once


#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
	void compileShaders(const char* vShaderCode, const char* fShaderCode);
	void compileShaders(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode);


public:

	static Shader defaultShader;
	static Shader blockShader;
	static Shader screenShader;
	static Shader shadowShader;

	static void InitShaders();

	unsigned int ID;

	Shader();

	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);

	void use() const;

	void setBool(const std::string name, const bool& value) const;
	void setInt(const std::string name, const int& value) const;
	void setFloat(const std::string name, const float& value) const;
	void setMat4(const std::string name, const glm::mat4& value) const;
	void setVec3f(const std::string name, const float& x, const float& y, const float& z) const;
	void setVec3(const std::string name, const glm::vec3& value) const;

};

