#pragma once
#include <d2d1.h>
#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")
#include <memory>
#include "Structs.h"


class Bitmap final
{
	friend class ResourceManager;

public:

	Bitmap() = default;
	/*Creates an empty bitmap with the given width and height.*/
	Bitmap(int width, int height, const D2D1_PIXEL_FORMAT& pixelFormat = D2D1_PIXEL_FORMAT{ DXGI_FORMAT_R8G8B8A8_UINT,D2D1_ALPHA_MODE_STRAIGHT });
	/*Creates a Bitmap object used by the engine from a Direct2D bitmap.*/
	Bitmap(ID2D1Bitmap* bitmap);
	/*Creates a bitmap from a given file path.*/
	Bitmap(const tstring& path);

	Bitmap(std::shared_ptr<ID2D1Bitmap> pSharedBitmap);

	//Bitmap(IWICBitmapSource* resource, const D2D1_PIXEL_FORMAT& pixelFormat = D2D1_PIXEL_FORMAT{ DXGI_FORMAT_R8G8B8A8_UINT,D2D1_ALPHA_MODE_STRAIGHT });
	
	~Bitmap();

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	Bitmap(const Bitmap& other);
	Bitmap(Bitmap&& other) noexcept;
	Bitmap& operator=(const Bitmap& other);
	Bitmap& operator=(Bitmap&& other) noexcept;

	// -------------------------
	// Public methods
	// ------------------------- 

	/*Copies the specified region from the specified bitmap into the current bitmap.
	@param test*/
	bool CopyFromBitmap(Bitmap* other, const URect* srcRect = nullptr, const IPoint2* destPoint = nullptr);
	bool CopyFromBitmap(ID2D1Bitmap* other, const URect* srcRect = nullptr, const IPoint2* destPoint = nullptr);
	bool CopyFromMemory(const void* srcData, UINT32 pitch, const URect* dstRect = nullptr);
	bool CopyFromRenderTarget(ID2D1RenderTarget* renderTarget, const URect* srcRect = nullptr, const IPoint2* destPoint = nullptr);
	inline void GetDpi(float* dpiX, float* dpiY) const { return m_pBitmap->GetDpi(dpiX, dpiY); }
	inline D2D1_PIXEL_FORMAT GetPixelFormat() const { return m_pBitmap->GetPixelFormat(); }
	inline USize GetPixelSize() const { return m_pBitmap->GetPixelSize(); }
	inline FSize GetSize() const { return m_pBitmap->GetSize(); }
	inline ID2D1Bitmap* Data() const { return m_pBitmap.get(); };

protected:

	std::shared_ptr<ID2D1Bitmap> m_pBitmap;
	UINT32 m_Id = 0;

};

class FileNotFoundException : public std::exception
{};