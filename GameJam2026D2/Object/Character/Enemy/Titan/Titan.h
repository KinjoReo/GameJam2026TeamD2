#pragma once

#include "../../../GameObject.h"
#include "../../../../Utility/Vector2D.h"

class Player;

class Enemy : public GameObject
{
private:
	Player* player;          // 監視対象

	float watchTime;         // 見続けた時間
	float watchLimit;        // ゲームオーバーになる時間

	bool isWatching;         // 今見つけているか
	bool isGameOver;         // ゲームオーバー状態

public:
	Enemy(Player* target);
	virtual ~Enemy();

	virtual void Initialize() override;
	virtual void Update(float delta_second) override;
	virtual void Draw() const override;

	bool IsGameOver() const { return isGameOver; }
};