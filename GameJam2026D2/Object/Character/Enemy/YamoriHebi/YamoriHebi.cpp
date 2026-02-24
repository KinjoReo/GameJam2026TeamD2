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
    location = Vector2D(1400, 1000);

    isActive = false;

    YamoriHebiimage = LoadGraph("Resource/Illustrator/Character/Enemy/YamoriHebi/YamoriHebi.png");

    if (YamoriHebiimage == -1)
    {
        printfDx("YamoriHebi画像の読み込み失敗\n");
    }

    // 登場SE読み込み
    YamoriHebiSE = LoadSoundMem("Resource/Sound/SE/ヤモリヘビ.mp3"); // 適切なSEファイルを指定
    if (YamoriHebiSE == -1)
        printfDx("Enemy2登場SEの読み込み失敗\n");
}

void Enemy2::Update(float delta_second)
{
    if (player == nullptr) return;

    if (!isActive) return;

    // ===== スピード切り替え =====
    float targetSpeed = player->IsHidden() ? 0.35f : 0.05f;     // しゃがみ中は速い、0.05fは通常速度

    // ===== 追跡処理 =====
    Vector2D dir = player->GetLocation() - location;

    float length = sqrtf(dir.x * dir.x + dir.y * dir.y);

    if (length > 0.0f)
    {
        dir.x /= length;
        dir.y /= length;

        // 画像は「上向きが正面」なので角度補正
        drawAngle = atan2f(dir.y, dir.x) + DX_PI_F / 1.75f;
    }

    // 近距離なら加速
    if (length < 940)
    {
        targetSpeed *= 10.0f;
    }

    speed += (targetSpeed - speed) * 0.1f;

    velocity = dir * speed;
    location += velocity;
}

void Enemy2::Draw() const
{
    if (YamoriHebiimage == -1) return;

    if (!isActive) return;

    // 画像の中心を基準に描く
    DrawRotaGraph(
        (int)location.x,
        (int)location.y,
        0.5,
        drawAngle,
        YamoriHebiimage,
        TRUE
    );
}

void Enemy2::Finalize()
{
    if (YamoriHebiimage != -1)
    {
        DeleteGraph(YamoriHebiimage);
        YamoriHebiimage = -1;
    }

    if (YamoriHebiSE != -1)
    {
        DeleteSoundMem(YamoriHebiSE);
        YamoriHebiSE = -1;
    }
}

void Enemy2::ResetPosition()
{
    location = Vector2D(1400, 1000);
    velocity = Vector2D(0, 0);
    watchTime = 0.0f;
    isActive = true;   // 出現ON

    // 登場時にSE再生
    if (YamoriHebiSE != -1)
        PlaySoundMem(YamoriHebiSE, DX_PLAYTYPE_BACK);
}

void Enemy2::Hide()
{
    isActive = false;
}