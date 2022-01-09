#pragma once

#include "Structs.h"

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