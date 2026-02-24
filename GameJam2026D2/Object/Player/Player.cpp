#include "Player.h"
#include "DxLib.h"
#include "../Character/Enemy/Titan/Titan.h"
#include "../../Utility/ResourceManager.h"

#include <cmath>


/// <summary>
/// コンストラクタ
/// 各種変数の初期化を行う
/// </summary>
Player::Player():
	move_animation(),
	dying_animation(),
	velocity(0.0f),
	player_state(ePlayerState::IDLE),
	animation_time(0.0f),
	animation_count(0),
	back_ground_sound(NULL),
	enemy(nullptr)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
}

/// <summary>
/// 初期化処理
/// プレイヤーの初期位置や初期状態を設定する
/// </summary>
void Player::Initialize()
{

	// 初期位置（現在画面下側中央）
	location = Vector2D(370, 720);

	// 上端到達回数リセット
	reachTopCount = 0;

	// 移動制限なし、壁方向制限解除
	lockDir = NONE;

	// しゃがみ解除
	isDownPressed = false;

	/*image = move_animation[0];*/

	// プレイヤー画像読み込み
	playerImage = LoadGraph("Resource/Illustrator/Character/Player/Nobori/PlayerAruki.png");
	GetGraphSize(playerImage, &playerW, &playerH);
	if (playerImage == -1)
	{
		printfDx("プレイヤー画像の読み込み失敗\n");
	}
	// しゃがみ画像
	crouchImage = LoadGraph("Resource/Illustrator/Character/Player/Syagami/PlayerSyagami.png");
	GetGraphSize(crouchImage, &crouchW, &crouchH);

	// しゃがみSE読み込み
	crouchSE = LoadSoundMem("Resource/Sound/SE/しゃがみ2.mp3");  // しゃがみSE
	if (crouchSE == -1)
		printfDx("しゃがみSEの読み込み失敗\n");

	isCrouchSEPlaying = false;

	// 数字画像読み込み（1枚画像から切り出し）
	int fullImg = LoadGraph("Resource/Illustrator/Background/Suuji/Suuji.png");
	if (fullImg == -1)
		printfDx("数字画像の読み込み失敗\n");

	// 数字の幅・高さ
	int w, h;
	GetGraphSize(fullImg, &w, &h);
	numberWidth = w / 10;  // 0～9 10個
	numberHeight = h;

	for (int i = 0; i < 10; i++)
	{
		numberImages[i] = DerivationGraph(fullImg, i * numberWidth, 0, numberWidth, numberHeight);
	}

	DeleteGraph(fullImg);  // 元画像は不要

	///////////////////////////////////////////////////
	// リソースマネージャーインスタンス取得
	ResourceManager* rm = ResourceManager::GetInstance();

	//(ファイルパス,総分割画像数,横分割数,縦分割数,横画像サイズ,縦画像サイズ);
	numbers = rm->GetImages("Resource/Illustrator/Background/Suuji/Suuji2.png", 10, 5, 2, 350, 300);
	/*numbers[0];*/

	///////////////////////////////////////////////////

}

