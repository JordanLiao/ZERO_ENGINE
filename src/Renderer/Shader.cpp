
#include "shader.h"

Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath, glm::mat4* projmat, glm::mat4* viewMat) {
	proj = projmat;
	view = viewMat;
	//pre assign the shader program id to be 0 for later error checking
	//it should not be zero if shader is compiled and attached correctly
	programID = 0;

	// Create the vertex shader and fragment shader.
	GLuint vertexShaderID = LoadSingleShader(vertexFilePath, vertex);
	GLuint fragmentShaderID = LoadSingleShader(fragmentFilePath, fragment);

	// Check both shaders.
	if (vertexShaderID == 0 || fragmentShaderID == 0) return;

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Link the program.
	std::cout << "Linking program" << std::endl;
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Check the program.
	glGetProgramiv(programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(programID, InfoLogLength, NULL, ProgramErrorMessage.data());
		std::string msg(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
		std::cerr << msg << std::endl;
		glDeleteProgram(programID);
		return;
	}
	else
	{
		std::cout << "Successfully linked program!\n" << std::endl;;
	}

	// Detach and delete the shaders as they are no longer needed.
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

Shader::~Shader()
{
	if(programID > 0)
		glDeleteProgram(programID);
}

GLuint Shader::LoadSingleShader(const char* shaderFilePath, ShaderType type)
{
	// Create a shader id.
	GLuint shaderID = 0;
	if (type == vertex)
		shaderID = glCreateShader(GL_VERTEX_SHADER);
	else if (type == fragment)
		shaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Try to read shader codes from the shader file.
	std::string shaderCode;
	std::ifstream shaderStream(shaderFilePath, std::ios::in);
	if (shaderStream.is_open())
	{
		std::string Line = "";
		while (getline(shaderStream, Line))
			shaderCode += "\n" + Line;
		shaderStream.close();
	}
	else
	{
		std::cerr << "Impossible to open " << shaderFilePath << ". "
			<< "Check to make sure the file exists and you passed in the "
			<< "right filepath!"
			<< std::endl;
		return 0;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Shader.
	std::cerr << "Compiling shader: " << shaderFilePath << std::endl;
	char const* sourcePointer = shaderCode.c_str();
	glShaderSource(shaderID, 1, &sourcePointer, NULL);
	glCompileShader(shaderID);

	// Check Shader.
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> shaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(shaderID, InfoLogLength, NULL, shaderErrorMessage.data());
		std::string msg(shaderErrorMessage.begin(), shaderErrorMessage.end());
		std::cerr << msg << std::endl;
		return 0;
	}
	else
	{
		if (type == vertex)
			std::cout << "Successfully compiled vertex shader!" << std::endl;
		else if (type == fragment)
			std::cout << "Successfully compiled fragment shader!" << std::endl;
	}

	return shaderID;
}

/*
	get the uniform location from cache uniformMap,
	or find the location and cache it in uniformMap.
*/

GLint Shader::getUniformLocation(const std::string& uName)
{
	if (uniformMap.find(uName) != uniformMap.end()) {
		return uniformMap[uName];
	}
	GLint location = glGetUniformLocation(programID, uName.c_str());
	uniformMap[uName] = location;
	return location;
}

GLuint Shader::getId() const
{
	return programID;
}

bool Shader::setUniformMat4(const std::string& uName, glm::mat4 uValue)
{
	glcheck(glUniformMatrix4fv(getUniformLocation(uName), 1, false, glm::value_ptr(uValue)));
	return true;
}

bool Shader::setUniformVec3(const std::string& uName, glm::vec3 uValue)
{
	glcheck(glUniform3fv(getUniformLocation(uName), 1, glm::value_ptr(uValue)));
	return true;
}

bool Shader::setUniform1I(const std::string& uName, GLint uValue)
{
	glcheck(glUniform1i(getUniformLocation(uName), uValue));
	return true;
}

bool Shader::setUniorm1F(const std::string& uName, GLfloat uValue)
{
	glcheck(glUniform1f(getUniformLocation(uName), uValue));
	return true;
}

void Shader::bind() {
	glcheck(glUseProgram(programID));
	glcheck(glUniformMatrix4fv(getUniformLocation("view"), 1, false, glm::value_ptr(*view)));
	glcheck(glUniformMatrix4fv(getUniformLocation("projection"), 1, false, glm::value_ptr(*proj)));
}

void Shader::unbind() const 
{
	glcheck(glUseProgram(0));
}