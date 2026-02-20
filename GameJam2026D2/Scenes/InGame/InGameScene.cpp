#include "InGameScene.h"

#include <DxLib.h>

InGameScene::InGameScene()
	:wallHeight(25)
	,wllTop(80)
	,InGameBGM(-1)
	,player(nullptr)
	,warpImage(-1)
	,warpScale(1.0f)
{
}

InGameScene::~InGameScene()
{
}

void InGameScene::Initialize()
{
	AssetsContainer* container = AssetsContainer::GetInstance();

	warpImage = container->GetImage("");

	const float TARGET_SIZE = 100.0f;
	warpScale = TARGET_SIZE;

	player = new Player;
	player->Initialize(0);

	InGameBGM = container->GetSound("");
	ChangeVolumeSoundMem(135, InGameBGM);

	if (InGameBGM != -1)
	{
		PlaySoundMem(InGameBGM, DX_PLAYTYPE_LOOP);
	}

	gWinnerPlayer = -1;
}

//更新
eSceneType InGameScene::Update()
{
	InputManager* input = InputManager::GetInstance();

	Vector2D prev = player->GetLocation();

	player->Update();

	CheckWarp(player, nextWarpTimePlayer);


}

//描画
void InGameScene::Draw()const
{
	const int screenW = 1280;
	const int screenH = 720;

	//外壁

	//プレイヤー
	player->Draw();
};

void InGameScene::Finalize()
{
	if (InGameBGM != -1)
	{
		StopSoundMem(InGameBGM);
		DeleteSoundMem(InGameBGM);
	}
}

//
