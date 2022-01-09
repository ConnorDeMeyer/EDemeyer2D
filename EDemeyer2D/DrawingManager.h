#pragma once

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <vector>

#include "Structs.h"

class Bitmap;
struct IDWriteTextFormat;

#define DRAW DrawingManager::GetInstance()

/**
* The Drawing manager class is a Singleton in charge of drawing to the screen.
* It contains a Render target variable that draw to the bitmap of the main window.
*/
class DrawingManager final
{
private:
	DrawingManager();
	~DrawingManager();

	DrawingManager(const DrawingManager& other) = delete;
	DrawingManager(DrawingManager&& other) = delete;
	DrawingManager& operator=(const DrawingManager& other) = delete;
	DrawingManager& operator=(DrawingManager&& other) = delete;

	static DrawingManager* m_Instance;

public:

	static DrawingManager* GetInstance() { if (!m_Instance) m_Instance = new DrawingManager(); return m_Instance; }
	static void Destroy() { delete m_Instance; m_Instance = nullptr; }

public:

	/** Sets the color that shapes will be drawn in.*/
	void SetDrawingColor(const FColor& color) { m_pSolidColorBrush->SetColor(color); }
	void SetDrawingColor(float r, float g, float b, float a = 1.f) { m_pSolidColorBrush->SetColor(D2D1::ColorF(r, g, b, a)); }
	
	/** Sets the opacity of the shapes that will be drawn.*/
	void SetOpacity(float a);

	/** Draws a straight line from beginPoint to endPoint.*/
	void DrawLine(const FPoint2& beginPoint, const FPoint2& endPoint, float width = 1.f);
	void DrawLine(const FPoint2& beginPoint, const FPoint2& endPoint, const FColor& color, float width = 1.f);
	
	/** Draws a rectangle with given line Width.*/
	void DrawRectangle(const FRect& rect, float lineWidth = 1.f);
	void DrawRectangle(const FRect& rect, const FColor& color, float lineWidth = 1.f);
	void DrawRectangle(const FPoint2 center, float width, float height, float lineWidth = 1.f);
	void DrawRectangle(const FPoint2 center, float width, float height, const FColor& color, float lineWidth = 1.f);
	
	/** Draws a rectangle with rounded edges that are defined by the radius variable.*/
	void DrawRoundedRectangle(const FRect& rect, float radius, float lineWidth = 1.f);
	void DrawRoundedRectangle(const FRect& rect, float radius, const FColor& color, float lineWidth = 1.f);
	void DrawRoundedRectangle(const FPoint2 center, float width, float height, float radius, float lineWidth = 1.f);
	void DrawRoundedRectangle(const FPoint2 center, float width, float height, float radius, const FColor& color, float lineWidth = 1.f);
	void DrawRoundedRectangle(const FRoundRect& rect, float lineWidth = 1.f);
	void DrawRoundedRectangle(const FRoundRect& rect, const FColor& color, float lineWidth = 1.f);
	
	/** Draws an ellipse.*/
	void DrawEllipse(const FEllipse& ellipse, float lineWidth = 1.f);
	void DrawEllipse(const FEllipse& ellipse, const FColor& color, float lineWith = 1.f);
	void DrawEllipse(const FPoint2& center, float radiusX, float radiusY, float lineWidth = 1.f);
	void DrawEllipse(const FPoint2& center, float radiusX, float radiusY, const FColor& color, float lineWith = 1.f);

	/** Draws a polygon*/
	void DrawPolygon(const FPoint2* vertices, size_t NrVertices, bool closed = true, float lineWidth = 1.f);
	void DrawPolygon(const FPoint2* vertices, size_t NrVertices, const FColor& color, bool closed = true, float lineWidth = 1.f);
	void DrawPolygon(const std::vector<FPoint2>& vertices, bool closed = true, float lineWidth = 1.f);
	void DrawPolygon(const std::vector<FPoint2>& vertices, const FColor& color, bool closed = true, float lineWidth = 1.f);

	/** Draws and fills a rectangle.*/
	void FillRectangle(const FRect& rect);
	void FillRectangle(const FRect& rect, const FColor& color);
	void FillRectangle(const FPoint2 center, float width, float height);
	void FillRectangle(const FPoint2 center, float width, float height, const FColor& color);
	
