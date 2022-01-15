#pragma once
#include <xaudio2.h>
#include "Structs.h"
#include <memory>

struct AudioBufferFormated
{
	AudioBufferFormated(XAUDIO2_BUFFER _buffer, WAVEFORMATEXTENSIBLE _format)
		: buffer{ _buffer }
		, format{ _format }
	{}

	AudioBufferFormated()
		: buffer{}
		, format{}
	{}

	virtual ~AudioBufferFormated()
	{
		delete buffer.pAudioData;
	}

	AudioBufferFormated(const AudioBufferFormated& other) = delete;
	AudioBufferFormated& operator=(const AudioBufferFormated& other) = delete;
	AudioBufferFormated(AudioBufferFormated&& other) = delete;
	AudioBufferFormated& operator=(AudioBufferFormated&& other) = delete;

	XAUDIO2_BUFFER buffer;
	WAVEFORMATEXTENSIBLE format;
};

class AudioBuffer final
{
	friend class ResourceManager;

public:

	AudioBuffer() = default;
	~AudioBuffer() = default;
	AudioBuffer(const tstring& pathName);
	AudioBuffer(AudioBufferFormated* buffer);
	AudioBuffer(std::shared_ptr<AudioBufferFormated> sharedBuffer);
	
	void SetAudioBuffer(AudioBufferFormated* pAudioBuffer);
	std::shared_ptr<AudioBufferFormated> GetAudioBuffer() { return m_pAudioBuffer; }

private:

	std::shared_ptr<AudioBufferFormated> m_pAudioBuffer;

};

