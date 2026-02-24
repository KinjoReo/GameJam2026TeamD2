#include "Nightmare.h"
#include "../../../GameObjectManager.h"
#include "Projecttile.h"
#include "../../../Player/Player.h"
#include <typeinfo>
#include <cmath>
#include "DxLib.h"

static Player* FindPlayerFromManager()
{
	GameObjectManager* gom = GameObjectManager::GetInstance();
	const auto& list = gom->GetObjectsList();

	for (GameObject* obj : list)
	{
		if (!obj) continue;
		if (auto* p = dynamic_cast<Player*>(obj))
			return p;
	}
	return nullptr;
}

//コンストラクタ
Nightmare::Nightmare()
{

}

//デストラクタ
Nightmare::~Nightmare()
{

}

void Nightmare::Initialize()
{
	printfDx("Nightmare Initialize\n");

	//親クラスの初期化
	__super::Initialize();

	//コリジョンの設定
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

	ResourceManager* rm = ResourceManager::GetInstance();
	animation = rm->GetImages("Resource / Illustrator / Character / Enemy / Nightmare / Nightmare.png");

	if (animation.empty()) {
		printfDx("Nightmare: animation  load failed or empty\n");
		image = -1;
	}
	else {
		image = animation[0];
	}

	velocity = Vector2D(0.0f, 0.0f);
}

void Nightmare::Update(float delta_second)
{
	// 親クラスの更新
	__super::Update(delta_second);
}

// 描画処理
void Nightmare::Draw() const
{
	DrawCircle((int)location.x, (int)location.y, 10, GetColor(255, 0, 0), TRUE);

	// 画像のずれ
	Vector2D offset;
	offset.x = 100.0f;
	offset.y = -50.0f;

	// 敵の描画
	DrawRotaGraphF(location.x + offset.x, location.y + offset.y,
		3.0, 0.0, image, TRUE, flip_flag);

	if (image != -1) {
		DrawRotaGraphF(location.x + offset.x, location.y + offset.y, 3.0, 0.0, image, TRUE, flip_flag);
	}

	// 親クラスの描画
	__super::Draw();
}

// 終了時処理
void Nightmare::Finalize()
{
	printfDx("Nightmare Finalize\n");

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

static float Len(const Vector2D& v)
{
	return std::sqrt(v.x * v.x + v.y * v.y);
}

// 移動処理
void Nightmare::Movement(float delta_second)
{
	// 親クラスの移動処理
	/*__super::Movement(delta_second);*/

	if (!player) return;

	//敵→プレイヤーの差分
	Vector2D dir = player->GetLocation() - location;
	float dist = Len(dir);
	if (dist < 0.001f) return;

	const float stopDist = 40.0f;
	if (dist <= stopDist) {
		velocity = Vector2D(0, 0); return;
	}

	dir.x /= dist;
	dir.y /= dist;

	//加速
	velocity += dir * (acce1 * delta_second);

	//速度制限
	float v = Len(velocity);
	if (v > maxSpeed)
	{
		velocity.x = velocity.x / v * maxSpeed;
		velocity.y = velocity.y / v * maxSpeed;
	}

	location += velocity;	//velocityを「１フレーム分」にしてるのでdelta_secondをかけない設計
	flip_flag = (velocity.x < 0.0f);

	//カクっと追尾（直進版）----------------------------------------------------------------------------
	////距離
	//float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
	//if (len < 0.001f) return;
	////近づきすぎたら止める（ガタつき防止）
	//const float stopDist = 40.0f;
	//if (len <= stopDist)  return;
	////方向を正規化（長さを1にする）
	//dir.x /= len;
	//dir.y /= len;
	////移動：speedは「1秒当たり」想定
	//location += dir * (speed * delta_second);
	////画像反転（プレイヤーが左にいるなら反転）
	//flip_flag = (dir.x < 0.0f);
	//--------------------------------------------------------------------------------------------------
}

//攻撃処理
void Nightmare::Attack(GameObject* hit_object)
{
	//攻撃SE再生
	PlaySoundMem(SE[0],DX_PLAYTYPE_BACK);

	Projecttile* hand;
	GameObjectManager* object = GameObjectManager::GetInstance();
	hand = object->CreateObject<Projecttile>(Vector2D(location.x + 20.0f, location.y - 5.0f));
	hand->SetTargetLocation(hit_object->GetLocation());
	hand->SetDamage(Damage);
}

// アニメーション制御処理
void Nightmare::AnimationControl(float delta_second)
{
	// 状態が切り替わったらカウントを初期化
	//if (old_state != now_state)
	//{
	//	Anim_count = 0;
	//	// 画像の読み込み
	//	ResourceManager* rm = ResourceManager::GetInstance();
	//	// 各状態のアニメーション画像に差し替え
	//	switch (now_state)
	//	{
	//	case State::idle:
	//		animation = rm->GetImages("Resource/Illustrator/Character/Enemy/Nightmare/Nightmare.png");
	//		image = animation[Anim_count];
	//		anim_max_count = 7;
	//		anim_rate = 0.2f;
	//		break;
	//	case State::Move:
	//		animation = rm->GetImages("Resource/Illustrator/Character/Enemy/Nightmare/Nightmare.png");
	//		image = animation[Anim_count];
	//		anim_max_count = 7;
	//		anim_rate = 0.2f;
	//		break;
	//	case State::Attack:
	//		animation = rm->GetImages("Resource/Illustrator/Character/Enemy/Nightmare/Nightmare.png");
	//		image = animation[Anim_count];
	//		anim_max_count = 9;
	//		anim_rate = 0.15f;
	//		on_hit = 4;
	//		break;
	//	}
	//}

	if (!anim_initialize || old_state != now_state)
	{
		anim_initialize = true;
		Anim_count = 0;
	}

	// 親クラスのアニメーション]
	__super::AnimationControl(delta_second);
}
// エフェクト制御処理
//void Nightmare::EffectControl(float delta_second)
//{
//	// 親クラスのエフェクト
//	__super::EffectControl(delta_second);
//}