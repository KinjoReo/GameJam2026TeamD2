#include "Nightmare.h"
#include "../../../GameObjectManager.h"

//コンストラクタ
Nightmare::Nightmare()
{
	count++;
}

//デストラクタ
Nightmare::~Nightmare()
{
	count--;
}

void Nightmare::Initialize()
{
	//親クラスの初期化
	__super::Initialize();

	//子リジョンの設定
	collision.collision_size = Vector2D(180.0f, 180.0f);
	collision.hitbox_size = Vector2D(200.0f, 300.0f);
	z_layer = 2;

	//画像反転あり
	flip_flag = true;

	//最初の状態にする
	now_state = State::Move;

	//攻撃（後から変更）
	Damage = 6.0f;

	//スピードの初期化
	speed = 20.0f;

	//リカバリータイム（硬直時間)
	recovery_time = 2.0f;
}

void Nightmare::Update(float delta_second)
{
	// 親クラスの更新
	__super::Update(delta_second);
}

// 描画処理
void Nightmare::Draw() const
{
	// 画像のずれ
	Vector2D offset;
	offset.x = 100.0f;
	offset.y = -50.0f;

	// 敵の描画
	DrawRotaGraphF(location.x + offset.x, location.y + offset.y,
		3.0, 0.0, image, TRUE, flip_flag);


	// 親クラスの描画
	__super::Draw();
}

// 終了時処理
void Nightmare::Finalize()
{
	// 親クラスの終了
	__super::Finalize();
}

// 当たり判定通知処理
void Nightmare::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void Nightmare::OnAreaDetection(GameObject* hit_object)
{
	// 親クラスの攻撃範囲通知処理
	__super::OnAreaDetection(hit_object);
}

// 攻撃範囲通知処理
void Nightmare::NoHit()
{

}

// 移動処理
void Nightmare::Movement(float delta_second)
{
	// 親クラスの移動処理
	__super::Movement(delta_second);
}

// アニメーション制御処理
void Nightmare::AnimationControl(float delta_second)
{
	// 状態が切り替わったらカウントを初期化
	if (old_state != now_state)
	{
		Anim_count = 0;

		// 画像の読み込み
		ResourceManager* rm = ResourceManager::GetInstance();

		// 各状態のアニメーション画像に差し替え
		switch (now_state)
		{
		case State::idle:
			//animation = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Idle.png", 8, 8, 1, 140, 93);
			image = animation[Anim_count];
			anim_max_count = 7;
			anim_rate = 0.2f;
			break;
		case State::Move:
			//animation = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Walk.png", 8, 8, 1, 140, 93);
			image = animation[Anim_count];
			anim_max_count = 7;
			anim_rate = 0.2f;
			break;
		case State::Attack:
			//animation = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Attack.png", 10, 10, 1, 140, 93);
			image = animation[Anim_count];
			anim_max_count = 9;
			anim_rate = 0.15f;
			on_hit = 4;
			break;
		}
	}

	// 親クラスのアニメーション]
	__super::AnimationControl(delta_second);
}
// エフェクト制御処理
void Nightmare::EffectControl(float delta_second)
{
	// 親クラスのエフェクト
	__super::EffectControl(delta_second);
}