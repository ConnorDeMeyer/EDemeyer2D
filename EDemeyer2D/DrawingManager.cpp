#include "DrawingManager.h"
#include "framework.h"
#include "Bitmap.h"

#include <cassert>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib") 

DrawingManager* DrawingManager::m_Instance = nullptr;

DrawingManager::DrawingManager()
{
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
    assert(SUCCEEDED(hr));

    auto windowSize = ENGINE->GetWindowResolution();
    auto hWindow = ENGINE->GetWindowHandle();

    hr = m_pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hWindow, D2D1::SizeU(windowSize.x, windowSize.y)),
        &m_pRenderTarget);
    assert(SUCCEEDED(hr));

    hr = m_pRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(0.9f,0.9f,0.9f),
        &m_pSolidColorBrush);
    assert(SUCCEEDED(hr));

    hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&m_pWriteFactory));
    assert(SUCCEEDED(hr));
}

DrawingManager::~DrawingManager()
{
    SafeRelease(m_pRenderTarget);
    SafeRelease(m_pSolidColorBrush);
    SafeRelease(m_pD2DFactory);
    SafeRelease(m_pWriteFactory);
}

//---------------------------
// Member functions
//---------------------------

void DrawingManager::SetOpacity(float a)
{
    FColor color = m_pSolidColorBrush->GetColor();
    color.a = a;
    m_pSolidColorBrush->SetColor(color);
}

//---------------------------
// Drawing functions
//---------------------------

#pragma region Drawing

void DrawingManager::DrawLine(const FPoint2& beginPoint, const FPoint2& endPoint, float width)
{
    m_pRenderTarget->DrawLine(D2D1::Point2F(beginPoint.x, beginPoint.y), D2D1::Point2F(endPoint.x, endPoint.y), m_pSolidColorBrush, width);
}

void DrawingManager::DrawLine(const FPoint2& beginPoint, const FPoint2& endPoint, const FColor& color, float width)
{
    m_pSolidColorBrush->SetColor(color);
    DrawLine(beginPoint, endPoint, width);
}

void DrawingManager::DrawRectangle(const FRect& rect, float lineWidth)
{
    m_pRenderTarget->DrawRectangle(rect, m_pSolidColorBrush, lineWidth);
}

void DrawingManager::DrawRectangle(const FRect& rect, const FColor& color, float lineWidth)
{
    m_pSolidColorBrush->SetColor(color);
    DrawRectangle(rect, lineWidth);
}

void DrawingManager::DrawRectangle(const FPoint2 center, float width, float height, float lineWidth)
{
    DrawRectangle(FRect(center, width, height), lineWidth);
}

void DrawingManager::DrawRectangle(const FPoint2 center, float width, float height, const FColor& color, float lineWidth)
{
    m_pSolidColorBrush->SetColor(color);
    DrawRectangle(center, width, height, lineWidth);
}

void DrawingManager::DrawRoundedRectangle(const FRoundRect& rect, float lineWidth)
{
    m_pRenderTarget->DrawRoundedRectangle(rect, m_pSolidColorBrush, lineWidth);
}

void DrawingManager::DrawRoundedRectangle(const FRoundRect& rect, const FColor& color, float lineWidth)
{
    m_pSolidColorBrush->SetColor(color);
    DrawRoundedRectangle(rect, lineWidth);
}

void DrawingManager::DrawRoundedRectangle(const FRect& rect, float radius, float lineWidth)
{
    DrawRoundedRectangle(D2D1::RoundedRect(rect, radius, radius), lineWidth);
}

void DrawingManager::DrawRoundedRectangle(const FRect& rect, float radius, const FColor& color, float lineWidth)
{
    m_pSolidColorBrush->SetColor(color);
    DrawRoundedRectangle(rect, radius, lineWidth);
}

void DrawingManager::DrawRoundedRectangle(const FPoint2 center, float width, float height, float radius, float lineWidth)
{
    DrawRoundedRectangle(FRect(center, width, height), radius, lineWidth);
}

void DrawingManager::DrawRoundedRectangle(const FPoint2 center, float width, float height, float radius, const FColor& color, float lineWidth)
{
    m_pSolidColorBrush->SetColor(color);
    DrawRoundedRectangle(center, width, height, lineWidth);
}

void DrawingManager::DrawEllipse(const FEllipse& ellipse, float lineWidth)
{
    m_pRenderTarget->DrawEllipse(ellipse, m_pSolidColorBrush, lineWidth);
}

void DrawingManager::DrawEllipse(const FEllipse& ellipse, const FColor& color, float lineWith)
{
    m_pSolidColorBrush->SetColor(color);
    DrawEllipse(ellipse, lineWith);
}

void DrawingManager::DrawEllipse(const FPoint2& center, float radiusX, float radiusY, float lineWidth)
{
    DrawEllipse(D2D1::Ellipse(D2D1::Point2F(center.x, center.y), radiusX, radiusY), lineWidth);
}

