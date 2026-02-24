#pragma once
#include "../Scenes/SceneBase/SceneBase.h"
#include "../Object/Player/Player.h"

class ResultScene
{
public:
    ResultScene();
    ~ResultScene();

    void Initialize();
    eSceneType Update();
    void Draw() const;
    void Finalize();

    void SetPlayer(Player* p) { player = p; }

private:
    int resultImage;
    int cursor_number;      // カーソル
    float blink_time;       // 点滅タイマー
    bool blink_flag;        // 点滅フラグ

    int cursorSE = -1; // カーソル移動音
    int decisionSE = -1; // 決定音

    Player* player = nullptr; // プレイヤーポインタ
    std::vector<int> numbers;	//数字画像

    int reachTopCount;           // 上端到達回数、プレイヤーが上端に到達したカウント

    int imageHandle;

    int oldPad;
    int oldEnter;
    int oldUp;
    int oldDown;

};