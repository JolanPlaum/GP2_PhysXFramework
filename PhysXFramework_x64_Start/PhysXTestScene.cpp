#include "stdafx.h"
#include "PhysXTestScene.h"
#include "CubePosColorNorm.h"

void PhysXTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	//CUBE ACTOR
	XMFLOAT3 actorDimensions{ 1.5f, 1.f, 1.f };
	m_pCube = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z);
	AddGameObject(m_pCube);

	//1. Store main PhysX Object
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//2. Create the Actor
	m_pCubeActor = pPhysX->createRigidDynamic(PxTransform(PxIdentity));

	//3. Create a Geometry
	PxBoxGeometry boxGeometry = PxBoxGeometry{ actorDimensions.x / 2.f, actorDimensions.y / 2.f, actorDimensions.z / 2.f };

	//4. Create a Material
	PxMaterial* pBoxMaterial = pPhysX->createMaterial(0.6f, 0.6f, 0.2f);

	//PxShape* pBoxShape = pPhysX->createShape(boxGeometry, *pBoxMaterial, true);
	//m_pCubeActor->attachShape(*pBoxShape);
	//m_pPhysxScene->addActor(*m_pCubeActor);

	//5. Create a Shape
	PxRigidActorExt::createExclusiveShape(*m_pCubeActor, boxGeometry, &pBoxMaterial, 1);

	//6. Attach to GameObject
	m_pCube->AttachRigidActor(m_pCubeActor);

	m_pCube->Translate(m_CubeTranslation.x, m_CubeTranslation.y, m_CubeTranslation.z);


	//GROUND PLANE
	PxRigidStatic* pGroundActor = pPhysX->createRigidStatic(PxTransform(PxQuat{ PxPiDivTwo, PxVec3{0.f, 0.f, 1.f} }));
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, &pBoxMaterial, 1);
	m_pPhysxScene->addActor(*pGroundActor);


	//CAMERA
	m_SceneContext.GetCamera()->SetPositionAndLookAt(m_CamPos, m_CamLookAt);


	//INPUT
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::ResetScene, InputTriggerState::pressed, 'R'));
}

void PhysXTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::ResetScene))
	{
		m_pCube->Translate(m_CubeTranslation.x, m_CubeTranslation.y, m_CubeTranslation.z);

		m_SceneContext.GetCamera()->SetPositionAndLookAt(m_CamPos, m_CamLookAt);
	}
}

void PhysXTestScene::Draw() const
{
}

void PhysXTestScene::OnSceneActivated()
{
}

void PhysXTestScene::OnSceneDeactivated()
{
}
