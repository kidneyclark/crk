#include "../front.hh"

// opengl
#include "opengl/renderer.hh"

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>

namespace crk::gfx
{

ModelData ModelDataCreateFromGLTF(const void *stream, size_t stream_size)
{
	cgltf_data *gltfData{nullptr};
	cgltf_options options;
	memset(&options, 0, sizeof(cgltf_options));

	auto res = cgltf_parse(&options, stream, stream_size, &gltfData);
	if (res != cgltf_result_success)
		return ModelData{};

	ModelData data;
	data.materials.resize(gltfData->materials_count);
	for (int i = 0; i < gltfData->materials_count; i++)
	{
		data.materials[i] = gltfData->materials[i].name;
	}

	data.buffers.resize(gltfData->buffers_count);
	for (int i = 0; i < gltfData->buffers_count; i++)
	{
		auto &buffer = gltfData->buffers[i];
		data.buffers[i].resize(buffer.size);
		assert((buffer.data != nullptr) && "Invalid buffer data!");
		memcpy(data.buffers[i].data(), buffer.data, buffer.size);
	}

	data.bufferViews.resize(gltfData->buffer_views_count);
	for (int i = 0; i < gltfData->buffer_views_count; i++)
	{
		auto &bufferView = gltfData->buffer_views[i];
		auto &myBufferView = data.bufferViews[i];
		int j;
		for (j = 0; j < gltfData->buffers_count &&
		            bufferView.buffer != (gltfData->buffers + j);
		     j++)
			;
		myBufferView.buffer = data.buffers[j].data();
		myBufferView.byteLength = bufferView.size;
		myBufferView.byteOffset = bufferView.offset;
		myBufferView.byteStride = bufferView.stride;
		switch (bufferView.type)
		{
		case cgltf_buffer_view_type_indices:
			myBufferView.type = ModelData::BufferViewType::Indices;
			break;
		case cgltf_buffer_view_type_vertices:
			myBufferView.type = ModelData::BufferViewType::Vertices;
			break;
		default:
			assert(false);
		}
	}

	data.accessors.resize(gltfData->accessors_count);
	for (int i = 0; i < gltfData->accessors_count; i++)
	{
		auto &accessor = gltfData->accessors[i];
		auto &myAccessor = data.accessors[i];
		int j;
		for (j = 0; j < gltfData->buffer_views_count &&
		            accessor.buffer_view != (gltfData->buffer_views + j);
		     j++)
			;
		myAccessor.bufferView = &data.bufferViews[j];
		myAccessor.byteOffset = accessor.offset;
		myAccessor.count = accessor.count;
		switch (accessor.type)
		{
		case cgltf_type_scalar:
			myAccessor.type = ModelData::ComponentType::Scalar;
			break;
		case cgltf_type_vec2:
			myAccessor.type = ModelData::ComponentType::Vec2;
			break;
		case cgltf_type_vec3:
			myAccessor.type = ModelData::ComponentType::Vec3;
			break;
		case cgltf_type_vec4:
			myAccessor.type = ModelData::ComponentType::Vec4;
			break;
		case cgltf_type_mat2:
			myAccessor.type = ModelData::ComponentType::Mat2;
			break;
		case cgltf_type_mat3:
			myAccessor.type = ModelData::ComponentType::Mat3;
			break;
		case cgltf_type_mat4:
			myAccessor.type = ModelData::ComponentType::Mat4;
			break;
		default:
			assert(false);
		}
	}

	data.meshes.resize(gltfData->meshes_count);
	for (int i = 0; i < gltfData->meshes_count; i++)
	{
		auto &mesh = gltfData->meshes[i];
		auto &myMesh = data.meshes[i];
		myMesh.primitives.resize(mesh.primitives_count);
		for (int j = 0; j < mesh.primitives_count; j++)
		{
			auto &primitive = mesh.primitives[j];
			auto &myPrimitive = myMesh.primitives[j];
			myPrimitive.attributes.resize(primitive.attributes_count);
			for (int k = 0; k < primitive.attributes_count; k++)
			{
				auto &attrib = primitive.attributes[k];
				auto &myAttrib = myPrimitive.attributes[k];
				myAttrib.name = std::string(attrib.name);
				int l;
				for (l = 0; l < gltfData->accessors_count &&
				            attrib.data != (gltfData->accessors + l);
				     l++)
					;
				myAttrib.accessor = &data.accessors[l];
			}
			int l;
			for (l = 0; l < gltfData->accessors_count &&
			            primitive.indices != (gltfData->accessors + l);
			     l++)
				;
			myPrimitive.indices = &data.accessors[l];
			for (l = 0; l < gltfData->materials_count &&
			            strcmp(primitive.material->name, gltfData->materials[l].name) != 0;
			     l++)
				;
			myPrimitive.material = &data.materials[l];
			switch (primitive.type)
			{
			case cgltf_primitive_type_points:
				myPrimitive.type = ModelData::PrimitiveType::Points;
				break;
			case cgltf_primitive_type_lines:
				myPrimitive.type = ModelData::PrimitiveType::Lines;
				break;
			case cgltf_primitive_type_line_loop:
				myPrimitive.type = ModelData::PrimitiveType::LineLoop;
				break;
			case cgltf_primitive_type_line_strip:
				myPrimitive.type = ModelData::PrimitiveType::LineStrip;
				break;
			case cgltf_primitive_type_triangles:
				myPrimitive.type = ModelData::PrimitiveType::Triangles;
				break;
			case cgltf_primitive_type_triangle_strip:
				myPrimitive.type = ModelData::PrimitiveType::TriangleStrip;
				break;
			case cgltf_primitive_type_triangle_fan:
				myPrimitive.type = ModelData::PrimitiveType::TriangleFan;
				break;
			default:
				assert(false);
			}
		}
	}

	cgltf_free(gltfData);

	return data;
}

void RendererCreate(IBaseRenderer **renderer, RendererSettings *settings)
{
	*renderer = new GLRenderer(settings);
}

void RendererRelease(IBaseRenderer **renderer)
{
	delete *renderer;
}

} // namespace crk::gfx