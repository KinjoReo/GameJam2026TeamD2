#include "InGameScene.h"
#include "../../Input/InputManager.h"

#include <DxLib.h>

#include <cstdlib>

#include <ctime>   // time()

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
    , titan(nullptr)
    , yamorihebi(nullptr)
    , spawnTitan(false)
    , spawnYamoriHebi(false)
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
    srand(GetNowCount());

    backgroundImage = LoadGraph("Resource/Illustrator/Background/Level1/Level1Haikei.png");

    backgroundImage2 = LoadGraph("Resource/Illustrator/Background/Level1/Level1Kave.png");

    backgroundImage3 = LoadGraph("Resource/Illustrator/Background/Level1/Level1Kaidan.png");

    player = new Player();
    player->Initialize();   // 引数なしで呼ぶ

    //if (spawnNightmare)
    //{
    //    nightmare = new Nightmare();
    //    nightmare->Initialize();

    //    // 画面内の適当な位置に出す（あなたの座標系に合わせて調整）
    //    nightmare->SetLocation(Vector2D(1100.0f, 600.0f));

    //    // 追尾したいのでプレイヤーを渡す（Nightmareに SetPlayer を実装してる場合）
    //    /*nightmare->SetPlayer(player);*/
    //}


    // それぞれ独立ランダム
    //spawnTitan = (GetRand(1) == 1);  // 50%
    //spawnYamoriHebi = (GetRand(1) == 1);  // 50%
    spawnTitan = true;
    spawnYamoriHebi = true;

    if (spawnTitan)
    {
        titan = new Enemy(player);
        titan->Initialize();
    }

    if (spawnYamoriHebi)
    {
        yamorihebi = new Enemy2(player);
        yamorihebi->Initialize();
    }

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

    /*if (nightmare)
        nightmare->Update(delta);*/

    if (titan)
    {
        titan->Update(delta);
        if (titan->IsGameOver())  // ← Enemy のフラグをチェック
            return eSceneType::eGameOver;
    }

    if (yamorihebi)
        yamorihebi->Update(delta);

    // 上端到達処理
    // プレイヤーが上端に到達したら監視リセット
    if (player->DidReachTop())
    {
        // ===== Titan 40% =====
        if (titan)
        {
            int rTitan = rand() % 100;   // 0～99

            if (rTitan < 40)
                titan->Spawn();
            else
                titan->Hide();
        }


        // ===== YamoriHebi 出現率上昇式 =====
        if (yamorihebi)
        {
            int reach = player->GetReachTopCount();

            int chance = 30 + reach * 5;   // 基本30%、1回ごと+5%

            if (chance > 80)
                chance = 80;

            int r = rand() % 100;

            if (r < chance)
                yamorihebi->ResetPosition();
            else
                yamorihebi->Hide();
        }

        player->ResetReachFlag();
    }

    // ===== 接触判定 =====
    if (yamorihebi)
    {
        Vector2D diff = player->GetLocation() - yamorihebi->GetLocation();
        float distance = sqrtf(diff.x * diff.x + diff.y * diff.y);

        if (distance < 780.0f)
        {
            return eSceneType::eGameOver;
        }
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
    /*if (nightmare)
        nightmare->Draw();*/

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

    if (titan)
        titan->Draw();

    if (yamorihebi)
        yamorihebi->Draw();
}

// =============================
// 終了処理
// =============================
void InGameScene::Finalize()
{
    /*if (nightmare)
    {
        nightmare->Finalize();
        delete nightmare;
        nightmare = nullptr;
    }*/

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

    if (titan)
    {
        titan->Finalize();
        delete titan;
        titan = nullptr;
    }

    if (yamorihebi)
    {
        yamorihebi->Finalize();
        delete yamorihebi;
        yamorihebi = nullptr;
    }
}