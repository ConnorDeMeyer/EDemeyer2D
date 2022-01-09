#include "LogSegment.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "framework.h"

LogSegment::LogSegment()
{
	m_pTexture = new Texture(RESOURCES->LoadBitmap(_T("Resources/Bitmaps/Frogger_Obstacles.png")));
	m_pTexture->SetSourceRect(FRect{ 48.f,16.f,64.f,32.f });
	AddComponent(m_pTexture);
}

void LogSegment::SetLogSegment(eLogSegment segment)
{
	switch (segment)
	{
	case eLogSegment::logMiddle:
		m_pTexture->SetSourceRect(FRect{ 48,16,64,32 });
		break;
	case eLogSegment::logEndLeft:
		m_pTexture->SetSourceRect(FRect{ 32,16,48,32 });
		break;
	case eLogSegment::logEndRight:
		m_pTexture->SetSourceRect(FRect{ 0,32,16,48 });
		break;
	}
}
