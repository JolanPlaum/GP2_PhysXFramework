#pragma once
#include "GameScene.h"

class AudioTestScene final : public GameScene
{
public:
	AudioTestScene() : GameScene(L"AudioTestScene") {}
	~AudioTestScene() override = default;
	AudioTestScene(const AudioTestScene& other) = delete;
	AudioTestScene(AudioTestScene&& other) noexcept = delete;
	AudioTestScene& operator=(const AudioTestScene& other) = delete;
	AudioTestScene& operator=(AudioTestScene&& other) noexcept = delete;

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

		Play2D,
		Play3D,
		IncreaseVolume,
		DecreaseVolume,
	};

	void InitActor();
	void LoadScene();

	GameObject* m_pSphere{ nullptr };

	FMOD::Channel* m_pChannel2D{};
	FMOD::Channel* m_pChannel3D{};
	FMOD_VECTOR m_PrevListenerPos{};

};

