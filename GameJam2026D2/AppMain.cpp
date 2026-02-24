#include"DxLib.h"
#include"title/TitleScene.h" // TitleInit, TitleDraw が定義されているヘッダ
#include"Scene/InGame/InGameScene.h"
#include"Result/ResultScene.h"
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

	eSceneType nextScene = eSceneType::eTitle;


	// InGameScene生成
	InGameScene inGame;

	// ResultScene生成
	ResultScene resultScene;
		
	// Title初期化
	TitleInit();

	float gameOverTimer = 0.0f;

	while (ProcessMessage() == 0 && ClearDrawScreen() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		float delta = 1.0f / 60.0f;

		// ここで毎フレームの更新処理（点滅フラグの切り替えなど）を行う
		/* TitleUpdate();*/ // もし更新関数を作ったらここに入れる

		switch (scene)
		{
		case eSceneType::eTitle:
			nextScene = TitleUpdate(delta);
			TitleDraw();
			break;

		case eSceneType::eInGame:
			nextScene = inGame.Update();   // ← 戻り値を受け取る
			inGame.Draw();
			break;

		case eSceneType::eGameOver:
			gameOverTimer += delta;

			DrawString(600, 300, "捕まった", GetColor(255, 0, 0));

			if (gameOverTimer >= 5.0f)
			{
				gameOverTimer = 0.0f;   // リセット
				nextScene = eSceneType::eResult;
			}
			else
			{
				nextScene = eSceneType::eGameOver;
			}
			break;

		case eSceneType::eResult:
			nextScene = resultScene.Update();
			resultScene.Draw();
			break;
		case eSceneType::eEnd:
			DxLib_End();
			return 0;
		}

		// ===== シーンが変わった瞬間 =====
		if (scene != nextScene)
		{
			// 終了処理
			if (scene == eSceneType::eInGame) inGame.Finalize();
			if (scene == eSceneType::eResult) resultScene.Finalize();

			// 初期化
			if (nextScene == eSceneType::eInGame) inGame.Initialize();
			if (nextScene == eSceneType::eResult) resultScene.Initialize();
			if (nextScene == eSceneType::eTitle) TitleInit();

			scene = nextScene;
		}

		//裏画面の内容を表画面に反映
		ScreenFlip();
	}

	WaitKey();

	DxLib_End();

	return 0;
}