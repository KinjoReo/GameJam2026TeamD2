#pragma once

#include "../../../GameObject.h"
#include "../../../../Utility/Vector2D.h"

class Player;       // Playerクラスの前方宣言（ヘッダ依存を減らすため）


/// <summary>
/// プレイヤーを監視するエネミークラス
/// 一定時間見続けるとゲームオーバーにする
/// </summary>
class Enemy : public GameObject
{
private:
	Player* player;          // 監視対象、監視するプレイヤーへのポインタ

	// 監視システム用変数
	float watchTime;         // 見続けている時間、現在の監視時間
	float watchLimit;        // ゲームオーバーになる時間、限界時間

	// 状態フラグ
	bool isWatching;         // プレイヤーを今見つけているか
	bool isGameOver;         // ゲームオーバー状態

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="target">監視対象のプレイヤー</param>
	Enemy(Player* target);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Enemy();


	/// <summary>
	/// 初期化処理
	/// 出現位置などを設定する
	/// </summary>
	virtual void Initialize() override;

	/// <summary>
	/// 毎フレーム更新処理
	/// 監視時間のカウントやゲームオーバー判定を行う
	/// </summary>
	virtual void Update(float delta_second) override;

	/// <summary>
	/// 描画処理
	/// 本体の描画および赤画面エフェクトを表示する
	/// </summary>
	virtual void Draw() const override;

	/// <summary>
	/// 監視中に画面を赤くする演出
	/// </summary>
	/// <param name="alpha">透明度（0.0f ～ 1.0f）</param>
	void DrawRedScreen(float alpha) const;

	/// <summary>
	/// ゲームオーバー状態を取得する
	/// </summary>
	/// <returns>true = ゲームオーバー</returns>
	bool IsGameOver() const { return isGameOver; }
};