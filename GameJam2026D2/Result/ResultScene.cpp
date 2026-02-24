#include "ResultScene.h"
#include <DxLib.h>

// =============================
// コンストラクタ
// =============================
ResultScene::ResultScene()
    : resultImage(-1)
    , cursor_number(0)
    , blink_time(0.0f)
    , blink_flag(true)
{
}

// =============================
// デストラクタ
// =============================
ResultScene::~ResultScene()
{
}

// =============================
// 初期化
// =============================
void ResultScene::Initialize()
{
    resultImage = LoadGraph("Resource/Illustrator/Background/Haikei/Haikei.jpg");
    

    cursor_number = 0;
    blink_time = 0.0f;
    blink_flag = true;

    if (resultImage == -1)
    {
        printfDx("画像の読み込み失敗\n");
    }

    // 現在の入力状態を取得して保存する
    oldPad = GetJoypadInputState(DX_INPUT_PAD1);
    oldEnter = CheckHitKey(KEY_INPUT_RETURN);
    oldUp = CheckHitKey(KEY_INPUT_UP);
    oldDown = CheckHitKey(KEY_INPUT_DOWN);
}

// =============================
// 更新
// =============================
eSceneType ResultScene::Update()
{

    int nowPad = GetJoypadInputState(DX_INPUT_PAD1);
    int nowEnter = CheckHitKey(KEY_INPUT_RETURN);
    int nowUp = CheckHitKey(KEY_INPUT_UP);
    int nowDown = CheckHitKey(KEY_INPUT_DOWN);

    float delta = 1.0f / 60.0f;

    // 点滅処理
    blink_time += delta;
    if (blink_time >= 0.5f)
    {
        blink_flag = !blink_flag;
        blink_time = 0.0f;
    }

    // ↓キー（押した瞬間）
    if ((nowDown && !oldDown) ||
        ((nowPad & PAD_INPUT_DOWN) && !(oldPad & PAD_INPUT_DOWN)))
    {
        cursor_number++;
        if (cursor_number > 1) cursor_number = 1;
    }

    // ↑キー（押した瞬間）
    if ((nowUp && !oldUp) ||
        ((nowPad & PAD_INPUT_UP) && !(oldPad & PAD_INPUT_UP)))
    {
        cursor_number--;
        if (cursor_number < 0) cursor_number = 0;
    }

    // 決定（押した瞬間）
    if ((nowEnter && !oldEnter) ||
        ((nowPad & PAD_INPUT_1) && !(oldPad & PAD_INPUT_1)))
    {
        if (cursor_number == 0)
            return eSceneType::eInGame;   // リトライ

        if (cursor_number == 1)
            return eSceneType::eTitle;    // タイトルへ
    }

    // 状態保存
    oldPad = nowPad;
    oldEnter = nowEnter;
    oldUp = nowUp;
    oldDown = nowDown;

    return eSceneType::eResult;
}

// =============================
// 描画
// =============================
void ResultScene::Draw() const
{
    if (resultImage != -1)
    {
        DrawGraph(0, 0, resultImage, TRUE);
    }

    DrawString(600, 450, "Retry", GetColor(255, 255, 255));
    DrawString(600, 500, "Back to Title", GetColor(255, 255, 255));

    // カーソル
    if (blink_flag)
    {
        int cy = cursor_number * 50;
        DrawTriangle(560, 455 + cy, 580, 470 + cy, 560, 485 + cy,
            GetColor(255, 0, 0), TRUE);
    }
}

// =============================
// 終了処理
// =============================
void ResultScene::Finalize()
{
    if (resultImage != -1)
    {
        DeleteGraph(resultImage);
        resultImage = -1;
    }
}