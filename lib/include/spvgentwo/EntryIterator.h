#pragma once

#include "Entry.h"

namespace spvgentwo
{
	template <class T>
	class EntryIterator
	{
	public:
		EntryIterator(Entry<T>* _pEntry = nullptr) : m_pEntry(_pEntry) {}
		EntryIterator(const EntryIterator<T>& _other) : m_pEntry(_other.m_pEntry) {}

		bool operator==(const EntryIterator<T>& _other) const;
		bool operator!=(const EntryIterator<T>& _other) const;

		bool operator==(nullptr_t) const;
		bool operator!=(nullptr_t) const;

		EntryIterator<T> operator+(const size_t n) const;
		EntryIterator<T> operator-(const size_t n) const;

		// pre
		EntryIterator<T>& operator++();
		EntryIterator<T>& operator--();

		// post
		EntryIterator<T> operator++(int);
		EntryIterator<T> operator--(int);

		T& operator*() { return m_pEntry->inner(); }
		const T& operator*() const { return m_pEntry->inner(); }

		T* operator->() { return m_pEntry->operator->(); }
		const T* operator->() const { return m_pEntry->operator->(); }

		Entry<T>* entry() { return m_pEntry; }
		const Entry<T>* entry()  const { return m_pEntry; }

		operator Entry<T>* () { return m_pEntry; }
		operator const Entry<T>* ()  const { return m_pEntry; }

		//operator bool() const { return m_pEntry != nullptr; }

	private:
		Entry<T>* m_pEntry = nullptr;
	};

	template<class T>
	inline bool EntryIterator<T>::operator==(const EntryIterator<T>& _other) const
	{
		return m_pEntry == _other.m_pEntry;
	}

	template<class T>
	inline bool EntryIterator<T>::operator!=(const EntryIterator<T>& _other) const
	{
		return m_pEntry != _other.m_pEntry;
	}

	template<class T>
	inline bool EntryIterator<T>::operator==(nullptr_t) const
	{
		return m_pEntry == nullptr;
	}

	template<class T>
	inline bool EntryIterator<T>::operator!=(nullptr_t) const
	{
		return m_pEntry != nullptr;
	}

	template<class T>
	inline EntryIterator<T>& EntryIterator<T>::operator++()
	{
		m_pEntry = m_pEntry->next();
		return *this;
	}
	template<class T>
	inline EntryIterator<T>& EntryIterator<T>::operator--()
	{
		m_pEntry = m_pEntry->prev();
		return *this;
	}
	template<class T>
	inline EntryIterator<T> EntryIterator<T>::operator++(int)
	{
		EntryIterator<T> ret(m_pEntry);
		m_pEntry = m_pEntry->next();
		return ret;
	}

	template<class T>
	inline EntryIterator<T> EntryIterator<T>::operator--(int)
	{
		EntryIterator<T> ret(m_pEntry);
		m_pEntry = m_pEntry->prev();
		return ret;
	}

	template<class T>
	EntryIterator<T> EntryIterator<T>::operator+(const size_t n) const
	{
		EntryIterator<T> ret(m_pEntry);
		for (size_t i = 0; i < n && ret != nullptr; i++, ++ret) {}
		return ret;
	}

	template<class T>
	EntryIterator<T> EntryIterator<T>::operator-(const size_t n) const
	{
		EntryIterator<T> ret(m_pEntry);
		for (size_t i = 0; i < n && ret != nullptr; i++, --ret) {}
		return ret;
	}
} // !spvgentwo