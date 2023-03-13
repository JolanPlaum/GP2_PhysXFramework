#include "stdafx.h"
#include "W2_AssignmentScene.h"
#include "CubePosColorNorm.h"
#include "SpherePosColorNorm.h"
#include "SoundManager.h"
#include "MeshObject.h"
#include "MeshObjectTex.h"
#include "ContentManager.h"

void W2_AssignmentScene::Initialize()
{
	EnablePhysxDebugRendering(true);
	m_pPhysxScene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LIMITS, 1.0f);


	//1. Store main PhysX Object
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();

	// Create a Material
	PxMaterial* pDefaultMaterial = pPhysX->createMaterial(1.f, 1.f, 0.f);
	InitLevel(pPhysX->createMaterial(1.f, 1.f, 0.f));
	InitSphere(pDefaultMaterial);
	InitBoxes(pPhysX->createMaterial(0.f, 0.f, 0.f));
	InitHatches(pDefaultMaterial);
	InitTriggers(pDefaultMaterial);
	InitSpheresTop(pDefaultMaterial);


	//GROUND PLANE
	auto temp = pPhysX->createMaterial(0.f, 0.f, 0.f);
	PxRigidStatic* pGroundActor = pPhysX->createRigidStatic(PxTransform(PxQuat{ PxPiDivTwo, PxVec3{0.f, 0.f, 1.f} }));
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, &temp, 1);
	m_pPhysxScene->addActor(*pGroundActor);


	//INPUT
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::ResetScene, InputTriggerState::pressed, 'R'));

	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::MoveLeft, InputTriggerState::down, VK_LEFT, -1, XINPUT_GAMEPAD_DPAD_LEFT));
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::MoveRight, InputTriggerState::down, VK_RIGHT, -1, XINPUT_GAMEPAD_DPAD_RIGHT));

	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::Jump, InputTriggerState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A));

	//SOUND
	auto pFmod = SoundManager::GetInstance()->GetSystem();
	FMOD_RESULT result = pFmod->createStream("Resources/Sounds/bell.mp3", FMOD_DEFAULT, nullptr, &m_pBells);
	SoundManager::GetInstance()->ErrorCheck(result);


	//Load the scene
	LoadScene();
}

void W2_AssignmentScene::InitLevel(PxMaterial* pMaterial)
{
	m_pTriangleLevel = new MeshObject(L"Resources/Meshes/Level.ovm");
	const auto pTriangleMeshData = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Level.ovpt");
	AddGameObject(m_pTriangleLevel);


	//1. Store main PhysX Object
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//2. Create the Actor
	auto pActor = pPhysX->createRigidStatic(PxTransform(PxIdentity));

	//3. Create a Geometry
	auto geometry = PxTriangleMeshGeometry{ pTriangleMeshData };

	////4. Create a Material
	//PxMaterial* pDefaultMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.2f);

	//5. Create a Shape
	PxRigidActorExt::createExclusiveShape(*pActor, geometry, &pMaterial, 1);

	//6. Attach to GameObject
	m_pTriangleLevel->AttachRigidActor(pActor);
}

void W2_AssignmentScene::InitSphere(PxMaterial* pMaterial)
{
	//ACTORS
	m_pSphere = new SpherePosColorNorm(1.f, 25, 25, XMFLOAT4{ Colors::Gray });
	AddGameObject(m_pSphere);

	//1. Store main PhysX Object
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//2. Create the Actor
	auto pActor = pPhysX->createRigidDynamic(PxTransform(0.f, 0.f, 0.f));

	//3. Create a Geometry
	auto geometry = PxSphereGeometry{ 1.f };

	//5. Create a Shape
	PxRigidActorExt::createExclusiveShape(*pActor, geometry, &pMaterial, 1);
	PxRigidBodyExt::updateMassAndInertia(*pActor, 5.f);

	//6. Attach to GameObject
	m_pSphere->AttachRigidActor(pActor);

	//Lock Sphere's Z-Axis
	auto d6Joint = PxD6JointCreate(*pPhysX, nullptr, PxTransform(PxIdentity), pActor, PxTransform(PxIdentity));
	d6Joint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);
}

void W2_AssignmentScene::InitBoxes(PxMaterial* pMaterial)
{
	//ACTORS
	XMFLOAT3 actorDimensions{ 1.5f, 1.5f, 1.5f };
	m_pBoxLeft = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z, m_ColorLeft);
	m_pBoxRight = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z, m_ColorRight);
	AddGameObject(m_pBoxLeft);
	AddGameObject(m_pBoxRight);

	//1. Store main PhysX Object
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//2. Create the Actor
	auto pActorLeft = pPhysX->createRigidDynamic(PxTransform(PxIdentity));
	auto pActorRight = pPhysX->createRigidDynamic(PxTransform(PxIdentity));

	//3. Create a Geometry
	auto geometry = PxBoxGeometry{ actorDimensions.x / 2.f, actorDimensions.y / 2.f, actorDimensions.z / 2.f };

	//5. Create a Shape
	PxRigidActorExt::createExclusiveShape(*pActorLeft, geometry, &pMaterial, 1);
	PxRigidActorExt::createExclusiveShape(*pActorRight, geometry, &pMaterial, 1);
	PxRigidBodyExt::updateMassAndInertia(*pActorLeft, 1.f);
	PxRigidBodyExt::updateMassAndInertia(*pActorRight, 1.f);

	//6. Attach to GameObject
	m_pBoxLeft->AttachRigidActor(pActorLeft);
	m_pBoxRight->AttachRigidActor(pActorRight);
}

