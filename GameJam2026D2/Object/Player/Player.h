#pragma once

#include "../GameObject.h"
#include <vector>
#include "../../Utility/Vector2D.h"

// プレイヤー状態
enum ePlayerState
{
	IDLE,		// 待機状態
	MOVE,		// 移動状態
	DIE,		// 死亡状態
};

// 進行方向の制限
enum LockDirection
{
	NONE,
	LOCK_RIGHT,
	LOCK_LEFT
};




/// プレイヤークラス（主人公）
class Player : public GameObject
{

//private:
//	// 進行方向状態
//	enum eDirectionState : unsigned char
//	{
//		UP,
//		RIGHT,
//		DOWN,
//		LEFT,
//		NONE,
//	};

private:
	std::vector<int> move_animation;		// 移動のアニメーション画像
	std::vector<int> dying_animation;		// 死亡のアニメーション画像
	Vector2D old_location;					// 前回のlocation
	Vector2D velocity;						// 移動量
	ePlayerState player_state;				// プレイヤー状態
	int back_ground_sound;		// 音源

	float animation_time;					// アニメーション時間
	int animation_count;					// アニメーション添字

	float downCooldown = 0.0f;   // しゃがみ後のクールタイム

	bool wasOnWall = false;      // 壁のクールタイム

	int reachTopCount;                      // プレイヤーが上端に到達したカウント

	int GetReachTopCount() const { return reachTopCount; }      // もし外から参照したいなら

	bool isDownPressed;


	// 移動アニメーションの順番
	const int animation_num[4] = { 1, 2, 3, 2, };

	LockDirection lockDir;   //

public:
	Player();
	virtual ~Player();

	virtual void Initialize() override;
	virtual void Update(float delta_second) override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	bool IsHidden() const { return isDownPressed; }         // Titanのgetter

	Vector2D GetLocation() const { return location; }       // 追いかけ

	/// <summary>
	/// 当たり判定通知処理
	/// </summary>
	/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
	//virtual void OnHitCollision(GameObjectBase* hit_object) override;

private:
	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	void Movement(float delta_second);
	/// <summary>
	/// アニメーション制御
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	void AnimationControl(float delta_second);

	void DrawDarkScreen(float alpha);

	void OnReachTop();

};