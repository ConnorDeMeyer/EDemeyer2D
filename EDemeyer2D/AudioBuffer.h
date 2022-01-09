#pragma once
#include <xaudio2.h>
#include "Structs.h"
#include <memory>

struct AudioBufferFormated
{
	XAUDIO2_BUFFER buffer;
	WAVEFORMATEXTENSIBLE format;
};

class AudioBuffer final
{
	friend class ResourceManager;

public:

	AudioBuffer() = default;
	AudioBuffer(const tstring& pathName);
	AudioBuffer(AudioBufferFormated* buffer);
	AudioBuffer(std::shared_ptr<AudioBufferFormated> sharedBuffer);
	
	void SetAudioBuffer(AudioBufferFormated* pAudioBuffer);
	std::shared_ptr<AudioBufferFormated> GetAudioBuffer() { return m_pAudioBuffer; }

private:

	std::shared_ptr<AudioBufferFormated> m_pAudioBuffer;

};

