#include "PlayerTool.h"
#include "Player.h"

PlayerTool::PlayerTool()
{
}

PlayerTool::PlayerTool(const PlayerTool& obj)
    :
    Object(obj)
{
}

PlayerTool::~PlayerTool()
{
}

void PlayerTool::SetPlayer(Player* player)
{
    m_pPlayer = player;
    SetPos(player->GetPos().x + posOffsetX, player->GetPos().y + posOffsetY);
}

void PlayerTool::Play()
{
    m_bEnableAnimation = true;
    switch (eToolState)
    {
    case TOOL_PICK:
        switch (m_pPlayer->GetState())
        {
        case Player::PlayerState::TOOL_DOWN:
            m_pAnimation->ChangeClip("PickDown");
            break;
        case Player::PlayerState::TOOL_LEFT:
            m_pAnimation->ChangeClip("PickLeft");
            break;
        case Player::PlayerState::TOOL_RIGHT:
            m_pAnimation->ChangeClip("PickRight");
            break;
        case Player::PlayerState::TOOL_UP:
            m_pAnimation->ChangeClip("PickUp");
            break;
        }
        break;
    case TOOL_AXE:
        switch (m_pPlayer->GetState())
        {
        case Player::PlayerState::TOOL_DOWN:
            m_pAnimation->ChangeClip("AxeDown");
            break;
        case Player::PlayerState::TOOL_LEFT:
            m_pAnimation->ChangeClip("AxeLeft");
            break;
        case Player::PlayerState::TOOL_RIGHT:
            m_pAnimation->ChangeClip("AxeRight");
            break;
        case Player::PlayerState::TOOL_UP:
            m_pAnimation->ChangeClip("AxeUp");
            break;
        }
        break;
    case TOOL_HOE:
        switch (m_pPlayer->GetState())
        {
        case Player::PlayerState::TOOL_DOWN:
            m_pAnimation->ChangeClip("HoeDown");
            break;
        case Player::PlayerState::TOOL_LEFT:
            m_pAnimation->ChangeClip("HoeLeft");
            break;
        case Player::PlayerState::TOOL_RIGHT:
            m_pAnimation->ChangeClip("HoeRight");
            break;
        case Player::PlayerState::TOOL_UP:
            m_pAnimation->ChangeClip("HoeUp");
            break;
        }
        break;
    case TOOL_WATER:
        switch (m_pPlayer->GetState())
        {
        case Player::PlayerState::TOOL_DOWN:
            m_pAnimation->ChangeClip("WaterDown");
            break;
        case Player::PlayerState::TOOL_LEFT:
            m_pAnimation->ChangeClip("WaterLeft");
            break;
        case Player::PlayerState::TOOL_RIGHT:
            m_pAnimation->ChangeClip("WaterRight");
            break;
        case Player::PlayerState::TOOL_UP:
            m_pAnimation->ChangeClip("WaterUp");
            break;
        }
        break;
    case TOOL_SICKLE:
        break;
    case TOOL_BLADE:
        break;
    }
}

bool PlayerTool::Init()
{
    SetPivot(0.0, 1.0f);
    SetSize(192.f, 256.f);

    SetTexture("ToolEnd", L"SV/ToolEnd.bmp");
    Animation* pAnim = CreateAnimation("PlayerTool_Animation");
    SAFE_RELEASE(pAnim);

    LoadAxe();
    LoadHoe();
    LoadPick();
    LoadWater();

    return true;
}

void PlayerTool::Input(float dt)
{
    Object::Input(dt);
}

int PlayerTool::Update(float dt)
{
    Object::Update(dt);
    return 0;
}

int PlayerTool::LateUpdate(float dt)
{
    Object::LateUpdate(dt);
    SetPos(m_pPlayer->GetPos().x + posOffsetX, m_pPlayer->GetPos().y + posOffsetY);
    return 0;
}

void PlayerTool::Collision(float dt)
{
    Object::Collision(dt);
}

void PlayerTool::Draw(HDC hDC, float dt)
{
    Object::Draw(hDC, dt);
}

PlayerTool* PlayerTool::Clone()
{
    return new PlayerTool(*this);
}

void PlayerTool::StateTransit(int iState)
{
    SetTexture("ToolEnd");
}

