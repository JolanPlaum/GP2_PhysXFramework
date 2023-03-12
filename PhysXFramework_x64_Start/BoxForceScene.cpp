#include "stdafx.h"
#include "BoxForceScene.h"
#include "CubePosColorNorm.h"

void BoxForceScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	//ACTORS
	XMFLOAT3 actorDimensions{ 1.f, 1.f, 1.f };
	m_pBox = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z);
	AddGameObject(m_pBox);

	//1. Store main PhysX Object
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//2. Create the Actor
	m_pBoxActor = pPhysX->createRigidDynamic(PxTransform(PxIdentity));

	//3. Create a Geometry
	auto geometry1 = PxBoxGeometry{ actorDimensions.x / 2.f, actorDimensions.y / 2.f, actorDimensions.z / 2.f };

	//4. Create a Material
	PxMaterial* pMaterial1 = pPhysX->createMaterial(1.f, 1.f, 0.f);

	//5. Create a Shape
	PxRigidActorExt::createExclusiveShape(*m_pBoxActor, geometry1, &pMaterial1, 1);

	//6. Attach to GameObject
	m_pBox->AttachRigidActor(m_pBoxActor);


	//GROUND PLANE
	PxMaterial* pPlaneMaterial = pPhysX->createMaterial(0.f, 0.f, 0.f);
	PxRigidStatic* pGroundActor = pPhysX->createRigidStatic(PxTransform(PxQuat{ PxPiDivTwo, PxVec3{0.f, 0.f, 1.f} }));
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, &pPlaneMaterial, 1);
	m_pPhysxScene->addActor(*pGroundActor);


	//INPUT
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::ResetScene, InputTriggerState::pressed, 'R'));

	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::MoveLeft, InputTriggerState::down, VK_LEFT, -1, XINPUT_GAMEPAD_DPAD_LEFT));
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::MoveRight, InputTriggerState::down, VK_RIGHT, -1, XINPUT_GAMEPAD_DPAD_RIGHT));
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::MoveUp, InputTriggerState::down, VK_UP, -1, XINPUT_GAMEPAD_DPAD_UP));
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::MoveDown, InputTriggerState::down, VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN));


	//Load the scene
	LoadScene();
}

void BoxForceScene::LoadScene()
{
	//Reset positions
	m_pBox->Translate(0.f, 0.f, 0.f);

	//CAMERA
	m_SceneContext.GetCamera()->SetPositionAndLookAt(XMFLOAT3{ 0.f, 30.f, -20.f }, XMFLOAT3{ 0.f, 0.f, 0.f });
}

void BoxForceScene::Update()
{
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::MoveLeft))
	{
		m_pBoxActor->addForce(PxVec3(-m_Force, 0.f, 0.f), m_ForceMode);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::MoveRight))
	{
		m_pBoxActor->addForce(PxVec3(m_Force, 0.f, 0.f), m_ForceMode);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::MoveDown))
	{
		m_pBoxActor->addForce(PxVec3(0.f, 0.f, -m_Force), m_ForceMode);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::MoveUp))
	{
		m_pBoxActor->addForce(PxVec3(0.f, 0.f, m_Force), m_ForceMode);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::ResetScene))
	{
		LoadScene();
	}
}

void BoxForceScene::Draw() const
{
}

void BoxForceScene::OnSceneActivated()
{
	LoadScene();
}

void BoxForceScene::OnSceneDeactivated()
{
}
