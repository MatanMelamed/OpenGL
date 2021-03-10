#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"




test::TestTexture2D::TestTexture2D()
	: m_TranslationA(0, 0, 0),
	m_Proj(glm::ortho<float>(0, 960, 0, 540, -1.0f, 1.0f)),
	m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
{
	float positions[] = {
		100.0f, 100.0f, 0.0f, 0.0f,
		200.0f, 100.0f, 1.0f, 0.0f,
		200.0f, 200.0f, 1.0f, 1.0f,
		100.0f, 200.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	m_VAO = std::make_unique<VertexArray>();
	m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float)); // create a vertex buffer - holds info of vertices.
	VertexBufferLayout layout; // create a vb layout - holds info about the structure of the vertex vuffer.
	layout.Push<float>(2);
	layout.Push<float>(2);

	m_VAO->AddBuffer(*m_VertexBuffer, layout);
	m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

	m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
	m_Shader->Bind();
	m_Texture = std::make_unique<Texture>("res/icon.png");
	m_Shader->SetUniform1i("u_Texture", 0);
}

test::TestTexture2D::~TestTexture2D()
{
}

void test::TestTexture2D::OnUpdate(float deltaTime)
{
}

void test::TestTexture2D::OnRender()
{
	GLCall(glClearColor(0, 0, 0, 1));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));

	Renderer renderer;

	m_Texture->Bind();

	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
	glm::mat4 mvp = m_Proj * m_View * model;
	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_MVP", mvp);
	renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
}

void test::TestTexture2D::OnImGuiRender()
{
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

}
