#include "DxLib.h"
#include "Object/Player/Player.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    ChangeWindowMode(TRUE);

    if (DxLib_Init() == -1)
        return -1;

    SetDrawScreen(DX_SCREEN_BACK);

    Player player;
    player.Initialize();

    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();

        player.Update(1.0f / 60.0f);
        player.Draw();

        ScreenFlip();
    }

    player.Finalize();
    DxLib_End();
    return 0;
}
