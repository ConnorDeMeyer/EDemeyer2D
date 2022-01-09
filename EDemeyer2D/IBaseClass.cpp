#include "IBaseClass.h"

IBaseClass::IBaseClass()
{
	m_pObjectReference = std::shared_ptr<IBaseClass>(this, [](IBaseClass* o) {});
}

IBaseClass::~IBaseClass()
{}

void IBaseClass::Draw(const FVector2& windowSize) const
{}

void IBaseClass::FixedUpdate()
{}

void IBaseClass::Update(float deltaTime)
{}

void IBaseClass::LateUpdate()
{}

void IBaseClass::BeginPlay()
{}

void IBaseClass::OnDestroy()
{}

void IBaseClass::Destroy()
{
	m_bDestroy = true;
}

void IBaseClass::CancelDestroy()
{
	m_bDestroy = false;
}

void IBaseClass::SetDrawingOrder(UINT32 drawingOrder)
{
	m_DrawingOrder = drawingOrder;
}

void IBaseClass::SetVisibility(bool bVisible)
{
	m_bVisible = bVisible;
}

void IBaseClass::SetActive(bool bActive)
{
	m_bActive = bActive;
}

std::weak_ptr<IBaseClass> IBaseClass::GetWeakReference() const
{
	return std::weak_ptr<IBaseClass>(m_pObjectReference);
}
