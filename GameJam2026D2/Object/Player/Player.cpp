#include "Player.h"
#include "DxLib.h"

#include <cmath>


Player::Player():
	move_animation(),
	dying_animation(),
	velocity(0.0f),
	player_state(ePlayerState::IDLE),
	animation_time(0.0f),
	animation_count(0),
	back_ground_sound(NULL)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	/*image = move_animation[0];*/
	location = Vector2D(300, 500); // 画面中央あたり

	reachTopCount = 0;

	lockDir = NONE;

	isDownPressed = false;
}

void Player::Update(float delta_second)
{
	
	float speed = 0.5f;
	velocity = Vector2D(0.0f, 0.0f);

	// クールタイム減少
	if (downCooldown > 0.0f)
	{
		downCooldown -= delta_second;
		if (downCooldown < 0.0f)
			downCooldown = 0.0f;
	}

	bool isOnWall = false;

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

	// 壁に「今フレーム初めて」当たった時だけ
	if (isOnWall && !wasOnWall)
	{
		downCooldown = 2.0f;  // 壁クールタイム
	}

	wasOnWall = isOnWall;

	

	// ----- 入力処理 -----

	// ----- パッド入力取得 -----
	unsigned int pad = GetJoypadInputState(DX_INPUT_PAD1);

	// ----- 移動判定（キーボード or パッド） -----
	bool moveRight = CheckHitKey(KEY_INPUT_RIGHT) || (pad & PAD_INPUT_RIGHT);
	bool moveLeft = CheckHitKey(KEY_INPUT_LEFT) || (pad & PAD_INPUT_LEFT);
	bool moveDown = CheckHitKey(KEY_INPUT_DOWN) || (pad & PAD_INPUT_DOWN);

	isDownPressed = moveDown;   // 青くなる

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

	// しゃがみ押した瞬間にクールタイム開始
	if (moveDown && downCooldown <= 0.0f)
	{
		downCooldown = 3.0f;      // 3秒クールタイム
	}

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

	// 正規化
	float length = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);

	if (length > 0.0f)
	{
		velocity.x = (velocity.x / length) * speed;
		velocity.y = (velocity.y / length) * speed;
	}

	// 移動
	location += velocity;
	

	// Y制限
	if (location.y <= 100.0f)
	{
		location.y = 100.0f;
		OnReachTop();
	}
	else if (location.y > 300.0f)
	{
		location.y = 300.0f;
	}

	//if (location.y < 100) {
	//	// プレイヤーが上端に到達した
	//	location.y = 100;      // Y制限
	//	OnReachTop();          // 暗転＋リスタート
	//}
	// else は何もしない（＝止まる）
}

void Player::Draw() const
{
    //__super::Draw();

	DrawFormatString(50, 50, GetColor(255, 255, 255),
		"% d", reachTopCount);


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
/// 移動処理
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


void Player::DrawDarkScreen(float alpha)
{
	// alpha = 0.0f ~ 1.0f
	int color = GetColor(50, 50, 80);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(alpha * 255));
	DrawBox(0, 0, 800, 600, color, TRUE); // 画面全体を黒で塗る
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Player::OnReachTop()
{

	reachTopCount++;

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
