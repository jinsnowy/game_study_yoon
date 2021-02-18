#pragma once
#include "../framework.h"
#include "Window.h"

class App
{
public:
	App();
	~App();
	// master frame / message loop
	int Go();
private:
	void Process();
	void Input(float dt);
	void Update(float dt);
	void LateUpdate(float dt);
	void Collision(float dt);
	void Draw(float dt);
	void Present();
private:
	Window wnd;
};

