#include "IObject.h"
#include "IComponent.h"
#include <memory>
#include "framework.h"

IObject::IObject()
{}


void IObject::SetPosition(const FVector2& pos)
{
	m_Pos = pos;
	for (IComponent* component : m_Components)
	{
		component->SetPosition(pos);
	}
}

void IObject::Update(float deltaTime)
{
	IBaseClass::Update(deltaTime);

	m_Components.sort([](IComponent* p0, IComponent* p1) {return p0->GetDrawingOrder() < p1->GetDrawingOrder(); });

	for (IComponent* component : m_Components)
		if (component->IsActive()) component->Update(deltaTime);
}

void IObject::Draw(const FVector2& windowSize) const
{
	IBaseClass::Draw(windowSize);

	for (IComponent* component : m_Components)
		if (component->IsVisible()) component->Draw(windowSize);
}

void IObject::BeginPlay()
{
	for (IComponent* component : m_Components)
		if (component->IsVisible()) component->BeginPlay();
}

void IObject::Move(const FVector2& pos)
{
	SetPosition(m_Pos + pos);
}

void IObject::DestroyFlaggedComponents()
{
	auto it{ m_Components.begin() };
	while (it != m_Components.end())
	{
		if ((*it)->GetDestroyFlag())
		{
			delete (*it);
			it = m_Components.begin(); // Restart the search to avoid 
		}
		++it;
	}
}

IObject::~IObject()
{
	while (!m_Components.empty())
	{
		auto placeholder = m_Components.front();
		m_Components.pop_front();
		delete placeholder;
	}
}

void IObject::Destroy()
{
	IBaseClass::Destroy();

	for (IComponent* component : m_Components)
		if (component->IsActive()) component->Destroy();
}

void IObject::AddComponent(IComponent* pComponent)
{
	if (pComponent) {
		m_Components.push_front(pComponent);
		m_Children.push_front(pComponent);
		pComponent->SetOwner(this);
	}
}

void IObject::RemoveComponent(IComponent* pComponent)
{
	if (!pComponent) return;

	for (IComponent* component : m_Components)
		if (pComponent == component)
		{
			component->Destroy();
			m_Components.remove(pComponent);
			break;
		}
	m_Children.remove(pComponent);
}