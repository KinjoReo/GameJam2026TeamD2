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
	location = Vector2D(370, 740);

	// 上端到達回数リセット
	reachTopCount = 0;

	// 移動制限なし、壁方向制限解除
	lockDir = NONE;

	// しゃがみ解除
	isDownPressed = false;

	/*image = move_animation[0];*/

	// しゃがみSE読み込み
	crouchSE = LoadSoundMem("Resource/Sound/SE/しゃがみ2.mp3");  // しゃがみSE
	if (crouchSE == -1)
		printfDx("しゃがみSEの読み込み失敗\n");

	isCrouchSEPlaying = false;

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
	// タイマー減算
	// =============================
	if (wallCooldown > 0.0f)
	{
		wallCooldown -= delta_second;
		if (wallCooldown < 0.0f) wallCooldown = 0.0f;
	}

	if (crouchActiveTime > 0.0f)
	{
		crouchActiveTime -= delta_second;
		if (crouchActiveTime < 0.0f) crouchActiveTime = 0.0f;
	}

	if (crouchCooldown > 0.0f)
	{
		crouchCooldown -= delta_second;
		if (crouchCooldown < 0.0f) crouchCooldown = 0.0f;
	}

	bool isOnWall = false;   // 壁


	// =============================
	// 壁判定（左右）
	// =============================

	// 右壁に到達
	if (location.x >= 370)
	{
		location.x = 370;
		lockDir = LOCK_RIGHT;   // 次は左方向しか許可しない
		isOnWall = true;
	}

	// 左壁に到達
	if (location.x <= 190)
	{
		location.x = 190;
		lockDir = LOCK_LEFT;  // 次は右方向しか許可しない
		isOnWall = true;
	}

	// 壁に初めて接触したときのみクールタイム発動
	if (isOnWall && !wasOnWall)
	{
		wallCooldown = 2.0f;  // 壁クールタイム
		crouchCooldown = 2.0f;   // しゃがみ再使用2秒
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
	bool moveDown = CheckHitKey(KEY_INPUT_DOWN) || (pad & PAD_INPUT_B);


	// =============================
    // しゃがみ開始判定
    // クールタイム中はしゃがめない
    // =============================
	if (moveDown && crouchCooldown <= 0.0f && crouchActiveTime <= 0.0f)
    {
        crouchActiveTime = 5.0f;   // 5秒間しゃがみ硬直
        crouchCooldown   = 1.0f;   // 再使用1秒
		// しゃがみ状態更新（Enemyが参照）
		isDownPressed = moveDown;   // しゃがみ
    }

    // しゃがみ状態更新
    isDownPressed = (crouchActiveTime > 0.0f);


	// =============================
	// 移動処理
	// =============================

	// 壁硬直 or しゃがみ硬直中は動けない
	if (wallCooldown > 0.0f || crouchActiveTime > 0.0f)
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
	else if (location.y > 700.0f)
	{
		location.y = 700.0f;
	}

	// しゃがみSE再生制御
	if (isDownPressed)
	{
		// 再生していなければ鳴らす
		if (!isCrouchSEPlaying && crouchSE != -1)
		{
			PlaySoundMem(crouchSE, DX_PLAYTYPE_LOOP); // ループ再生
			isCrouchSEPlaying = true;
		}
	}
	else
	{
		// しゃがみ解除なら停止
		if (isCrouchSEPlaying)
		{
			StopSoundMem(crouchSE);
			isCrouchSEPlaying = false;
		}
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

	if (crouchSE != -1)
	{
		DeleteSoundMem(crouchSE);
		crouchSE = -1;
	}
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
	int color = GetColor(80, 80, 140);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(alpha * 255));
	DrawBox(0, 0, 1280, 720, color, TRUE);            // 画面全体を塗る
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
	location = Vector2D(380, 740);

	// 画面明転フェードイン
	for (float alpha = 1.0f; alpha >= 0.0f; alpha -= 0.05f)
	{
		ClearDrawScreen();
		DrawDarkScreen(alpha);  // 徐々に明るく
		ScreenFlip();
		WaitTimer(16);
	}
}
