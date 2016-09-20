#include "MyApp.h"
#define _USE_MATH_DEFINES
#include <math.h>

//http://www.learnopengles.com/tag/triangle-strips/

RenderingGeometry::RenderingGeometry()
{
	glfwInit();

	window = glfwCreateWindow(1080, 720, "Window", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	cam.setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	cam.setPerspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	cam.setSpeed(10);

	Gizmos::create();

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST);	//Enables the Depth Buffer
	
}

bool RenderingGeometry::Start()
{
	MakeShpere(1, 20, 20);	
	MakeCube();
	MakePlane();

	//Shaders
	const char* vsSource;						//Vertex Shader
	std::string vs = GetShader("shader.glsl");	//Get shader in the form of a string
	vsSource = vs.c_str();						//Makes string into a const char array.

	const char* fsSource;		//Fragment Shader
	std::string fs = GetShader("frag.glsl");
	fsSource = fs.c_str();

	//Compiles Shaders
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
		return true;
	}
	return false;
}

void RenderingGeometry::Draw()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Get the info
	glUseProgram(m_programID);
	
	//Sending the Matrix here
	unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(cam.getProjectionView()));
	//Draw
	DrawPlane();
	DrawCube();
	DrawSphere();
	
	cam.update(delta, window);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void RenderingGeometry::Shutdown()
{
	//Data Now needs to be cleared out
	glDeleteProgram(m_programID);		//Delete the Program
	ClearPlaneBuffers();
	ClearCubeBuffers();
	ClearSphereBuffers();
	Gizmos::destroy();

	glfwDestroyWindow(window);			//Terminate window
	glfwTerminate();
}