void PlayerTool::LoadWater()
{
    wstring basePath;
    vector<wstring> vecFileName;
    wstringstream ss;

    {
        basePath = L"Player/SV/WaterTool/ToolDown/";
        vecFileName.clear();
        for (int i = 1; i <= 3; ++i)
        {
            ss << basePath << L"WaterDown" << i << ".bmp";
            vecFileName.push_back(ss.str());
            ss.clear();
            ss.str(L"");
        }
        AddAnimationClip("WaterDown",
            AT_FRAME, AO_ONCE_RETURN,
            0.0f, 0.4f,
            3, 1,
            0, 0,
            3, 1,
            0.f, "WaterDown_Anim", vecFileName);
        SetClipColorKey("WaterDown", 255, 255, 255);

        basePath = L"Player/SV/WaterTool/ToolLeft/";
        vecFileName.clear();
        for (int i = 1; i <= 3; ++i)
        {
            ss << basePath << L"WaterLeft" << i << ".bmp";
            vecFileName.push_back(ss.str());
            ss.clear();
            ss.str(L"");
        }
        AddAnimationClip("WaterLeft",
            AT_FRAME, AO_ONCE_RETURN,
            0.0f, 0.4f,
            3, 1,
            0, 0,
            3, 1,
            0.f, "WaterLeft_Anim", vecFileName);
        SetClipColorKey("WaterLeft", 255, 255, 255);

        basePath = L"Player/SV/WaterTool/ToolRight/";
        vecFileName.clear();
        for (int i = 1; i <= 3; ++i)
        {
            ss << basePath << L"WaterRight" << i << ".bmp";
            vecFileName.push_back(ss.str());
            ss.clear();
            ss.str(L"");
        }
        AddAnimationClip("WaterRight",
            AT_FRAME, AO_ONCE_RETURN,
            0.0f, 0.4f,
            3, 1,
            0, 0,
            3, 1,
            0.f, "WaterRight_Anim", vecFileName);
        SetClipColorKey("WaterRight", 255, 255, 255);

        basePath = L"Player/SV/WaterTool/ToolUp/";
        vecFileName.clear();
        for (int i = 1; i <= 3; ++i)
        {
            ss << basePath << L"WaterUp" << i << ".bmp";
            vecFileName.push_back(ss.str());
            ss.clear();
            ss.str(L"");
        }
        AddAnimationClip("WaterUp",
            AT_FRAME, AO_ONCE_RETURN,
            0.0f, 0.4f,
            3, 1,
            0, 0,
            3, 1,
            0.f, "WaterUp_Anim", vecFileName);
        SetClipColorKey("WaterUp", 255, 255, 255);
    }
}

void PlayerTool::LoadPick()
{
    wstring basePath;
    vector<wstring> vecFileName;
    wstringstream ss;

    // Axe 애니메이션
    {
        basePath = L"Player/SV/PickTool/ToolDown/";
        vecFileName.clear();
        for (int i = 1; i <= 3; ++i)
        {
            ss << basePath << L"PickDown" << i << ".bmp";
            vecFileName.push_back(ss.str());
            ss.clear();
            ss.str(L"");
        }
        AddAnimationClip("PickDown",
            AT_FRAME, AO_ONCE_RETURN,
            0.0f, 0.4f,
            3, 1,
            0, 0,
            3, 1,
            0.f, "PickDown_Anim", vecFileName);
        SetClipColorKey("PickDown", 255, 255, 255);

        basePath = L"Player/SV/PickTool/ToolLeft/";
        vecFileName.clear();
        for (int i = 1; i <= 3; ++i)
        {
            ss << basePath << L"PickLeft" << i << ".bmp";
            vecFileName.push_back(ss.str());
            ss.clear();
            ss.str(L"");
        }
        AddAnimationClip("PickLeft",
            AT_FRAME, AO_ONCE_RETURN,
            0.0f, 0.4f,
            3, 1,
            0, 0,
            3, 1,
            0.f, "PickLeft_Anim", vecFileName);
        SetClipColorKey("PickLeft", 255, 255, 255);

        basePath = L"Player/SV/PickTool/ToolRight/";
        vecFileName.clear();
        for (int i = 1; i <= 3; ++i)
        {
            ss << basePath << L"PickRight" << i << ".bmp";
            vecFileName.push_back(ss.str());
            ss.clear();
            ss.str(L"");
        }
        AddAnimationClip("PickRight",
            AT_FRAME, AO_ONCE_RETURN,
            0.0f, 0.4f,
            3, 1,
            0, 0,
            3, 1,
            0.f, "PickRight_Anim", vecFileName);
        SetClipColorKey("PickRight", 255, 255, 255);

        basePath = L"Player/SV/PickTool/ToolUp/";
        vecFileName.clear();
        for (int i = 1; i <= 3; ++i)
        {
            ss << basePath << L"PickUp" << i << ".bmp";
            vecFileName.push_back(ss.str());
            ss.clear();
            ss.str(L"");
        }
        AddAnimationClip("PickUp",
            AT_FRAME, AO_ONCE_RETURN,
            0.0f, 0.4f,
            3, 1,
            0, 0,
            3, 1,
            0.f, "PickUp_Anim", vecFileName);
        SetClipColorKey("PickUp", 255, 255, 255);
    }
}

