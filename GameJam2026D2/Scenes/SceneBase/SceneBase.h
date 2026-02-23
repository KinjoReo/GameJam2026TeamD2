#pragma once

enum class eSceneType
{
	eTitle,
	eInGame,
	eHelp,
	eResult,
	eGameOver,
	eEnd,
};

class SceneBase
{
protected:
	// 全シーンで所有するメンバ変数を宣言

public:
	SceneBase() {}
	virtual ~SceneBase() {}

	// 初期化処理
	virtual void Initialize() {}

	// 更新処理
	// 引　数：1フレーム当たりの時間
	// 戻り値：次のシーンタイプ
	virtual eSceneType Update()
	{
		return GetNowSceneType();
	}

	// 描画処理
	virtual void Draw() const {}

	// 終了時処理
	virtual void Finalize() {}

	// 現在のシーンタイプ（オーバーライド必須）
	virtual eSceneType GetNowSceneType() const = 0;
};