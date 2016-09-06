#include "MyApp.h"

RenderingGeometry::RenderingGeometry()
{
}

bool RenderingGeometry::Start()
{
	if (glfwInit() == false)	//FailSafe
		return false;

	window = glfwCreateWindow(1280, 720, "Anthony is the Best!!", nullptr, nullptr);

	if (window == nullptr) {	//FailSafe
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {	//FailSafe
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	mat4 view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));

	mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	m_projectionViewMatrix = projection * view;

	
	// create shaders
	const char* vsSource = "#version 410\n \ layout(location=0) in vec4 position; \ layout(location=1) in vec4 colour; \ out vec4 vColour; \ uniform mat4 projectionViewWorldMatrix; \ void main() { vColour = colour; gl_Position = projectionViewWorldMatrix * position;}";
	const char* fsSource = "#version 410\n \ in vec4 vColour; \ out vec4 fragColor; \ void main() { fragColor = vColour; }";

	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glUseProgram(m_programID);
	unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(m_projectionViewMatrix));
	glBindVertexArray(m_VAO);
	unsigned int indexCount = 4;
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	glEnable(GL_DEPTH_TEST);	//Enables the Depth Buffer
	return true;
}

bool RenderingGeometry::Update()
{
	//If window is open
	if (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		return true;
	}
	return false;
}

void RenderingGeometry::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void RenderingGeometry::Shutdown()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void RenderingGeometry::generateBuffers() {
	Vertex vertices[4];
	unsigned int indices[4] = { 0,1,2,3 };

	vertices[0].position = vec4(-5, 0, -5, 1);
	vertices[1].position = vec4(5, 0, -5, 1);
	vertices[2].position = vec4(-5, 0, 5, 1);
	vertices[3].position = vec4(5, 0, 5, 1);

	vertices[0].color = vec4(1, 0, 0, 1);
	vertices[1].color = vec4(0, 1, 0, 1);
	vertices[2].color = vec4(0, 0, 1, 1);
	vertices[3].color = vec4(1, 1, 1, 1);
	
	//Gen Buffers
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	//VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}