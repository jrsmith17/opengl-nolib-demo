/*
	License provided at the end of the file
*/

#include <Windows.h>
#include <stdio.h>

#include "render.h"

bool globalRunning = false;
HWND globalWindow;

// NOTE: I needed this in my specific project (code separated out into different DLLs). The demo project, as it
// stands, does not require it. Leaving it here in case you end up setting your code in a similar manner and need it.
//#define GLE(ret, name, ...) ret (*gl##name)(__VA_ARGS__);
//	OPENGL_LIST
//#undef GLE

static void win32_processPendingMessages() {
	MSG message;

	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
		UINT msg = message.message;
		
		switch (msg) {
		case WM_QUIT: {
			globalRunning = false;
		} break;

		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP: {
			// TODO(JR): eventually support different keyboard layouts
			unsigned int vkCode = (unsigned int)message.wParam;

			// NOTE(JR): must use == and != to convert bit tests to actual 0 or 1 vaues for wasDown/isDown
			bool wasDown = ((message.lParam & (1 << 30)) != 0);
			bool isDown = ((message.lParam & (1 << 31)) == 0);

			if (wasDown != isDown) {
				if (msg == WM_KEYDOWN) {
					// Stub
				}
				if (message.message == WM_KEYUP) {
					// Stub
				}
			}

		} break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN: {
			// Stub
		} break;

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP: {
			// Stub
		} break;

		case WM_MOUSEMOVE: {
			// Stub
		} break;

		default: {
			TranslateMessage(&message);
			DispatchMessage(&message);
		} break;
		}
	}
}

static LRESULT CALLBACK win32_mainWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;

	switch (message) {
	case WM_CLOSE: {
		globalRunning = false;
	} break;

	case WM_DESTROY: {
		globalRunning = false;
	} break;

	case WM_ACTIVATEAPP: {

	} break;

	case WM_MOUSEWHEEL: {
		// Stub
	} break;

	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP: {
		//Assert(!"Keyboard input came in through a non-dispatch message!");
	} break;

	case WM_PAINT: {
		PAINTSTRUCT paint;
		HDC deviceContext = BeginPaint(window, &paint);
		EndPaint(window, &paint);
	} break;

	default: {
		result = DefWindowProcA(window, message, wParam, lParam);
	} break;
	}

	return result;
}

void win32_initOpenGL(HWND window) {
	HDC windowDC = GetDC(window);

	PIXELFORMATDESCRIPTOR desiredPixelFormat = {};
	desiredPixelFormat.nSize = sizeof(desiredPixelFormat);
	desiredPixelFormat.nVersion = 1;
	desiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	desiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
	desiredPixelFormat.cColorBits = 32;
	desiredPixelFormat.cAlphaBits = 8;
	desiredPixelFormat.iLayerType = PFD_MAIN_PLANE;

	int suggestedPixelFormatIndex = ChoosePixelFormat(windowDC, &desiredPixelFormat);
	PIXELFORMATDESCRIPTOR suggestedPixelFormat;
	DescribePixelFormat(windowDC, suggestedPixelFormatIndex, sizeof(suggestedPixelFormat), &suggestedPixelFormat);
	SetPixelFormat(windowDC, suggestedPixelFormatIndex, &suggestedPixelFormat);

	HGLRC legacyContext = wglCreateContext(windowDC);
	if (!legacyContext){
		int error = glGetError();
		printf("ERROR: wglCreateContext failed with code %d", error);
		return;
	}

	if (!wglMakeCurrent(windowDC, legacyContext)) {
		int error = glGetError();
		printf("ERROR: wglMakeCurrent failed with code %d", error);
	}

	int flags = WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
#if SHMUP_DEBUG
	flags |= WGL_CONTEXT_DEBUG_BIT_ARB;
#endif

	const int contextAttributes[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB, flags,
		//WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB
		= (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if (!wglCreateContextAttribsARB){
		printf("ERROR: Failed querying entry point for wglCreateContextAttribsARB!");
		return;
	}

	HGLRC renderingContext = wglCreateContextAttribsARB(windowDC, 0, contextAttributes);
	if (!renderingContext){
		int error = glGetError();
		printf("ERROR: Couldn't create rendering context! Error code is: %d", error);
		return;
	}
	
	// Destroy dummy context
	BOOL res;
	res = wglMakeCurrent(windowDC, NULL);
	res = wglDeleteContext(legacyContext);
	
	if (!wglMakeCurrent(windowDC, renderingContext)){
		int error = glGetError();
		printf("ERROR: wglMakeCurrent failed with code %d", error);
		return;
	}

	ReleaseDC(window, windowDC);

	HMODULE module = LoadLibraryA("opengl32.dll");


#define GLE(ret, name, ...) gl##name = (name##proc *) wglGetProcAddress("gl" #name); \
	if(gl##name == 0){ gl##name = (name##proc *) GetProcAddress(module, "gl" #name); } \
	if(gl##name == 0){ printf("%s didn't load\n", "gl" #name); } \
	else{ printf("%s loaded successfully\n", "gl" #name); }
	OPENGL_LIST
#undef GLE		
}

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR commandLine, int showCode) {
	if (!AllocConsole())
		MessageBoxW(NULL, L"The console window was not created", NULL, MB_ICONEXCLAMATION);
	freopen("CONOUT$", "w", stdout);

	WNDCLASSEX windowClass;

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = win32_mainWindowCallback;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = instance;
	windowClass.hIcon = LoadIcon(instance, MAKEINTRESOURCE(IDI_APPLICATION));
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = "LevelEditorWindowClass";
	windowClass.hIconSm = LoadIcon(windowClass.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (RegisterClassEx(&windowClass)) {
		globalWindow = CreateWindow(
			windowClass.lpszClassName,
			"OpenGL NoLib Example",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT,
			1600, 800,
			NULL,
			NULL,
			instance,
			NULL
		);
		
		if (globalWindow) {
			win32_initOpenGL(globalWindow);
			globalRunning = true;

			while(globalRunning){
				win32_processPendingMessages();
			}
		}
	}
}

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2018 JR Smith
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/