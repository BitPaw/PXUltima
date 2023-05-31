#include "PXDirectX.hpp"

PX::DirectX::DirectX()
{
	PXDirectXContextConstruct(this);
}

PX::DirectX::~DirectX()
{
	PXDirectXContextDestruct(this);
}

void PX::DirectX::ContextCreate()
{
	PXDirectXContextCreate(this);
}