#pragma once
#include <vector>
#include "BufferLayout.hpp"

namespace msen {

	class VertexBuffer {
	public:
		enum class EUsage {
			Static,
			Dynamic,
			Stream
		};
		VertexBuffer(const void* data, const size_t size, BufferLayout buffer_latyout, const EUsage usage = VertexBuffer::EUsage::Static);
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer& operator=(VertexBuffer&& vertexBuffer) noexcept;
		VertexBuffer(VertexBuffer&& vertexBuffer) noexcept;

		void bind() const;
		static void unbind();

		const BufferLayout& get_layout() const { return m_buffer_layout; }

	private:
		unsigned int m_id = 0;
		BufferLayout m_buffer_layout;

	};



}