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

	cam.setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));				//Sets the FlyCamera
	cam.setPerspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	cam.setSpeed(10);

	glEnable(GL_DEPTH_TEST);	//Enables the Depth Buffer
	Gizmos::create();
	return true;
}

bool RenderingGeometry::Update()
{
	//Calculate delta time
	current = (float)glfwGetTime();
	delta = current - previous;
	previous = current;

	//If window is open
	if (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Gizmos::clear();

		//Update Code go here

		return true;
	}
	return false;
}

void RenderingGeometry::Draw()
{

	Gizmos::addTransform(glm::mat4(1));
	vec4 white(1);
	vec4 black(.25, .25, .25, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	//Draw
	cam.update(delta, window);
	Gizmos::draw(cam.getProjectionView());

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void RenderingGeometry::Shutdown()
{
	Gizmos::destroy();

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
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_IBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (cols - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete auiIndices;
	delete aoVertices;
}