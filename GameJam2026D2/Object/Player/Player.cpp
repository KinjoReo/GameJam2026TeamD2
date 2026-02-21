#include "Player.h"
#include "DxLib.h"
#include "../Character/Enemy/Titan/Titan.h"

#include <cmath>


/// <summary>
/// コンストラクタ
/// 各種変数の初期化を行う
/// </summary>
Player::Player():
	move_animation(),
	dying_animation(),
	velocity(0.0f),
	player_state(ePlayerState::IDLE),
	animation_time(0.0f),
	animation_count(0),
	back_ground_sound(NULL),
	enemy(nullptr)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
}

/// <summary>
/// 初期化処理
/// プレイヤーの初期位置や初期状態を設定する
/// </summary>
void Player::Initialize()
{

	// 初期位置（現在画面下側中央）
	location = Vector2D(300, 500);

	// 上端到達回数リセット
	reachTopCount = 0;

	// 移動制限なし、壁方向制限解除
	lockDir = NONE;

	// しゃがみ解除
	isDownPressed = false;

	/*image = move_animation[0];*/

}

/// <summary>
/// 毎フレーム更新処理
/// 入力・移動・制限処理などを行う
/// </summary>
/// <param name="delta_second">1フレームあたりの経過時間</param>
void Player::Update(float delta_second)
{
	
	float speed = 0.5f;
	velocity = Vector2D(0.0f, 0.0f);


	// =============================
	// クールタイム減少処理
	// =============================

	// クールタイム減少
	if (downCooldown > 0.0f)
	{
		downCooldown -= delta_second;
		if (downCooldown < 0.0f)
			downCooldown = 0.0f;
	}

	bool isOnWall = false;   // 壁


	// =============================
	// 壁判定（左右）
	// =============================

	// 右壁に到達
	if (location.x >= 290)
	{
		location.x = 290;
		lockDir = LOCK_RIGHT;   // 次は左方向しか許可しない
		isOnWall = true;
	}

	// 左壁に到達
	if (location.x <= 180)
	{
		location.x = 180;
		lockDir = LOCK_LEFT;  // 次は右方向しか許可しない
		isOnWall = true;
	}

	// 壁に初めて接触したときのみクールタイム発動
	if (isOnWall && !wasOnWall)
	{
		downCooldown = 2.0f;  // 壁クールタイム
	}

	wasOnWall = isOnWall;

	
	// =============================
	// 入力取得
	// =============================

	// ----- 入力処理 -----

	// ----- パッド入力取得 -----
	unsigned int pad = GetJoypadInputState(DX_INPUT_PAD1);

	// ----- 移動判定（キーボード or パッド） -----
	bool moveRight = CheckHitKey(KEY_INPUT_RIGHT) || (pad & PAD_INPUT_RIGHT);
	bool moveLeft = CheckHitKey(KEY_INPUT_LEFT) || (pad & PAD_INPUT_LEFT);
	bool moveDown = CheckHitKey(KEY_INPUT_DOWN) || (pad & PAD_INPUT_DOWN);


	// しゃがみ状態更新（Enemyが参照）
	isDownPressed = moveDown;   // しゃがみ



	//if (isDownPressed)
	//{
	//	velocity = Vector2D(0.0f, 0.0f);
	//}

	//// 右キー
	//if (!isDownPressed && moveRight && lockDir != LOCK_RIGHT)
	//{
	//	if (lockDir != LOCK_RIGHT)  // 右がロックされていない
	//	{
	//		velocity.x = 1.0f;
	//		velocity.y = -0.6f;
	//	}
	//}

	//// 左キー
	//if (!isDownPressed && moveLeft && lockDir != LOCK_LEFT)
	//{
	//	if (lockDir != LOCK_LEFT)   // 左がロックされていない
	//	{
	//		velocity.x = -1.0f;
	//		velocity.y = -0.6f;
	//	}
	//}



	// =============================
	// しゃがみクールタイム開始
	// =============================

	// しゃがみ押した瞬間にクールタイム開始
	if (moveDown && downCooldown <= 0.0f)
	{
		downCooldown = 3.0f;      // 3秒クールタイム
	}


	// =============================
	// 移動処理
	// =============================

	// クールタイム中は動かない
	if (downCooldown > 0.0f)
	{
		velocity = Vector2D(0.0f, 0.0f);
	}
	else
	{
		// ----- 通常移動 -----

		if (moveRight && lockDir != LOCK_RIGHT)
		{
			velocity.x = 1.0f;
			velocity.y = -0.6f;
		}

		if (moveLeft && lockDir != LOCK_LEFT)
		{
			velocity.x = -1.0f;
			velocity.y = -0.6f;
		}
	}

	
	// =============================
	// ベクトル正規化
	// =============================

	float length = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);

	if (length > 0.0f)
	{
		velocity.x = (velocity.x / length) * speed;
		velocity.y = (velocity.y / length) * speed;
	}


	// =============================
	// 位置更新
	// =============================

	// 移動
	location += velocity;
	

	// =============================
	// Y座標制限・上端判定
	// =============================

	if (location.y <= 100.0f)
	{
		location.y = 100.0f;
		OnReachTop();
	}
	else if (location.y > 300.0f)
	{
		location.y = 300.0f;
	}
}


