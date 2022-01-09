#pragma once

#include "IBaseClass.h"
#include <forward_list>
#include <vector>
#include <memory>
#include "Structs.h"

class ICollisionComponent;
class IComponent;

/**
* The IObject class is an interface where all other objects should derive from.
* It is responsible for managing its children components and communicating with SceneManager
*/
class IObject : public IBaseClass
{
	friend class Scene;

public:

	IObject();

protected:

	/** 
	* An IObject can only be desrtoyed by the SceneManager at the end of each frame.
	* Call the Destroy method to delete.
	* its children components will also be destroyed
	*/
	virtual ~IObject();

public:

	/**
	* Called every frame before the Draw call.
	* Also updates all the active children components
	*/
	virtual void Update(float deltaTime) override;

	/**
	* Called every frame after the Update call.
	* Draw will also be called from the active children of this object.
	*/
	virtual void Draw(const FVector2& windowSize) const override;

	/** Called after it has been added to the scene
	* Will also call beginPlay on components
	*/
	virtual void BeginPlay() override;

	/** Returns the first component that is of type T.
	* This method is slow, you shouldn't use it in time critical situations.*/
	template <typename T>
	T* GetComponent() const;

	/** Returns all the components that are of type T.
	* This method is slow, you shouldn't use it in time critical situations.*/
	template <typename T>
	std::vector<T*> GetComponents() const;

	/** 
	* Adds a component to the object
	* Gives the engine the ability to manage the component and makes sure the component gets automatically deleted
	*/
	void AddComponent(IComponent* pComponent);

	/** Removes the component from the object.
	* The component will be removed at the end of the frame.*/
	void RemoveComponent(IComponent* pComponent);
	
	/** Removes the first component of type T from the object.
	* The component will be removed at the end of the frame.
	* Returns true if an object was removed.*/
	template <typename T>
	void RemoveComponent();

	/** Removes all the components of type T from the object.
	* The components will be removed at the end of the frame.
	* Returns true if an object was removed.*/
	template <typename T>
	void RemoveComponents();

	/** Flags the component to be destroyed at the end of the frame.*/
	virtual void Destroy() override;

	/** Sets the position of the object and children components*/
	void SetPosition(const FVector2& pos);

	/** Add the given vector to the current position*/
	void Move(const FVector2& pos);

	/** Returns the position of the object*/
	const FVector2& GetPosition() const { return m_Pos; }
	FVector2 GetPosition() { return m_Pos; }

	virtual void OnOverlapObject(IObject* other, ICollisionComponent* component = nullptr, ICollisionComponent* otherComponent = nullptr) {};

	void DestroyFlaggedComponents();

private:

	/** Position of the object in world space*/
	FVector2 m_Pos;

	/** 
	* List of the children components of the object.
	* these children may have child components of their own.
	*/
	std::forward_list<IComponent*> m_Children;

	/** List of all the components connected to the object*/
	std::forward_list<IComponent*> m_Components;

};

template<typename T>
inline T* IObject::GetComponent() const
{
	for (IComponent* component : m_Components)
		if (auto returnValue{ dynamic_cast<T*>(component) }) return returnValue;
	return nullptr;
}

template<typename T>
inline std::vector<T*> IObject::GetComponents() const
{
	std::vector<T*> components;
	for (IComponent* component : m_Components)
		if (auto value{ dynamic_cast<T>(component) }) components.push_back(value);
	return components;
}

template<typename T>
inline void IObject::RemoveComponent()
{
	for (IComponent* component : m_Components)
		if (dynamic_cast<T*>(component))
		{
			component->Destroy();
			m_Components.remove(component);
			m_Children.remove(component);
		}
}

template<typename T>
inline void IObject::RemoveComponents()
{
	m_Components.remove_if([this](IComponent* component) {
		if (dynamic_cast<T*>(component))
		{
			component->Destroy();
			m_Children.remove(component);
			return true;
		}
		return false;
		});
}
