#pragma once
#include "GameScene.h"

class BoxTorqueScene final : public GameScene
{
public:
	BoxTorqueScene() : GameScene(L"BoxTorqueScene") {}
	~BoxTorqueScene() override = default;
	BoxTorqueScene(const BoxTorqueScene& other) = delete;
	BoxTorqueScene(BoxTorqueScene&& other) noexcept = delete;
	BoxTorqueScene& operator=(const BoxTorqueScene& other) = delete;
	BoxTorqueScene& operator=(BoxTorqueScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:

	enum InputId
	{
		ResetScene,

		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
	};

	GameObject* m_pBox{};
	PxRigidDynamic* m_pBoxActor{};

	float m_Torque{ 5.f };
	PxForceMode::Enum m_Mode{ PxForceMode::eFORCE };

	void LoadScene();

};

