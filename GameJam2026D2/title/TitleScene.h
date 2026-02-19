#pragma once

#include "SceneType.h"
// タイトル画面の初期化/更新/描画

int TitleInit(void); // 画像読み込みとカーソル初期化
eSceneType TitleUpdate(float delta_second); // 入力処理と遷移判定

void TitleDraw(void); // タイトルとUI描画


