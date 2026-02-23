#include"TitleScene.h"
#include"../Utility/Vector2D.h"
#include<DxLib.h>

int title_image = -1;
float blink_time = 0.0f;
int cursor_number = 0;
bool blink_flag = true;
int oldPad = 0;
int oldEnter = 0;
int oldUp = 0;
int oldDown = 0;

// =============================
// 初期化
// =============================
void TitleInit()
{
	title_image = LoadGraph("Resource/Illustrator/SceneImage/Title/Title1.png"); // タイトル画像の読み込み

	if (title_image == -1)
	{
		printfDx("タイトル画像の読み込みに失敗しました\n");
	}

	cursor_number = 0; // メニューカーソル初期位置
	blink_flag = true; // 点滅表示の初期状態
	blink_time = 0.0f; // 点滅用タイマー初期化

	// 現在の入力状態を取得して保存（超重要）
	oldPad = GetJoypadInputState(DX_INPUT_PAD1);
	oldEnter = CheckHitKey(KEY_INPUT_RETURN);
	oldUp = CheckHitKey(KEY_INPUT_UP);
	oldDown = CheckHitKey(KEY_INPUT_DOWN);
}


// =============================
// 更新
// =============================
eSceneType TitleUpdate(float delta_second)
{

	int nowPad = GetJoypadInputState(DX_INPUT_PAD1);
	int nowEnter = CheckHitKey(KEY_INPUT_RETURN);
	int nowUp = CheckHitKey(KEY_INPUT_UP);
	int nowDown = CheckHitKey(KEY_INPUT_DOWN);

	// =============================
	// 点滅処理
	// =============================
	blink_time += delta_second;
	if (blink_time >= 1.5f)
	{
		blink_flag = !blink_flag;
		blink_time = 0.0f;
	}

	// =============================
	// カーソル下移動（押した瞬間）
	// =============================
	if ((nowDown && !oldDown) ||  // キーボード
		((nowPad & PAD_INPUT_DOWN) && !(oldPad & PAD_INPUT_DOWN))) // コントローラー
	{
		cursor_number++;

		// メニュー数制限（例：2個なら0～1）
		if (cursor_number > 1) cursor_number = 1;
	}

	// =============================
	// カーソル上移動（押した瞬間）
	// =============================
	if ((nowUp && !oldUp) ||
		((nowPad & PAD_INPUT_UP) && !(oldPad & PAD_INPUT_UP)))
	{
		cursor_number--;

		if (cursor_number < 0) cursor_number = 0;
	}

	// =============================
	// 決定（押した瞬間）
	// =============================
	if ((nowEnter && !oldEnter) ||
		((nowPad & PAD_INPUT_1) && !(oldPad & PAD_INPUT_1)))
	{
		if (cursor_number == 0)
			return eSceneType::eInGame;

		if (cursor_number == 1)
			return eSceneType::eEnd;   // 例：終了
	}

	// 状態保存
	oldPad = nowPad;
	oldEnter = nowEnter;
	oldUp = nowUp;
	oldDown = nowDown;

	return eSceneType::eTitle;

}

// =============================
// 描画
// =============================
void TitleDraw(void)
{
	if (title_image != -1)
	{
		int w, h;
		GetGraphSize(title_image, &w, &h);

		float scaleX = 1280.0f / w;
		float scaleY = 720.0f / h;

		float scale = min(scaleX, scaleY);

		DrawRotaGraph(
			650, 360,   // 画面中央
			scale,
			0.0,
			title_image,
			TRUE
		);
	}
    //カーソルの矢印を点滅表示
	if (blink_flag)
	{
		int cy = cursor_number * 46; // 行ごとのオフセット
			DrawTriangle(820, 530 + cy, 840, 545 + cy, 820, 560 + cy, GetColor(255, 50, 50), TRUE);
	}
}
