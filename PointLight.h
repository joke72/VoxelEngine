#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Constants.h"
#include "Shader.h"
#include "VertexArray.h"


class PointLight
{

private:

public:

	float m_Constant;
	float m_Linear;
	float m_Quadratic;

	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;

	glm::vec3 m_Position;
	int ID;
	VertexArray m_VAO;
	VertexBuffer m_VBO;


	static std::vector<PointLight*> lights;
	

	static void Init(Shader* lShader);
	static void Deinit();

	static void InitShaderAll(const Shader& shader);

	PointLight(glm::vec3 pos, glm::vec3 color, float attenuation);

	PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	void InitShader(const Shader& shader) const;

	static void UpdateShaderAll(const Shader& shader, glm::mat4 view);

	void SetAttenuation(const Shader& shader, float attenuation);

	void UpdateShader(const Shader& shader, glm::mat4& view) const;

	static void DrawLights(glm::mat4 view, glm::mat4 projection);

	void Draw(const glm::mat4& view, const glm::mat4& projection) const;

};