	/** Draws and fills a rounded rectangle.*/
	void FillRoundedRectangle(const FRoundRect& rect);
	void FillRoundedRectangle(const FRoundRect& rect, const FColor& color);
	void FillRoundedRectangle(const FRect& rect, float radius);
	void FillRoundedRectangle(const FRect& rect, float radius, const FColor& color);
	void FillRoundedRectangle(const FPoint2 center, float width, float height, float radius);
	void FillRoundedRectangle(const FPoint2 center, float width, float height, float radius, const FColor& color);
	
	/** Draws and fills an ellipse.*/
	void FillEllipse(const FEllipse& ellipse);
	void FillEllipse(const FEllipse& ellipse, const FColor& color);
	void FillEllipse(const FPoint2& center, float radiusX, float radiusY);
	void FillEllipse(const FPoint2& center, float radiusX, float radiusY, const FColor& color);

	/** Fill a polygon*/
	void FillPolygon(const FPoint2* vertices, size_t NrVertices);
	void FillPolygon(const FPoint2* vertices, size_t NrVertices, const FColor& color);
	void FillPolygon(const std::vector<FPoint2>& vertices);
	void FillPolygon(const std::vector<FPoint2>& vertices, const FColor& color);

	/** 
	* Draws a bitmap to the screen.
	* @param bitmap: pointer to a bitmap.
	* @param dest: the rectangle bounds that it will be drawn into.
	* @param srcRect (optional): the rectangle bounds that will be drawn from the given bitmap.
	* @param opacity: value from 0 to 1 that determines the opacity of the bitmap when drawn.
	*/
	void DrawBitmap(const Bitmap& bitmap, const FRect& dest, float opacity = 1.f);
	void DrawBitmap(ID2D1Bitmap* bitmap, const FRect& dest, float opacity = 1.f);
	void DrawBitmap(const Bitmap& bitmap, const FRect& dest, const FRect& srcRect, float opacity = 1.f);
	void DrawBitmap(ID2D1Bitmap* bitmap, const FRect& dest, const FRect& srcRect, float opacity = 1.f);
	void DrawBitmap(const Bitmap& bitmap, const FPoint2& pos, float opacity = 1.f);
	void DrawBitmap(ID2D1Bitmap* bitmap, const FPoint2& pos, float opacity = 1.f);

	//TODO add description
	void DrawText(const FRect& drawingRect, const tstring& text, const tstring& font, float fontSize);
	void DrawText(const FRect& drawingRect, const tstring& text, IDWriteTextFormat* format);
	void DrawText(const FRect& drawingRect, const tstring& text, IDWriteTextFormat* format, const FColor& color);

	void ClearBackground(FColor color = FColor{ 0,0,0,1 });

	/** Returns the render target used to draw to the screen.*/
	ID2D1HwndRenderTarget* GetRenderTarget() const { return m_pRenderTarget; }

	/** 
	* Signals the render target to start drawing.
	* The engine calls this automatically before the draw function is called on objects
	*/
	void BeginDraw() { m_pRenderTarget->BeginDraw(); }

	/**
	* Signals the render target to stop drawing to the screen
	* The engine calls this automatically before after all the draw functions are called in a frame
	*/
	HRESULT EndDraw() { return m_pRenderTarget->EndDraw(); }

	/** Returns the write factory responsible for creating text formats*/
	IDWriteFactory* GetWriteFactory() const { return m_pWriteFactory; }

	void SetInterpolationMode(D2D1_BITMAP_INTERPOLATION_MODE mode) { m_InterpolationMode = mode; }

	//void ChangeWindowHandle(const HWND& handle);

private:

	/** Factory responsible for creating drawing resources*/
	ID2D1Factory* m_pD2DFactory = nullptr;

	/** Render target linked to the bitmap of the screen*/
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;

	/** 
	* Brush responsible for drawing the different shapes
	* when changing the drawing color once, the subsequence drawings will also be in that color
	*/
	ID2D1SolidColorBrush* m_pSolidColorBrush = nullptr;

	/** Factory responsible for creating text formats*/
	IDWriteFactory* m_pWriteFactory = nullptr;

	D2D1_BITMAP_INTERPOLATION_MODE m_InterpolationMode{ D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR };
};
