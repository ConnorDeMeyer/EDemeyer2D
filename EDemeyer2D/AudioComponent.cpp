#include "AudioComponent.h"
#include "ResourceManager.h"

AudioComponent::AudioComponent()
{

}

AudioComponent::AudioComponent(const tstring& audioFile)
{
	m_AudioBuffer = RESOURCES->LoadAudio(audioFile);
	HRESULT hr = RESOURCES->CreateSourceVoice(m_AudioBuffer, &m_pSourceVoice);
	if (FAILED(hr)) {
		m_pSourceVoice = nullptr;
	}
}

AudioComponent::~AudioComponent()
{
	if (m_pSourceVoice) m_pSourceVoice->DestroyVoice();
}

void AudioComponent::Play()
{
	if (m_pSourceVoice) m_pSourceVoice->Start();
}
