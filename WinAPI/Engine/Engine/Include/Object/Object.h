#pragma once
#include "../framework.h"
#include "../Application/Graphics.h"

class Object
{
protected:
	Pos m_Pos;
	Size  m_Size;
	std::string m_Tag;
public:
	Object();
	virtual ~Object();
	Pos GetPos() const { return m_Pos; }
	Size GetSize() const { return m_Size; }
	std::string GetTag() const { return m_Tag; }
	void SetPos(const Pos& pos) { m_Pos = pos; }
	void SetSize(const Size& size) { m_Size = size; }
	void SetTag(const std::string& tag) { m_Tag = tag; }
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(Graphics& gfx, float dt);
};