void W2_AssignmentScene::InitHatches(PxMaterial* pMaterial)
{
	//ACTORS
	XMFLOAT3 actorDimensions{ 2.f, 0.2f, 5.f };
	m_pHatchLeft = new CubePosColorNorm(actorDimensions.x , actorDimensions.y, actorDimensions.z, m_ColorLeft);
	m_pHatchRight = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z, m_ColorRight);
	AddGameObject(m_pHatchLeft);
	AddGameObject(m_pHatchRight);

	//1. Store main PhysX Object
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//2. Create the Actor
	auto pActorLeft = pPhysX->createRigidDynamic(PxTransform(PxIdentity));
	auto pActorRight = pPhysX->createRigidDynamic(PxTransform(PxIdentity));

	//3. Create a Geometry
	auto geometry = PxBoxGeometry{ actorDimensions.x / 2.f, actorDimensions.y / 2.f, actorDimensions.z / 2.f };

	//5. Create a Shape
	pActorLeft->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	pActorRight->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	PxRigidActorExt::createExclusiveShape(*pActorLeft, geometry, &pMaterial, 1);
	PxRigidActorExt::createExclusiveShape(*pActorRight, geometry, &pMaterial, 1);

	//6. Attach to GameObject
	m_pHatchLeft->AttachRigidActor(pActorLeft);
	m_pHatchRight->AttachRigidActor(pActorRight);
	//m_pJointLeft = PxRevoluteJointCreate(*pPhysX, pActorLeft, PxTransform{ 1.f, 0.f, 0.f }, NULL, PxTransform{ -8.f, 17.f, 0.f, PxQuat{PxPiDivTwo, PxVec3{0.f, 1.f, 0.f}} });
	//m_pJointLeft = PxRevoluteJointCreate(*pPhysX, NULL, PxTransform{ -8.f, 17.f, 0.f }, pActorLeft, PxTransform{ -8.f, 17.f, 0.f });
	//m_pJointRight = PxRevoluteJointCreate(*pPhysX, pActorRight, PxTransform{ 1.f, 0.f, 0.f }, NULL, PxTransform{ 8.f, 17.f, 0.f });

	//m_pJointLeft->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
	//m_pJointLeft->setLimit(PxJointAngularLimitPair(0.f, PxPiDivTwo, 0.01f));
	//m_pJointLeft->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);

	//m_pJointRight->setLimit(PxJointAngularLimitPair(-PxPiDivTwo, 0.f, 0.01f));
	//m_pJointRight->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
	//m_pJointRight->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);
}

void W2_AssignmentScene::InitTriggers(PxMaterial* pMaterial)
{
	//ACTOR
	XMFLOAT3 actorDimensions{ 2.f, 1.f, 5.f };

	//1. Store main PhysX Object
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//2. Create a Geometry
	auto geometry = PxBoxGeometry{ actorDimensions.x / 2.f, actorDimensions.y / 2.f, actorDimensions.z / 2.f };


	//TRIGGER LEFT
	GameObject* pLeftTrigger = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z, m_ColorLeft);
	AddGameObject(pLeftTrigger);
	m_pTriggerLeft = pPhysX->createRigidStatic(PxTransform{ -7.5f, 3.5f, 0.f });
	auto pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerLeft, geometry, *pMaterial);
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false); //NO SIMULATION
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true); //BE A TRIGGER (CAN'T BE SIMULATION IF TRIGGER)
	pLeftTrigger->AttachRigidActor(m_pTriggerLeft);
	pLeftTrigger->Translate(-7.5f, 2.5f, 0.f);


	//TRIGGER RIGHT
	GameObject* pRightTrigger = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z, m_ColorRight);
	AddGameObject(pRightTrigger);
	m_pTriggerRight = pPhysX->createRigidStatic(PxTransform{ 6.5f, 3.5f, 0.f });
	pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerRight, geometry, *pMaterial);
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false); //NO SIMULATION
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true); //BE A TRIGGER (CAN'T BE SIMULATION IF TRIGGER)
	pRightTrigger->AttachRigidActor(m_pTriggerRight);
	pRightTrigger->Translate(6.5f, 2.5f, 0.f);
}

