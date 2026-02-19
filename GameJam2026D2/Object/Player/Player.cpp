#include "Player.h"
#include "DxLib.h"

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
	location = Vector2D(300, 200); // 画面中央あたり
}

void Player::Update(float delta_second)
{
	// プレイヤー状態によって、動作を変える
	switch (player_state)
	{
	case ePlayerState::IDLE:
		// 画像の設定
		/*image = move_animation[0];*/
		break;
	case ePlayerState::MOVE:
		// 移動処理
		Movement(delta_second);
		// アニメーション制御
		AnimationControl(delta_second);
		break;
	case ePlayerState::DIE:
		// 死亡中のアニメーション
		animation_time += delta_second;
		if (animation_time >= 0.07f)
		{
			animation_time = 0.0f;
			animation_count++;
			// 復活させる
			if (animation_count >= dying_animation.size())
			{
				player_state = ePlayerState::IDLE;
				animation_count = 0;
			}
		}
		image = dying_animation[animation_count];
		break;
	default:
		break;
	}
}

void Player::Draw() const
{
    //__super::Draw();

	//// 赤色を取得
	//int redColor = GetColor(255, 0, 0);

	//// プレイヤーの位置を取得（GameObjectにlocationがある前提）
	//int x = static_cast<int>(location.x);
	//int y = static_cast<int>(location.y);

	//// 20x20 の赤い四角形を描画
	//DrawBox(x - 10, y - 10, x + 10, y + 10, redColor, TRUE);

	//DrawBox(0, 0, 100, 100, GetColor(255, 0, 0), TRUE);

	DrawString(200, 200, "PLAYER DRAW", GetColor(255, 255, 255));


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
	// velocityを使ってマリオの位置座標を変更する
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
