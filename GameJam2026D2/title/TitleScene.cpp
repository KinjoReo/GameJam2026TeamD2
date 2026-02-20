#include"TitleScene.h"
#include"../Utility/Vector2D.h"
#include<DxLib.h>

int title_image;
int blink_time;
int cursor_number;
float blink_flag;

int TitleInit(void)
{
	title_image = LoadGraph("../assets/images/Title.png"); // タイトル画像の読み込み

	if (title_image == -1)
	{
		return FALSE; // 読み込み失敗
	}

	cursor_number = 0; // メニューカーソル初期位置
	blink_flag = TRUE; // 点滅表示の初期状態
	blink_time = 0.0f; // 点滅用タイマー初期化

	return TRUE; // 初期化成功
}

eSceneType TitleUpdate(float delta_second)
{
	return eTitle;
}

void TitleDraw(void)
{
	DrawRotaGraph(0, 0, 1.0, 0.0, title_image,TRUE); // タイトル画像描画
    //カーソルの矢印を点滅表示
	if (blink_flag == TRUE)
	{
		int cy = cursor_number * 50; // 行ごとのオフセット
			DrawTriangle(240, 255 + cy, 260, 270 + cy, 240, 285 + cy, GetColor(255, 0, 0), TRUE);
	}
}