void W2_AssignmentScene::InitSpheresTop(PxMaterial* pMaterial)
{
	//ACTORS
	m_pSphereLeft = new SpherePosColorNorm(0.75f, 10, 10, XMFLOAT4{ Colors::Gray });
	m_pSphereRight = new SpherePosColorNorm(0.75f, 10, 10, XMFLOAT4{ Colors::Gray });
	AddGameObject(m_pSphereLeft);
	AddGameObject(m_pSphereRight);

	//1. Store main PhysX Object
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//2. Create the Actor
	auto pActor1 = pPhysX->createRigidDynamic(PxTransform(0.f, 0.f, 0.f));
	auto pActor2 = pPhysX->createRigidDynamic(PxTransform(0.f, 0.f, 0.f));

	//3. Create a Geometry
	auto geometry = PxSphereGeometry{ 0.75f };

	//5. Create a Shape
	PxRigidActorExt::createExclusiveShape(*pActor1, geometry, &pMaterial, 1);
	PxRigidActorExt::createExclusiveShape(*pActor2, geometry, &pMaterial, 1);

	//6. Attach to GameObject
	m_pSphereLeft->AttachRigidActor(pActor1);
	m_pSphereRight->AttachRigidActor(pActor2);
}

void W2_AssignmentScene::LoadScene()
{
	//Reset positions
	m_pSphere->Translate(0.f, 15.f, 0.f);
	m_pSphere->Rotate(0.f, 0.f, 0.f);

	m_pSphereLeft->Translate(-6.f, 20.f, 0.f);
	m_pSphereRight->Translate(6.f, 20.f, 0.f);

	m_pBoxLeft->Translate(-3.f, 7.f, 0.f);
	m_pBoxRight->Translate(3.f, 7.f, 0.f);
	m_pHatchLeft->Translate(-9.f, 17.f, 0.f);
	m_pHatchRight->Translate(9.f, 17.f, 0.f);

	m_pBoxLeft->Rotate(0.f, 0.f, 0.f);
	m_pBoxRight->Rotate(0.f, 0.f, 0.f);
	m_pHatchLeft->Rotate(0.f, 0.f, 0.f);
	m_pHatchRight->Rotate(0.f, 0.f, 0.f);
	m_IsLeftTrigger = false;
	m_IsRightTrigger = false;


	//CAMERA
	m_SceneContext.GetCamera()->SetPositionAndLookAt(XMFLOAT3{ 0.f, 15.f, -40.f }, XMFLOAT3{ 0.f, 10.f, 0.f });
}

void W2_AssignmentScene::Update()
{
	float force{ 100.f };
	float jumpForce{ 100.f };
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::Jump))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce(PxVec3{ 0.f, jumpForce, 0.f }, PxForceMode::eIMPULSE);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::MoveLeft))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque(PxVec3{ 0.f, 0.f, force });
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::MoveRight))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque(PxVec3{ 0.f, 0.f, -force });
	}

	float dt = m_SceneContext.GetGameTime()->GetElapsed();
	if (m_IsLeftTrigger)
	{
		m_DeltaTimeLeft += dt;
		if (m_DeltaTimeLeft >= 1.f)
		{
			m_DeltaTimeLeft = 1.f;
			m_IsLeftTrigger = false;
		}
		float angle = PxPiDivTwo * m_DeltaTimeLeft;
		m_pHatchLeft->Rotate(0.f, 0.f, angle);
		PxVec3 pos{ -8.f, 17.f, 0.f };
		pos.x -= cosf(angle);
		pos.y -= sinf(angle);
		m_pHatchLeft->Translate(pos.x, pos.y, pos.z);
	}
	if (m_IsRightTrigger)
	{
		m_DeltaTimeRight += dt;
		if (m_DeltaTimeRight >= 1.f)
		{
			m_DeltaTimeRight = 1.f;
			m_IsRightTrigger = false;
		}
		float angle = -PxPiDivTwo * m_DeltaTimeRight;
		m_pHatchRight->Rotate(0.f, 0.f, angle);
		PxVec3 pos{ 8.f, 17.f, 0.f };
		pos.x += cosf(angle);
		pos.y += sinf(angle);
		m_pHatchRight->Translate(pos.x, pos.y, pos.z);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::ResetScene))
	{
		LoadScene();
	}
}

void W2_AssignmentScene::Draw() const
{
}

void W2_AssignmentScene::OnSceneActivated()
{
	LoadScene();
}

void W2_AssignmentScene::OnSceneDeactivated()
{
}

void W2_AssignmentScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (PxU32 i{ 0 }; i < count; ++i)
	{
		//ignore if deleted
		if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_OTHER | PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER)) continue;

		const PxTriggerPair& pair = pairs[i];
		if (pair.triggerActor == m_pTriggerLeft && reinterpret_cast<GameObject*>(pair.otherActor->userData) == m_pBoxLeft)
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) // ENTER
			{
				Logger::GetInstance()->LogInfo(L"ENTERED Trigger Left");
				m_IsLeftTrigger = true;
				SoundManager::GetInstance()->GetSystem()->playSound(m_pBells, nullptr, false, &m_pChannel);
			}
		}
		else if (pair.triggerActor == m_pTriggerRight && reinterpret_cast<GameObject*>(pair.otherActor->userData) == m_pBoxRight)
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) // ENTER
			{
				Logger::GetInstance()->LogInfo(L"ENTERED Trigger Right");
				m_IsRightTrigger = true;
				SoundManager::GetInstance()->GetSystem()->playSound(m_pBells, nullptr, false, &m_pChannel);
			}
		}
	}
}
