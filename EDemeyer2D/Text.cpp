#include "Text.h"
#include "framework.h"

Text::Text()
	: m_Font{ _T("Verdana") }
{
	UpdateFont();
}

Text::Text(const tstring& text, const tstring& font, float textSize, DWRITE_FONT_WEIGHT fontWeight)
	: m_Text		{ text }
	, m_Font		{ font }
	, m_TextSize	{ textSize }
	, m_FontWeight	{ fontWeight }
{
	UpdateFont();
}

Text::~Text()
{
	SafeRelease(m_pTextFormat);
}

void Text::Draw(const FVector2& windowSize) const
{
	auto& position{ GetPosition() };
	auto draw = DRAW;
	draw->SetDrawingColor(m_TextColor);
	draw->DrawText(FRect{
		position.x - m_DrawingWidth * 0.5f,
		position.y - m_TextSize * 0.5f,
		position.x + m_DrawingWidth * 0.5f,
		position.y + m_TextSize * 0.5f }, 
		m_Text, m_pTextFormat);
}

void Text::SetTextSize(float size)
{
	if (m_TextSize != size)
	{
		m_TextSize = size;
		UpdateFont();
	}
}

void Text::SetFontStyle(DWRITE_FONT_STYLE style)
{
	if (m_FontStyle != style)
	{
		m_FontStyle = style;
		UpdateFont();
	}
}

void Text::SetFontStretch(DWRITE_FONT_STRETCH stretch)
{
	if (m_FontStretch != stretch)
	{
		m_FontStretch = stretch;
		UpdateFont();
	}
}

void Text::SetFontWeight(DWRITE_FONT_WEIGHT weight)
{
	if (m_FontWeight != weight)
	{
		m_FontWeight = weight;
		UpdateFont();
	}
}

void Text::SetFont(const tstring& font)
{
	if (m_Font != font)
	{
		m_Font = font;
		UpdateFont();
	}
}

void Text::SetFlowDirection(DWRITE_FLOW_DIRECTION direction)
{
	if (m_pTextFormat) m_pTextFormat->SetFlowDirection(direction);
}

void Text::SetIncrementalTabStop(float tabStop)
{
	if (m_pTextFormat) m_pTextFormat->SetIncrementalTabStop(tabStop);
}

void Text::SetLineSpacing(DWRITE_LINE_SPACING_METHOD spacing, float lineSpacing, float baseline)
{
	if (m_pTextFormat) m_pTextFormat->SetLineSpacing(spacing, lineSpacing, baseline);
}

void Text::SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT alignment)
{
	if (m_pTextFormat) m_pTextFormat->SetParagraphAlignment(alignment);
}

void Text::SetReadingDirection(DWRITE_READING_DIRECTION direction)
{
	if (m_pTextFormat) m_pTextFormat->SetReadingDirection(direction);
}

void Text::SetTextAlignment(DWRITE_TEXT_ALIGNMENT alignment)
{
	if (m_pTextFormat) m_pTextFormat->SetTextAlignment(alignment);
}

void Text::SetTrimming(const DWRITE_TRIMMING* trimmingOptions, IDWriteInlineObject* trimmingSign)
{
	if (m_pTextFormat) m_pTextFormat->SetTrimming(trimmingOptions, trimmingSign);
}

void Text::SetWordWrapping(DWRITE_WORD_WRAPPING wrapping)
{
	if (m_pTextFormat) m_pTextFormat->SetWordWrapping(wrapping);
}

void Text::UpdateFont()
{
	auto writeFactory = DRAW->GetWriteFactory();
	IDWriteTextFormat* newTextFormat{};

	HRESULT hr = writeFactory->CreateTextFormat(
		m_Font.c_str(),
		NULL,
		m_FontWeight,
		m_FontStyle,
		m_FontStretch,
		m_TextSize,
		_T(""),
		&newTextFormat
	);
	if (!SUCCEEDED(hr))
	{
		SafeRelease(m_pTextFormat);
		throw UnableToCreateFontFormatException();
	}

	if (m_pTextFormat) {

		// Copy the previous parameters
		newTextFormat->SetFlowDirection(m_pTextFormat->GetFlowDirection());
		newTextFormat->SetIncrementalTabStop(m_pTextFormat->GetIncrementalTabStop());
		newTextFormat->SetParagraphAlignment(m_pTextFormat->GetParagraphAlignment());
		newTextFormat->SetReadingDirection(m_pTextFormat->GetReadingDirection());
		newTextFormat->SetTextAlignment(m_pTextFormat->GetTextAlignment());
		newTextFormat->SetWordWrapping(m_pTextFormat->GetWordWrapping());

		DWRITE_LINE_SPACING_METHOD lineSpacingMethod{};
		float lineSpacing, baseline;
		m_pTextFormat->GetLineSpacing(&lineSpacingMethod, &lineSpacing, &baseline);
		newTextFormat->SetLineSpacing(lineSpacingMethod, lineSpacing, baseline);

		DWRITE_TRIMMING trim{};
		IDWriteInlineObject* sign{};
		m_pTextFormat->GetTrimming(&trim, &sign);
		newTextFormat->SetTrimming(&trim, sign);
	}

	SafeRelease(m_pTextFormat);
	m_pTextFormat = newTextFormat;
}
