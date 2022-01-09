#include "Bitmap.h"
#include "GameEngine.h"
#include "DrawingManager.h"
#include "ResourceManager.h"
#include <cassert>
#include <string>

#if defined(DEBUG) || defined(_DEBUG)
void ReleaseBitmap(ID2D1Bitmap* bitmap) { 
	bitmap->Release();
	OutputDebugStringW((std::to_wstring((int)bitmap) + L" Released\n").c_str());
}
#else
void ReleaseBitmap(ID2D1Bitmap* bitmap) { bitmap->Release(); }
#endif

Bitmap::Bitmap(int width, int height, const D2D1_PIXEL_FORMAT& pixelFormat)
{
	ID2D1Bitmap* bitmap;
	DRAW->GetRenderTarget()->CreateBitmap(
		D2D1::SizeU(width, height),
		D2D1_BITMAP_PROPERTIES{ pixelFormat, 72.f, 72.f },
		&bitmap);
	m_pBitmap = std::shared_ptr<ID2D1Bitmap>(bitmap, ReleaseBitmap);
}

Bitmap::Bitmap(ID2D1Bitmap* bitmap)
	: m_pBitmap{ bitmap, ReleaseBitmap }
{}

Bitmap::Bitmap(const tstring& path) : m_pBitmap{nullptr}
{
	*this = RESOURCES->LoadBitmap(path);
}

Bitmap::Bitmap(std::shared_ptr<ID2D1Bitmap> pSharedBitmap)
	: m_pBitmap{ pSharedBitmap }
{}

Bitmap::~Bitmap()
{
	
}

Bitmap::Bitmap(const Bitmap& other)
{
	m_pBitmap = other.m_pBitmap;
	m_Id = other.m_Id;
}

Bitmap::Bitmap(Bitmap&& other) noexcept
{
	m_pBitmap = other.m_pBitmap;
	m_Id = other.m_Id;
}

Bitmap& Bitmap::operator=(const Bitmap& other)
{
	if (&other != this) {
		m_pBitmap = other.m_pBitmap;
		m_Id = other.m_Id;
	}
	return *this;
}

Bitmap& Bitmap::operator=(Bitmap&& other) noexcept
{
	if (&other != this) {
		m_pBitmap = other.m_pBitmap;
		m_Id = other.m_Id;
	}
	return *this;
}

inline bool Bitmap::CopyFromBitmap(Bitmap* other, const URect* srcRect, const IPoint2* destPoint)
{
	D2D1_POINT_2U destination = destPoint ? D2D1::Point2U(destPoint->x, destPoint->y) : D2D1::Point2U(0, 0);
	HRESULT hr = m_pBitmap->CopyFromBitmap((destPoint ? &destination : nullptr), other->m_pBitmap.get(), srcRect);
	return SUCCEEDED(hr);
}

inline bool Bitmap::CopyFromBitmap(ID2D1Bitmap* other, const URect* srcRect, const IPoint2* destPoint)
{
	D2D1_POINT_2U destination = destPoint ? D2D1::Point2U(destPoint->x, destPoint->y) : D2D1::Point2U(0, 0);
	return SUCCEEDED(m_pBitmap->CopyFromBitmap((destPoint ? &destination : nullptr), other, srcRect));
}

inline bool Bitmap::CopyFromMemory(const void* srcData, UINT32 pitch, const URect* dstRect)
{
	return SUCCEEDED(m_pBitmap->CopyFromMemory(dstRect, srcData, pitch));
}

inline bool Bitmap::CopyFromRenderTarget(ID2D1RenderTarget* renderTarget, const URect* srcRect, const IPoint2* destPoint)
{
	D2D1_POINT_2U destination = destPoint ? D2D1::Point2U(destPoint->x, destPoint->y) : D2D1::Point2U(0, 0);
	return SUCCEEDED(m_pBitmap->CopyFromRenderTarget((destPoint ? &destination : nullptr), renderTarget, srcRect));
}
