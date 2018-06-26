#include <Windows.h>
#include <stdio.h>

#include "render.h"

bool globalRunning = false;
HWND globalWindow;

//#define GLE(ret, name, ...) ret (*gl##name)(__VA_ARGS__);
//	OPENGL_LIST
//#undef GLE

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
				
			}
		}
	}
}