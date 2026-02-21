//#include "ResultScene.h"
//#include "../Utility/Input.h"
//#include <DxLib.h>
//
//float time; // フェード用タイマー(秒)
//int ResultInit(void)
//{
//	// タイマー初期化
//	time = 0.0f;
//	return TRUE;
//}
//
//int ResultInit(void)
//{
//	result_image = LoadGraph("assets/images/.png"); // リザルト画像の読み込み
//
//	if (title_image == -1)
//	{
//		return FALSE; // 読み込み失敗
//	}
//	.png
//	cursor_number = 0; // メニューカーソル初期位置
//	blink_flag = TRUE; // 点滅表示の初期状態
//	blink_time = 0.0f; // 点滅用タイマー初期化
//
//	return TRUE; // 初期化成功
//}
//
//
//eSceneType ResultUpdate(float delta_second)
//{
//	// 一定時間経過後は入力でタイトルへ戻れる
//	if (time >= 5.0f)
//	{
//		if (GetKeyInputState(KEY_INPUT_Z) == ePressed || GetButtonState
//		(XINPUT_BUTTON_A) == ePressed)
//		{
//			return eTitle;
//		}
//		time = 5.0f; // 上限固定
//	}
//	else
//	{
//		// フェード時間を進める
//		time += delta_second;
//	}
//	return eResult; // リザルト継続
//}
//
//void ResultDraw(void)
//{
//	// フェードインでゲームオーバー表示
//	SetFontSize(50);
//
//	int alpha = (int)(time / 5.0f * 255.0f);
//
//	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
//	DrawString(10, 10, "G A M E O V E R", GetColor(255, 0, 0));
//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
//	SetFontSize(20);
//
//	// 完全表示になったら「A」ボタン案内を表示
//	if (alpha >= 255)
//	{
//		DrawString(10, 400, "Presse the A button", GetColor(255, 0, 0));
//	}
//}