#include "stdafx.h"
#include "BouncingBallScene.h"
#include "SpherePosColorNorm.h"

void BouncingBallScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	//ACTORS
	float radius{ 1.f };
	AddGameObject(new SpherePosColorNorm(radius, 10, 10, XMFLOAT4(Colors::Green)));
	AddGameObject(new SpherePosColorNorm(radius, 10, 10, XMFLOAT4(Colors::Green)));
	AddGameObject(new SpherePosColorNorm(radius, 10, 10, XMFLOAT4(Colors::Green)));

	//1. Store main PhysX Object
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//2. Create the Actor
	PxRigidDynamic* pActorBounce = pPhysX->createRigidDynamic(PxTransform(PxIdentity));
	PxRigidDynamic* pActorHalf = pPhysX->createRigidDynamic(PxTransform(PxIdentity));
	PxRigidDynamic* pActorNot = pPhysX->createRigidDynamic(PxTransform(PxIdentity));

	//3. Create a Geometry
	auto geometry1 = PxSphereGeometry{ radius };
	auto geometry2 = PxSphereGeometry{ radius };
	auto geometry3 = PxSphereGeometry{ radius };

	//4. Create a Material
	PxMaterial* pBouncing = pPhysX->createMaterial(0.5f, 0.5f, 1.f);
	PxMaterial* pHalfBouncing = pPhysX->createMaterial(0.5f, 0.5f, 0.5f);
	PxMaterial* pNotBouncing = pPhysX->createMaterial(0.5f, 0.5f, 0.f);

	//5. Create a Shape
	PxRigidActorExt::createExclusiveShape(*pActorBounce, geometry1, &pBouncing, 1);
	PxRigidActorExt::createExclusiveShape(*pActorHalf, geometry2, &pHalfBouncing, 1);
	PxRigidActorExt::createExclusiveShape(*pActorNot, geometry3, &pNotBouncing, 1);

	//6. Attach to GameObject
	m_GameObjects[0]->AttachRigidActor(pActorBounce);
	m_GameObjects[1]->AttachRigidActor(pActorHalf);
	m_GameObjects[2]->AttachRigidActor(pActorNot);


	//GROUND PLANE
	PxRigidStatic* pGroundActor = pPhysX->createRigidStatic(PxTransform(PxQuat{ PxPiDivTwo, PxVec3{0.f, 0.f, 1.f} }));
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, &pHalfBouncing, 1);
	m_pPhysxScene->addActor(*pGroundActor);


	//INPUT
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::ResetScene, InputTriggerState::pressed, 'R'));

	
	//Load the scene
	LoadScene();
}

void BouncingBallScene::LoadScene()
{
	//Reset positions
	m_GameObjects[0]->Translate(-5.f, 10.f, 0.f);
	m_GameObjects[1]->Translate(0.f,  10.f, 0.f);
	m_GameObjects[2]->Translate(5.f,  10.f, 0.f);

	//CAMERA
	m_SceneContext.GetCamera()->SetPositionAndLookAt(XMFLOAT3{ 0.f, 8.f, -30.f }, XMFLOAT3{});
}

void BouncingBallScene::Update()
{
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::ResetScene))
	{
		LoadScene();
	}
}

void BouncingBallScene::Draw() const
{
}

void BouncingBallScene::OnSceneActivated()
{
	LoadScene();
}

void BouncingBallScene::OnSceneDeactivated()
{
}
