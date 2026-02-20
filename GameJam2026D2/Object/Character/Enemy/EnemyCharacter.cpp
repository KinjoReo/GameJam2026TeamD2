#include "EnemyCharacter.h"

// コンストラクタ
EnemyCharacter::EnemyCharacter() :
	now_state(),
	old_state()
{

}

// デストラクタ
EnemyCharacter::~EnemyCharacter()
{

}

// 初期化処理
void EnemyCharacter::Initialize()
{

}

// 更新処理
void EnemyCharacter::Update(float delta_second)
{

}

// 描画処理
void EnemyCharacter::Draw() const
{
	// オフセット値を基に画像の描画を行う
	DrawRotaGraphF(location.x, location.y, 1.0, 0.0, image, TRUE);
}

// 終了時処理
void EnemyCharacter::Finalize()
{

}

// 移動処理
void EnemyCharacter::Movement(float delta_second)
{

}

// アニメーション制御処理
void EnemyCharacter::AnimationControl(float delta_second)
{

}
// エフェクト制御処理
void EnemyCharacter::EffectControl(float delta_second)
{

}

// SE制御処理
void EnemyCharacter::SoundControl()
{

}

// 攻撃処理
void EnemyCharacter::Attack(GameObject* hit_object)
{

}