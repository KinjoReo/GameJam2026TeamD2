#include"DxLib.h"
#include"title/TitleScene.h" // TitleInit, TitleDraw が定義されているヘッダ
#include"Object/Player/Player.h"
#include"Object/Character/Enemy/Titan/Titan.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_
	LPSTR lpCmdLine, _In_ int nShowCmd)
{
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1)
	{
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	Player player;              // 一旦ここに書いてる
	player.Initialize();        // 一旦

	Enemy enemy(&player);   // 一旦
	enemy.Initialize();     // 一旦


	while (ProcessMessage() == 0 && ClearDrawScreen() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		float delta = 1.0f / 60.0f;

		player.Update(delta);      // 一旦ここに書いてる
		enemy.Update(delta);   // 一旦
		player.Draw();         // 一旦
		enemy.Draw();          // 一旦

		// プレイヤーが上端に到達したら監視リセット
		if (player.DidReachTop())
		{
			enemy.ResetWatchTime();
			player.ResetReachFlag();
		}

		// ここで毎フレームの更新処理（点滅フラグの切り替えなど）を行う
		/* TitleUpdate();*/ // もし更新関数を作ったらここに入れる

		//描画関数の呼び出し
		TitleDraw();

		//裏画面の内容を表画面に反映
		ScreenFlip();
	}

	player.Finalize();           // 一旦

	WaitKey();

	DxLib_End();

	return 0;
}