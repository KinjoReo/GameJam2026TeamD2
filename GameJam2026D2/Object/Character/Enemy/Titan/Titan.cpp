#include "../Titan/Titan.h"
#include "../../../Player/Player.h"
#include "DxLib.h"


/// <summary>
/// コンストラクタ
/// 監視対象のプレイヤーを受け取り、各種変数を初期化する
/// </summary>
/// <param name="target">監視するプレイヤー</param>
Enemy::Enemy(Player* target)                                                 // TitanはEnemy
	: player(target),         // 監視対象(プレイヤー)を保存
	watchTime(0.0f),          // 監視経過時間を0に初期化
	watchLimit(22.0f),         // 秒数指定してその秒間見られたらゲームオーバー
	isWatching(false),        // 初期状態では監視していない
	isGameOver(false),         // ゲームオーバーではない(false)
	isActive(false)
{
}


/// <summary>
/// デストラクタ
/// （現在は特別な解放処理なし）
/// </summary>
Enemy::~Enemy()
{
}


/// <summary>
/// 初期化処理
/// エネミーの出現位置を設定する
/// </summary>
void Enemy::Initialize()
{
	location = Vector2D(400, 1000);  // 上の方に出現

	Titanimage = LoadGraph("Resource/Illustrator/Character/Enemy/Titan/Titan.png");

	if (Titanimage == -1)
	{
		printfDx("画像読み込み失敗\n");
	}

	// 登場SE読み込み
	TitanSE = LoadSoundMem("Resource/Sound/SE/タイタン.mp3"); // 適切なSEファイルを指定
	if (TitanSE == -1)
		printfDx("Enemy登場SEの読み込み失敗\n");
}


/// <summary>
/// 毎フレーム更新処理
/// プレイヤーの状態を確認し、監視時間をカウントする
/// </summary>
/// <param name="delta_second">1フレームあたりの経過時間</param>
void Enemy::Update(float delta_second)
{
	if (!isActive) return;   // 出現していなければ何もしない
	if (isGameOver) return;               // すでにゲームオーバーなら何もしない

	// プレイヤーが隠れていないなら監視
	if (!player->IsHidden())
	{
		isWatching = true;                 // 監視状態ON(true)
		watchTime += delta_second;         // 経過時間を加算

		// 制限時間を超えたらゲームオーバー
		if (watchTime >= watchLimit)
		{
			isGameOver = true;                      // ゲームオーバー(true)
		}
	}
	else
	{
		// プレイヤーが隠れた場合
		// 監視状態を解除し、監視時間をリセット
		isWatching = false;
		watchTime = 0.0f;
	}
}


/// <summary>
/// 描画処理
/// エネミー本体および監視演出を描画する
/// </summary>
void Enemy::Draw() const
{
	if (!isActive) return;

	// ===============================
	// エネミー本体描画
	// ===============================
	if (Titanimage == -1) return;

	// 画像の中心を基準に描く
	DrawRotaGraph(
		(int)location.x,
		(int)location.y,
		0.6,
		drawAngle,
		Titanimage,
		TRUE
	);


	// ===============================
	// 監視演出（画面赤表示）
	// ===============================

	// 監視中なら赤表示
	if (isWatching)
	{
		// 監視進行度を0.0～1.0に変換
		float ratio = watchTime / watchLimit;

		// 上限を1.0に制限
		if (ratio > 1.0f)
			ratio = 1.0f;

		DrawRedScreen(ratio);       // 画面を赤くする
	}


	// ===============================
	// ゲームオーバー表示
	// ===============================
	if (isGameOver)
	{
		DrawFormatString(300, 250, GetColor(255, 0, 0),"GAME OVER");
	}
}


/// <summary>
/// 画面全体を赤くする演出
/// 監視時間に応じて透明度を変化させる
/// </summary>
/// <param name="alpha">透明度（0.0f ～ 1.0f）</param>
void Enemy::DrawRedScreen(float alpha) const
{
	// alpha 0.0f ～ 1.0f

	int color = GetColor(255, 0, 0);      // 赤色を取得

	// αブレンド設定（180は最大透明度）（最大180で強くなりすぎないよう調整）
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(alpha * 180));   // 180 にしているのは真っ赤になりすぎないように。

	// 画面全体を塗りつぶす（現在800x600固定）
	DrawBox(0, 0, 1280, 720, color, TRUE);

	// ブレンドモードを元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


/// <summary>
/// 監視時間リセット処理
/// プレイヤーが上端到達した時などに呼び出す
/// </summary>
void Enemy::ResetWatchTime()
{
	watchTime = 0.0f;
	isWatching = false;
}

void Enemy::Finalize()
{
	if (Titanimage != -1)
	{
		DeleteGraph(Titanimage);
		Titanimage = -1;
	}

	if (TitanSE != -1)
	{
		DeleteSoundMem(TitanSE);
		TitanSE = -1;
	}
}

void Enemy::Spawn()
{
	location = Vector2D(400, 1000);
	watchTime = 0.0f;
	isWatching = false;
	isGameOver = false;
	isActive = true;    // 出現ON

	// 登場時にSE再生
	if (TitanSE != -1)
		PlaySoundMem(TitanSE, DX_PLAYTYPE_BACK);
}

void Enemy::Hide()
{
	isActive = false;
}