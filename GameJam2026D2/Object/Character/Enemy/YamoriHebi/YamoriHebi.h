#pragma once

#include "../../../GameObject.h"
#include "../../../../Utility/Vector2D.h"
#include "../../../Player/Player.h"

class Player;   // 前方宣言

class Enemy2 : public GameObject                  // YamoriHebiはEnemy2
{
private:

    Vector2D velocity;      // 移動ベクトル
    float speed;            // 移動速度

    Player* player;         // 追跡対象

    bool facingRight;       // 向き（true = 右）

    float watchTime;        // 監視時間（必要なら使用）

    int YamoriHebiimage;

    float drawAngle;

    int YamoriHebiSE; // 登場時のSE


public:

    Enemy2();
    Enemy2(Player* p);
    virtual ~Enemy2();

    virtual void Initialize() override;
    virtual void Update(float delta_second) override;
    virtual void Draw() const override;
    virtual void Finalize() override;

    // プレイヤー設定
    void SetPlayer(Player* p) { player = p; }

    // 上端到達時などに呼ばれる
    void ResetWatchTime() { watchTime = 0.0f; }

    void ResetPosition();

    Vector2D GetLocation() const { return location; }

    bool isActive;
    void Hide();

};