#include "ResultScene.h"
#include "../Utility/ResourceManager.h"
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

    // SE読み込み
    cursorSE = LoadSoundMem("Resource/Sound/SE/選択.mp3");
    if (cursorSE == -1) printfDx("カーソルSE読み込み失敗\n");

    decisionSE = LoadSoundMem("Resource/Sound/SE/決定.mp3");
    if (decisionSE == -1) printfDx("決定SE読み込み失敗\n");


    ///////////////////////////////////////////////////
    // リソースマネージャーインスタンス取得
    ResourceManager* rm = ResourceManager::GetInstance();

    //(ファイルパス,総分割画像数,横分割数,縦分割数,横画像サイズ,縦画像サイズ);
    numbers = rm->GetImages("Resource/Illustrator/Background/Suuji/Suuji2.png", 10, 5, 2, 350, 300);
    /*numbers[0];*/

    ///////////////////////////////////////////////////
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

        // SE再生
        if (cursorSE != -1) PlaySoundMem(cursorSE, DX_PLAYTYPE_BACK);
    }

    // ↑キー（押した瞬間）
    if ((nowUp && !oldUp) ||
        ((nowPad & PAD_INPUT_UP) && !(oldPad & PAD_INPUT_UP)))
    {
        cursor_number--;
        if (cursor_number < 0) cursor_number = 0;

        // SE再生
        if (cursorSE != -1) PlaySoundMem(cursorSE, DX_PLAYTYPE_BACK);
    }

    // 決定（押した瞬間）
    if ((nowEnter && !oldEnter) ||
        ((nowPad & PAD_INPUT_1) && !(oldPad & PAD_INPUT_1)))
    {
        // SE再生
        if (decisionSE != -1) PlaySoundMem(decisionSE, DX_PLAYTYPE_BACK);

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

    DrawString(600, 470, "リトライ", GetColor(255, 255, 255));
    DrawString(600, 520, "タイトルへ戻る", GetColor(255, 255, 255));

    // カーソル
    if (blink_flag)
    {
        int cy = cursor_number * 50;
        DrawTriangle(560, 455 + cy, 580, 470 + cy, 560, 485 + cy,
            GetColor(255, 0, 0), TRUE);
    }

    // ===============================
    // プレイヤーの上端到達回数を数字で描画
    // ===============================
    if (player != nullptr && numbers.size() >= 10)
    {
        int count = player->GetReachTopCount(); // reachTopCount を Player から取得
        int digits[10];
        int digitCount = 0;

        if (count == 0) { digits[0] = 0; digitCount = 1; }
        else { while (count > 0) { digits[digitCount++] = count % 10; count /= 10; } }

        int startX = 25;
        int startY = 25;
        float scaleX = 0.3f;
        float scaleY = 0.3f;
        int spacing = 4;

        // 右から左に描画
        for (int i = 0; i < digitCount; i++)
        {
            int digit = digits[digitCount - 1 - i];
            int img = numbers[digit];

            if (img != -1)
            {
                int w = 350 * scaleX;
                int h = 300 * scaleY;
                DrawExtendGraph(
                    startX + i * (w + spacing), startY,
                    startX + i * (w + spacing) + w, startY + h,
                    img,
                    TRUE
                );
            }
        }
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

    if (cursorSE != -1)
    {
        DeleteSoundMem(cursorSE);
        cursorSE = -1;
    }

    if (decisionSE != -1)
    {
        DeleteSoundMem(decisionSE);
        decisionSE = -1;
    }
}