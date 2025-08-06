#ifndef CRK_GFX_IMPL_OPENGL_GENERICMODEL_H
#define CRK_GFX_IMPL_OPENGL_GENERICMODEL_H

#include "../../types.hh"

#include <crk/io/front.h>
#include <crk/gfx/front.hh>
#include <crk/res/types.hh>

using namespace crk::gfx;

class GenericModel : IModel
{
	ModelData data;
	std::vector<crk::Ref<IMaterial>> materials;
public:
	static IModel *FromModelData(IBaseRenderer *renderer, const std::string &name, const std::string &path)
	{
		if (!(name == std::string("generic")))
			return nullptr;
		GenericModel *model = new GenericModel;
		auto fileData = crk::io::GetRealDisk()->ReadFile(path.c_str());
		if (fileData.empty())
			return nullptr;
		model->data = crk::gfx::ModelDataCreateFromGLTF(fileData.data(), fileData.size());
		for(int i = 0; i < model->data.materials.size(); i++)
		{
			crk::Ref<IMaterial> mat = renderer->LoadMaterial(model->data.materials[i].name.c_str());
			model->materials.push_back(mat);
		}
		return model;
	}

	void Render()
	{
		
	}
};

#endif