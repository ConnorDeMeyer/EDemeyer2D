#pragma once
#include "IComponent.h"
#include "Structs.h"

#include <dwrite.h>

class Text : public IComponent
{
public:

	Text();
	/**https://docs.microsoft.com/en-us/windows/win32/api/dwrite/ne-dwrite-dwrite_font_weight*/
	Text(const tstring& text, const tstring& font, float textSize, DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL);
	virtual ~Text();

	virtual void Draw(const FVector2& windowSize) const override;

	void SetDrawingWidth(float width) { m_DrawingWidth = width; }
	void SetText(const tstring& text) { m_Text = text; }
	void SetText(tstring&& text) { m_Text = text; }
	void SetColor(const FColor& color) { m_TextColor = color; }
	void SetTextSize(float size);
	void SetFontStyle(DWRITE_FONT_STYLE style);
	void SetFontStretch(DWRITE_FONT_STRETCH stretch);
	void SetFontWeight(DWRITE_FONT_WEIGHT weight);
	void SetFont(const tstring& font);
	void SetFlowDirection(DWRITE_FLOW_DIRECTION direction);
	void SetIncrementalTabStop(float tabStop);
	void SetLineSpacing(DWRITE_LINE_SPACING_METHOD spacing, float lineSpacing, float baseline);
	void SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT alignment);
	void SetReadingDirection(DWRITE_READING_DIRECTION direction);
	void SetTextAlignment(DWRITE_TEXT_ALIGNMENT alignment);
	void SetTrimming(const DWRITE_TRIMMING* trimmingOptions, IDWriteInlineObject* trimmingSign);
	void SetWordWrapping(DWRITE_WORD_WRAPPING wrapping);

	tstring GetText() const { return m_Text; };
	IDWriteTextFormat* GetTextFormat() const { return m_pTextFormat; }

private:

	void UpdateFont();

protected:

	struct IDWriteTextFormat* m_pTextFormat = nullptr;

	tstring m_Text = {};
	FColor m_TextColor = FColor{ 1,1,1,1 };
	float m_DrawingWidth = {200.f};


	tstring m_Font{};
	float m_TextSize{ 32.f };

	DWRITE_FONT_STYLE m_FontStyle{ DWRITE_FONT_STYLE_NORMAL };
	DWRITE_FONT_STRETCH m_FontStretch{ DWRITE_FONT_STRETCH_NORMAL };
	DWRITE_FONT_WEIGHT m_FontWeight{ DWRITE_FONT_WEIGHT_NORMAL };

};

class UnableToCreateFontFormatException : std::exception
{};

