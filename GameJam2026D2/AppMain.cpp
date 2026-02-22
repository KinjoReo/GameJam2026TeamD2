#include"DxLib.h"
#include"title/TitleScene.h" // TitleInit, TitleDraw が定義されているヘッダ
#include"Scene/InGame/InGameScene.h"
#include"Object/Player/Player.h"
#include"Object/Character/Enemy/Titan/Titan.h"
#include"Object/Character/Enemy/YamoriHebi/YamoriHebi.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_
	LPSTR lpCmdLine, _In_ int nShowCmd)
{
	ChangeWindowMode(TRUE);
	SetGraphMode(1280, 720, 32);

	if (DxLib_Init() == -1)
	{
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	// 現在のシーン
	eSceneType scene = eSceneType::eTitle;

	// InGameScene生成
	InGameScene inGame;

	// Title初期化
	TitleInit();


	while (ProcessMessage() == 0 && ClearDrawScreen() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		float delta = 1.0f / 60.0f;

		// ここで毎フレームの更新処理（点滅フラグの切り替えなど）を行う
		/* TitleUpdate();*/ // もし更新関数を作ったらここに入れる

		switch (scene)
		{
		case eTitle:
			scene = TitleUpdate(delta);
			TitleDraw();
			break;

		case eInGame:
		{
			static bool first = true;
			if (first)
			{
				inGame.Initialize();
				first = false;
			}

			scene = inGame.Update();   // ← 戻り値を受け取る
			inGame.Draw();
		}
		break;
		}

		//裏画面の内容を表画面に反映
		ScreenFlip();
	}

	WaitKey();

	DxLib_End();

	return 0;
}