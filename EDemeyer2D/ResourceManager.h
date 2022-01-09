#pragma once

#include <vector>
#include <map>
#include <memory>
#include <d2d1.h>
#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")
#include <xaudio2.h>

#include "Structs.h"
#include "Bitmap.h"
#include "AudioBuffer.h"

#define RESOURCES ResourceManager::GetInstance()

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

/**
* Class responsible for loading resources like bitmap or audio sources from a file.
* It also manages their lifetime. When loading the same resource multiple times it will only do so once
*/
class ResourceManager
{
private:

	ResourceManager();
	~ResourceManager();

	ResourceManager(const ResourceManager& other) = delete;
	ResourceManager(ResourceManager&& other) = delete;
	ResourceManager& operator=(const ResourceManager& other) = delete;
	ResourceManager& operator=(ResourceManager&& other) = delete;

	static ResourceManager* m_Instance;

public:

	static ResourceManager* GetInstance() { if (!m_Instance) m_Instance = new ResourceManager(); return m_Instance; }
	static void Destroy() { delete m_Instance; m_Instance = nullptr; }

	/** Loads a bitmap if it ins't already loaded and returns it*/
	Bitmap LoadBitmap(const tstring& pathName);

	/** 
	* Permanently loads a bitmap and keeps it in memory until the end of the session.
	* Handy for player/enemy/ui sprites as they are continually needed in between scenes.
	*/
	Bitmap PermanentlyLoadBitmap(const tstring& pathName);

	AudioBuffer LoadAudio(const tstring& pathName);

	HRESULT CreateSourceVoice(const AudioBuffer& audioBuffer, IXAudio2SourceVoice** pVoice);

	//void UpdateBitmapRenderTargets(ID2D1RenderTarget* pRenderTarget);

private:

	ID2D1Bitmap* CreateBitmapFromFile(const tstring& pathName);

	/** Keeps a shared pointer to the bitmaps so they will never go out of scope and get destroyed.*/
	std::vector<std::shared_ptr<ID2D1Bitmap>> m_PermanentlyLoadedBitmaps;

	/** Keeps weak pointers to loaded bitmaps so they can go out of scope and can be reloaded when they do so*/
	std::vector<std::weak_ptr<ID2D1Bitmap>> m_Bitmaps;

	/** Map for keeping track of thebitmaps in the m_Bitmaps vector*/
	std::map<tstring, int> m_BitmapIndices;

	IXAudio2* m_pXAudio2{};
	IXAudio2MasteringVoice* m_pMasterVoice{};

	//std::vector<std::shared_ptr<AudioBufferFormated>> m_PermanentlyLoadedAudioBuffers;

	std::vector<std::weak_ptr<AudioBufferFormated>> m_AudioBuffers;

	std::map<tstring, int> m_AudioBufferIndices;

};


	//https://docs.microsoft.com/en-us/windows/win32/xaudio2/how-to--load-audio-data-files-in-xaudio2
	//HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	//HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);