/// <summary>
/// 毎フレーム更新処理
/// 入力・移動・制限処理などを行う
/// </summary>
/// <param name="delta_second">1フレームあたりの経過時間</param>
void Player::Update(float delta_second)
{
	
	if (isFading) return; // フェード中は更新しない


	float speed = 0.5f;
	velocity = Vector2D(0.0f, 0.0f);


	// =============================
	// タイマー減算
	// =============================
	if (wallCooldown > 0.0f)
	{
		wallCooldown -= delta_second;
		if (wallCooldown < 0.0f) wallCooldown = 0.0f;
	}

	if (crouchActiveTime > 0.0f)
	{
		crouchActiveTime -= delta_second;
		if (crouchActiveTime < 0.0f) crouchActiveTime = 0.0f;
	}

	if (crouchCooldown > 0.0f)
	{
		crouchCooldown -= delta_second;
		if (crouchCooldown < 0.0f) crouchCooldown = 0.0f;
	}

	bool isOnWall = false;   // 壁


	// =============================
	// 壁判定（左右）
	// =============================

	// 右壁に到達
	if (location.x >= 370)
	{
		location.x = 370;
		lockDir = LOCK_RIGHT;   // 次は左方向しか許可しない
		isOnWall = true;
	}

	// 左壁に到達
	if (location.x <= 190)
	{
		location.x = 190;
		lockDir = LOCK_LEFT;  // 次は右方向しか許可しない
		isOnWall = true;
	}

	// 壁に初めて接触したときのみクールタイム発動
	if (isOnWall && !wasOnWall)
	{
		wallCooldown = 2.0f;  // 壁クールタイム
		crouchCooldown = 2.0f;   // しゃがみ再使用2秒
	}

	wasOnWall = isOnWall;

	
	// =============================
	// 入力取得
	// =============================

	// ----- 入力処理 -----

	// ----- パッド入力取得 -----
	unsigned int pad = GetJoypadInputState(DX_INPUT_PAD1);

	// ----- 移動判定（キーボード or パッド） -----
	bool moveRight = CheckHitKey(KEY_INPUT_RIGHT) || (pad & PAD_INPUT_RIGHT);
	bool moveLeft = CheckHitKey(KEY_INPUT_LEFT) || (pad & PAD_INPUT_LEFT);
	bool moveDown = CheckHitKey(KEY_INPUT_DOWN) || (pad & PAD_INPUT_B);

	if (moveRight)
	{
		isFacingRight = true;
	}

	if (moveLeft)
	{
		isFacingRight = false;
	}

	// =============================
    // しゃがみ開始判定
    // クールタイム中はしゃがめない
    // =============================
	if (moveDown && crouchCooldown <= 0.0f && crouchActiveTime <= 0.0f)
    {
        crouchActiveTime = 5.0f;   // 5秒間しゃがみ硬直
        crouchCooldown   = 1.0f;   // 再使用1秒
		// しゃがみ状態更新（Enemyが参照）
		isDownPressed = moveDown;   // しゃがみ
    }

    // しゃがみ状態更新
    isDownPressed = (crouchActiveTime > 0.0f);


	// =============================
	// 移動処理
	// =============================

	// 壁硬直 or しゃがみ硬直中は動けない
	if (wallCooldown > 0.0f || crouchActiveTime > 0.0f)
	{
		velocity = Vector2D(0.0f, 0.0f);
	}
	else
	{
		// ----- 通常移動 -----

		if (moveRight && lockDir != LOCK_RIGHT)
		{
			velocity.x = 1.0f;
			velocity.y = -0.6f;
		}

		if (moveLeft && lockDir != LOCK_LEFT)
		{
			velocity.x = -1.0f;
			velocity.y = -0.6f;
		}
	}

	
	// =============================
	// ベクトル正規化
	// =============================

	float length = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);

	if (length > 0.0f)
	{
		velocity.x = (velocity.x / length) * speed;
		velocity.y = (velocity.y / length) * speed;
	}


	// =============================
	// 位置更新
	// =============================

	// 移動
	location += velocity;
	

	// =============================
	// Y座標制限・上端判定
	// =============================

	if (location.y <= 100.0f)
	{
		location.y = 100.0f;
		OnReachTop();
	}
	else if (location.y > 700.0f)
	{
		location.y = 700.0f;
	}

	// しゃがみSE再生制御
	if (isDownPressed)
	{
		// 再生していなければ鳴らす
		if (!isCrouchSEPlaying && crouchSE != -1)
		{
			PlaySoundMem(crouchSE, DX_PLAYTYPE_LOOP); // ループ再生
			isCrouchSEPlaying = true;
		}
	}
	else
	{
		// しゃがみ解除なら停止
		if (isCrouchSEPlaying)
		{
			StopSoundMem(crouchSE);
			isCrouchSEPlaying = false;
		}
	}
}


