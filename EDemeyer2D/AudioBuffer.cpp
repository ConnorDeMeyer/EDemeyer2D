#include "AudioBuffer.h"
#include "ResourceManager.h"

AudioBuffer::AudioBuffer(const tstring& pathName)
{
	m_pAudioBuffer = RESOURCES->LoadAudio(pathName).m_pAudioBuffer;
}

AudioBuffer::AudioBuffer(AudioBufferFormated* buffer)
	: m_pAudioBuffer{ std::shared_ptr<AudioBufferFormated>(buffer) }
{
}

AudioBuffer::AudioBuffer(std::shared_ptr<AudioBufferFormated> sharedBuffer)
	: m_pAudioBuffer{ std::shared_ptr<AudioBufferFormated>(sharedBuffer) }
{
}

void AudioBuffer::SetAudioBuffer(AudioBufferFormated* pAudioBuffer)
{
	m_pAudioBuffer = std::shared_ptr<AudioBufferFormated>(pAudioBuffer);
}
