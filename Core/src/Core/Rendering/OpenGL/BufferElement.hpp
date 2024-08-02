#pragma once
#include "ShaderDataType.hpp"
#include <vector>

namespace msen{
	struct BufferElement {
		ShaderDataType::DataTypes type;
		uint32_t component_type;
		size_t components_count;
		size_t size;
		size_t offset;

		BufferElement(const ShaderDataType::DataTypes type);
	};



}