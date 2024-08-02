#include "VertexArray.hpp"
#include "core/Log.hpp"
#include "BufferElement.hpp"

#include <glad/glad.h>

namespace msen {

	VertexArray::VertexArray() {
		glGenVertexArrays(1, &m_id);
	}
	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_id);
	}

	VertexArray& VertexArray::operator=(VertexArray&& vertexArray) noexcept {
		m_id = vertexArray.m_id;
		m_elements_count = vertexArray.m_elements_count;
		vertexArray.m_id = 0;
		vertexArray.m_elements_count = 0;
		return *this;


	}
	VertexArray::VertexArray(VertexArray&& vertexArray) noexcept 
		:m_id(vertexArray.m_id)
		, m_elements_count(vertexArray.m_elements_count)
	{
		vertexArray.m_id = 0;
		vertexArray.m_elements_count = 0;
	}

	void VertexArray::bind() const {
		glBindVertexArray(m_id);
	}
	void VertexArray::unbind() {
		glBindVertexArray(0);
	}

	void VertexArray::add_vertex_buffer(const VertexBuffer& vertex_buffer) {
		bind();
		vertex_buffer.bind();

		for (const BufferElement& current_element : vertex_buffer.get_layout().get_elements()) {
			glEnableVertexAttribArray(m_elements_count);
			glVertexAttribPointer(
				m_elements_count, 
				static_cast<GLint>(current_element.components_count),
				current_element.component_type, 
				GL_FALSE, 
				static_cast<GLsizei>(vertex_buffer.get_layout().get_stride()),
				reinterpret_cast<const void*>(current_element.offset)
			);
			++m_elements_count;
		}

		
	}

	void VertexArray::set_index_buffer(const IndexBuffer& index_buffer) {
		bind();
		index_buffer.bind();

		m_indices_count = index_buffer.get_count();

	}

		
}