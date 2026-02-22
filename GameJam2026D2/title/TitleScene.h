#pragma once

#include "../Scenes/SceneBase/SceneBase.h"
// タイトル画面の初期化/更新/描画

// =============================
// TitleScene
// =============================

// 初期化（画像読み込みなど）
void TitleInit(void);// 画像読み込みとカーソル初期化

// 更新（入力処理・シーン遷移判定）
eSceneType TitleUpdate(float delta_second); // 入力処理と遷移判定

// 描画
void TitleDraw(void); // タイトルとUI描画


