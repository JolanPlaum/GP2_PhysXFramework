#pragma once
#include "GameScene.h"

class W1_AssignmentScene final : public GameScene
{
public:
	W1_AssignmentScene() : GameScene(L"W1_AssignmentScene") {}
	~W1_AssignmentScene() override = default;
	W1_AssignmentScene(const W1_AssignmentScene& other) = delete;
	W1_AssignmentScene(W1_AssignmentScene&& other) noexcept = delete;
	W1_AssignmentScene& operator=(const W1_AssignmentScene& other) = delete;
	W1_AssignmentScene& operator=(W1_AssignmentScene&& other) noexcept = delete;

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

		Jump,
	};

	GameObject* m_pSphere{};
	float m_Force{ 1000.f };
	float m_JumpForce{ 100.f };
	PxForceMode::Enum m_ForceMode{ PxForceMode::eFORCE };

	int m_WallSize{ 10 };
	std::vector<GameObject*> m_pBlocks{};

	void InitFloor();
	void InitPlayerSphere();
	void InitWall();

	void LoadScene();

	float RandomFloat(float min, float max);

};

