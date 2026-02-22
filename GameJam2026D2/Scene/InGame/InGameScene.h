#pragma once

#include "../../Object/Player/Player.h"
#include "../../Scenes/SceneBase/SceneBase.h"

class InGameScene : public SceneBase
{
private:
    int wallHeight;
    int wallTop;

    Player* player = nullptr;


    // BGM
    int InGameBGM = -1;

public:
    InGameScene();
    virtual ~InGameScene();

    void Initialize() override;
    eSceneType Update() override;
    void Draw() const override;
    void Finalize() override;

    eSceneType GetNowSceneType() const override
    {
        return eSceneType::eInGame;   // enum class‘Î‰ž
    }
};