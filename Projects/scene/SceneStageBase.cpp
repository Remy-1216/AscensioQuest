#include "SceneStageBase.h"
#include "Player.h"
#include "UIBar.h"
#include "Camera.h"
#include "Pad.h"
#include "Font.h"
#include "PlayerStatus.h"

namespace
{
	//フェードイン、フェードアウトの数値
	constexpr int kFadeValue = 255;

	//フェード値の増減
	constexpr int kFadeUpDown = 5;

	//カーソルの移動量
	constexpr float kCursorMove = 1.0f;

	//タイトル画面に戻るカーソルの位置
	constexpr float kBackToTitle = 1.0f;

	//ゲームに戻る文字位置
	constexpr float kGameCharacterPosX = 605.0f;
	constexpr float kGameCharacterPosY = 450.0f;

	//タイトルに戻る文字位置
	constexpr float kTitleCharacterPosX = 535.0f;
	constexpr float kTitleCharacterPosY = 700.0f;
}

SceneStageBase::SceneStageBase() :m_operationHandle(-1), m_specialMoveOperationHandle(-1), m_warpPointOperationHandle(-1), m_allOperatioHandle(-1),
m_enemyNum(0), m_cursorPosY(0.0f),m_isWarpPoint(false),  m_isSpecialMoveAvailable(false), m_isGameClear(false), m_isGameOver(false),m_cameraAngle(VGet(0.0f, 0.0f, 0.0f))
{
	m_pCamera = std::make_shared<Camera>();

	m_pPlayer = std::make_shared<Player>();

	m_pPlayerStatus = std::make_shared<PlayerStatus>();

	m_pUIBar = std::make_shared<UIBar>();

	m_fadeAlpha = kFadeValue;

	//UI関連画像のロード
	m_operationHandle = LoadGraph("data/UI/operation.png");
	m_specialMoveOperationHandle = LoadGraph("data/UI/specialmoveoperation.png");
	m_warpPointOperationHandle = LoadGraph("data/UI/warpOperation.png");
	m_allOperatioHandle = LoadGraph("data/UI/alloperation.png");

	//メニュー画面画像のロード
	m_menuHandle = LoadGraph("data/UI/pause.png");
}

SceneStageBase::~SceneStageBase()
{
	m_pPlayer.reset();
	m_pPlayer = nullptr;

	m_pCamera.reset();
	m_pCamera = nullptr;

	m_pPlayerStatus.reset();
	m_pPlayerStatus = nullptr;

	m_pUIBar.reset();
	m_pUIBar = nullptr;

	//UI画像の削除
	DeleteGraph(m_operationHandle);
	DeleteGraph(m_specialMoveOperationHandle);
	DeleteGraph(m_warpPointOperationHandle);
	DeleteGraph(m_allOperatioHandle);

	//メニュー画面画像の削除
	DeleteGraph(m_menuHandle);
}

//タイトル画面に戻る
void SceneStageBase::BackToTitle(const Pad& pad)
{
	if (pad.IsTrigger("START"))
	{
		m_isOpenMenu = true;
	}

	if (m_isOpenMenu)
	{
		if (pad.IsTrigger("down"))
		{
			m_cursorPosY += kCursorMove;
		}
		if (pad.IsTrigger("up"))
		{
			m_cursorPosY -= kCursorMove;
		}

		if (pad.IsTrigger("A"))
		{
			if (m_cursorPosY == 0.0f)
			{
				m_isOpenMenu = false;
			}
			if (m_cursorPosY == kBackToTitle)
			{
				m_isOpenMenu = false;
				m_isBackToTheTitle = true;
			}
		}


	}

	if (m_cursorPosY < 0.0f)
	{
		m_cursorPosY = kBackToTitle;
	}
	if (m_cursorPosY > kBackToTitle)
	{
		m_cursorPosY = 0.0f;
	}

}

//操作説明
void SceneStageBase::OperationUI()
{
	if (!m_isWarpPoint && !m_isSpecialMoveAvailable)
	{
		DrawGraph(0, 0, m_operationHandle, true);
	}
	if (m_isWarpPoint && !m_isSpecialMoveAvailable)
	{
		DrawGraph(0, 0, m_warpPointOperationHandle, true);
	}
	if (m_isSpecialMoveAvailable && m_isWarpPoint)
	{
		DrawGraph(0, 0, m_allOperatioHandle, true);
	}

	if (m_isSpecialMoveAvailable && !m_isWarpPoint)
	{
		DrawGraph(0, 0, m_specialMoveOperationHandle, true);
	}
}

void SceneStageBase::DrawMenu()
{
	if (m_isOpenMenu)
	{
		DrawGraph(0,0,m_menuHandle,true);

		DrawMenuText();
	}
}

void SceneStageBase::DrawMenuText()
{
	if (m_cursorPosY ==0.0f)
	{
		DrawFormatStringFToHandle(kGameCharacterPosX, kGameCharacterPosY, GetColor(150, 150, 150), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize96_4)], "ゲームを続ける\n");
	}
	else
	{
		DrawFormatStringFToHandle(kGameCharacterPosX, kGameCharacterPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize96_4)], "ゲームを続ける\n");
	}

	if (m_cursorPosY == kBackToTitle)
	{
		DrawFormatStringFToHandle(kTitleCharacterPosX, kTitleCharacterPosY, GetColor(150, 150, 150), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize96_4)], "タイトル画面に戻る\n");
	}
	else
	{
		DrawFormatStringFToHandle(kTitleCharacterPosX, kTitleCharacterPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize96_4)], "タイトル画面に戻る\n");
	}
}

//フェード関係
void SceneStageBase::Fade()
{
	//フェードイン、フェードアウト
	if (!m_isGameOver && !m_isGameClear &&!m_isBackToTheTitle)
	{
		m_fadeAlpha -= kFadeUpDown;
		if (m_fadeAlpha <= 0)
		{
			m_fadeAlpha = 0;
		}
	}
	if (m_isGameClear)
	{
		m_fadeAlpha += kFadeUpDown;
		if (m_fadeAlpha >= kFadeValue)
		{
			m_fadeAlpha = kFadeValue;
		}
	}
	if (m_isGameOver)
	{
		m_fadeAlpha += kFadeUpDown;
		if (m_fadeAlpha >= kFadeValue)
		{
			m_fadeAlpha = kFadeValue;
		}
	}
	if (m_isBackToTheTitle)
	{
		m_fadeAlpha += kFadeUpDown;
		if (m_fadeAlpha >= kFadeValue)
		{
			m_fadeAlpha = kFadeValue;
		}
	}
}
