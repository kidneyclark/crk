#ifndef CRK_GFX_IMPL_OPENGL_RENDERER_H
#define CRK_GFX_IMPL_OPENGL_RENDERER_H

#include "../../types.hh"

#include <crk/gfx/front.hh>
#include <crk/io/front.h>
#include <crk/res/types.hh>
#include <crk/std/map.h>
#include <glad/glad.h>

#include <array>
#include <cstddef>
#include <functional>
#include <tuple>
#include <utility>

#include <simdjson.h>

#include "generic_mdl.hh"
#include "material.hh"
#include "shader.hh"

using namespace crk::gfx;

template <auto &&_fn, typename _tupleType>
inline constexpr void fnTupleUnpack(_tupleType &&tuple)
{
	// epic compile time unpacking
	[&]<size_t... _P>(std::index_sequence<_P...>) {
		_fn(std::get<_P>(std::forward<_tupleType>(tuple))...);
	}(std::make_index_sequence<
	    std::tuple_size_v<std::remove_cvref_t<_tupleType>>>{});
}

class GLRenderer : public IBaseRenderer
{
	Color cachedClearColor = {0, 0, 0, 0};
	f32 cachedClearDepth = 1;
	i32 cachedClearStencil = 0;

	crk::RefCache<IShader> shaderCache;
	crk::RefCache<IMaterial> materialCache;
	crk::RefCache<IModel> modelCache;

public:
	GLRenderer(RendererSettings *settings)
	{
		gladLoadGLLoader((GLADloadproc)settings->backendLoadFunc);
	}

	void Clear(ClearMask mask)
	{
		i32 maskGL = 0;
		if (mask & ClearMask::ClearColor)
			maskGL |= GL_COLOR_BUFFER_BIT;
		if (mask & ClearMask::ClearDepth)
			maskGL |= GL_DEPTH_BUFFER_BIT;
		if (mask & ClearMask::ClearStencil)
			maskGL |= GL_STENCIL_BUFFER_BIT;
		glClear(maskGL);
	}

	void SetClearColor(Color color)
	{
		if (color != cachedClearColor)
		{
			cachedClearColor = color;
			fnTupleUnpack<glClearColor>(color);
		}
	}

	void SetClearDepth(f32 depth)
	{
		if (depth != cachedClearDepth)
		{
			cachedClearDepth = depth;
			glClearDepthf(depth);
		}
	}

	void SetClearStencil(i32 stencil)
	{
		if (stencil != cachedClearStencil)
		{
			cachedClearStencil = stencil;
			glClearStencil(stencil);
		}
	}

	crk::Ref<IModel> LoadModel(const char *name)
	{
		if (modelCache.Exists(name))
			return modelCache.Request(name);

		simdjson::dom::parser parser;
		auto json = parser.load("resources/" + std::string(name));
		auto root = json.get_object();

		auto mdlTypeEntry = root["type"];
		if (mdlTypeEntry.is_null() || !mdlTypeEntry.is_string())
			return nullptr;
		std::string_view mdlType = mdlTypeEntry.get_string();

		constexpr std::array mdlLoaders =
		    std::to_array<IModel *(*)(const std::string &name, const std::string &path)>(
		        {GenericModel::FromModelData});

		auto glbPathEntry = root["glb"];
		if (glbPathEntry.is_null() || !glbPathEntry.is_string())
			return nullptr;
		std::string_view glbPath = glbPathEntry.get_string();
		std::string glbPathString(glbPath.begin(), glbPath.end());
		glbPathString = "resources/" + glbPathString;

		using namespace crk::io;
		IDisk *disk = crk::io::GetRealDisk();
		IStream *file = disk->OpenFile(glbPathString.c_str(),
		                               crk::io::IDisk::DISK_OPENREAD);
		if (!file->IsValid())
			return nullptr;
		size_t size = file->GetSize();
		std::vector<u8> rawData(size);
		file->Read(rawData.data(), size, 1);
		disk->CloseFile(file);

		ModelData data =
		    crk::gfx::ModelDataCreateFromGLTF(rawData.data(), rawData.size());

		IModel *model = nullptr;
		std::for_each(mdlLoaders.begin(), mdlLoaders.end(), [&](auto &&loader) {
			auto loaded = loader(data);
			if (loaded)
			{
				assert((model) && "Conflicting model types!");
				// Ideally resolved at compiled time but no
				model = loaded;
			}
		});
		if (!model)
			std::cerr << "Unsupported model type!" << std::endl;

		crk::Ref<IModel> ref = crk::Ref<IModel>(model);
		modelCache.Set(name, ref);

		return ref;
	}

	crk::Ref<IMaterial> LoadMaterial(const char *name)
	{
		if (materialCache.Exists(name))
			return materialCache.Request(name);

		simdjson::dom::parser parser;
		auto json = parser.load("resources/" + std::string(name));
		auto root = json.get_object();

		auto shaderEntry = root["shader"];
		if (shaderEntry.is_null() || !shaderEntry.is_string())
			return nullptr;
		std::string_view shader = shaderEntry.get_string();

		auto dataEntry = root["data"];
		if (dataEntry.is_null() || !dataEntry.is_object())
			return nullptr;

		std::map<std::string, std::string> attribs;

		for (auto d : dataEntry.get_object())
		{
			if (d.value.is_string())
			{
				auto val = d.value.get_string().value();
				attribs[{d.key.begin(), d.key.end()}] = {val.begin(),
				                                         val.end()};
			}
		}

		std::string shaderName(shader);
		crk::Ref<IShader> shaderRes = LoadShader(shaderName.c_str());
		GLMaterial *mat = new GLMaterial(name, shaderRes);

		auto ref = crk::Ref<IMaterial>(mat);
		materialCache.Set(name, ref);

		return ref;
	}

	crk::Ref<IShader> LoadShader(const char *name)
	{
		if (shaderCache.Exists(name))
			return shaderCache.Request(name);

		simdjson::dom::parser parser;
		auto json = parser.load("resources/" + std::string(name));
		auto root = json.get_object();

		auto vertexEntry = root["vertex"];
		if (vertexEntry.is_null() || !vertexEntry.is_string())
			return nullptr;

		auto fragEntry = root["fragment"];
		if (fragEntry.is_null() || !fragEntry.is_string())
			return nullptr;

		std::string_view vertView = vertexEntry.get_string();
		std::string_view fragView = fragEntry.get_string();
		std::string vertPath = "resources/" + std::string(vertView);
		std::string fragPath = "resources/" + std::string(fragView);
		using namespace crk::io;
		IDisk *disk = GetRealDisk();
		std::vector<u8> vertRaw = disk->ReadFile(vertPath.c_str());
		std::vector<u8> fragRaw = disk->ReadFile(fragPath.c_str());

		GLShader *glShader = new GLShader(name);
		glShader->Attach(GL_VERTEX_SHADER, vertRaw.data(), vertRaw.size());
		glShader->Attach(GL_FRAGMENT_SHADER, fragRaw.data(), fragRaw.size());
		glShader->Link();

		auto ret = crk::Ref<IShader>(glShader);
		shaderCache.Set(name, ret);
		return ret;
	}
};

#endif