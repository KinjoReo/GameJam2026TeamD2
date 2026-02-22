#include "InGameScene.h"

#include <DxLib.h>

// =============================
// コンストラクタ
// =============================
InGameScene::InGameScene()
    : wallHeight(25)
    , wallTop(80)
    , player(nullptr)
    , InGameBGM(-1)
{
}

// =============================
// デストラクタ
// =============================
InGameScene::~InGameScene()
{
    if (player)
    {
        delete player;
        player = nullptr;
    }
}

// =============================
// 初期化
// =============================
void InGameScene::Initialize()
{
    player = new Player();
    player->Initialize();   // 引数なしで呼ぶ

    // BGMがあるならここで読み込み
    // InGameBGM = LoadSoundMem("bgm.wav");
    // PlaySoundMem(InGameBGM, DX_PLAYTYPE_LOOP);
}

// =============================
// 更新
// =============================
eSceneType InGameScene::Update()
{
    float delta = 1.0f / 60.0f;

    player->Update(delta);

    // ESCでタイトルへ戻る例
    if (CheckHitKey(KEY_INPUT_ESCAPE))
    {
        return eSceneType::eTitle;
    }

    return eSceneType::eInGame;
}

// =============================
// 描画
// =============================
void InGameScene::Draw()const
{
    if (player)
    {
        player->Draw();
    }
}

// =============================
// 終了処理
// =============================
void InGameScene::Finalize()
{
    if (InGameBGM != -1)
    {
        StopSoundMem(InGameBGM);
        DeleteSoundMem(InGameBGM);
        InGameBGM = -1;
    }
}