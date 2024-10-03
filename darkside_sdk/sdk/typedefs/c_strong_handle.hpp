#pragma once

template <class T>
struct resource_binding {
	T* m_data;
	unsigned long long m_unk;
	unsigned long long m_unk2;
	volatile unsigned int* m_ref_count;
};

template <class T>
class c_strong_handle {
public:
	operator T* ( ) {

		if ( !m_binding )
			return nullptr;

		return m_binding->m_data;
	}
	T* operator->( ) {

		if ( !m_binding )
			return nullptr;

		return m_binding->m_data;
	}
public:
	const resource_binding<T>* m_binding;
};

