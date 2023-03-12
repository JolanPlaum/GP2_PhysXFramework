#include "stdafx.h"
#include "W1_AssignmentScene.h"
#include "CubePosColorNorm.h"
#include "SpherePosColorNorm.h"
#include "PhysxHelpers.h"

void W1_AssignmentScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	InitFloor();
	InitPlayerSphere();
	InitWall();


	//INPUT
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::ResetScene, InputTriggerState::pressed, 'R'));

	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::MoveLeft, InputTriggerState::down, VK_LEFT, -1, XINPUT_GAMEPAD_DPAD_LEFT));
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::MoveRight, InputTriggerState::down, VK_RIGHT, -1, XINPUT_GAMEPAD_DPAD_RIGHT));
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::MoveUp, InputTriggerState::down, VK_UP, -1, XINPUT_GAMEPAD_DPAD_UP));
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::MoveDown, InputTriggerState::down, VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN));

	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::Jump, InputTriggerState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A));


	//Load the scene
	LoadScene();
}

void W1_AssignmentScene::LoadScene()
{
	//Reset positions
	m_pSphere->Translate(0.f, 2.f, -40.f);
	m_pSphere->Rotate(0.f, 0.f, 0.f);
	m_pSphere->Scale(1.f, 1.f, 1.f);

	if (m_pBlocks.size() == m_WallSize * m_WallSize)
	{
		float xBoxSize{ 2.f }, yBoxSize{ 1.6f };
		float xOffset{ -xBoxSize * m_WallSize / 2.f };
		float yOffset{ yBoxSize / 2.f };
		for (int i{}; i < m_pBlocks.size(); ++i)
		{
			int row{ i / m_WallSize };
			int col{ i % m_WallSize };

			m_pBlocks[i]->Translate((row * xBoxSize) + xOffset, (col * yBoxSize) + yOffset, 0.f);
			m_pBlocks[i]->RotateDegrees(0.f, RandomFloat(-30.f, 30.f), 0.f);
			m_pBlocks[i]->Scale(1.f, 1.f, 1.f);
		}
	}


	//CAMERA
	m_SceneContext.GetCamera()->SetPositionAndLookAt(XMFLOAT3{ -10.f, 15.f, -60.f }, XMFLOAT3{ 0.f, 0.f, 0.f });
}

void W1_AssignmentScene::Update()
{
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::Jump))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce(PxVec3{ 0.f, m_Force, 0.f }, PxForceMode::eIMPULSE);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::MoveLeft))
	{
		auto direction = ToPxVec3(m_SceneContext.GetCamera()->GetForward()) * m_Force;
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque(direction, m_ForceMode);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::MoveRight))
	{
		auto direction = ToPxVec3(m_SceneContext.GetCamera()->GetForward()) * -m_Force;
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque(direction, m_ForceMode);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::MoveDown))
	{
		auto direction = ToPxVec3(m_SceneContext.GetCamera()->GetRight()) * -m_Force;
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque(direction, m_ForceMode);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::MoveUp))
	{
		auto direction = ToPxVec3(m_SceneContext.GetCamera()->GetRight()) * m_Force;
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque(direction, m_ForceMode);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::ResetScene))
	{
		LoadScene();
	}
}

void W1_AssignmentScene::Draw() const
{
}

void W1_AssignmentScene::OnSceneActivated()
{
	LoadScene();
}

void W1_AssignmentScene::OnSceneDeactivated()
{
}

void W1_AssignmentScene::InitFloor()
{
	//ACTOR
	XMFLOAT3 dimensions{ 100.f, 1.f, 100.f };
	CubePosColorNorm* pFloor = new CubePosColorNorm(dimensions.x, dimensions.y, dimensions.z, XMFLOAT4{ Colors::LightGray });
	AddGameObject(pFloor);

	//1. Store main PhysX Object
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//2. Create the Actor
	PxRigidStatic* pFloorActor = pPhysX->createRigidStatic(PxTransform(PxIdentity));

	//3. Create a Geometry
	auto geometry1 = PxBoxGeometry{ dimensions.x / 2.f, dimensions.y / 2.f, dimensions.z / 2.f };

	//4. Create a Material
	PxMaterial* pMaterial1 = pPhysX->createMaterial(1.f, 1.f, 0.f);

	//5. Create a Shape
	PxRigidActorExt::createExclusiveShape(*pFloorActor, geometry1, &pMaterial1, 1);

	//6. Attach to GameObject
	pFloor->AttachRigidActor(pFloorActor);

	//Positions
	pFloor->Translate(0.f, -dimensions.y / 2.f, 0.f);
}

void W1_AssignmentScene::InitPlayerSphere()
{
	//ACTOR
	float radius{ 1.5f };
	m_pSphere = new SpherePosColorNorm(radius, 25, 25, XMFLOAT4{ Colors::Yellow });
	AddGameObject(m_pSphere);

	//1. Store main PhysX Object
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//2. Create the Actor
	PxRigidDynamic* pSphereActor = pPhysX->createRigidDynamic(PxTransform(PxIdentity));

	//3. Create a Geometry
	auto geometry = PxSphereGeometry{ radius };

	//4. Create a Material
	PxMaterial* pMaterial = pPhysX->createMaterial(0.5f, 1.f, 0.f);

	//5. Create a Shape
	PxRigidActorExt::createExclusiveShape(*pSphereActor, geometry, &pMaterial, 1);
	auto centerOfMass = PxVec3{ 0.f, radius, 0.f };
	PxRigidBodyExt::updateMassAndInertia(*pSphereActor, 10.f);

	//6. Attach to GameObject
	m_pSphere->AttachRigidActor(pSphereActor);
}

void W1_AssignmentScene::InitWall()
{
	//Actor dimensions
	XMFLOAT3 actorDimensions{ 1.5f, 1.5f, 1.5f };
	m_pBlocks.reserve(m_WallSize * m_WallSize);

	//1. Store main PhysX Object
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//Create wall
	for (int i{}; i < m_WallSize * m_WallSize; ++i)
	{
		//ACTOR
		auto pBox = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z);
		m_pBlocks.push_back(pBox);
		AddGameObject(pBox);

		//2. Create a Geometry
		auto geometry = PxBoxGeometry{ actorDimensions.x / 2.f, actorDimensions.y / 2.f, actorDimensions.z / 2.f };

		//3. Create a Material
		PxMaterial* pMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.1f);

		//4. Create the Actor
		PxRigidDynamic* pBoxActor = pPhysX->createRigidDynamic(PxTransform(PxIdentity));

		//5. Create a Shape
		PxRigidActorExt::createExclusiveShape(*pBoxActor, geometry, &pMaterial, 1);

		//6. Attach to GameObject
		pBox->AttachRigidActor(pBoxActor);
	}
}

float W1_AssignmentScene::RandomFloat(float min, float max)
{
	float range = max - min;
	return (range * (float(rand()) / RAND_MAX)) + min;
}