void PlayerTool::LoadHoe()
{
    wstring basePath;
    vector<wstring> vecFileName;
    wstringstream ss;

    // Axe 애니메이션
    {
        basePath = L"Player/SV/HoeTool/ToolDown/";
        vecFileName.clear();
        for (int i = 1; i <= 3; ++i)
        {
            ss << basePath << L"HoeDown" << i << ".bmp";
            vecFileName.push_back(ss.str());
            ss.clear();
            ss.str(L"");
        }
        AddAnimationClip("HoeDown",
            AT_FRAME, AO_ONCE_RETURN,
            0.0f, 0.4f,
            3, 1,
            0, 0,
            3, 1,
            0.f, "HoeDown_Anim", vecFileName);
        SetClipColorKey("HoeDown", 255, 255, 255);

        basePath = L"Player/SV/HoeTool/ToolLeft/";
        vecFileName.clear();
        for (int i = 1; i <= 3; ++i)
        {
            ss << basePath << L"HoeLeft" << i << ".bmp";
            vecFileName.push_back(ss.str());
            ss.clear();
            ss.str(L"");
        }
        AddAnimationClip("HoeLeft",
            AT_FRAME, AO_ONCE_RETURN,
            0.0f, 0.4f,
            3, 1,
            0, 0,
            3, 1,
            0.f, "HoeLeft_Anim", vecFileName);
        SetClipColorKey("HoeLeft", 255, 255, 255);

        basePath = L"Player/SV/HoeTool/ToolRight/";
        vecFileName.clear();
        for (int i = 1; i <= 3; ++i)
        {
            ss << basePath << L"HoeRight" << i << ".bmp";
            vecFileName.push_back(ss.str());
            ss.clear();
            ss.str(L"");
        }
        AddAnimationClip("HoeRight",
            AT_FRAME, AO_ONCE_RETURN,
            0.0f, 0.4f,
            3, 1,
            0, 0,
            3, 1,
            0.f, "HoeRight_Anim", vecFileName);
        SetClipColorKey("HoeRight", 255, 255, 255);

        basePath = L"Player/SV/HoeTool/ToolUp/";
        vecFileName.clear();
        for (int i = 1; i <= 3; ++i)
        {
            ss << basePath << L"HoeUp" << i << ".bmp";
            vecFileName.push_back(ss.str());
            ss.clear();
            ss.str(L"");
        }
        AddAnimationClip("HoeUp",
            AT_FRAME, AO_ONCE_RETURN,
            0.0f, 0.4f,
            3, 1,
            0, 0,
            3, 1,
            0.f, "HoeUp_Anim", vecFileName);
        SetClipColorKey("HoeUp", 255, 255, 255);
    }
}

void PlayerTool::LoadAxe()
{
    wstring basePath;
    vector<wstring> vecFileName;
    wstringstream ss;

    // Axe 애니메이션
    {
        basePath = L"Player/SV/AxeTool/ToolDown/";
        vecFileName.clear();
        for (int i = 1; i <= 3; ++i)
        {
            ss << basePath << L"AxeDown" << i << ".bmp";
            vecFileName.push_back(ss.str());
            ss.clear();
            ss.str(L"");
        }
        AddAnimationClip("AxeDown",
            AT_FRAME, AO_ONCE_RETURN,
            0.0f, 0.4f,
            3, 1,
            0, 0,
            3, 1,
            0.f, "AxeDown_Anim", vecFileName);
        SetClipColorKey("AxeDown", 255, 255, 255);

        basePath = L"Player/SV/AxeTool/ToolLeft/";
        vecFileName.clear();
        for (int i = 1; i <= 3; ++i)
        {
            ss << basePath << L"AxeLeft" << i << ".bmp";
            vecFileName.push_back(ss.str());
            ss.clear();
            ss.str(L"");
        }
        AddAnimationClip("AxeLeft",
            AT_FRAME, AO_ONCE_RETURN,
            0.0f, 0.4f,
            3, 1,
            0, 0,
            3, 1,
            0.f, "AxeLeft_Anim", vecFileName);
        SetClipColorKey("AxeLeft", 255, 255, 255);

        basePath = L"Player/SV/AxeTool/ToolRight/";
        vecFileName.clear();
        for (int i = 1; i <= 3; ++i)
        {
            ss << basePath << L"AxeRight" << i << ".bmp";
            vecFileName.push_back(ss.str());
            ss.clear();
            ss.str(L"");
        }
        AddAnimationClip("AxeRight",
            AT_FRAME, AO_ONCE_RETURN,
            0.0f, 0.4f,
            3, 1,
            0, 0,
            3, 1,
            0.f, "AxeRight_Anim", vecFileName);
        SetClipColorKey("AxeRight", 255, 255, 255);

        basePath = L"Player/SV/AxeTool/ToolUp/";
        vecFileName.clear();
        for (int i = 1; i <= 3; ++i)
        {
            ss << basePath << L"AxeUp" << i << ".bmp";
            vecFileName.push_back(ss.str());
            ss.clear();
            ss.str(L"");
        }
        AddAnimationClip("AxeUp",
            AT_FRAME, AO_ONCE_RETURN,
            0.0f, 0.4f,
            3, 1,
            0, 0,
            3, 1,
            0.f, "AxeUp_Anim", vecFileName);
        SetClipColorKey("AxeUp", 255, 255, 255);
    }
}

