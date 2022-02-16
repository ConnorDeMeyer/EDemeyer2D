#pragma once
#include <chrono>
#include <d2d1.h>
#include <tchar.h>
#include <string>

#include "Vector.h"

typedef std::chrono::steady_clock::time_point TimeStamp;
typedef std::chrono::duration<double> TimeIntervalD;
typedef std::chrono::duration<float> TimeIntervalF;

typedef Vector2<float> FPoint2;
typedef Vector2<int> IPoint2;
typedef Vector2<float> FVector2;
typedef Vector2<int> IVector2;
typedef D2D1_COLOR_F FColor;
typedef D2D1_RECT_F FRect;
typedef D2D1_RECT_U URect;
typedef D2D1_ROUNDED_RECT FRoundRect;
typedef D2D1_ELLIPSE FEllipse;
typedef D2D1_SIZE_F FSize;
typedef D2D1_SIZE_U USize;

#define FRect(center, width, height) D2D1::RectF(center.x - width * 0.5f, center.y - height * 0.5f, center.x + width * 0.5f, center.y + height * 0.5f)

#ifdef _UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif // _UNICODE

// custom Defines

#define SafeRelease(pInterface) if (pInterface) {pInterface->Release(); pInterface = nullptr;}
#define SafeDelete(pointer) if (pointer) {delete pointer; pointer = nullptr;}
#define SafeDestroyWindow(hWnd) if (hWnd) {DestroyWindow(hWnd); hWnd = nullptr;}

#ifndef Clamp
#define Clamp(value, minValue, maxValue) (min(max(value, minValue), maxValue))
#endif // !Clamp


// Circle struct
template <typename T>
struct Circle
{
	Vector2<T> center;
	T radius;
};

typedef Circle<float> FCircle;
typedef Circle<double> DCircle;

struct FlipFlop
{
	bool val = true;

	bool operator() ()
	{
		val = !val;
		return !val;
	}
};

template <typename T>
inline T Square(const T& value)
{
	return value * value;
}