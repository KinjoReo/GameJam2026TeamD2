#pragma once

#include "../../Object/Player/Player.h"
#include "../../Scenes/SceneBase/SceneBase.h"
#include "../../Object/Character/Enemy/Titan/Titan.h"
#include "../../Object/Character/Enemy/YamoriHebi/YamoriHebi.h"
#include "../../Object/Character/Enemy/First/Nightmare.h"  

class InGameScene : public SceneBase
{
private:
    int wallHeight;
    int wallTop;

    Player* player = nullptr;
    Enemy* enemy = nullptr;
    Enemy2* enemy2 = nullptr;

    int backgroundImage = -1;

    int backgroundImage2 = -1;

    int backgroundImage3 = -1;

    // BGM
    int InGameBGM = -1;

    /*Nightmare* nightmare = nullptr;
    bool spawnNightmare = true;*/

public:
    InGameScene();
    virtual ~InGameScene();

    void Initialize() override;
    eSceneType Update() override;
    void Draw() const override;
    void Finalize() override;

    Enemy* titan;
    Enemy2* yamorihebi;

    bool spawnTitan;
    bool spawnYamoriHebi;

    eSceneType GetNowSceneType() const override
    {
        return eSceneType::eInGame;   // enum class???
    }
};