void DrawingManager::DrawEllipse(const FPoint2& center, float radiusX, float radiusY, const FColor& color, float lineWith)
{
    m_pSolidColorBrush->SetColor(color);
    DrawEllipse(center, radiusX, radiusY, lineWith);
}

void DrawingManager::DrawPolygon(const FPoint2* vertices, size_t NrVertices, bool closed, float lineWidth)
{
    if (NrVertices < 2) return;

    ID2D1PathGeometry* pPath;
    HRESULT hr = m_pD2DFactory->CreatePathGeometry(&pPath);
    assert(SUCCEEDED(hr));

    ID2D1GeometrySink* pSink;
    hr = pPath->Open(&pSink);
    assert(SUCCEEDED(hr));

    pSink->BeginFigure(D2D1::Point2F(vertices[0].x, vertices[0].y), D2D1_FIGURE_BEGIN_HOLLOW);

    pSink->AddLines(reinterpret_cast<const D2D1_POINT_2F*>(vertices + 1), NrVertices - 1);

    pSink->EndFigure(closed ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN);

    hr = pSink->Close();
    assert(SUCCEEDED(hr));

    m_pRenderTarget->DrawGeometry(pPath, m_pSolidColorBrush, lineWidth);

    SafeRelease(pSink);
    SafeRelease(pPath);
}

void DrawingManager::DrawPolygon(const FPoint2* vertices, size_t NrVertices, const FColor& color, bool closed, float lineWidth)
{
    m_pSolidColorBrush->SetColor(color);
    DrawPolygon(vertices, NrVertices, closed, lineWidth);
}

void DrawingManager::DrawPolygon(const std::vector<FPoint2>& vertices, bool closed, float lineWidth)
{
    DrawPolygon(vertices.data(), vertices.size(), closed, lineWidth);
}

void DrawingManager::DrawPolygon(const std::vector<FPoint2>& vertices, const FColor& color, bool closed, float lineWidth)
{
    m_pSolidColorBrush->SetColor(color);
    DrawPolygon(vertices, closed);
}

void DrawingManager::FillRectangle(const FRect& rect)
{
    m_pRenderTarget->FillRectangle(rect, m_pSolidColorBrush);
}

void DrawingManager::FillRectangle(const FRect& rect, const FColor& color)
{
    m_pSolidColorBrush->SetColor(color);
    FillRectangle(rect);
}

void DrawingManager::FillRectangle(const FPoint2 center, float width, float height)
{
    m_pRenderTarget->FillRectangle(FRect(center, width, height), m_pSolidColorBrush);
}

void DrawingManager::FillRectangle(const FPoint2 center, float width, float height, const FColor& color)
{
    m_pSolidColorBrush->SetColor(color);
    FillRectangle(center, width, height);
}

void DrawingManager::FillRoundedRectangle(const FRoundRect& rect)
{
    m_pRenderTarget->FillRoundedRectangle(rect, m_pSolidColorBrush);
}

void DrawingManager::FillRoundedRectangle(const FRoundRect& rect, const FColor& color)
{
    m_pSolidColorBrush->SetColor(color);
    FillRoundedRectangle(rect);
}

void DrawingManager::FillRoundedRectangle(const FRect& rect, float radius)
{
    m_pRenderTarget->FillRoundedRectangle(D2D1::RoundedRect(rect, radius, radius), m_pSolidColorBrush);
}

void DrawingManager::FillRoundedRectangle(const FRect& rect, float radius, const FColor& color)
{
    m_pSolidColorBrush->SetColor(color);
    FillRoundedRectangle(rect, radius);
}

void DrawingManager::FillRoundedRectangle(const FPoint2 center, float width, float height, float radius)
{
    FillRoundedRectangle(FRect(center, width, height), radius);
}

void DrawingManager::FillRoundedRectangle(const FPoint2 center, float width, float height, float radius, const FColor& color)
{
    m_pSolidColorBrush->SetColor(color);
    FillRoundedRectangle(center, width, height, radius);
}

void DrawingManager::FillEllipse(const FEllipse& ellipse)
{
    m_pRenderTarget->FillEllipse(ellipse, m_pSolidColorBrush);
}

void DrawingManager::FillEllipse(const FEllipse& ellipse, const FColor& color)
{
    m_pSolidColorBrush->SetColor(color);
    FillEllipse(ellipse);
}

void DrawingManager::FillEllipse(const FPoint2& center, float radiusX, float radiusY)
{
    FillEllipse(D2D1::Ellipse(D2D1::Point2F(center.x, center.y), radiusX, radiusY));
}

void DrawingManager::FillEllipse(const FPoint2& center, float radiusX, float radiusY, const FColor& color)
{
    m_pSolidColorBrush->SetColor(color);
    FillEllipse(center, radiusX, radiusY);
}

