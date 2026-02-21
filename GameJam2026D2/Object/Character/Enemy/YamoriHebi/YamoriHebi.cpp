#include "../YamoriHebi/YamoriHebi.h"
#include "../../../Player/Player.h"
#include "DxLib.h"
#include <cmath>

Enemy2::Enemy2(Player* p)                       // YamoriHebiはEnemy2
    : velocity(0.0f, 0.0f),
    speed(0.15f),
    player(nullptr),
    facingRight(true),
    watchTime(0.0f)
{
    player = p;
}

Enemy2::~Enemy2()
{
}

void Enemy2::Initialize()
{
    // 画面下から出現
    location = Vector2D(300, 400);
}

void Enemy2::Update(float delta_second)
{
    if (player == nullptr) return;

    // ===== スピード切り替え =====
    if (player->IsHidden())
        speed = 0.35f;   // しゃがみ中は速い
    else
        speed = 0.05f;   // 通常速度

    // ===== 追跡処理 =====
    Vector2D dir = player->GetLocation() - location;

    float length = sqrtf(dir.x * dir.x + dir.y * dir.y);

    if (length > 0.0f)
    {
        dir.x /= length;
        dir.y /= length;
    }

    velocity = dir * speed;
    location += velocity;
}

void Enemy2::Draw() const
{
    int x = (int)location.x;
    int y = (int)location.y;

    int color = GetColor(0, 255, 0);  // 緑

    // 向きで三角っぽく描画
    if (facingRight)
    {
        DrawTriangle(x - 10, y - 10,
            x - 10, y + 10,
            x + 15, y,
            color, TRUE);
    }
    else
    {
        DrawTriangle(x + 10, y - 10,
            x + 10, y + 10,
            x - 15, y,
            color, TRUE);
    }
}

void Enemy2::Finalize()
{
}

void Enemy2::ResetPosition()
{
    location = Vector2D(300, 400);
    velocity = Vector2D(0, 0);
    watchTime = 0.0f;
}