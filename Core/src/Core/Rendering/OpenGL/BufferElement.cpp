#include "BufferElement.hpp"
#include "ShaderDataType.hpp"
#include "Core/Log.hpp"

#include <glad/glad.h>

namespace msen {
	//to do: move that from here
	constexpr unsigned int ShaderDataType::shader_data_type_to_components_count(const DataTypes type) {
		switch (type) {
		case DataTypes::Float:
		case DataTypes::Int:
			return 1;
		case DataTypes::Float2:
		case DataTypes::Int2:
			return 2;
		case DataTypes::Float3:
		case DataTypes::Int3:
			return 3;
		case DataTypes::Float4:
		case DataTypes::Int4:
			return 4;
		}
		LOG_ERROR("shader_data_type_to_components_count: unknown ShaderDataType");
		return 0;
	}

	constexpr unsigned int ShaderDataType::shader_data_type_size(const DataTypes type) {
		switch (type) {
		case DataTypes::Float:
		case DataTypes::Float2:
		case DataTypes::Float3:
		case DataTypes::Float4:
			return sizeof(GLfloat) * shader_data_type_to_components_count(type);
		case DataTypes::Int:
		case DataTypes::Int2:
		case DataTypes::Int3:
		case DataTypes::Int4:
			return sizeof(GLint) * shader_data_type_to_components_count(type);
		}
		LOG_ERROR("shader_data_type_size: unknown ShaderDataType");
		return 0;
	}

	constexpr unsigned int ShaderDataType::shader_data_type_to_component_type(const DataTypes type) {
		switch (type) {
		case DataTypes::Float:
		case DataTypes::Float2:
		case DataTypes::Float3:
		case DataTypes::Float4:
			return GL_FLOAT;
		case DataTypes::Int:
		case DataTypes::Int2:
		case DataTypes::Int3:
		case DataTypes::Int4:
			return GL_INT;
		}
		LOG_ERROR("shader_data_type_to_component_type: unknown ShaderDataType");
		return GL_FLOAT;
	}

	BufferElement::BufferElement(const ShaderDataType::DataTypes _type)
		: type(_type)
		, component_type(ShaderDataType::shader_data_type_to_component_type(_type))
		, components_count(ShaderDataType::shader_data_type_to_components_count(_type))
		, size(ShaderDataType::shader_data_type_size(_type))
		, offset(0)

	{}

}