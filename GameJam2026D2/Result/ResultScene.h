#pragma once
#include "../Scenes/SceneBase/SceneBase.h"

class ResultScene
{
public:
    ResultScene();
    ~ResultScene();

    void Initialize();
    eSceneType Update();
    void Draw() const;
    void Finalize();

private:
    int resultImage;
    int cursor_number;      // カーソル
    float blink_time;       // 点滅タイマー
    bool blink_flag;        // 点滅フラグ

    int imageHandle;

    int oldPad;
    int oldEnter;
    int oldUp;
    int oldDown;

};