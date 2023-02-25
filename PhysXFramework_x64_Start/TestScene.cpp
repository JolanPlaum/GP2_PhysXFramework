#include "stdafx.h"
#include "TestScene.h"

#include "CubePosColorNorm.h"
#include "TorusPosColorNorm.h"
#include "CylinderPosColorNorm.h"
#include "Logger.h"

void TestScene::Initialize()
{
	//GEOMETRY
	m_pBox = new CubePosColorNorm(5.f, 2.f, 1.f, XMFLOAT4{ Colors::Crimson });
	AddGameObject(m_pBox);

	m_pTorus = new TorusPosColorNorm(8.f, 50, 1.f, 10, XMFLOAT4{ Colors::Green });
	AddGameObject(m_pTorus);

	auto pCylinder = new CylinderPosColorNorm(1.5f, 10, 4.f, XMFLOAT4{ Colors::Blue });
	m_pTorus->AddChild(pCylinder);

	pCylinder->Translate(0.f, 8.f, 0.f);
	m_pTorus->Translate(20.f, 0.f, 0.f);


	//CAMERA
	m_SceneContext.GetCamera()->SetPosition(XMFLOAT3{ 0.f, 10.f, -25.f });
	m_SceneContext.GetCamera()->SetForward(XMFLOAT3{ 0.f, -0.3f, 1.f });


	//INPUT
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::MoveLeft, InputTriggerState::down, VK_LEFT, -1, XINPUT_GAMEPAD_DPAD_LEFT));
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::MoveRight, InputTriggerState::down, VK_RIGHT, -1, XINPUT_GAMEPAD_DPAD_RIGHT));
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::MoveUp, InputTriggerState::down, VK_UP, -1, XINPUT_GAMEPAD_DPAD_UP));
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::MoveDown, InputTriggerState::down, VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN));
}

void TestScene::Update()
{
	const float deltaT = GetSceneContext().GetGameTime()->GetElapsed();
	const float totalT = GetSceneContext().GetGameTime()->GetTotal();
	m_pBox->RotateDegrees(0.f, 90.f * totalT, 0.f);

	//XMFLOAT3 translation{ m_pBox->GetPosition() }; //option 1
	XMFLOAT3 translation{}; //option 2
	const float movementSpeed{ 10.f * deltaT };
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::MoveLeft))
	{
		translation.x -= movementSpeed;
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::MoveRight))
	{
		translation.x += movementSpeed;
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::MoveDown))
	{
		translation.y -= movementSpeed;
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::MoveUp))
	{
		translation.y += movementSpeed;
	}

	auto currPos = XMLoadFloat3(&m_pBox->GetPosition()); //option 2
	auto newPos = currPos + XMLoadFloat3(&translation); //option 2
	XMStoreFloat3(&translation, newPos); //option 2
	
	m_pBox->Translate(translation.x, translation.y, translation.z); //option 1 & 2
}

void TestScene::Draw() const
{

}

void TestScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void TestScene::OnSceneDeactivated()
{
}
