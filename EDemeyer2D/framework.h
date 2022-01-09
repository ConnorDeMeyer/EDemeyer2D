// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#pragma comment(lib,"user32.lib")
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <chrono>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d2d1")

// Custom headers
#include "Structs.h"
#include "GameEngine.h"
#include "DrawingManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "PhysicsManager.h"
#include "Delegates.h"


