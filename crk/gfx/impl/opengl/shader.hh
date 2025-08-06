#ifndef CRK_GFX_IMPL_OPENGL_SHADER_H
#define CRK_GFX_IMPL_OPENGL_SHADER_H

#include <cassert>
#include <crk/gfx/front.hh>
#include <glad/glad.h>
#include <iostream>

using namespace crk::gfx;

class GLShader : public IShader
{
public:
	struct Uniform
	{
		int index{-1};
		int size;
		GLenum type;
		std::string name;
	};

	struct Attribute
	{
		int index{-1};
		int size;
		GLenum type;
		std::string name;
	};

private:
	std::string m_Name;
	GLuint m_ProgramID{0};
	bool m_Linked{false};
	std::vector<Uniform> uniforms;
	std::vector<Attribute> attributes;
	std::map<std::string, Uniform> uniformMap;
	std::map<std::string, Attribute> attribMap;

public:
	GLShader(const std::string &name)
	    : m_Name(name)
	{
		m_ProgramID = glCreateProgram();
	}

	void Use()
	{
		assert((m_Linked) && "Using non-linked shader program!");
		glUseProgram(m_ProgramID);
	}

	void Attach(GLenum type, const void *data, size_t data_length)
	{
		GLuint id = glCreateShader(type);

		GLint dl = data_length;
		glShaderSource(id, 1, (const GLchar **)&data, &dl);
		glCompileShader(id);

		GLint status = GL_FALSE;
		glGetShaderiv(id, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint infoLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLength);
			std::vector<char> infoLog(infoLength);
			glGetShaderInfoLog(id, infoLength, &infoLength, infoLog.data());

			std::string stringLog(infoLog.data(), infoLength);
			std::cerr << "SHADER COMPILATION ERROR: " << stringLog << std::endl;

			glDeleteShader(id);
			return;
		}

		glAttachShader(m_ProgramID, id);

		glDeleteShader(id);
	}

	void Link()
	{
		assert((!m_Linked) && "Linking already linked program!");
		glLinkProgram(m_ProgramID);

		GLint status = GL_FALSE;
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint infoLength = 0;
			glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &infoLength);
			std::vector<char> infoLog(infoLength);
			glGetProgramInfoLog(m_ProgramID, infoLength, &infoLength,
			                    infoLog.data());

			std::string stringLog(infoLog.data(), infoLength);
			std::cerr << "PROGRAM LINKING ERROR: " << stringLog << std::endl;

			glDeleteProgram(m_ProgramID);
			m_ProgramID = glCreateProgram();
			return;
		}

		m_Linked = true;

		uniforms = GetActiveUniforms();
		attributes = GetActiveAttributes();
		std::for_each(uniforms.begin(), uniforms.end(),
		              [&](const GLShader::Uniform &u) {
			              uniformMap.emplace(std::pair{u.name, u});
		              });
		std::for_each(attributes.begin(), attributes.end(),
		              [&](const GLShader::Attribute &u) {
			              attribMap.emplace(std::pair{u.name, u});
		              });
	}

	~GLShader()
	{
		glDeleteProgram(m_ProgramID);
	}

	const char *GetName() const override
	{
		return m_Name.c_str();
	}

	const Uniform &GetUniform(const std::string &name)
	{
		return uniformMap[name];
	}

	const Attribute &GetAttribute(const std::string &name)
	{
		return attribMap[name];
	}

	std::vector<Uniform> GetActiveUniforms() const
	{
		GLint uniCount = 0, uniNameLength = 0;
		glGetProgramiv(m_ProgramID, GL_ACTIVE_UNIFORMS, &uniCount);
		glGetProgramiv(m_ProgramID, GL_ACTIVE_UNIFORM_MAX_LENGTH,
		               &uniNameLength);
		std::string uniName;
		uniName.resize(uniNameLength);
		GLsizei realUniNameLength = 0;
		GLint uniSize = 0;
		GLenum uniType = 0;

		std::vector<Uniform> uniforms(uniCount);

		for (int i = 0; i < uniCount; i++)
		{
			glGetActiveUniform(m_ProgramID, i, uniNameLength,
			                   &realUniNameLength, &uniSize, &uniType,
			                   uniName.data());
			uniforms[i] = {.index = i,
			               .size = uniSize,
			               .type = uniType,
			               .name = uniName};
		}
		return uniforms;
	}

	std::vector<Attribute> GetActiveAttributes() const
	{
		GLint attrCount = 0, attrNameLength = 0;
		glGetProgramiv(m_ProgramID, GL_ACTIVE_ATTRIBUTES, &attrCount);
		glGetProgramiv(m_ProgramID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
		               &attrNameLength);
		std::string attrName;
		attrName.resize(attrNameLength);
		GLsizei realattrNameLength = 0;
		GLint attrSize = 0;
		GLenum attrType = 0;

		std::vector<Attribute> attributes(attrCount);

		for (int i = 0; i < attrCount; i++)
		{
			glGetActiveAttrib(m_ProgramID, i, attrNameLength,
			                  &realattrNameLength, &attrSize, &attrType,
			                  attrName.data());
			attributes[i] = {.index = i,
			                 .size = attrSize,
			                 .type = attrType,
			                 .name = attrName};
		}
		return attributes;
	}
};

#endif