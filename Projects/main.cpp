#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include "Pad.h"
#include "Game.h"
#include "Font.h"
#include "SceneManager.h"


// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetWindowText("AscensioQuest");

	// 一部の関数はDxLib_Init()の前に実行する必要がある
	ChangeWindowMode(true);

	//画面サイズの変更
	SetGraphMode(Game::kScreenWindidth, Game::kScreenHeight, Game::kColorDepth);

	// DirectX11を使用するようにする。(DirectX9も可、一部機能不可)
	// Effekseerを使用するには必ず設定する。
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// Effekseerの初期化
	Effekseer_Init(8000);

	// フルスクリーン切り替え時におかしくならないように
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// Zバッファの設定
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	// ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	SceneManager* m_pSceneManager = new SceneManager;
	m_pSceneManager->Init();

	Pad* m_pPad = new Pad;

	Font::Load();

	// ゲームループ
	while (ProcessMessage() != -1)
	{
		// このフレームの開始時刻を覚えておく
		LONGLONG start = GetNowHiPerformanceCount();

		// 描画を行う前に画面をクリアする
		ClearDrawScreen();

		// ゲームの処理
		m_pPad->Update();
		m_pSceneManager->Update(*m_pPad);

		//ゲーム画面の描画
		m_pSceneManager->Draw();

		// 画面が切り替わるのを待つ
		ScreenFlip();

		// escキーでゲーム終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// FPS60に固定する
		while (GetNowHiPerformanceCount() - start < 16667)
		{
			// 16.66ミリ秒(16667マイクロ秒)経過するまで待つ
		}
	}
	Font::UnLoad();

	// Effekseerを終了する。
	Effkseer_End();

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}