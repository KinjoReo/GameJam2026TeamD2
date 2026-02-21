#pragma once

#include "Object/Player/Player.h"

class InGameScene : public SceneBase
{
private:
	int wallHeight;
	int wallTop;
	Player* player;

	//BGM
	int InGameBGM = -1;

public:
	InGameScene();
	virtual ~InGameScene();

private:
	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

public:
	virtual eSceneType GetNowSceneType() const override
	{
		return eSceneType::eInGame;
	}

};