#include "stdafx.h"
#include "AudioTestScene.h"
#include "SpherePosColorNorm.h"
#include "SoundManager.h"
#include "Logger.h"

void AudioTestScene::Initialize()
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


	//SOUND 2D
	auto pFmod = SoundManager::GetInstance()->GetSystem();
	FMOD::Sound* pSound{};
	FMOD_RESULT result = pFmod->createStream("Resources/Sounds/wave.mp3", FMOD_DEFAULT | FMOD_LOOP_NORMAL, nullptr, &pSound);
	SoundManager::GetInstance()->ErrorCheck(result);
	
	result = pFmod->playSound(pSound, nullptr, true, &m_pChannel2D);
	SoundManager::GetInstance()->ErrorCheck(result);

	//SOUND 3D
	result = pFmod->createStream("Resources/Sounds/Thrones.mp3", FMOD_3D | FMOD_3D_LINEARROLLOFF, nullptr, &pSound);
	SoundManager::GetInstance()->ErrorCheck(result);
	
	result = pFmod->playSound(pSound, nullptr, false, &m_pChannel3D);
	SoundManager::GetInstance()->ErrorCheck(result);

	m_pChannel3D->setMode(FMOD_LOOP_NORMAL);
	m_pChannel3D->set3DMinMaxDistance(0.f, 100.f);


	//INPUT
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::ResetScene, InputTriggerState::pressed, 'R'));

	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::MoveLeft, InputTriggerState::down, VK_LEFT, -1, XINPUT_GAMEPAD_DPAD_LEFT));
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::MoveRight, InputTriggerState::down, VK_RIGHT, -1, XINPUT_GAMEPAD_DPAD_RIGHT));
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::MoveUp, InputTriggerState::down, VK_UP, -1, XINPUT_GAMEPAD_DPAD_UP));
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::MoveDown, InputTriggerState::down, VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN));

	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::Play2D, InputTriggerState::down, VK_SPACE, -1, XINPUT_GAMEPAD_DPAD_LEFT));
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::Play3D, InputTriggerState::down, 'B', -1, XINPUT_GAMEPAD_DPAD_RIGHT));
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::IncreaseVolume, InputTriggerState::down, VK_F7, -1, XINPUT_GAMEPAD_DPAD_UP));
	m_SceneContext.GetInput()->AddInputAction(InputAction(InputId::DecreaseVolume, InputTriggerState::down, VK_F6, -1, XINPUT_GAMEPAD_DPAD_DOWN));


	//Load the scene
	LoadScene();
}

void AudioTestScene::InitActor()
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
	PxMaterial* pDefaultMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.2f);

	//5. Create a Shape
	PxRigidActorExt::createExclusiveShape(*pActor, geometry, &pDefaultMaterial, 1);

	//6. Attach to GameObject
	m_pSphere->AttachRigidActor(pActor);
}

void AudioTestScene::LoadScene()
{
	//Reset positions
	m_pSphere->Translate(0.f, 2.f, 0.f);


	//CAMERA
	m_SceneContext.GetCamera()->SetPositionAndLookAt(XMFLOAT3{ 0.f, 13.f, -30.f }, XMFLOAT3{ 0.f, 5.f, 0.f });
}

inline FMOD_VECTOR ToFmod(XMFLOAT3 v)
{
	return FMOD_VECTOR{ v.x, v.y, v.z };
}
inline FMOD_VECTOR ToFmod(PxVec3 v)
{
	return FMOD_VECTOR{ v.x, v.y, v.z };
}

void AudioTestScene::Update()
{
	//Sphere Dynamics
	const float force{ 5.f };
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::MoveLeft))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0, 0, force });
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::MoveRight))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0, 0, -force });
	}

	//Sound Controls
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::Play2D))
	{
		bool isPaused{};
		m_pChannel2D->getPaused(&isPaused);
		m_pChannel2D->setPaused(!isPaused);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::Play3D))
	{
		bool isPaused{};
		m_pChannel3D->getPaused(&isPaused);
		m_pChannel3D->setPaused(!isPaused);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::IncreaseVolume))
	{
		float volume{};
		m_pChannel2D->getVolume(&volume);

		volume += 0.1f;
		Clamp(volume, 1.f, 0.f);

		m_pChannel2D->setVolume(volume);
		m_pChannel3D->setVolume(volume);
		Logger::GetInstance()->LogFormat(LogLevel::Info, L"Sound Volume Changed > %f", volume);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::DecreaseVolume))
	{
		float volume{};
		m_pChannel2D->getVolume(&volume);

		volume -= 0.1f;
		Clamp(volume, 1.f, 0.f);

		m_pChannel2D->setVolume(volume);
		m_pChannel3D->setVolume(volume);
		Logger::GetInstance()->LogFormat(LogLevel::Info, L"Sound Volume Changed > %f", volume);
	}

	//3D SOUND CONFIG
	// Listener
	auto pCam = m_SceneContext.GetCamera();
	auto pos = ToFmod(pCam->GetPosition());
	auto forward = ToFmod(pCam->GetForward());
	auto up = ToFmod(pCam->GetUp());

	FMOD_VECTOR vel{};
	auto deltaT = m_SceneContext.GetGameTime()->GetElapsed();
	vel.x = (pos.x - m_PrevListenerPos.x) / deltaT;
	vel.y = (pos.y - m_PrevListenerPos.y) / deltaT;
	vel.z = (pos.z - m_PrevListenerPos.z) / deltaT;
	m_PrevListenerPos = pos;

	SoundManager::GetInstance()->GetSystem()->set3DListenerAttributes(0, &pos, &vel, &forward, &up);

	//Source
	auto spherePos = ToFmod(m_pSphere->GetPosition());
	auto sphereVel = ToFmod(m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->getLinearVelocity());
	m_pChannel3D->set3DAttributes(&spherePos, &sphereVel);


	if (m_SceneContext.GetInput()->IsActionTriggered(InputId::ResetScene))
	{
		LoadScene();
	}
}

void AudioTestScene::Draw() const
{
}

void AudioTestScene::OnSceneActivated()
{
	LoadScene();
}

void AudioTestScene::OnSceneDeactivated()
{
}
