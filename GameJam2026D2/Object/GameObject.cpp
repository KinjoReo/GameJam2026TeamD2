#include "GameObject.h"
#include "DxLib.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

// 初期化処理
void GameObject::Initialize()
{
}

// 更新処理
void GameObject::Update(float delta_second)
{
}

// 描画処理
void GameObject::Draw() const
{
    if (image != -1)
    {
        // オフセット値を基に画像の描画を行う
        DrawGraph((int)location.x, (int)location.y, image, TRUE);
    }
}

// 終了時処理
void GameObject::Finalize()
{
}

// 位置座標取得処理
const Vector2D& GameObject::GetLocation() const
{
    return location;
}

const Collision& GameObject::GetCollision() const
{
    return collision;
}

// Zレイヤー情報取得処理
const unsigned char GameObject::GetZLayer() const
{
    return z_layer;
}

// 可動性情報の取得処理
const bool GameObject::GetMobility() const
{
    return is_mobility;
}

/// <summary>
/// InGameの情報取得
/// </summary>
/// <param name="point">InGameSceneのPointer</param>
void GameObject::SetInGamePoint(InGame* point)
{
    Ingame = point;
}

// 当たり判定通知処理
void GameObject::OnHitCollision(GameObject* hit_object)
{
}

// 攻撃範囲通知処理
void GameObject::OnAreaDetection(GameObject* hit_object)
{
}

// 攻撃範囲通知処理
void GameObject::NoHit()
{
}

// 位置情報変更処理
void GameObject::SetLocation(const Vector2D& location)
{
    this->location = location;
}

// HP管理処理
void GameObject::HPControl(float Damage)
{
    this->HP -= Damage;
    if (this->HP < 0)
    {
        this->HP = 0;
    }
}

// HP取得処理
const int GameObject::GetHP() const
{
    return this->HP;
}