#pragma once
#include "GameScene.h"

class TriggerTestScene final : public GameScene
{
public:
	TriggerTestScene() : GameScene(L"TriggerTestScene") {}
	~TriggerTestScene() override = default;
	TriggerTestScene(const TriggerTestScene& other) = delete;
	TriggerTestScene(TriggerTestScene&& other) noexcept = delete;
	TriggerTestScene& operator=(const TriggerTestScene& other) = delete;
	TriggerTestScene& operator=(TriggerTestScene&& other) noexcept = delete;

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
		MoveUp,
		MoveDown,
	};

	void InitActor();
	void LoadScene();

	GameObject* m_pSphere{ nullptr };
	PxRigidStatic* m_pTriggerLeft{ nullptr };
	PxRigidStatic* m_pTriggerRight{ nullptr };

	bool m_IsLeftTrigger{ false };
	bool m_IsRightTrigger{ false };

};

