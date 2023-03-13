#pragma once
#include "GameScene.h"

class W2_AssignmentScene final : public GameScene
{
public:
	W2_AssignmentScene() : GameScene(L"W2_AssignmentScene") {}
	~W2_AssignmentScene() override = default;
	W2_AssignmentScene(const W2_AssignmentScene& other) = delete;
	W2_AssignmentScene(W2_AssignmentScene&& other) noexcept = delete;
	W2_AssignmentScene& operator=(const W2_AssignmentScene& other) = delete;
	W2_AssignmentScene& operator=(W2_AssignmentScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;

private:

	enum InputId
	{
		ResetScene,

		MoveLeft,
		MoveRight,
		Jump,
	};

	void InitLevel(PxMaterial* pMaterial);
	void InitSphere(PxMaterial* pMaterial);
	void InitBoxes(PxMaterial* pMaterial);
	void InitHatches(PxMaterial* pMaterial);
	void InitTriggers(PxMaterial* pMaterial);
	void InitSpheresTop(PxMaterial* pMaterial);

	void LoadScene();

	GameObject* m_pTriangleLevel{ nullptr };
	GameObject* m_pSphere{ nullptr };
	FMOD::Channel* m_pChannel{};
	FMOD::Sound* m_pBells{};

	GameObject* m_pBoxLeft{ nullptr };
	GameObject* m_pBoxRight{ nullptr };
	GameObject* m_pHatchLeft{ nullptr };
	GameObject* m_pHatchRight{ nullptr };
	//PxRevoluteJoint* m_pJointLeft{ nullptr };
	//PxRevoluteJoint* m_pJointRight{ nullptr };
	PxRigidStatic* m_pTriggerLeft{ nullptr };
	PxRigidStatic* m_pTriggerRight{ nullptr };
	XMFLOAT4 m_ColorLeft{ Colors::Blue };
	XMFLOAT4 m_ColorRight{ Colors::Red };
	bool m_IsLeftTrigger{ false };
	bool m_IsRightTrigger{ false };
	float m_DeltaTimeLeft{ 0.f };
	float m_DeltaTimeRight{ 0.f };

	GameObject* m_pSphereLeft{ nullptr };
	GameObject* m_pSphereRight{ nullptr };

};