/// <summary>
/// 描画処理
/// プレイヤー本体とUI表示、上端到達回数表示
/// </summary>
void Player::Draw() const
{

	// 上端到達回数表示
	DrawFormatString(50, 50, GetColor(255, 255, 255),"% d", reachTopCount);


	// しゃがみ中は青、それ以外は赤
	int color;

	if (isDownPressed)
	{
		color = GetColor(0, 0, 255); // 青
	}
	else
	{
		color = GetColor(255, 0, 0); // 赤
	}

	int x = (int)location.x;
	int y = (int)location.y;

	DrawBox(x - 10, y - 10, x + 10, y + 10, color, TRUE);


}


/// <summary>
/// 終了処理
/// 動的配列の解放
/// </summary>
void Player::Finalize()
{
	// 動的配列の解放
	move_animation.clear();
	dying_animation.clear();
}



//void Player::OnHitCollision(GameObjectBase* hit_object)
//{
//}



/// <summary>
/// 移動処理（現在は未使用）
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void Player::Movement(float delta_second)
{
	// velocityを使ってプレイヤーの位置座標を変更する
	location += velocity;
}


 //<summary>
 //アニメーション制御
 //</summary>
 //<param name="delta_second">1フレームあたりの時間</param>
void Player::AnimationControl(float delta_second)
{
//	// 移動中のアニメーション
//	animation_time += delta_second;
//	if (animation_time >= (1.0f / 8.0f))
//	{
//		animation_time = 0.0f;
//		animation_count++;
//		if (animation_count >= 4)
//		{
//			animation_count = 0;
//		}
//		// 画像の設定
//		image = move_animation[animation_num[animation_count]];
//
//	}
}


/// <summary>
/// 画面暗転演出
/// alpha値に応じて画面全体をフェードさせる
/// </summary>
void Player::DrawDarkScreen(float alpha)
{
	// alpha = 0.0f ~ 1.0f
	int color = GetColor(50, 50, 80);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(alpha * 255));
	DrawBox(0, 0, 800, 600, color, TRUE);            // 画面全体を塗る
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


/// <summary>
/// 上端到達時処理
/// 到達回数加算
/// Enemy監視時間リセット
/// フェード演出
/// フェードアウト → 位置リセット → フェードイン
/// </summary>
void Player::OnReachTop()
{

	reachTopCount++;

	reachedTopThisFrame = true;

	// エネミーが存在するなら監視リセット
	if (enemy != nullptr)
	{
		enemy->ResetWatchTime();
	}

	// 画面暗転フェードアウト
	for (float alpha = 0.0f; alpha <= 1.0f; alpha += 0.05f)
	{
		ClearDrawScreen();
		DrawDarkScreen(alpha);  // 徐々に暗く
		ScreenFlip();
		WaitTimer(16);          // 60FPS相当
	}

	// プレイヤーの位置リセット
	location = Vector2D(300, 500);

	// 画面明転フェードイン
	for (float alpha = 1.0f; alpha >= 0.0f; alpha -= 0.05f)
	{
		ClearDrawScreen();
		DrawDarkScreen(alpha);  // 徐々に明るく
		ScreenFlip();
		WaitTimer(16);
	}
}
