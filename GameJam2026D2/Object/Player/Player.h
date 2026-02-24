#pragma once

#include "../GameObject.h"
#include <vector>
#include "../../Utility/Vector2D.h"


// ============================================================
// プレイヤー状態を表す列挙型
// ============================================================
/// <summary>
/// プレイヤーの現在状態
/// </summary>
enum ePlayerState
{
	IDLE,		// 待機状態
	MOVE,		// 移動状態
	DIE,		// 死亡状態
};


// ========================================
// 壁接触時の移動制限方向
// ========================================
/// <summary>
/// 壁に接触した際の進行方向ロック用
/// </summary>
// 進行方向の制限
enum LockDirection
{
	NONE,            // 制限なし
	LOCK_RIGHT,      // 右方向に進めない
	LOCK_LEFT        // 左方向に進めない
};


class Enemy;        // Enemyクラスの前方宣言


// ============================================================
// Player クラス
// ============================================================

// プレイヤークラス（主人公）
// 移動・しゃがみ・壁制限・到達カウントなどを管理する
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
	// ========================================
	// アニメーション関連
	// ========================================

	std::vector<int> move_animation;		// 移動のアニメーション画像
	std::vector<int> dying_animation;		// 死亡のアニメーション画像

	float animation_time;					// アニメーション経過時間
	int animation_count;					// アニメーション番号

	// 移動アニメーションの順番
	const int animation_num[4] = { 1, 2, 3, 2, };    // 再生順


	// ========================================
	// 移動関連
	// ========================================

	Vector2D old_location;					// 前回のフレームの位置
	Vector2D velocity;						// 移動量

	ePlayerState player_state;				// 現在のプレイヤー状態
	LockDirection lockDir;                  // 壁接触時の移動方向制限


	// ========================================
	// 入力・制限関連
	// ========================================

	float downCooldown = 0.0f;   // しゃがみ後のクールタイム
	bool wasOnWall = false;      // 前フレームで壁に接触していたか
	bool isDownPressed;          // 現在しゃがんでいるか
	float wallCooldown = 0.0f;
	float crouchActiveTime = 0.0f;   // しゃがみ中の硬直時間
	float crouchCooldown = 0.0f;     // 再しゃがみ待ち時間

	// ========================================
	// ゲーム進行関連
	// ========================================

	int reachTopCount;           // 上端到達回数、プレイヤーが上端に到達したカウント
	int back_ground_sound;		 // 音源、後ろで流れるBGMやSE



	/// <summary>
	/// 監視エネミーへの参照
	/// 上端到達時に監視時間をリセットするため使用
	/// </summary>
	Enemy* enemy;   // 監視エネミー


	/// <summary>
	/// このフレームで上端到達したかどうか
	/// 外部（Sceneなど）への通知用
	/// </summary>
	bool reachedTopThisFrame = false;

	int crouchSE;           // しゃがみSE
	bool isCrouchSEPlaying; // 再生中かどうか


public:

	// ========================================
	// 基本関数
	// ========================================

	Player();
	virtual ~Player();

	// 初期化処理
	virtual void Initialize() override;

	// 毎フレーム更新処理
	virtual void Update(float delta_second) override;

	// 描画処理
	virtual void Draw() const override;

	// 終了処理
	virtual void Finalize() override;


	// ========================================
	// 外部参照用 Getter
	// ========================================

	/// <summary>
	/// しゃがみ状態を取得
	/// true = 隠れている
	/// </summary>
	bool IsHidden() const { return isDownPressed; }


	/// <summary>
	/// 現在位置を取得（追跡用など）
	/// </summary>
	Vector2D GetLocation() const { return location; }


	/// <summary>
	/// 上端到達したかどうか取得
	/// </summary>
	bool DidReachTop() const { return reachedTopThisFrame; }


	/// <summary>
	/// 到達フラグリセット
	/// </summary>
	void ResetReachFlag() { reachedTopThisFrame = false; }


	/// <summary>
	/// 上端到達回数取得（内部用）
	/// </summary>
	int GetReachTopCount() const { return reachTopCount; }      // もし外から参照したいなら

	/// <summary>
	/// 当たり判定通知処理
	/// </summary>
	/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
	//virtual void OnHitCollision(GameObjectBase* hit_object) override;


private:

	// ========================================
	// 内部処理
	// ========================================

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


	/// <summary>
	/// 画面暗転演出
	/// </summary>
	void DrawDarkScreen(float alpha);


	/// <summary>
	/// 上端到達時処理
	/// フェード演出＋位置リセット
	/// </summary>
	void OnReachTop();


	/// <summary>
	/// Enemy設定
	/// </summary>
	void SetEnemy(Enemy* e) { enemy = e; }

};