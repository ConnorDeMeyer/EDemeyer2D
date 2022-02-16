#pragma once

#include "Structs.h"
#include <algorithm>

inline bool IsOverlapping(const FRect& rect0, const FRect& rect1)
{
	return !(
		rect1.left		>	rect0.right		||
		rect1.right		<	rect0.left		||
		rect1.top		>	rect0.bottom	||
		rect1.bottom	<	rect0.top);
}

inline bool IsOverlapping(const FPoint2& point, const FRect& rect)
{
	return !(point.x < rect.left || point.y < rect.top || point.x > rect.right || point.y > rect.bottom);
}

inline bool IsOverlapping(const FCircle& circle0, const FCircle& circle1)
{
	return (SqrDistance(circle0.center, circle1.center) < Square(circle0.radius + circle1.radius));
}

inline bool IsOverlapping(const FPoint2& point, const FCircle& circle)
{
	return SqrDistance(point, circle.center) < Square(circle.radius);
}

inline bool IsOverlapping(const FRect& rect, const FCircle& circle)
{
	auto x = Clamp(circle.center.x, rect.left, rect.right) - circle.center.x;
	auto y = Clamp(circle.center.y, rect.top, rect.bottom) - circle.center.y;
	return Square(x) + Square(y) <= Square(circle.radius);
}