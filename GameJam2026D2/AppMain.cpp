#include"DxLib.h"
#include"title/TitleScene.h" // TitleInit, TitleDraw が定義されているヘッダ

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_
	LPSTR lpCmdLine, _In_ int nShowCmd)
{
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1)
	{
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	while (ProcessMessage() == 0 && ClearDrawScreen() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// ここで毎フレームの更新処理（点滅フラグの切り替えなど）を行う
		/* TitleUpdate();*/ // もし更新関数を作ったらここに入れる

		//描画関数の呼び出し
		TitleDraw();

		//裏画面の内容を表画面に反映
		ScreenFlip();
	}

	WaitKey();

	DxLib_End();

	return 0;
}