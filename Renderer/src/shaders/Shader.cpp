#include "Shader.h"

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string path = "src/shaders/";
	std::string fullPathVertex = path + vertexFile;
	std::string fullPathFragment = path + fragmentFile;

	std::string vertexCode = loadShaderFromFile(fullPathVertex.c_str());
	std::string fragmentCode = loadShaderFromFile(fullPathFragment.c_str());

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}