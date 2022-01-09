#pragma once
#include "IComponent.h"
#include "Bitmap.h"
#include "Structs.h"
#include "Delegates.h"

class Sprite : public IComponent
{
public:

	Sprite() = default;
	virtual ~Sprite() = default;

	virtual void Draw(const FVector2& windowSize) const override;

	virtual void Update(float deltaTime) override;

	void SetBitmap(const Bitmap& bitmap) { m_Bitmap = bitmap; }

	void SetFrameDimension(const FVector2& dimension) { m_FrameDimension = dimension; }

	/** 
	* Sets the position of the first frame in the bitmap
	* usefull for when you want to skip frames
	*/
	void SetFrameOffset(int offset) { m_FrameOffset = offset; SetCurrentFrame(m_CurrentFrame + offset); }

	void SetTotalFrames(int amount) { m_TotalFrames = amount; }

	void SetTimePerFrame(float value) { m_TimePerFrame = value; }

	void SetCurrentFrame(int frame);

	void Reset();

	void Pause(bool isPaused) { m_bPauseTime = isPaused; }

	void SetLooping(bool isLooping) { m_bLoop = isLooping; }

	IDelegate<> OnAnimationEnd;

protected:

	Bitmap m_Bitmap;

	FVector2 m_FrameDimension = { 32.f,32.f };
	int m_FrameOffset = {};

	int m_CurrentFrame = {};
	int m_TotalFrames = 1;

	int m_CurrentFrameX = {};
	int m_CurrentFrameY = {};

	float m_TimePerFrame = 0.25f;
	float m_AccumulatedTime = {};

	bool m_bPauseTime = {};
	bool m_bLoop = { true };
};