/// <summary>
/// 描画処理
/// プレイヤー本体とUI表示、上端到達回数表示
/// </summary>
void Player::Draw() const
{


	// しゃがみ中は青、それ以外は赤
	int color;

	if (isDownPressed)
	{
		color = GetColor(0, 0, 255); // 青
	}
	else
	{
		color = GetColor(255, 0, 0); // 赤
	}

	int x = (int)location.x;
	int y = (int)location.y;

	int baseSize = 40;   // 横幅基準

	int img;
	int w, h;

	if (isDownPressed)
	{
		img = crouchImage;
		w = crouchW;
		h = crouchH;
	}
	else
	{
		img = playerImage;
		w = playerW;
		h = playerH;
	}

	// 縦横比を維持
	float scale = (float)baseSize / w;
	int drawW = baseSize;
	int drawH = (int)(h * scale);

	if (isFacingRight)
	{
		DrawExtendGraph(
			x + drawW / 2,
			y - drawH / 2,
			x - drawW / 2,
			y + drawH / 2,
			img,
			TRUE
		);
	}
	else
	{
		DrawExtendGraph(
			x - drawW / 2,
			y - drawH / 2,
			x + drawW / 2,
			y + drawH / 2,
			img,
			TRUE
		);
	}

	// 上端到達回数を数字画像で描画
	// 数字描画
	int count = reachTopCount;
	int digits[5];
	int digitCount = 0;

	// 0の場合
	if (count == 0) { digits[0] = 0; digitCount = 1; }
	else {
		while (count > 0) { digits[digitCount++] = count % 10; count /= 10; }
	}

	int startX = 25;
	int startY = 25;
	float scaleX = 0.3f; // 横拡大率（350px → 105pxくらい）
	float scaleY = 0.3f; // 縦拡大率（300px → 90pxくらい）
	int spacing = 4;

	// 右から左に描画
	for (int i = 0; i < digitCount; i++)
	{
		int digit = digits[digitCount - 1 - i];

		// numbers配列の0～9番に数字画像が対応している場合
		// もし数字が10～11番にある場合は調整
		int img = numbers[digit];

		if (img != -1)
		{
			int w = 350 * scaleX;
			int h = 300 * scaleY;

			DrawExtendGraph(
				startX + i * (w + spacing), startY,
				startX + i * (w + spacing) + w, startY + h,
				img,
				TRUE
			);
		}
	}

	// 上端到達回数表示
	//DrawFormatString(50, 50, GetColor(255, 255, 255), "% d", reachTopCount);

	// 数字描画（最後に描画）
	 // 右から左に描画するので逆順
	

}


/// <summary>
/// 終了処理
/// 動的配列の解放
/// </summary>
void Player::Finalize()
{
	// 動的配列の解放
	move_animation.clear();
	dying_animation.clear();

	if (playerImage != -1)
	{
		DeleteGraph(playerImage);
		playerImage = -1;
	}

	if (crouchSE != -1)
	{
		DeleteSoundMem(crouchSE);
		crouchSE = -1;
	}
}



//void Player::OnHitCollision(GameObjectBase* hit_object)
//{
//}



/// <summary>
/// 移動処理（現在は未使用）
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void Player::Movement(float delta_second)
{
	// velocityを使ってプレイヤーの位置座標を変更する
	location += velocity;
}


 //<summary>
 //アニメーション制御
 //</summary>
 //<param name="delta_second">1フレームあたりの時間</param>
void Player::AnimationControl(float delta_second)
{
//	// 移動中のアニメーション
//	animation_time += delta_second;
//	if (animation_time >= (1.0f / 8.0f))
//	{
//		animation_time = 0.0f;
//		animation_count++;
//		if (animation_count >= 4)
//		{
//			animation_count = 0;
//		}
//		// 画像の設定
//		image = move_animation[animation_num[animation_count]];
//
//	}
}


/// <summary>
/// 画面暗転演出
/// alpha値に応じて画面全体をフェードさせる
/// </summary>
void Player::DrawDarkScreen(float alpha)
{
	// alpha = 0.0f ~ 1.0f
	int color = GetColor(80, 80, 140);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(alpha * 255));
	DrawBox(0, 0, 1280, 720, color, TRUE);            // 画面全体を塗る
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


/// <summary>
/// 上端到達時処理
/// 到達回数加算
/// Enemy監視時間リセット
/// フェード演出
/// フェードアウト → 位置リセット → フェードイン
/// </summary>
void Player::OnReachTop()
{

	reachTopCount++;

	reachedTopThisFrame = true;

	// フェード開始
	isFading = true;

	// エネミーが存在するなら監視リセット
	if (enemy != nullptr)
	{
		enemy->ResetWatchTime();
	}

	// 画面暗転フェードアウト
	for (float alpha = 0.0f; alpha <= 1.0f; alpha += 0.05f)
	{
		ClearDrawScreen();
		DrawDarkScreen(alpha);  // 徐々に暗く
		ScreenFlip();
		WaitTimer(16);          // 60FPS相当
	}

	// プレイヤーの位置リセット
	location = Vector2D(380, 720);


	// 画面明転フェードイン
	for (float alpha = 1.0f; alpha >= 0.0f; alpha -= 0.05f)
	{
		ClearDrawScreen();
		DrawDarkScreen(alpha);  // 徐々に明るく
		ScreenFlip();
		WaitTimer(16);
	}

	isFading = false; // フェード終了
}

bool Player::IsFading() const
{
	return isFading;
}
