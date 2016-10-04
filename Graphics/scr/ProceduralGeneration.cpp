#include "MyApp.h"
#include <stb_image.h>

ProceduralGeneration::ProceduralGeneration()
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

	cam.setLookAt(vec3(60, 60, 60), vec3(0), vec3(0, 1, 0));
	cam.setPerspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	cam.setSpeed(10);

	glEnable(GL_BLEND);
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST);	//Enables the Depth Buffer

}

std::string ProceduralGeneration::GetShader(std::string text)
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

void ProceduralGeneration::MakePlane(const int size)
{
	//Create Vertex Points
	PGVertex* vertices = new PGVertex[size * size];

	int vertsCount = 0;

	//Vertex
	for(int z = 0; z < size; ++z) {
		for (int x = 0; x < size; ++x) {
			vertices[z * size + x].position = vec4(x - size * 0.5f, 0, z - size * 0.5f, 1);	//Setting ceter position
			vertices[z * size + x].uv = vec2(x * (1.f / size), z * (1.f / size));			// setting up UVs
		}
	}

	//Indicies
	planeindexCount = (size - 1) * (size - 1) * 6;
	unsigned int* indices = new unsigned int[planeindexCount];	
	
	unsigned int index = 0;	//The current index
	for (int z = 0; z < (size - 1); ++z) {
		for (int x = 0; x < (size - 1); ++x) {
			//First Triangle
			indices[index++] = z * size + x;
			indices[index++] = (z + 1) * size + x;
			indices[index++] = (z + 1) * size + (x + 1);
			//Second Triangle
			indices[index++] = z * size + x;
			indices[index++] = (z + 1) * size + (x + 1);
			indices[index++] = z * size + (x + 1);
		}
	}

	//Generate buffers
	glGenBuffers(1, &planeVBO);
	glGenBuffers(1, &planeIBO);

	//Generate Vertex Array Object.
	glGenVertexArrays(1, &planeVAO);

	//Changes go here
	glBindVertexArray(planeVAO);

	//Buffer Vertex Data
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, (size * size) * sizeof(PGVertex), vertices, GL_STATIC_DRAW);

	//Buffer Indeies data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, planeindexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	//Position of Verteies
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(PGVertex), 0);

	//UVs
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(PGVertex), (void*)(sizeof(vec4)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

float* ProceduralGeneration::MakePerlinNoise()
{
	int dims = 64;									//Dimitions
	float *perlin_data = new float[dims * dims];	//Data for the Perlin Noise Function
	float scale = (1.0f / dims) * 3;
	int octaves = 6;								//How many passes of extra perlin noise will we do per vertex.

	for (int x = 0; x < 64; ++x)
	{
		for (int y = 0; y < 64; ++y)
		{
			float amplitude = 1.f;
			float persistence = 0.3f;
			perlin_data[y* dims + x] = glm::perlin(vec2(x, y) * scale) * 0.5f + 0.5f;

			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, (float)o);
				float perlin_sample = glm::perlin(vec2((float)x, (float)y) * scale * freq) * 0.5f + 0.5f;
				perlin_data[y * dims + x] += perlin_sample * amplitude;
				amplitude *= persistence;
			}
		}
	}

	return perlin_data;
}

bool ProceduralGeneration::Start()
{
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	unsigned char* data;

	float* perlinData = MakePerlinNoise();

	glGenTextures(1, &perlin_texture);
	glBindTexture(GL_TEXTURE_2D, perlin_texture);
											//64 is length and width of plane in verticies
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 64, 64, 0, GL_RED, GL_FLOAT, perlinData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	data = stbi_load("textures/rock.jpg", &imageWidth, &imageHeight, &imageFormat, STBI_default);
	glGenTextures(1, &rock_texture);
	glBindTexture(GL_TEXTURE_2D, rock_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//Since data isn't being reused we don't have to free it up.

	//Shaders
	const char* vsSource;						//Vertex Shader
	std::string vs = GetShader("PGvs.glsl");	//Get shader in the form of a string
	vsSource = vs.c_str();						//Makes string into a const char array.

	const char* fsSource;						//Fragment Shader
	std::string fs = GetShader("PGfs.glsl");
	fsSource = fs.c_str();

	//Compiles Shaders
	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	m_program = glCreateProgram();
	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragmentShader);
	glLinkProgram(m_program);

	glGetProgramiv(m_program, GL_LINK_STATUS, &success);	//Debugging the shader in case of improper linking
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_program, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	MakePlane(64);	//Making the Plane data

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	return true;
}

bool ProceduralGeneration::Update()
{
	//Calculate delta time
	current = (float)glfwGetTime();
	delta = current - previous;
	previous = current;

	//If window is open
	if (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		return true;
	}
	return false;
}

void ProceduralGeneration::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use our texture program
	glUseProgram(m_program);

	int loc = glGetUniformLocation(m_program, "view_proj");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(cam.getProjectionView()[0][0]));
	
	//Bind Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, perlin_texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, rock_texture);

	//Set Shader
	loc = glGetUniformLocation(m_program, "perlin_texture");
	glUniform1i(loc, 0);

	loc = glGetUniformLocation(m_program, "otherTexture");
	glUniform1i(loc, 1);

	// draw
	glBindVertexArray(planeVAO);
	glDrawElements(GL_TRIANGLES, planeindexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	cam.update(delta, window);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void ProceduralGeneration::Shutdown()
{
	//Data Now needs to be cleared out
	glDeleteProgram(m_program);		//Delete the Program
	glDeleteVertexArrays(1, &planeVAO);	//Clear vertex Arraies
	glDeleteBuffers(1, &planeVBO);			//Clear Buffers
	glDeleteBuffers(1, &planeIBO);

	glfwDestroyWindow(window);			//Terminate window
	glfwTerminate();
}