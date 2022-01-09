#pragma once

#include "IBaseClass.h"
#include <forward_list>

class IObject;

/**
* The base class for a component object.
* A Component can be added to an Object class and will receive update and draw calls when the owning actor does aswell.
*/
class IComponent : public IBaseClass
{
	friend class IObject;

public:

	IComponent();

protected:

	/** Call the destroy method to delete the component*/
	virtual ~IComponent();

	IComponent(const IComponent& other)				= delete;
	IComponent(IComponent&& other)					= delete;
	IComponent& operator=(const IComponent& other)	= delete;
	IComponent& operator=(IComponent&& other)		= delete;

public:

	/** Returns the owner of the component.*/
	inline IObject* GetOwner() const { return m_pOwningObject; }

	/** Set the owner if the component doesn't have one yet.*/
	inline void SetOwner(IObject* pOwner) { if (!m_pOwningObject) m_pOwningObject = pOwner; }

	/** Gets called every frame.
	Make sure to call the Base class's Update method before implementing yours.*/
	virtual void Update(float deltaTime) override;
	
	/**
	* Gets called every frame.
	* Make sure to call the Base class's Draw method before implementing yours.*/
	virtual void Draw(const FVector2& windowSize) const override;

	/**
	* Flags the component to be destroyed at the end of the frame.
	* Child components will also be deleted
	*/
	virtual void Destroy() override;

	/** 
	* Sets the position of this component and its children
	* This method is mainly used for the Object to move its children with it
	*/
	virtual void SetPosition(const FVector2& pos);

	/** Sets the local position of the component and moves its children with it*/
	virtual void SetLocalPosition(const FVector2& pos);

	/** returns the position of the component*/
	const FVector2& GetPosition() const { return m_Pos; }
	FVector2 GetPosition() { return m_Pos; }

	/** Move the the component and its children in local space*/
	virtual void Move(const FVector2& pos);

	/** Add a child component to this component*/
	void AddComponent(IComponent* component);

	float GetRotation() const { return m_Rotation; }

	const FVector2& GetScale() const { return m_Scale; }

private:

	void RemoveFromObject();

private:

	/** The position of the object in world space*/
	FVector2 m_Pos = {};

	/** The difference in position between this component and his parent*/
	FVector2 m_LocalPos = {};

	float m_Rotation = {};

	FVector2 m_Scale = { 1,1 };

	/** The owning object of the component*/
	IObject* m_pOwningObject = nullptr;

	/** containers for the children components of this component*/
	std::forward_list<IComponent*> m_Children;

};

