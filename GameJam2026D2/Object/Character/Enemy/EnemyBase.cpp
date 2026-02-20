#include "EnemyBase.h"
#include "../../GameObjectManager.h"

//コンストラクタ
EnemyBase::EnemyBase() :
	anim_max_count(),
	recovery_flame(),
	recovery_time(),
	anim_rate(),
	effect_flame(),
	effect(),
	effect_count(),
	effect_max_count(),
	SE(),
	speed(),
	Damage(),
	on_hit(),
	is_attack(false),
	old_light(false)
{

}

//デストラクタ
EnemyBase::~EnemyBase()
{

}

//初期化処理
void EnemyBase::Initialize()
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	// 闇エフェクト
	//effect_image[0] = rm->GetImages("Resource/Images/Effect/Enemy/Smoke.png", 19, 4, 5, 80, 80);
	effect[0] = effect_image[0][0];
	effect_max_count[0] = 18;
	// 持続ダメージエフェクト
	//effect_image[1] = rm->GetImages/*("Resource/Images/Effect/Enemy/Spark.png", 30, 5, 6, 150, 150)*/;
	effect[1] = effect_image[1][0];
	effect_max_count[1] = 29;

	// 音源の読み込み
	/*SE[0] = rm->GetSounds("Resource/Sounds/EnemySE/Damage1.mp3");
	SE[1] = rm->GetSounds("Resource/Sounds/EnemySE/Damage2.mp3");
	SE[2] = rm->GetSounds("Resource/Sounds/EnemySE/Death.mp3");
	SE[3] = rm->GetSounds("Resource/Sounds/EnemySE/Attack02.mp3");
	SE[4] = rm->GetSounds("Resource/Sounds/UnitSE/Ranged/Ranged_Attack.mp3");*/

	// 音量設定
	for (int i = 0; i < 5; i++)
	{
		ChangeVolumeSoundMem(200, SE[i]);
	}

	alpha = 200;
	add = -ALPHA_ADD;

	// フラグ設定
	is_mobility = true;
	is_aggressive = true;

	// コリジョン設定
	collision.is_blocking = true;
	collision.object_type = eObjectType::Enemy;
	collision.hit_object_type.push_back(eObjectType::Player);
}

// 更新処理
void EnemyBase::Update(float delta_second)
{

	// 移動処理
	if (now_state == State::Move)
	{
		Movement(delta_second);
	}
	// 待機処理(後で触る）
	else if (now_state == State::idle)
	{
		recovery_flame += delta_second;

		// 待機時間が終わったら攻撃状態にする
		if (recovery_flame >= recovery_time)
		{
			now_state = State::Move;
		}
	}

	// アニメーション管理処理
	AnimationControl(delta_second);

	// エフェクト管理処理
	EffectControl(delta_second);
}


// 描画処理
void EnemyBase::Draw() const
{
	//if (ProjectConfig::DEBUG)
	//{
	//	int color;
	//	if (in_light == true) {
	//		color = 0xffffff;
	//	}
	//	else {
	//		color = 0xff0000;
	//	}
	//
	//	// 中心を表示
	//	DrawCircle((int)location.x, (int)location.y, 2, 0x0000ff, TRUE);
	//	// 当たり判定表示
	//	DrawBox((int)(location.x - collision.collision_size.x / 2), (int)(location.y - collision.collision_size.y / 2),
	//		(int)(location.x + collision.collision_size.x / 2), (int)(location.y + collision.collision_size.y / 2), 0xff0000, FALSE);
	//	// 攻撃範囲を表示
	//	DrawBox((int)location.x, (int)(location.y - collision.hitbox_size.y / 2),
	//		(int)(location.x + collision.hitbox_size.x), (int)(location.y + collision.hitbox_size.y / 2), 0xff0000, FALSE);
	//}
}

// 終了時処理
void EnemyBase::Finalize()
{
	GameObjectManager* object = GameObjectManager::GetInstance();
	object->DestroyObject(this);
}

// 当たり判定通知処理
void EnemyBase::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void EnemyBase::OnAreaDetection(GameObject* hit_object)
{
	// 検知したオブジェクトのコリジョン情報
	Collision hit_col = hit_object->GetCollision();

	// 検知したオブジェクトがプレイヤーだったら
	if (hit_col.object_type == eObjectType::Player)
	{
		// 移動状態なら攻撃状態にする
		if (now_state == State::Move)
		{
			now_state = State::Attack;
		}
		// 攻撃状態なら攻撃する
		else if (now_state == State::Attack)
		{
			if (Anim_count == on_hit)
			{
				if (is_attack == false)
				{
					// 攻撃処理
					Attack(hit_object);

					is_attack = true;
				}
			}
		}
	}
}


// 攻撃範囲通知処理
void EnemyBase::NoHit()
{

}

void EnemyBase::Attack(GameObject* hit_object)
{
	// 攻撃対象にダメージを与える
	/*hit_object->HPControl(Damage);*/
}

// 移動処理
void EnemyBase::Movement(float delta_second)
{
	// 右向きに移動させる
	velocity.x = speed;

	// 移動の実行
	location += velocity * delta_second;
}

// アニメーション制御処理
void EnemyBase::AnimationControl(float delta_second)
{
	// 状態更新処理
	old_state = now_state;

	// アニメーションの実行
	switch (now_state)
	{
	case State::idle:
		image = animation[Anim_count];
		break;
	case State::Move:
		image = animation[Anim_count];
		break;
	case State::Attack:
		image = animation[Anim_count];
		// 硬直開始
		if (Anim_count == anim_max_count)
		{
			now_state = State::idle;
			is_attack = false;
			recovery_flame = 0;
		}
		else if (Anim_count == 0)
		{
			// 攻撃SE再生
			PlaySoundMem(SE[3], DX_PLAYTYPE_BACK);
		}
		break;
	}

	// アニメーションの更新
	Anim_flame += delta_second;

}