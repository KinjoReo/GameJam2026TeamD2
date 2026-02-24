#pragma once

#include "../Scenes/SceneBase/SceneBase.h"

// SE & BGM
extern int cursorSE;    // カーソル移動音
extern int decisionSE;  // 決定音
extern int titleBGM;    // タイトルBGM

// 初期化 / 更新 / 描画
void TitleInit(void);
eSceneType TitleUpdate(float delta_second);
void TitleDraw(void);
void TitleFinalize(void);

// タイトル画面の初期化/更新/描画
class TitleScene
{
private:
    void Finalize();

    // SE用変数名を統一
    int cursorSE = -1;    // カーソル移動音
    int decisionSE = -1;  // 決定音

    int titleBGM = -1;    // タイトルBGM
};

// 初期化（画像読み込みなど）
void TitleInit(void); // 画像読み込みとカーソル初期化

// 更新（入力処理・シーン遷移判定）
eSceneType TitleUpdate(float delta_second); // 入力処理と遷移判定

// 描画
void TitleDraw(void); // タイトルとUI描画