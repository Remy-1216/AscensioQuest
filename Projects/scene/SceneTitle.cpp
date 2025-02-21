#include "SceneTitle.h"
#include "SceneSelect.h"
#include"SoundManager.h"
#include "PlayerStatus.h"
#include "TitlePlayer.h"
#include "Stage.h"
#include "Pad.h"
#include "Game.h"
#include "Font.h"
#include "DxLib.h"

namespace
{
	//フェードイン、フェードアウトの数値
	constexpr int kFadeValue = 255;

	//フェード値の増減
	constexpr int kFadeUpDown = 8;

	//カメラの座標
	constexpr float kCameraPosX = 200.0f;
	constexpr float kCameraPosY = 250.0f;
	constexpr float kCameraPosZ = -350.0f;

	//カメラターゲットの座標
	constexpr  float kCameraTargetPosX = -350.0f;
	constexpr  float kCameraTargetPosY = 120.0f;

	//カーソルの移動量
	constexpr float kCarsorMove = 135.0f;

	//はじめから始める位置
	constexpr float kStartPos = 0.0f;

	//続きから始める位置
	constexpr float kContinuePos = 135.0f;

	//ゲーム終了する位置
	constexpr float kGameEndPos = 270.0f;

	//文字の描画位置
	constexpr float kCharacterPosX = 700.0f;

	constexpr float kNewGamePosY = 650.0f;

	constexpr float kLoadGamePosY = 800.0f;

	constexpr float kExitGamePosY = 950.0f;

	//上下の最大移動量
	constexpr int kMove = 180;

	//カーソルが動く時間
	constexpr int kCursorMoveTime = 25;
}

SceneTitle::SceneTitle() :  m_count(0), m_isSelect(false), m_isPlayGame(false), m_isPressPad(false), m_isPressPadTime(false),m_cameraPos(VGet(0.0f,0.0f,0.0f)),m_cameraTarget(VGet(0.0f,0.0f,0.0f)),
m_cursorPos(VGet(0.0f, 0.0f, 0.0f)), m_newGamePos(VGet(0.0f, 0.0f, 0.0f)), m_loadGamePos(VGet(0.0f, 0.0f, 0.0f)), m_exitGamePos(VGet(0.0f, 0.0f, 0.0f))
{
}

SceneTitle::~SceneTitle()
{
	m_pSoundManager.reset();
	m_pSoundManager = nullptr;

	m_pTitlePlayer.reset();
	m_pTitlePlayer = nullptr;

	m_pStage.reset();
	m_pStage = nullptr;

	//画像の削除
	DeleteGraph(m_bgHandle);
}

void SceneTitle::Init()
{
	//画像のロード
	m_bgHandle = LoadGraph("data/BG/title.png");

	//タイトルロゴのロード
	m_logoHandle = LoadGraph("data/UI/logo.png");

	//ステージ
	m_pStage = std::make_shared<Stage>(Title);

	//BGMの再生
	m_pSoundManager = std::make_shared<SoundManager>();
	m_pSoundManager->TitleBGM();

	m_pPlayerStatus = std::make_shared<PlayerStatus>();

	m_pTitlePlayer = std::make_shared<TitlePlayer>();
	m_pTitlePlayer->Init();

	//初期位置の設定
	m_newGamePos.y = kNewGamePosY;

	m_loadGamePos.y = kLoadGamePosY;

	m_exitGamePos.y = kExitGamePosY;

	m_cursorCount = kCursorMoveTime;

	//カメラ座標
	m_cameraPos = VAdd(m_pTitlePlayer->GetPos(), VGet(kCameraPosX, kCameraPosY, kCameraPosZ));

	// 注視点
	m_cameraTarget = VAdd(m_pTitlePlayer->GetPos(), VGet(kCameraTargetPosX, kCameraTargetPosY, 0.0f));

	//フェード値の初期設定
	m_fadeAlpha = kFadeValue;

	m_isSelect = false;
}

std::shared_ptr<SceneBase> SceneTitle::Update(const Pad&pad)
{
	//フェード関係
	Fade();

	//カーソルの動き
	CursorMotion(pad);

	//プレイヤーの更新
	m_pTitlePlayer->Update(m_cursorPos);

	//カメラの位置設定
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	if (m_isSelect && m_fadeAlpha >= kFadeValue)
	{
		return std::make_shared<SceneSelect>();
	}

	return shared_from_this();
}

