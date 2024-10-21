#include "PXList.hpp"

template<typename T>
PX::List<T>::List(const PXSize startSize)
{
	PXListInitialize(&_list, sizeof(T), startSize);
}

template<typename T>
PX::List<T>::~List()
{
	PXListRelease(&_list);
}

template<typename T>
void PX::List<T>::Resize(const PXSize entrys)
{
	PXListReserve(&_list, entrys);
}

template<typename T>
bool PX::List<T>::Add(const T& element)
{
	return PXListAdd(&_list, &element);
}

template<typename T>
bool PX::List<T>::Remove(const T& element)
{
	return false;
}