void RenderingGeometry::MakePlane()
{
	//Create Vertex Points
	Vertex vertices[4];
	unsigned int indices[4] = { 0,1,2,3 };

	planeindexCount = 4;

	vertices[0].position = vec4(-5, -2, -5, 1);
	vertices[1].position = vec4(5, -2, -5, 1);
	vertices[2].position = vec4(-5, -2, 5, 1);
	vertices[3].position = vec4(5, -2, 5, 1);

	vertices[0].color = vec4(1, 0, 0, 1);
	vertices[1].color = vec4(0, 1, 0, 1);
	vertices[2].color = vec4(0, 0, 1, 1);
	vertices[3].color = vec4(1, 1, 1, 1);

	//Create the Data for OpenGL to look at

	//Generate buffers
	glGenBuffers(1, &planeVBO);
	glGenBuffers(1, &planeIBO);

	//Generate Vertex Array Object. VAO
	glGenVertexArrays(1, &planeVAO);

	//Changes will be put on this guy
	glBindVertexArray(planeVAO);

	//Set the Vertex Buffer's data
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//Indeies data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	//Position of Verteies
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//Colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

std::string RenderingGeometry::GetShader(std::string text)
{
	std::string line;
	std::string shader;
	std::ifstream file(text);
	if (file.is_open())
	{
		while (std::getline(file, line)) {
			shader += line + "\n";
		}
		file.close();
	}

	return shader;
}

void RenderingGeometry::MakeCube()
{
	//Create Vertex Points
	Vertex vertices[8];
	unsigned int indices[17] = { 0,1,2,3,6,7,4,5,0,1,5,3,7,6,4,2,0};		//http://www.learnopengles.com/tag/triangle-strips/

	cubeindexCount = 17;

	vertices[0].position = vec4(-2, 2, -2, 1);
	vertices[1].position = vec4(2, 2, -2, 1);
	vertices[2].position = vec4(-2, 2, 2, 1);
	vertices[3].position = vec4(2, 2, 2, 1);
	vertices[4].position = vec4(-2, 6, -2, 1);
	vertices[5].position = vec4(2, 6, -2, 1);
	vertices[6].position = vec4(-2, 6, 2, 1);
	vertices[7].position = vec4(2, 6, 2, 1);

	vertices[0].color = vec4(1, 0, 0, 1);
	vertices[1].color = vec4(0, 1, 0, 1);
	vertices[2].color = vec4(0, 0, 1, 1);
	vertices[3].color = vec4(.5, .5, .5, 1);
	vertices[4].color = vec4(1, 1, 0, 1);
	vertices[5].color = vec4(0, 1, 1, 1);
	vertices[6].color = vec4(1, 0, 1, 1);
	vertices[7].color = vec4(.35, .35, .35, 1);

	//Create the Data for OpenGL to look at

	//Generate buffers
	glGenBuffers(1, &cubeVBO);
	glGenBuffers(1, &cubeIBO);

	//Generate Vertex Array Object. VAO
	glGenVertexArrays(1, &cubeVAO);

	//Changes will be put on this guy
	glBindVertexArray(cubeVAO);

	//Set the Vertex Buffer's data
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//Indeies data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 17 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	//Position of Verteies
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//Colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RenderingGeometry::MakeShpere(const int radius, const unsigned int verts, const unsigned int halfCircles)
{
	const unsigned int size = (verts) * (halfCircles);

	Vertex* vertices = new Vertex[size];
	unsigned int* indices;

	//vertices = GenHalfCircleVertexes(verts, radius);
	Vertex* halfCircleVerts = GenHalfCircleVertexes(verts, radius);			//Generate Half Circle
	vertices = GenSphereVerts(verts, halfCircles, halfCircleVerts);			//Generate Sphere Verticies

	indices = GenSphereIndicies(verts, halfCircles);

	//Create the Data for OpenGL to look at
	//Generate buffers
	glGenBuffers(1, &sphereVBO);
	glGenBuffers(1, &sphereIBO);

	//Generate Vertex Array Object. VAO
	glGenVertexArrays(1, &sphereVAO);

	//Changes will be put on this guy
	glBindVertexArray(sphereVAO);

	//Set the Vertex Buffer's data
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//Indeies data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sphereindexCount * sizeof(unsigned int)), indices, GL_STATIC_DRAW);

	//Position of Verteies
	glEnableVertexAttribArray(0);	//pos of vertex in shader
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//Colors
	glEnableVertexAttribArray(1);	//color in vertex shader
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Vertex* RenderingGeometry::GenHalfCircleVertexes(unsigned int p, const int &rad)
{
	Vertex* verts = new Vertex[p];

	for (int i = 0; i < p; i++)
	{
		float angle = PI * i / (p - 1);
		verts[i].position = vec4(rad * std::cos(angle), rad * std::sin(angle), 0, 1);
	}
	return verts;
}

Vertex* RenderingGeometry::GenSphereVerts(const unsigned int &sides, const unsigned int &mirid, Vertex *&halfCircle) {
	int cont = 0;
	Vertex* verts = new Vertex[(sides) * (mirid )];

	//Lathe Half sphere.
	for (int i = 0; i < mirid; i++) {
		float phi = 2.f * PI * (float)i / (float)(mirid);
		for (int j = 0; j < sides; j++, cont++) {
			float x = halfCircle[j].position.x;
			float y = halfCircle[j].position.y * std::cos(phi) - halfCircle[j].position.z * std::sin(phi);
			float z = halfCircle[j].position.z * std::cos(phi) + halfCircle[j].position.y * std::sin(phi);

			verts[cont].position = vec4(x, y, z, 1);
			verts[cont].color = vec4(0,0,.5,1);
		}
	}
	return verts;
}

unsigned int * RenderingGeometry::GenSphereIndicies(const unsigned int &verts, const unsigned int &mird)
{
	//2     5   8   11  14  17
	//1     4   7   10  13  16
	//0     3   6   9   12  15 

	unsigned int* index = new unsigned int[2 * (verts * (mird + 1))];	//put info in my own data structure
	sphereindexCount = 2 * (verts * (mird + 1));

	for (unsigned int i = 0; i < mird; i++)	//Get the info
	{
		unsigned int start = i * verts;
		for (int j = 0; j < verts; j++)
		{
			unsigned int botR = ((start + verts + j) % (verts * mird));
			unsigned int botL = ((start + j) % (verts * mird));
			indicesSTD.push_back(botL);
			indicesSTD.push_back(botR);
		}
		indicesSTD.push_back(0xFFFF);
	}

	for (int i = 0; i < indicesSTD.size(); i++) {
		index[i] = indicesSTD[i];
	}
	return index;
}

void RenderingGeometry::ClearPlaneBuffers()
{
	glDeleteVertexArrays(1, &planeVAO);	//Clear vertex Arraies
	glDeleteBuffers(1, &planeVBO);			//Clear Buffers
	glDeleteBuffers(1, &planeIBO);
}

void RenderingGeometry::ClearCubeBuffers()
{
	glDeleteVertexArrays(1, &cubeVAO);	//Clear vertex Arraies
	glDeleteBuffers(1, &cubeVBO);			//Clear Buffers
	glDeleteBuffers(1, &cubeIBO);
}

void RenderingGeometry::ClearSphereBuffers()
{
	glDeleteVertexArrays(1, &sphereVAO);	//Clear vertex Arraies
	glDeleteBuffers(1, &sphereVBO);			//Clear Buffers
	glDeleteBuffers(1, &sphereIBO);
}

void RenderingGeometry::DrawPlane()
{
	glBindVertexArray(planeVAO);
	glDrawElements(GL_TRIANGLE_STRIP, planeindexCount, GL_UNSIGNED_INT, 0);
}

void RenderingGeometry::DrawCube()
{
	glBindVertexArray(cubeVAO);
	glDrawElements(GL_TRIANGLE_STRIP, cubeindexCount, GL_UNSIGNED_INT, 0);
}

void RenderingGeometry::DrawSphere()
{
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);

	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, sphereindexCount, GL_UNSIGNED_INT, 0);
}
