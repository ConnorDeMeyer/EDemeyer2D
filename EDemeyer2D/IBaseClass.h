#pragma once

#include "Structs.h"
#include <memory>

class IBaseClass
{

protected:

	IBaseClass();
	
	/**
	* An IBaseClass can only be desrtoyed by the SceneManager at the end of each frame.
	* Call the Destroy method to delete.
	* Will call the onDestroy method before deletion
	*/
	virtual ~IBaseClass();

public:

	/**
	* Called every frame after the Update call.
	* The drawing order depends on the drawing order variable
	* Smaller orders will be drawn first
	*/
	virtual void Draw(const FVector2& windowSize) const;
	
	/** Called before physics and collision checks and player inputs*/
	virtual void FixedUpdate();
	
	/**
	* Called every frame before the Draw call.
	*/
	virtual void Update(float deltaTime);
	
	/** Called after all the update calls and before the draw call*/
	virtual void LateUpdate();
	
	/** Called after it has been added to the scene*/
	virtual void BeginPlay();

	/** called just before it is destroyed*/
	virtual void OnDestroy();

	/** Flags the component to be destroyed at the end of the frame.*/
	virtual void Destroy();

	/** Cancels the destroy flag so it wont be destroyed at the end of the frame*/
	void CancelDestroy();

	/** 
	* Changes the drawing order of the Instance
	* Instances with smaller draw order will be drawn first
	*/
	void SetDrawingOrder(UINT32 drawingOrder);

	/** Returns the drawing order of the instance*/
	UINT32 GetDrawingOrder() const { return m_DrawingOrder; };

	/** 
	* Set the visibility of the class
	* The draw method wont be called if invisible
	*/
	void SetVisibility(bool bVisible);

	bool IsVisible() const { return m_bVisible; }

	/**
	* Set the active state of the component
	* Update calls will be ignored if deactivated
	*/
	void SetActive(bool bActive);

	/** Returns the active state of the component*/
	bool IsActive() { return m_bActive; }

	/** returns a weak reference to this instance*/
	std::weak_ptr<IBaseClass> GetWeakReference() const;

	bool GetDestroyFlag() const { return m_bDestroy; }

private:
	std::shared_ptr<IBaseClass> m_pObjectReference;
	UINT32 m_DrawingOrder = 10'000;
	bool m_bActive = true;
	bool m_bVisible = true;
	bool m_bDestroy = false;
};