void DrawingManager::FillPolygon(const FPoint2* vertices, size_t NrVertices)
{
    if (NrVertices < 2) return;

    ID2D1PathGeometry* pPath;
    HRESULT hr = m_pD2DFactory->CreatePathGeometry(&pPath);
    assert(SUCCEEDED(hr));

    ID2D1GeometrySink* pSink;
    hr = pPath->Open(&pSink);
    assert(SUCCEEDED(hr));

    pSink->BeginFigure(D2D1::Point2F(vertices[0].x, vertices[0].y), D2D1_FIGURE_BEGIN_FILLED);

    pSink->AddLines(reinterpret_cast<const D2D1_POINT_2F*>(vertices + 1), NrVertices - 1);

    pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

    hr = pSink->Close();
    assert(SUCCEEDED(hr));

    m_pRenderTarget->FillGeometry(pPath, m_pSolidColorBrush);

    SafeRelease(pSink);
    SafeRelease(pPath);
}

void DrawingManager::FillPolygon(const FPoint2* vertices, size_t NrVertices, const FColor& color)
{
    m_pSolidColorBrush->SetColor(color);
    FillPolygon(vertices, NrVertices);
}

void DrawingManager::FillPolygon(const std::vector<FPoint2>& vertices)
{
    FillPolygon(vertices.data(), vertices.size());
}

void DrawingManager::FillPolygon(const std::vector<FPoint2>& vertices, const FColor& color)
{
    m_pSolidColorBrush->SetColor(color);
    FillPolygon(vertices);
}

void DrawingManager::DrawBitmap(ID2D1Bitmap* bitmap, const FRect& dest, float opacity)
{
    FRect sourceRectangle{ 0,0,float(bitmap->GetPixelSize().width), float(bitmap->GetPixelSize().height) };
    m_pRenderTarget->DrawBitmap(bitmap, dest, opacity, m_InterpolationMode, sourceRectangle);
}

void DrawingManager::DrawBitmap(const Bitmap& bitmap, const FRect& dest, float opacity)
{
    DrawBitmap(bitmap.Data(), dest, opacity);
}

void DrawingManager::DrawBitmap(ID2D1Bitmap* bitmap, const FRect& dest, const FRect& srcRect, float opacity)
{
    m_pRenderTarget->DrawBitmap(bitmap, dest, opacity, m_InterpolationMode, srcRect);
}

void DrawingManager::DrawBitmap(const Bitmap& bitmap, const FRect& dest, const FRect& srcRect, float opacity)
{
    DrawBitmap(bitmap.Data(), dest, srcRect, opacity);
}

void DrawingManager::DrawBitmap(ID2D1Bitmap* bitmap, const FPoint2& pos, float opacity)
{
    FRect destRect = D2D1::RectF(pos.x, pos.y, float(bitmap->GetPixelSize().width), float(bitmap->GetPixelSize().height));
    DrawBitmap(bitmap, destRect, opacity);
}

void DrawingManager::DrawBitmap(const Bitmap& bitmap, const FPoint2& pos, float opacity)
{
    DrawBitmap(bitmap.Data(), pos, opacity);
}

void DrawingManager::DrawText(const FRect& drawingRect, const tstring& text, const tstring& font, float fontSize)
{
    IDWriteTextFormat* format = nullptr;
    HRESULT hr = m_pWriteFactory->CreateTextFormat(
        font.c_str(),
        NULL,
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        fontSize,
        _T("en-us"),
        &format);
    assert(SUCCEEDED(hr));

    m_pRenderTarget->DrawText(
        text.c_str(),
        text.size(),
        format,
        drawingRect,
        m_pSolidColorBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE);

    SafeRelease(format);
}

void DrawingManager::DrawText(const FRect& drawingRect, const tstring& text, IDWriteTextFormat* format)
{
    m_pRenderTarget->DrawText(text.c_str(), text.size(), format, drawingRect, m_pSolidColorBrush);
}

void DrawingManager::DrawText(const FRect& drawingRect, const tstring& text, IDWriteTextFormat* format, const FColor& color)
{
    m_pSolidColorBrush->SetColor(color);
    DrawText(drawingRect, text, format);
}

void DrawingManager::ClearBackground(FColor color)
{
    m_pRenderTarget->Clear(color);
}

#pragma endregion Drawing

//void DrawingManager::ChangeWindowHandle(const HWND& handle)
//{
//    m_pRenderTarget->Resize()
//
//    SafeRelease(m_pSolidColorBrush);
//    SafeRelease(m_pRenderTarget);
//
//    auto windowSize = ENGINE->GetWindowResolution();
//
//    HRESULT hr = m_pD2DFactory->CreateHwndRenderTarget(
//        D2D1::RenderTargetProperties(),
//        D2D1::HwndRenderTargetProperties(handle, D2D1::SizeU(windowSize.x, windowSize.y)),
//        &m_pRenderTarget);
//    assert(SUCCEEDED(hr));
//
//    hr = m_pRenderTarget->CreateSolidColorBrush(
//        D2D1::ColorF(0.1f, 0.1f, 0.1f),
//        &m_pSolidColorBrush);
//    assert(SUCCEEDED(hr));
//
//    m_pRenderTarget->CreateCompatibleRenderTarget()
//}

