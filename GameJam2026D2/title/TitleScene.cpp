#include"TitleScene.h"
#include"../Utility/Vector2D.h"
#include<DxLib.h>

int title_image;
float blink_time;
int cursor_number;
bool blink_flag;

// =============================
// 初期化
// =============================
void TitleInit()
{
	title_image = LoadGraph("../assets/images/Title.png"); // タイトル画像の読み込み

	//if (title_image == -1)
	//{
	//	return FALSE; // 読み込み失敗
	//}

	cursor_number = 0; // メニューカーソル初期位置
	blink_flag = true; // 点滅表示の初期状態
	blink_time = 0.0f; // 点滅用タイマー初期化

	//return TRUE; // 初期化成功
}


// =============================
// 更新
// =============================
eSceneType TitleUpdate(float delta_second)
{
	static int oldPad = 0;
	int nowPad = GetJoypadInputState(DX_INPUT_PAD1);

	//// Aボタンが押された瞬間
	//if ((nowPad & PAD_INPUT_1) && !(oldPad & PAD_INPUT_1))
	//{
	//	return eInGame;  // InGameへ遷移
	//}

	// 点滅処理
	blink_time += delta_second;
	if (blink_time >= 0.5f)
	{
		blink_flag = !blink_flag;
		blink_time = 0.0f;
	}

	// Enterキー または Aボタン
	if (CheckHitKey(KEY_INPUT_RETURN) ||
		((nowPad & PAD_INPUT_1) && !(oldPad & PAD_INPUT_1)))
	{
		return eSceneType::eInGame;   // enum class対応
	}

	oldPad = nowPad;

	return eSceneType::eTitle;        // enum class対応

}

// =============================
// 描画
// =============================
void TitleDraw(void)
{
	DrawRotaGraph(0, 0, 1.0, 0.0, title_image,TRUE); // タイトル画像描画
    //カーソルの矢印を点滅表示
	if (blink_flag)
	{
		int cy = cursor_number * 50; // 行ごとのオフセット
			DrawTriangle(240, 255 + cy, 260, 270 + cy, 240, 285 + cy, GetColor(255, 0, 0), TRUE);
	}
}
