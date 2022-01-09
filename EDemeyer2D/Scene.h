#pragma once

#include "IBaseClass.h"
#include <vector>
#include <forward_list>
#include "Structs.h"

class IObject;

class Scene : public IBaseClass
{
public:

	Scene();
	virtual ~Scene();

	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene* operator=(Scene&& other) = delete;

	/**
	* Called every frame after the Update call.
	* The Drawing Manager will call all the draw methods sequentually depending on the drawing order variable
	*/
	virtual void Draw(const FVector2& windowSize) const override;

	/** Called before physics and collision checks and player inputs*/
	virtual void FixedUpdate() override;

	/**
	* Called every frame before the Draw call.
	*/
	virtual void Update(float deltaTime) override;

	/** Called after all the update calls and before the draw call*/
	virtual void LateUpdate() override;

	/** Called after it has been added to the scene*/
	virtual void BeginPlay() override;

	/** called just before it is destroyed*/
	virtual void OnDestroy() override;

	/** Flags the component to be destroyed at the end of the frame.*/
	virtual void Destroy() override;

	void AddObjectToScene(IObject* pObject);

	void InitializeNewObjects();

	void DestroyFlaggedObjects();

private:

	std::vector<IObject*> m_Objects;
	std::forward_list<IObject*> m_NewlyAddedObjects;

};

