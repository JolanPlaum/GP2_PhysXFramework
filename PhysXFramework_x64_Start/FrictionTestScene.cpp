#include "stdafx.h"
#include "FrictionTestScene.h"
#include "CubePosColorNorm.h"

void FrictionTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	//ACTORS
	InitBoxes();
	InitRamps();


	//GROUND PLANE
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();
	auto temp = pPhysX->createMaterial(0.f, 0.f, 0.f);
	PxRigidStatic* pGroundActor = pPhysX->createRigidStatic(PxTransform(PxQuat{ PxPiDivTwo, PxVec3{0.f, 0.f, 1.f} }));
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, &temp, 1);
	m_pPhysxScene->addActor(*pGroundActor);


	//INPUT
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::ResetScene, InputTriggerState::pressed, 'R'));


	//Load the scene
	LoadScene();
}

void FrictionTestScene::InitBoxes()
{
	//ACTORS
	XMFLOAT3 actorDimensions{ 1.f, 1.f, 1.f };
	m_Box1 = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z);
	m_Box2 = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z);
	m_Box3 = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z);
	AddGameObject(m_Box1);
	AddGameObject(m_Box2);
	AddGameObject(m_Box3);

	//1. Store main PhysX Object
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//2. Create the Actor
	PxRigidDynamic* pActor1 = pPhysX->createRigidDynamic(PxTransform(PxIdentity));
	PxRigidDynamic* pActor2 = pPhysX->createRigidDynamic(PxTransform(PxIdentity));
	PxRigidDynamic* pActor3 = pPhysX->createRigidDynamic(PxTransform(PxIdentity));

	//3. Create a Geometry
	auto geometry1 = PxBoxGeometry{ actorDimensions.x / 2.f, actorDimensions.y / 2.f, actorDimensions.z / 2.f };
	auto geometry2 = PxBoxGeometry{ actorDimensions.x / 2.f, actorDimensions.y / 2.f, actorDimensions.z / 2.f };
	auto geometry3 = PxBoxGeometry{ actorDimensions.x / 2.f, actorDimensions.y / 2.f, actorDimensions.z / 2.f };

	//4. Create a Material
	PxMaterial* pMaterial1 = pPhysX->createMaterial(1.0f, 1.0f, 0.f);
	PxMaterial* pMaterial2 = pPhysX->createMaterial(0.5f, 0.5f, 0.f);
	PxMaterial* pMaterial3 = pPhysX->createMaterial(0.0f, 0.0f, 0.f);

	//5. Create a Shape
	PxRigidActorExt::createExclusiveShape(*pActor1, geometry1, &pMaterial1, 1);
	PxRigidActorExt::createExclusiveShape(*pActor2, geometry2, &pMaterial2, 1);
	PxRigidActorExt::createExclusiveShape(*pActor3, geometry3, &pMaterial3, 1);

	//6. Attach to GameObject
	m_Box1->AttachRigidActor(pActor1);
	m_Box2->AttachRigidActor(pActor2);
	m_Box3->AttachRigidActor(pActor3);
}

void FrictionTestScene::InitRamps()
{
	//ACTORS
	XMFLOAT3 actorDimensions{ 7.5f, 1.f, 2.f };
	GameObject* pGO1 = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z);
	GameObject* pGO2 = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z);
	GameObject* pGO3 = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z);
	AddGameObject(pGO1);
	AddGameObject(pGO2);
	AddGameObject(pGO3);

	//1. Store main PhysX Object
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//2. Create the Actor
	PxRigidStatic* pActor1 = pPhysX->createRigidStatic(PxTransform(PxIdentity));
	PxRigidStatic* pActor2 = pPhysX->createRigidStatic(PxTransform(PxIdentity));
	PxRigidStatic* pActor3 = pPhysX->createRigidStatic(PxTransform(PxIdentity));

	//3. Create a Geometry
	auto geometry1 = PxBoxGeometry{ actorDimensions.x / 2.f, actorDimensions.y / 2.f, actorDimensions.z / 2.f };
	auto geometry2 = PxBoxGeometry{ actorDimensions.x / 2.f, actorDimensions.y / 2.f, actorDimensions.z / 2.f };
	auto geometry3 = PxBoxGeometry{ actorDimensions.x / 2.f, actorDimensions.y / 2.f, actorDimensions.z / 2.f };

	//4. Create a Material
	PxMaterial* pMaterial1 = pPhysX->createMaterial(0.f, 0.f, 0.f);
	PxMaterial* pMaterial2 = pPhysX->createMaterial(0.f, 0.f, 0.f);
	PxMaterial* pMaterial3 = pPhysX->createMaterial(0.f, 0.f, 0.f);

	//5. Create a Shape
	PxRigidActorExt::createExclusiveShape(*pActor1, geometry1, &pMaterial1, 1);
	PxRigidActorExt::createExclusiveShape(*pActor2, geometry2, &pMaterial2, 1);
	PxRigidActorExt::createExclusiveShape(*pActor3, geometry3, &pMaterial3, 1);

	//6. Attach to GameObject
	pGO1->AttachRigidActor(pActor1);
	pGO2->AttachRigidActor(pActor2);
	pGO3->AttachRigidActor(pActor3);


	pGO1->Translate(-10.f, 5.f, 0.f);
	pGO2->Translate(0.f,   5.f, 0.f);
	pGO3->Translate(10.f,  5.f, 0.f);
	pGO1->RotateDegrees(0.f, 0.f, -30.f);
	pGO2->RotateDegrees(0.f, 0.f, -30.f);
	pGO3->RotateDegrees(0.f, 0.f, -30.f);
}

void FrictionTestScene::LoadScene()
{
	//Reset positions
	m_Box1->Translate(-10.f, 10.f, 0.f);
	m_Box2->Translate(0.f, 10.f, 0.f);
	m_Box3->Translate(10.f, 10.f, 0.f);
	m_Box1->RotateDegrees(0.f, 0.f, -30.f);
	m_Box2->RotateDegrees(0.f, 0.f, -30.f);
	m_Box3->RotateDegrees(0.f, 0.f, -30.f);

	//CAMERA
	m_SceneContext.GetCamera()->SetPositionAndLookAt(XMFLOAT3{ 0.f, 13.f, -30.f }, XMFLOAT3{ 0.f, 5.f, 0.f });
}

void FrictionTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::ResetScene))
	{
		LoadScene();
	}
}

void FrictionTestScene::Draw() const
{
}

void FrictionTestScene::OnSceneActivated()
{
	LoadScene();
}

void FrictionTestScene::OnSceneDeactivated()
{
}
