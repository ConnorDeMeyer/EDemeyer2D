#pragma once
#include "IComponent.h"
#include <xaudio2.h>
#include "AudioBuffer.h"

class AudioComponent :
    public IComponent
{
public:

    AudioComponent();
    AudioComponent(const tstring& audioFile);
    virtual ~AudioComponent();

    void Play();
    void Pause();
    void Resume();

protected:

    AudioBuffer m_AudioBuffer;
    IXAudio2SourceVoice* m_pSourceVoice{};

};

