#include "MyApp.h"

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
	MakeDisc();	//Generates disc

	//Shaders
	const char* vsSource;						//Vertex Shader
	std::string vs = GetShader("shader.vert");	//Get shader in the form of a string
	vsSource = vs.c_str();						//Makes string into a const char array.

	const char* fsSource;		//Fragment Shader

	std::string fs = GetShader("frag.frag");
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
		Gizmos::clear();
		return true;
	}
	return false;
}

void RenderingGeometry::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Get the info
	glUseProgram(m_programID);
	
	//Sending the Matrix here
	unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	unsigned int timeHandle = glGetUniformLocation(m_programID, "Time");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(cam.getProjectionView()));
	glUniform1f(timeHandle, current);
	//Draw
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
	
	cam.update(delta, window);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void RenderingGeometry::Shutdown()
{
	//Data Now needs to be cleared out
	glDeleteProgram(m_programID);		//Delete the Program
	glDeleteVertexArrays(1, &m_VAO);	//Clear vertex Arraies
	glDeleteBuffers(1, &m_VBO);			//Clear Buffers
	glDeleteBuffers(1, &m_IBO);
	Gizmos::destroy();

	glfwDestroyWindow(window);			//Terminate window
	glfwTerminate();
}

void RenderingGeometry::MakePlane()
{
	//Create Vertex Points
	Vertex vertices[4];
	unsigned int indices[4] = { 0,1,2,3 };

	indexCount = 4;

	vertices[0].position = vec4(-5, 0, -5, 1);
	vertices[1].position = vec4(5, 0, -5, 1);
	vertices[2].position = vec4(-5, 0, 5, 1);
	vertices[3].position = vec4(5, 0, 5, 1);

	vertices[0].color = vec4(1, 0, 0, 1);
	vertices[1].color = vec4(0, 1, 0, 1);
	vertices[2].color = vec4(0, 0, 1, 1);
	vertices[3].color = vec4(1, 1, 1, 1);

	//Create the Data for OpenGL to look at

	//Generate buffers
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	//Generate Vertex Array Object. VAO
	glGenVertexArrays(1, &m_VAO);

	//Changes will be put on this guy
	glBindVertexArray(m_VAO);

	//Set the Vertex Buffer's data
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//Indeies data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
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

	indexCount = 17;

	vertices[0].position = vec4(-2, 0, -2, 1);
	vertices[1].position = vec4(2, 0, -2, 1);
	vertices[2].position = vec4(-2, 0, 2, 1);
	vertices[3].position = vec4(2, 0, 2, 1);
	vertices[4].position = vec4(-2, 4, -2, 1);
	vertices[5].position = vec4(2, 4, -2, 1);
	vertices[6].position = vec4(-2, 4, 2, 1);
	vertices[7].position = vec4(2, 4, 2, 1);

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
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	//Generate Vertex Array Object. VAO
	glGenVertexArrays(1, &m_VAO);

	//Changes will be put on this guy
	glBindVertexArray(m_VAO);

	//Set the Vertex Buffer's data
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//Indeies data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
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

void RenderingGeometry::MakeDisc()
{
	const int radius = 5;
	const int verts = 100;						//Number of oustide verts
	Vertex vertices[verts + 1];					//+1 accounts for center we will set static
	unsigned int indices[(verts * 3) + 3];		//off set by 3. 
	indexCount = (verts * 3) + 3;
	const float PI = 3.14159265;				//My constant for PI

	//Generate verts
	vertices[0].position = vec4(0, 0, 0, 1);	//Static set center
	vertices[0].color = vec4(1,1,0,1);
	
	for (int i = 1; i <= verts; i++) {	//Calculate all outside verticies
		vertices[i].position = vec4 (radius * std::cos(i * ((PI * 2)/ verts)), 0 , radius * std::sin(i * ((PI * 2) / verts)), 1);
		vertices[i].color = vec4(1, 1, 0, 1);
	}

	int num = 1;	//Tells what vert will be filled into the slot
	//Gen Indicies	 Pattern that seem to work on an earlier version: 0,1,2,0,2,3,0,3,4,0,4,1
	for (int i = 0; i < indexCount; i+=3, num++) {	//Increace by three for three vertexes for each triangle. Set three indicies at once per loop
		if (num > verts) {	
			indices[i] = 0;
			indices[i + 1] = num - 1;
			indices[i + 2] = 1;
		}

		else {
			indices[i] = 0;
			indices[i + 1] = num;
			indices[i + 2] = num + 1;
		}
	}

	//Create the Data for OpenGL to look at
	//Generate buffers
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	//Generate Vertex Array Object. VAO
	glGenVertexArrays(1, &m_VAO);

	//Changes will be put on this guy
	glBindVertexArray(m_VAO);

	//Set the Vertex Buffer's data
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (verts + 1) * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//Indeies data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indexCount * sizeof(unsigned int)), indices, GL_STATIC_DRAW);

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
