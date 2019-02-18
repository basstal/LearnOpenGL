#include <shader.h>

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath, const GLchar * geometryPath)
{
    std::string vertexCode;
    std::string fragmentCode;
	std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
	std::ifstream gShaderFile;

    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
		if (geometryPath != "")
			gShaderFile.open(geometryPath);

        std::stringstream vShaderStram, fShaderStram, gShaderStram;
        vShaderStram << vShaderFile.rdbuf();
        fShaderStram << fShaderFile.rdbuf();
		if (geometryPath != "")
			gShaderStram << gShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();
		if (geometryPath != "")
			gShaderFile.close();

        vertexCode = vShaderStram.str();
        fragmentCode = fShaderStram.str();
		if (geometryPath != "")
			geometryCode = gShaderStram.str();
    }
    catch(std::exception e)
    {
        std::cout<< "ERROR::SHADER::FILE NOT SUCCESSFULLY READ"<<std::endl;
    };
	const char * vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();
	const char * gShaderCode = NULL;
	if (!geometryCode.empty())
		gShaderCode = geometryCode.c_str();

    unsigned int vertexShader, fragmentShader, geometryShader;
    int success;
	char infoLog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::VERTEX SHADER::COMPILE FAILED:\n " << infoLog << std::endl;
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::FRAGMENT SHADER::COMPILE FAILED:\n " << infoLog << std::endl;
	}

	if (gShaderCode != NULL)
	{
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &gShaderCode, NULL);
		glCompileShader(geometryShader);
		glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
		if (! success)
		{
			glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
			std::cout << "ERROR::GEOMETRY SHADER::COMPILE FAILED:\n " << infoLog << std::endl;
		}
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	if (gShaderCode != NULL)
		glAttachShader(ID, geometryShader);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM LINK FAILED:\n " << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (gShaderCode != NULL)
		glDeleteShader(geometryShader);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setInt(const std::string &varName, int val) const
{
    glUniform1i(glGetUniformLocation(ID, varName.c_str()), val);
}

void Shader::setBool(const std::string &varName, bool val) const
{
    glUniform1i(glGetUniformLocation(ID, varName.c_str()), (int)val);
}

void Shader::setFloat(const std::string &varName, float val) const
{
    glUniform1f(glGetUniformLocation(ID, varName.c_str()), val);
}

void Shader::setMat4(const std::string &varName, glm::mat4 val) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, varName.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}

void Shader::setVec3(const std::string &varName, glm::vec3 val) const
{
	glUniform3fv(glGetUniformLocation(ID, varName.c_str()), 1, glm::value_ptr(val));
}