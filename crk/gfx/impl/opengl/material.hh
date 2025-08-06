#ifndef CRK_GFX_IMPL_OPENGL_MATERIAL_HH
#define CRK_GFX_IMPL_OPENGL_MATERIAL_HH

#include <memory>
#include <algorithm>
#include "shader.hh"


class GLMaterial : public IMaterial
{
	std::shared_ptr<IShader> m_Program;
	std::string m_Name;
public:
	GLMaterial(const std::string &name, const std::shared_ptr<IShader> &shader)
		: m_Program(shader)
		, m_Name(name)
	{
		auto uniforms = ((GLShader*)m_Program.get())->GetActiveUniforms();
		auto attributes = ((GLShader*)m_Program.get())->GetActiveAttributes();
		std::cout << "Uniforms:\n";
		for (auto &uni : uniforms)
		{
			std::cout << std::format("#{}: {} x {} of {}", uni.index, uni.name, uni.size, uni.type) << std::endl;
		}
		std::cout << "Attributes:\n";
		for (auto &attr : attributes)
		{
			std::cout << std::format("#{}: {} x {} of {}", attr.index, attr.name, attr.size, attr.type) << std::endl;
		}
		std::cout.flush();
	}

	const char *GetName() const override
	{
		return m_Name.c_str();
	}
};

#endif