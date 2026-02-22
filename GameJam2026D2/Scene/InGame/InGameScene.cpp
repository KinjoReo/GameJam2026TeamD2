#include "InGameScene.h"

#include <DxLib.h>

// =============================
// コンストラクタ
// =============================
InGameScene::InGameScene()
    : wallHeight(25)
    , wallTop(80)
    , player(nullptr)
    , enemy(nullptr)
    , enemy2(nullptr)
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

    if (enemy)
    {
        delete enemy;
        enemy = nullptr;
    }

    if (enemy2)
    {
        delete enemy2;
        enemy2 = nullptr;
    }
}

// =============================
// 初期化
// =============================
void InGameScene::Initialize()
{
    backgroundImage = LoadGraph("Resource/Illustrator/Background/Level1/Level1Haikei.png");

    backgroundImage2 = LoadGraph("Resource/Illustrator/Background/Level1/Level1Kave.png");

    backgroundImage3 = LoadGraph("Resource/Illustrator/Background/Level1/Level1Kaidan.png");

    player = new Player();
    player->Initialize();   // 引数なしで呼ぶ

    enemy = new Enemy(player);
    enemy->Initialize();

    enemy2 = new Enemy2(player);
    enemy2->Initialize();

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
    enemy->Update(delta);
    enemy2->Update(delta);

    // 上端到達処理
    // プレイヤーが上端に到達したら監視リセット
    if (player->DidReachTop())
    {
        enemy->ResetWatchTime();
        enemy2->ResetPosition();
        player->ResetReachFlag();
    }

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
    if (backgroundImage != -1)
    {
        int w, h;
        GetGraphSize(backgroundImage, &w, &h);

        float scaleX = 1390.0f / w;
        float scaleY = 884.0f / h;

        float scale = min(scaleX, scaleY);

        DrawRotaGraph(
            815, 440,   // 画面中央
            scale,
            0.0,
            backgroundImage,
            TRUE
        );
    }

    if (backgroundImage2 != -1)
    {
        int w, h;
        GetGraphSize(backgroundImage2, &w, &h);

        float scaleX = 1230.0f / w;
        float scaleY = 724.0f / h;

        float scale = min(scaleX, scaleY);

        DrawRotaGraph(
            418, 360,   // 画面中央
            scale,
            0.0,
            backgroundImage2,
            TRUE
        );
    }

    if (backgroundImage3 != -1)
    {
        int w, h;
        GetGraphSize(backgroundImage3, &w, &h);

        float scaleX = 1230.0f / w;
        float scaleY = 724.0f / h;

        float scale = min(scaleX, scaleY);

        DrawRotaGraph(
            340, 360,   // 画面中央
            scale,
            0.0,
            backgroundImage3,
            TRUE
        );
    }

    player->Draw();
    enemy->Draw();
    enemy2->Draw();
}

// =============================
// 終了処理
// =============================
void InGameScene::Finalize()
{
    if (backgroundImage != -1)
    {
        DeleteGraph(backgroundImage);
        backgroundImage = -1;
    }

    if (backgroundImage2 != -1)
    {
        DeleteGraph(backgroundImage2);
        backgroundImage2 = -1;
    }

    if (backgroundImage3 != -1)
    {
        DeleteGraph(backgroundImage3);
        backgroundImage3 = -1;
    }

    if (InGameBGM != -1)
    {
        StopSoundMem(InGameBGM);
        DeleteSoundMem(InGameBGM);
        InGameBGM = -1;
    }
}