#pragma once
#include "GameScene.h"

class BoxForceScene final : public GameScene
{
public:
	BoxForceScene() : GameScene(L"BoxForceScene") {}
	~BoxForceScene() override = default;
	BoxForceScene(const BoxForceScene& other) = delete;
	BoxForceScene(BoxForceScene&& other) noexcept = delete;
	BoxForceScene& operator=(const BoxForceScene& other) = delete;
	BoxForceScene& operator=(BoxForceScene&& other) noexcept = delete;

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

	float m_Force{ 5.f };
	PxForceMode::Enum m_ForceMode{ PxForceMode::eFORCE };

	void LoadScene();

};

