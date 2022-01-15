#include "ResourceManager.h"
#include <cassert>
#include <stdexcept>
#include "DrawingManager.h"
#include "framework.h"
#include <algorithm>

ResourceManager* ResourceManager::m_Instance = nullptr;

ResourceManager::ResourceManager()
{
	HRESULT hr;
	hr = XAudio2Create(&m_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(hr));

	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasterVoice);
	assert(SUCCEEDED(hr));


}

ResourceManager::~ResourceManager()
{
}

Bitmap ResourceManager::LoadBitmap(const tstring& pathName)
{
	// check if the bitmap isnt already loaded
	auto it = m_BitmapIndices.find(pathName);
	if (it != m_BitmapIndices.end()) {
		int bitmapsIndex{ it->second };
		auto weakBitmapPtr{ m_Bitmaps[bitmapsIndex] };
		if (!weakBitmapPtr.expired()) return Bitmap(weakBitmapPtr.lock());
	}
	
	auto bitmap = CreateBitmapFromFile(pathName);

	// create the rapper around the bitmap
	Bitmap bitmapWrapper{ bitmap };

	// add the resource to the manager
	m_Bitmaps.push_back(std::weak_ptr<ID2D1Bitmap>(bitmapWrapper.m_pBitmap));

	int bitmapId{ int(m_Bitmaps.size()) - 1 };
	bitmapWrapper.m_Id = bitmapId;
	m_BitmapIndices.insert(std::pair<tstring, int>(pathName, bitmapId));

#if defined(DEBUG) || defined(_DEBUG)
	OutputDebugStringW((std::to_wstring((int)bitmap) + L" Created\n").c_str());
#endif

	return bitmapWrapper;
}

Bitmap ResourceManager::PermanentlyLoadBitmap(const tstring& pathName)
{
	auto bitmap{ LoadBitmap(pathName) };
	m_PermanentlyLoadedBitmaps.push_back(bitmap.m_pBitmap);
	return bitmap;
}

ID2D1Bitmap* ResourceManager::CreateBitmapFromFile(const tstring& pathName)
{
	// create wic factory
	IWICImagingFactory* wicFactory = nullptr;
	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		//IID_IWICImagingFactory,
		//(LPVOID*)&wicFactory);
		__uuidof(**(&wicFactory)), IID_PPV_ARGS_Helper(&wicFactory));
	assert(SUCCEEDED(hr));

	// create image decoder
	IWICBitmapDecoder* wicDecoder = nullptr;
	hr = wicFactory->CreateDecoderFromFilename(
		(LPCWSTR)pathName.c_str(),
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&wicDecoder);

	ID2D1Bitmap* bitmap;

	if (SUCCEEDED(hr)) {

		// read a frame from the image
		IWICBitmapFrameDecode* wicFrame = nullptr;
		hr = wicDecoder->GetFrame(0, &wicFrame);
		assert(SUCCEEDED(hr));

		// create a converter
		IWICFormatConverter* wicConverter = nullptr;
		hr = wicFactory->CreateFormatConverter(&wicConverter);
		assert(SUCCEEDED(hr));

		// initialize converter
		hr = wicConverter->Initialize(
			wicFrame,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			1.f,
			WICBitmapPaletteTypeCustom);
		assert(SUCCEEDED(hr));

		// create the bitmap
		hr = DRAW->GetRenderTarget()->CreateBitmapFromWicBitmap(
			wicConverter,
			NULL,
			&bitmap);
		assert(SUCCEEDED(hr));

		// release resources
		SafeRelease(wicConverter);
		SafeRelease(wicFrame);
	}
	else throw FileNotFoundException();

	// release resources
	SafeRelease(wicFactory);
	SafeRelease(wicDecoder);

	return bitmap;
}

//void ResourceManager::UpdateBitmapRenderTargets(ID2D1RenderTarget* pRenderTarget)
//{
//	auto replaceBitmap = [this](const std::pair<tstring, int>& item)
//	{
//		if (!m_Bitmaps[item.second].expired())
//		{
//			auto test = m_Bitmaps[item.second].lock();
//			test->Release();
//			auto newPointer = LoadBitmap(item.first);
//		}
//	};
//
//	std::for_each(m_BitmapIndices.begin(), m_BitmapIndices.end(), replaceBitmap);
//}

//https://docs.microsoft.com/en-us/windows/win32/xaudio2/how-to--load-audio-data-files-in-xaudio2
HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;

}

HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}

AudioBuffer ResourceManager::LoadAudio(const tstring& pathName)
{
	auto it = m_AudioBufferIndices.find(pathName);
	if (it != m_AudioBufferIndices.end()) {
		int bitmapsIndex{ it->second };
		auto weakBitmapPtr{ m_AudioBuffers[bitmapsIndex] };
		if (!weakBitmapPtr.expired()) return AudioBuffer(weakBitmapPtr.lock());
	}

	AudioBufferFormated* Audiobuffer{ new AudioBufferFormated() };

	HANDLE hFile = CreateFile(
		pathName.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE) throw FileNotFoundException();

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) throw FileNotFoundException();

	DWORD dwChunkSize{};
	DWORD dwChunkPosition{};
	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	assert(filetype == fourccWAVE);

	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &Audiobuffer->format, dwChunkSize, dwChunkPosition);

	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE* pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	Audiobuffer->buffer.AudioBytes = dwChunkSize;  //size of the audio buffer in bytes
	Audiobuffer->buffer.pAudioData = pDataBuffer;  //buffer containing audio data
	Audiobuffer->buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

	auto bufferWrapper = AudioBuffer(std::shared_ptr<AudioBufferFormated>(Audiobuffer));
	m_AudioBuffers.push_back(std::weak_ptr<AudioBufferFormated>(bufferWrapper.m_pAudioBuffer));

	return bufferWrapper;
}

HRESULT ResourceManager::CreateSourceVoice(const AudioBuffer& audioBuffer, IXAudio2SourceVoice** pVoice)
{
	HRESULT hr;
	if (FAILED( hr = m_pXAudio2->CreateSourceVoice(pVoice, (WAVEFORMATEX*)&audioBuffer.m_pAudioBuffer->format))) return hr;
	if (FAILED(hr = (*pVoice)->SubmitSourceBuffer(&audioBuffer.m_pAudioBuffer->buffer))) return hr;
	return hr;
}