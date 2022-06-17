#include "PointLight.h"

std::vector<PointLight*> PointLight::lights;


PointLight::PointLight(glm::vec3 pos, glm::vec3 color, float attenuation) :
	m_Constant(1.0f),
	m_Linear(1 / attenuation),
	m_Quadratic((1 / attenuation)* (0.0019f / 0.022f)),
	m_Ambient(color.x * 0.5f, color.y * 0.5f, color.z * 0.5f),
	m_Diffuse(color.x, color.y, color.z),
	m_Specular(1.0f, 1.0f, 1.0f),
	m_Position(pos),
	ID(lights.size()),
	m_VAO(),
	m_VBO(CUBE_MESH, sizeof(CUBE_MESH), GL_STATIC_DRAW),
	m_Layout()
{
	std::cout << ID << std::endl;

	m_Layout.AddVertexAttribute(GL_FLOAT, 3);

	m_VAO.AddBuffer(m_VBO, m_Layout);

	lights.push_back(this);
}

PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
m_Constant(1.0f),
m_Linear(0.022f),
m_Quadratic(0.0019),

m_Ambient(ambient),
m_Diffuse(diffuse),
m_Specular(specular),
m_Position(position),
ID(lights.size()),
m_VAO(),
m_VBO(CUBE_MESH, sizeof(CUBE_MESH), GL_STATIC_DRAW),
m_Layout()
{
	m_Layout.AddVertexAttribute(GL_FLOAT, 3);

	m_VAO.AddBuffer(m_VBO, m_Layout);

	lights.push_back(this);
}

void PointLight::InitShaderAll(const Shader& shader)
{
	for (PointLight* light : lights)
	{
		light->InitShader(shader);
	}
	shader.setInt("activePointLights", lights.size());
}
void PointLight::InitShader(const Shader& shader) const
{
	shader.use();
	shader.setFloat("pointLights[" + std::to_string(ID) + "].constant", 1.0f);
	shader.setFloat("pointLights[" + std::to_string(ID) + "].linear", m_Linear);
	shader.setFloat("pointLights[" + std::to_string(ID) + "].quadratic", m_Quadratic);

	shader.setVec3f("pointLights[" + std::to_string(ID) + "].ambient", m_Ambient.x, m_Ambient.y, m_Ambient.z);
	shader.setVec3f("pointLights[" + std::to_string(ID) + "].diffuse", m_Diffuse.x, m_Diffuse.y, m_Diffuse.z);
	shader.setVec3f("pointLights[" + std::to_string(ID) + "].specular", m_Specular.x, m_Specular.y, m_Specular.z);
}

void PointLight::Init(Shader* lShader)
{
}
void PointLight::Deinit()
{
}

void PointLight::UpdateShaderAll(const Shader& shader, glm::mat4 view)
{
	for (PointLight* light : lights)
	{
		light->UpdateShader(shader, view);
	}
	shader.setInt("activePointLights", lights.size());
}
void PointLight::SetAttenuation(const Shader& shader, float attenuation)
{
	shader.use();

	m_Constant = 1.0f;
	if (attenuation == 0)
		return;
	m_Linear = 1 / attenuation;
	m_Quadratic = (1 / attenuation) * (0.0019f / 0.022f);

	shader.setFloat("pointLights[" + std::to_string(ID) + "].constant", m_Constant);
	shader.setFloat("pointLights[" + std::to_string(ID) + "].linear", m_Linear);
	shader.setFloat("pointLights[" + std::to_string(ID) + "].quadratic", m_Quadratic);
}

void PointLight::UpdateShader(const Shader& shader, glm::mat4& view) const
{
	shader.use();
	shader.setVec3("pointLights[" + std::to_string(ID) + "].position", m_Position);
}

void PointLight::DrawLights(glm::mat4 view, glm::mat4 projection)
{
	for (PointLight* light : lights)
	{
		light->Draw(view, projection);
	}
}
void PointLight::Draw(const glm::mat4& view, const glm::mat4& projection) const
{

	m_VAO.Bind();

	Shader::defaultShader.use();
	Shader::defaultShader.setMat4("view", view);
	Shader::defaultShader.setMat4("projection", projection);
	Shader::defaultShader.setVec3("col", m_Diffuse);

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, m_Position);
	model = glm::scale(model, glm::vec3(0.2f));
	Shader::defaultShader.setMat4("model", model);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	m_VAO.Unbind();
}
