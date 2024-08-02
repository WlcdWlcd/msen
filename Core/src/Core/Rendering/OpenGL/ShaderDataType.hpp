#pragma once
//#include "Core/Log.hpp"
//#include "glad/glad.h"

namespace msen {

	class ShaderDataType {
	public:
		enum DataTypes {
			Float,
			Float2,
			Float3,
			Float4,
			Int,
			Int2,
			Int3,
			Int4,
		};
		static constexpr unsigned int ShaderDataType::shader_data_type_to_components_count(const DataTypes type);
		static constexpr unsigned int ShaderDataType::shader_data_type_size(const DataTypes type);
		static constexpr unsigned int ShaderDataType::shader_data_type_to_component_type(const DataTypes type);
	};
}