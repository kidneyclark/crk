#ifndef CRK_GFX_TYPES_H
#define CRK_GFX_TYPES_H

#include "../com/types.h"
#include <crk/res/types.hh>
#include <tuple>
#include <vector>
#include <string>

namespace crk::gfx
{

using Color = std::tuple<f32, f32, f32, f32>;

enum ClearMask
{
	ClearNone = 0,
	ClearColor = 1,
	ClearDepth = 2,
	ClearStencil = 4,
};

struct Material
{
	std::string name;
	std::string surfaceType;
};

struct ModelData
{
	enum class BufferViewType
	{
		None,
		Indices,
		Vertices,
		Max
	};
	enum class ComponentType
	{
		None,
		Scalar,
		Vec2,
		Vec3,
		Vec4,
		Mat2,
		Mat3,
		Mat4,
		Max
	};
	enum class PrimitiveType
	{
		None,
		Points,
		Lines,
		LineLoop,
		LineStrip,
		Triangles,
		TriangleStrip,
		TriangleFan,
		Max
	};
	std::vector<Material> materials;
	std::vector<std::vector<u8>> buffers;
	struct BufferView
	{
		u8 *buffer{nullptr};
		uint64_t byteLength{0};
		uint64_t byteOffset{0};
		uint64_t byteStride{0};
		BufferViewType type{BufferViewType::None};
	};
	std::vector<BufferView> bufferViews;
	struct Accessor
	{
		BufferView *bufferView{nullptr};
		uint64_t byteOffset{0};
		uint64_t count{0};
		ComponentType type{ComponentType::None};
	};
	std::vector<Accessor> accessors;
	struct Attribute
	{
		std::string name;
		Accessor *accessor;
	};
	struct Primitive
	{
		std::vector<Attribute> attributes;
		Accessor *indices;
		Material *material;
		PrimitiveType type{PrimitiveType::None};
	};
	struct Mesh
	{
		std::vector<Primitive> primitives;
	};
	std::vector<Mesh> meshes;
};

struct RendererSettings
{
	void *backendLoadFunc {nullptr};
};

class IShader
{
public:
	virtual ~IShader() = default;
	virtual const char *GetName() const = 0;
};

class IMaterial
{
public:
	virtual ~IMaterial() = default;
	virtual const char *GetName() const = 0;
};

class IModel
{
public:
	virtual ~IModel() = default;
	virtual void Render() = 0;
};

class IBaseRenderer
{
public:
	virtual ~IBaseRenderer() = default;
	virtual void Clear(ClearMask mask) = 0;
	virtual void SetClearColor(Color color) = 0;
	virtual void SetClearDepth(f32 depth) = 0;
	virtual void SetClearStencil(i32 stencil) = 0;
	virtual crk::Ref<IModel> LoadModel(const char *name) = 0;
	virtual crk::Ref<IMaterial> LoadMaterial(const char *name) = 0;
	virtual crk::Ref<IShader> LoadShader(const char *name) = 0;

};

} // namespace crk::gfx

#endif