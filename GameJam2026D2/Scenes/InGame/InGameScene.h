#pragma once

#include "../SceneBase/SceneBase.h"
#include "../../Object/Player/Player.h"

class InGameScene :public SceneBase
{
private:
	int wallHeight;
	int wallTop;
	Player* player;

	//BGM
	int InGameBGM = -1;

	//ƒtƒHƒ“ƒg
	int timerFont;
	int titleFont;

public:
	InGameScene();
	virtual ~InGameScene();

private:
	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	//

public:
	virtual eSceneType::eInGame;
};