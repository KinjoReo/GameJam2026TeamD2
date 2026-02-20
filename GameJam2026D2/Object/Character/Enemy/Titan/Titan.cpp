#include "../Titan/Titan.h"
#include "../../../Player/Player.h"
#include "DxLib.h"

Enemy::Enemy(Player* target)
	: player(target),
	watchTime(0.0f),
	watchLimit(3.0f),   // 3秒見られたらゲームオーバー
	isWatching(false),
	isGameOver(false)
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	location = Vector2D(400, 150);  // 上の方に出現
}

void Enemy::Update(float delta_second)
{
	if (isGameOver) return;

	// プレイヤーが隠れていないなら監視
	if (!player->IsHidden())
	{
		isWatching = true;
		watchTime += delta_second;

		if (watchTime >= watchLimit)
		{
			isGameOver = true;
		}
	}
	else
	{
		// 隠れていればリセット
		isWatching = false;
		watchTime = 0.0f;
	}
}

void Enemy::Draw() const
{
	int x = (int)location.x;
	int y = (int)location.y;

	// エネミー本体
	DrawBox(x - 15, y - 15, x + 15, y + 15,
		GetColor(255, 255, 0), TRUE);

	// 監視中なら赤表示
	if (isWatching)
	{
		DrawFormatString(350, 50, GetColor(255, 0, 0),
			"Watching... %.2f", watchTime);
	}

	// ゲームオーバー表示
	if (isGameOver)
	{
		DrawFormatString(300, 250, GetColor(255, 0, 0),
			"GAME OVER");
	}
}