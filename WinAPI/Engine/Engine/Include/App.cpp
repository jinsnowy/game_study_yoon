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
			DoFrame();
			// optional to deal with some functions after frame update
			/*
			  msg.message = WM_RENDER_RESET;
			  DispatchMessage(&msg);
			*/
		}
	}

	return (int)msg.wParam;
}

void App::DoFrame()
{
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c, c, 1.0f);
 	wnd.Gfx().EndFrame();
}
