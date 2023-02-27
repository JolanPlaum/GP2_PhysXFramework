#include "stdafx.h"
#include "TriggerTestScene.h"
#include "SpherePosColorNorm.h"
#include "Logger.h"

void TriggerTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	//ACTORS
	InitActor();


	//GROUND PLANE
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();
	auto temp = pPhysX->createMaterial(1.f, 1.f, 0.f);
	PxRigidStatic* pGroundActor = pPhysX->createRigidStatic(PxTransform(PxQuat{ PxPiDivTwo, PxVec3{0.f, 0.f, 1.f} }));
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, &temp, 1);
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

void TriggerTestScene::InitActor()
{
	//ACTORS
	XMFLOAT3 actorDimensions{ 1.f, 1.f, 1.f };
	m_pSphere = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray });
	AddGameObject(m_pSphere);


	//1. Store main PhysX Object
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//2. Create the Actor
	auto pActor = pPhysX->createRigidDynamic(PxTransform(0.f, 2.f, 0.f));

	//3. Create a Geometry
	auto geometry = PxSphereGeometry{ 1.f };

	//4. Create a Material
	PxMaterial* pDefaultMaterial = pPhysX->createMaterial(100000.f, 0.f, 0.f);

	//5. Create a Shape
	PxRigidActorExt::createExclusiveShape(*pActor, geometry, &pDefaultMaterial, 1);

	//6. Attach to GameObject
	m_pSphere->AttachRigidActor(pActor);


	//TRIGGER LEFT
	m_pTriggerLeft = pPhysX->createRigidStatic(PxTransform{ -8.f, 0.5f, 0.f });
	auto pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerLeft, PxBoxGeometry{ 0.5f, 0.5f, 0.5f }, *pDefaultMaterial);
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	m_pPhysxScene->addActor(*m_pTriggerLeft);


	//TRIGGER RIGHT
	m_pTriggerRight = pPhysX->createRigidStatic(PxTransform{ 8.f, 0.5f, 0.f });
	pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerRight, PxBoxGeometry{ 0.5f, 0.5f, 0.5f }, *pDefaultMaterial);
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	m_pPhysxScene->addActor(*m_pTriggerRight);
}

void TriggerTestScene::LoadScene()
{
	//Reset positions
	m_pSphere->Translate(0.f, 2.f, 0.f);


	//CAMERA
	m_SceneContext.GetCamera()->SetPositionAndLookAt(XMFLOAT3{ 0.f, 13.f, -30.f }, XMFLOAT3{ 0.f, 5.f, 0.f });
}

void TriggerTestScene::Update()
{
	const float force{ 5.f };
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::MoveLeft))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0, 0, force });
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::MoveRight))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0, 0, -force });
	}


	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::ResetScene))
	{
		LoadScene();
	}
}

void TriggerTestScene::Draw() const
{
}

void TriggerTestScene::OnSceneActivated()
{
	LoadScene();
}

void TriggerTestScene::OnSceneDeactivated()
{
}

void TriggerTestScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (PxU32 i{ 0 }; i < count; ++i)
	{
		//ignore if deleted
		if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_OTHER | PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER)) continue;

		const PxTriggerPair& pair = pairs[i];
		if (pair.triggerActor == m_pTriggerLeft)
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) // ENTER
			{
				Logger::GetInstance()->LogInfo(L"ENTERED Trigger Left");
				m_IsLeftTrigger = true;
			}
			else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST) // LEFT
			{
				Logger::GetInstance()->LogInfo(L"LEFT Trigger Left");
			}
		}
		else if (pair.triggerActor == m_pTriggerRight)
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) // ENTER
			{
				Logger::GetInstance()->LogInfo(L"ENTERED Trigger Right");
				m_IsRightTrigger = true;
			}
			else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST) // LEFT
			{
				Logger::GetInstance()->LogInfo(L"LEFT Trigger Right");
			}
		}
	}
}
