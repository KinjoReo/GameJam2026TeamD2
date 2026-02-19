#include"TitleScene.h"
#include<DxLib.h>

int title_image;
int blink_time;
int cursor_number;
float blink_flag;

int TitleInit(void)
{
	title_image = LoadGraph("assets/images/Title.bmp"); // タイトル画像の読み込み
	if (title_image == -1)
	{
		return FALSE; // 読み込み失敗
	}
	cursor_number = 0; // メニューカーソル初期位置
	blink_flag = TRUE; // 点滅表示の初期状態
	blink_time = 0.0f; // 点滅用タイマー初期化

	return TRUE; // 初期化成功
}
