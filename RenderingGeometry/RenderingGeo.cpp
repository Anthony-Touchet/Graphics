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

	generateGrid(10, 10);

	//Creates Shaders. "in" keyword for input. "out" keyword for output.
	const char* vsSource = "#version 410\n \ layout(location=0) in vec4 position; \ layout(location=1) in vec4 color; \ out vec4 vColor; \ uniform mat4 projectionViewWorldMatrix; \ void main() { vColor = color; gl_Position = projectionViewWorldMatrix * position;}";

	const char* fsSource = "#version 410\n \ in vec4 vColour; \ out vec4 fragColor; \ void main() { fragColor = vColour; }";

	//compileShaders
	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	//Create program to put the shaders on.
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
		printf("Error: Failed to link Shader Program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);	



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
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	glfwSwapBuffers(window);
	glfwPollEvents();
}

void RenderingGeometry::Shutdown()
{


	glfwDestroyWindow(window);
	glfwTerminate();
}


// function to create a grid
void RenderingGeometry::generateGrid(unsigned int rows, unsigned int cols) {
	Vertex* aoVertices = new Vertex[ rows * cols];
	for (unsigned int r = 0; r < rows; r++) {
		for (unsigned int c = 0; c < cols; c++) {
			aoVertices[r* cols + c].position = vec4((float)c, 0, (float)r, 1);

			//create some arbitrary color based off something
			//that might not be related to tiling texture
			vec3 color = vec3(sinf((c / (float)(cols - 1))* (r / (float)(rows - 1))));
			aoVertices[r * cols + c].color = vec4(color, 1);
		}
	}

	unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];

	unsigned int index = 0;
	for (unsigned int r = 0; r < (rows - 1); r++) {
		for (unsigned int c = 0; c < (cols - 1); c++) {
			//triangle 1
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);

			//Triangle 2
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = r * cols + (c + 1);
		}
	}
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glGenVertexArrays(1, &m_VAO);

	//VBO Fill
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//IBO Fill
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (cols - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete auiIndices;
	delete aoVertices;
}