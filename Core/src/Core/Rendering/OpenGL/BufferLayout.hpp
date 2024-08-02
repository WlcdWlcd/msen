#pragma once
#include "BufferElement.hpp"


namespace msen {
	class BufferLayout {
	public:
		BufferLayout(std::initializer_list<BufferElement> elements) :
			m_elements(std::move(elements))
		{
			size_t offset = 0;
			m_stride = 0;
			for (auto& element : m_elements) {
				element.offset = offset;
				offset += element.size;
				m_stride += element.size;
			}
		}

		const std::vector<BufferElement>& get_elements()const { return m_elements; }
		size_t get_stride() const { return m_stride; }
	private:
		std::vector<BufferElement> m_elements;
		size_t m_stride = 0;
	};


}