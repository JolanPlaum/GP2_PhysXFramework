#include "stdafx.h"
#include "PhysXMeshTestScene.h"
#include "MeshObject.h"
#include "MeshObjectTex.h"
#include "ContentManager.h"

void PhysXMeshTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	//ACTORS
	XMFLOAT3 actorDimensions{ 1.f, 1.f, 1.f };
	m_pConvecChair = new MeshObject(L"Resources/Meshes/Chair.ovm");
	auto pConvexMesh = ContentManager::GetInstance()->Load<PxConvexMesh>(L"Resources/Meshes/Chair.ovpc");
	AddGameObject(m_pConvecChair);


	//1. Store main PhysX Object
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//2. Create the Actor
	auto pActor = pPhysX->createRigidDynamic(PxTransform(PxIdentity));

	//3. Create a Geometry
	auto geometry = PxConvexMeshGeometry{ pConvexMesh };

	//4. Create a Material
	PxMaterial* pDefaultMaterial = pPhysX->createMaterial(.5f, .5f, .2f);

	//5. Create a Shape
	PxRigidActorExt::createExclusiveShape(*pActor, geometry, &pDefaultMaterial, 1);
	//auto centerOfMass = PxVec3{ 0.f, 5.f, 0.f };
	PxRigidBodyExt::updateMassAndInertia(*pActor, 10.f/*, &centerOfMass*/);

	//6. Attach to GameObject
	m_pConvecChair->AttachRigidActor(pActor);


	m_pTriangleChair = new MeshObject(L"Resources/Meshes/Chair.ovm");
	const auto pTriangleMeshData = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Chair.ovpt");
	AddGameObject(m_pTriangleChair);


	//1. Store main PhysX Object
	//PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//2. Create the Actor
	//auto pActor2 = pPhysX->createRigidStatic(PxTransform(PxIdentity));
	m_pTriangleActor = pPhysX->createRigidDynamic(PxTransform(PxIdentity));

	//3. Create a Geometry
	auto geometry2 = PxTriangleMeshGeometry{ pTriangleMeshData };

	//4. Create a Material
	//PxMaterial* pDefaultMaterial2 = pPhysX->createMaterial(.5f, .5f, .2f);

	//5. Create a Shape
	m_pTriangleActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	PxRigidActorExt::createExclusiveShape(*m_pTriangleActor, geometry2, &pDefaultMaterial, 1);

	//6. Attach to GameObject
	m_pTriangleChair->AttachRigidActor(m_pTriangleActor);


	//GROUND PLANE
	auto temp = pPhysX->createMaterial(0.f, 0.f, 0.f);
	PxRigidStatic* pGroundActor = pPhysX->createRigidStatic(PxTransform(PxQuat{ PxPiDivTwo, PxVec3{0.f, 0.f, 1.f} }));
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, &pDefaultMaterial, 1);
	m_pPhysxScene->addActor(*pGroundActor);


	//INPUT
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::ResetScene, InputTriggerState::pressed, 'R'));


	//Load the scene
	LoadScene();
}

void PhysXMeshTestScene::LoadScene()
{
	//Reset positions
	m_pConvecChair->Translate(0.f, 12.f, 0.f);
	//m_pConvecChair->RotateDegrees(30.f, 0.f, 0.f);

	m_pTriangleChair->Translate(10.f, 0.f, 0.f);
	m_KinematicPosition = { 10.f, 0.f, 0.f };


	//CAMERA
	m_SceneContext.GetCamera()->SetPositionAndLookAt(XMFLOAT3{ 0.f, 13.f, -30.f }, XMFLOAT3{ 0.f, 5.f, 0.f });
}

void PhysXMeshTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_KinematicPosition.x -= 5.f * m_SceneContext.GetGameTime()->GetElapsed();
		m_pTriangleActor->setKinematicTarget(PxTransform{ m_KinematicPosition });
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::ResetScene))
	{
		LoadScene();
	}
}

void PhysXMeshTestScene::Draw() const
{
}

void PhysXMeshTestScene::OnSceneActivated()
{
	LoadScene();
}

void PhysXMeshTestScene::OnSceneDeactivated()
{
}
