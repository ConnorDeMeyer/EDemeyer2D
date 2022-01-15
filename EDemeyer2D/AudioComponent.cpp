#include "AudioComponent.h"
#include "ResourceManager.h"

AudioComponent::AudioComponent()
{

}

AudioComponent::AudioComponent(const tstring& audioFile)
{
	m_AudioBuffer = RESOURCES->LoadAudio(audioFile);
}

AudioComponent::~AudioComponent()
{
	if (m_pSourceVoice) m_pSourceVoice->DestroyVoice();
}

void AudioComponent::Play()
{
	if (m_pSourceVoice) {
		m_pSourceVoice->DestroyVoice();
	}

	HRESULT hr = RESOURCES->CreateSourceVoice(m_AudioBuffer, &m_pSourceVoice);
	if (FAILED(hr)) {
		m_pSourceVoice = nullptr;
	}
	else
	{
		m_pSourceVoice->Start();
	}
}

void AudioComponent::Stop()
{
	if (m_pSourceVoice) m_pSourceVoice->Stop();
}

void AudioComponent::SetLoopInterval(float startSecond, float endSecond)
{
	if (endSecond < startSecond) return;

	auto& buffer = m_AudioBuffer.GetAudioBuffer()->buffer;
	auto& samplesRate = m_AudioBuffer.GetAudioBuffer()->format.Format.nSamplesPerSec;
	
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	buffer.LoopBegin = (UINT32)(startSecond * samplesRate);
	buffer.LoopLength = (UINT32)((endSecond - startSecond) * samplesRate);
}
