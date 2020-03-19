#pragma once

#include "spvgentwo/Vector.h"
#include "HeapAllocator.h"

namespace spvgentwo
{
	template <class U>
	class HeapVector : public Vector<U>
	{
	public:
		using T = typename stdrep::remove_cv_t<U>;

		using Vector<U>::Vector;

		HeapVector(size_t _size = 0u) : Vector<U>(HeapAllocator::instance(), _size) {}

		// copy from array
		HeapVector(const T* _pData, size_t _size = 0u) : Vector<U>(HeapAllocator::instance(), _pData, _size) {}

		template <size_t Size>
		HeapVector(const T(&_array)[Size]) : Vector<U>(HeapAllocator::instance(), _array) {}

		template <typename ...Args>
		HeapVector(const T& _first, Args&& ... _args) : Vector<U>(HeapAllocator::instance(), _first, stdrep::forward<Args>(_args)...) {}

		virtual ~HeapVector() override = default;
	};
} // !spvgentwo