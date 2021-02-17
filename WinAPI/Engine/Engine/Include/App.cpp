#include "App.h"

App::App()
    : wnd(800, 600, "My First Game")
{
}

int App::Go()
{
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				return -1;
			}
		}
		else {
			// Game Frame goes

			msg.message = WM_RENDER_RESET;
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

void App::DoFrame()
{
}