void SceneTitle::Draw()
{
	//背景の描画
	DrawGraph(0, 0, m_bgHandle, false);

	//ステージを描画
	m_pStage->Draw();

	//ロゴの描画
	DrawGraph(0, 0, m_logoHandle, true);

	//プレイヤーの描画
	m_pTitlePlayer->Draw();

	//文字を描画する
	DrawTitle();

	//フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
	DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく

#ifdef _DEBUG
	DrawFormatString(0, 0, GetColor(0,0,0), "SceneTitle");

#endif
}

void SceneTitle::End()
{
}

void SceneTitle::Fade()
{
	//フェードイン、フェードアウト
	if (m_isSelect)
	{
		m_fadeAlpha += kFadeUpDown;
		if (m_fadeAlpha > kFadeValue)
		{
			m_fadeAlpha = kFadeValue;
		}
	}
	else
	{
		m_fadeAlpha -= kFadeUpDown;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}
	}
}

void SceneTitle::CursorMotion(const Pad& pad)
{
	//カーソルの移動
	if (pad.IsPress("up"))
	{
		m_isPressPad = true;
	}

	if (pad.IsPress("down"))
	{
		m_isPressPad = true;
	}

	if (m_isPressPad)
	{
		if (m_cursorCount >= kCursorMoveTime)
		{
			if (pad.IsPress("up"))
			{
				m_cursorPos.y -= kCarsorMove;
				m_pSoundManager->MoveCursorSE();
			}
			else if (pad.IsPress("down"))
			{
				m_cursorPos.y += kCarsorMove;
				m_pSoundManager->MoveCursorSE();
			}

			m_cursorCount = 0;
		}

		if (pad.IsRelase("up") || pad.IsRelase("down"))
		{
			m_isPressPad = false;
		}

		m_cursorCount++;
	}
	else
	{
		m_cursorCount = kCursorMoveTime;
	}

	//カーソルが一番上に行った時に、一番下に戻る
	if (m_cursorPos.y < kStartPos)
	{
		m_cursorPos.y = kGameEndPos;
	}

	//カーソルが一番下に行った時に、一番上に行く
	if (m_cursorPos.y > kGameEndPos)
	{
		m_cursorPos.y = kStartPos;
	}


	//スティックを倒しているかどうか
	if (m_isPressPad)
	{
		if (m_cursorCount >= kCursorMoveTime)
		{
			if (pad.IsPress("up"))
			{
				m_cursorPos.y -= kCarsorMove;

			}
			else if (pad.IsPress("down"))
			{
				m_cursorPos.y += kCarsorMove;
			}

			m_cursorCount = 0;
		}

		m_cursorCount++;
	}

	//Aボタンを押すと移行する
	if (m_cursorPos.y == kStartPos && pad.IsTrigger("A"))
	{
		//初めから始めるので、ステータスをリセットする
		m_pPlayerStatus->ResetOutPut();

		//SEの再生
		m_pSoundManager->DeterminationSE();

		m_isSelect = true;
	}
	else if (m_cursorPos.y == kContinuePos && pad.IsTrigger("A"))
	{
		//SEの再生
		m_pSoundManager->DeterminationSE();
		m_isSelect = true;
	}
	else if (m_cursorPos.y == kGameEndPos && pad.IsTrigger("A"))
	{
		DxLib_End();
	}
}

//タイトル画面に文字を描画する
void SceneTitle::DrawTitle()
{
	if (m_cursorPos.y == kStartPos)
	{
		DrawFormatStringFToHandle(kCharacterPosX, m_newGamePos.y, GetColor(150, 150, 150), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize96_4)], "NewGame\n");
	}
	else
	{
		DrawFormatStringFToHandle(kCharacterPosX, m_newGamePos.y, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize96_4)], "NewGame\n");
	}
	if (m_cursorPos.y == kContinuePos)
	{
		DrawFormatStringFToHandle(kCharacterPosX, m_loadGamePos.y, GetColor(150, 150, 150), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize96_4)], "LoadGame\n");
	}
	else
	{
		DrawFormatStringFToHandle(kCharacterPosX, m_loadGamePos.y, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize96_4)], "LoadGame\n");
	}
	if (m_cursorPos.y == kGameEndPos)
	{
		DrawFormatStringFToHandle(kCharacterPosX, m_exitGamePos.y, GetColor(150, 150, 150), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize96_4)], "ExitGame\n");
	}
	else
	{
		DrawFormatStringFToHandle(kCharacterPosX, m_exitGamePos.y, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize96_4)], "ExitGame\n");
	}
}
