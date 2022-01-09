#include "IComponent.h"
#include "IObject.h"

IComponent::IComponent()
{

}

IComponent::~IComponent()
{
	RemoveFromObject();
	for (IComponent* component : m_Children)
	{
		delete component;
	}
}

void IComponent::Update(float deltaTime)
{
	for (IComponent* component : m_Children) component->Update(deltaTime);
}

void IComponent::Draw(const FVector2& windowSize) const
{
	for (IComponent* component : m_Children) component->Draw(windowSize);
}

void IComponent::Destroy()
{
	IBaseClass::Destroy();

	for (IComponent* component : m_Children) component->Destroy();
}

void IComponent::SetPosition(const FVector2& pos)
{
	m_Pos = pos + m_LocalPos;
	for (IComponent* component : m_Children) component->SetPosition(pos);
}

void IComponent::SetLocalPosition(const FVector2& pos)
{
	auto difference = pos - m_LocalPos;
	Move(difference);
	for (IComponent* component : m_Children) component->Move(difference);
}

void IComponent::Move(const FVector2& pos)
{
	m_LocalPos += pos;
	m_Pos += pos;
	for (IComponent* component : m_Children) component->Move(pos);
}

void IComponent::AddComponent(IComponent* component)
{
	m_Children.push_front(component);
	component->SetOwner(m_pOwningObject);
}

void IComponent::RemoveFromObject()
{
	m_pOwningObject->RemoveComponent(this);
}
