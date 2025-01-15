#include "SceneSelect.h"
#include "SceneStage.h"
#include "SceneStatus.h"
#include "SoundManager.h"
#include "Pad.h"
#include "Game.h"

namespace
{
	//カーソルの移動量
	constexpr float kCarsorMove = 200.0f;

	//カーソル位置
	constexpr float kStage1Pos = 0.0f;
	constexpr float kStage2Pos = 200.0f;
	constexpr float kStatusPos = 400.0f;
	constexpr float kExplanationPos = 600.0f;
	constexpr float kGameEndPos = 800.0f;

	//フェードイン、フェードアウトの数値
	constexpr int kFadeValue = 255;

	//フェード値の増減
	constexpr int kFadeUpDown = 8;

}

SceneSelect::SceneSelect():m_stage1Handle(-1), m_stage2Handle(-1), m_statusHandle(-1), m_explanationHandle(-1),
m_gameEndHandle(-1), m_cursorHandle(-1), m_isStage1(false), m_isStage2(false), m_isStatus(false), m_cursorPos(VGet(0.0f,0.0f,0.0f))
{
}

SceneSelect::~SceneSelect()
{
	m_pSoundManager.reset();
	m_pSoundManager = nullptr;

	//画像の削除
	DeleteGraph(m_bgHandle);
	DeleteGraph(m_stage1Handle);
	DeleteGraph(m_stage2Handle);
	DeleteGraph(m_statusHandle);
	DeleteGraph(m_explanationHandle);
	DeleteGraph(m_gameEndHandle);
	DeleteGraph(m_cursorHandle);
}

void SceneSelect::Init()
{

	//画像のロード
	m_bgHandle = LoadGraph("data/BG/SelectBG.png");
	m_stage1Handle = LoadGraph("data/BG/Stage1.png");
	m_stage2Handle = LoadGraph("data/BG/Stage2.png");
	m_statusHandle = LoadGraph("data/BG/Status.png");
	m_explanationHandle = LoadGraph("data/BG/Explanation.png");
	m_gameEndHandle = LoadGraph("data/BG/GameEnd.png");
	m_cursorHandle = LoadGraph("data/UI/cursor.png");

	//カーソルの位置初期化
	m_cursorPos = VGet(0.0f, 0.0f, 0.0f);

	//フェード値の初期設定
	m_fadeAlpha = kFadeValue;

	//BGMを再生する
	m_pSoundManager->SelectBGM();

	//初期化
	m_isStage1 = false;
	m_isStage2 = false;
	m_isStatus = false;

}

std::shared_ptr<SceneBase> SceneSelect::Update(const Pad& pad)
{
	//フェード関係
	Fade();
	
	//カーソルの動き
	CursorMotion(pad);

	//どのシーンに遷移するかを返す
	return SceneChange(pad);
}

void SceneSelect::Draw()
{
	//背景の描画
	DrawGraph(0, 0, m_bgHandle, true);

	//カーソルの描画
	DrawGraph(static_cast<int>(m_cursorPos.x), static_cast<int>(m_cursorPos.y), m_cursorHandle, true);

	//カーソルの位置によって描画する画面を変更する
	if (m_cursorPos.y == kStage1Pos)
	{
		DrawGraph(0, 0, m_stage1Handle, true);
	}
	else if(m_cursorPos.y == kStage2Pos)
	{ 
		DrawGraph(0, 0, m_stage2Handle, true);
	}
	else if (m_cursorPos.y == kStatusPos)
	{
		DrawGraph(0, 0, m_statusHandle, true);
	}
	else if (m_cursorPos.y == kExplanationPos)
	{
		DrawGraph(0, 0, m_explanationHandle, true);
	}
	else if (m_cursorPos.y == kGameEndPos)
	{
		DrawGraph(0, 0, m_gameEndHandle, true);
	}

	//フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
	DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく

}

void SceneSelect::End()
{
}

void SceneSelect::CursorMotion(const Pad& pad)
{
	//カーソルの移動
	if (pad.IsTrigger("up"))
	{
		m_cursorPos.y -= kCarsorMove;
	}
	if (pad.IsTrigger("down"))
	{
		m_cursorPos.y += kCarsorMove;
	}

	//上限を超える一番下の項目に飛ぶ
	if (m_cursorPos.y < kStage1Pos)
	{
		m_cursorPos.y = kGameEndPos;
	}

	//上限を超えると、一番上の項目に飛ぶ
	if (m_cursorPos.y > kGameEndPos)
	{
		m_cursorPos.y = kStage1Pos;
	}
}

std::shared_ptr<SceneBase> SceneSelect::SceneChange(const Pad& pad)
{

	//カーソルの位置によって、遷移するシーンを変更する
	//ステージ1に遷移する
	if (m_cursorPos.y == kStage1Pos && pad.IsTrigger("A") )
	{
		//SEの再生
		m_pSoundManager->DeterminationSE();
		m_isStage1 = true;
	}
	if(m_isStage1 && m_fadeAlpha >= kFadeValue)
	{
		return std::make_shared<SceneStage>(Stage1);
	}

	//ステージ2に遷移する
	if (m_cursorPos.y == kStage2Pos && pad.IsTrigger("A"))
	{
		//SEの再生
		m_pSoundManager->DeterminationSE();
		m_isStage2 = true;
	}
	if (m_isStage2 && m_fadeAlpha >= kFadeValue)
	{
		return std::make_shared<SceneStage>(Stage2);
	}

	//ステータスに遷移する
	if (m_cursorPos.y == kStatusPos && pad.IsTrigger("A"))
	{
		//SEの再生
		m_pSoundManager->DeterminationSE();
		m_isStatus = true;
	}
	if (m_isStatus && m_fadeAlpha >= kFadeValue)
	{
		return std::make_shared<SceneStatus>();
	}

	//ゲームを終了する
	if (m_cursorPos.y == kGameEndPos && pad.IsTrigger("A"))
	{
		DxLib_End();

		return 0;
	}

	return shared_from_this();
}

void SceneSelect::Fade()
{
	//フェードイン、フェードアウト

	if (!m_isStage1 && !m_isStage2 && !m_isStatus)
	{
		m_fadeAlpha -= kFadeUpDown;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}
	}
	if (m_isStage1)
	{
		m_fadeAlpha += kFadeUpDown;
		if (m_fadeAlpha > kFadeValue)
		{
			m_fadeAlpha = kFadeValue;
		}
	}
	if (m_isStage2)
	{
		m_fadeAlpha += kFadeUpDown;
		if (m_fadeAlpha > kFadeValue)
		{
			m_fadeAlpha = kFadeValue;
		}
	}
	if (m_isStatus)
	{
		m_fadeAlpha += kFadeUpDown;
		if (m_fadeAlpha > kFadeValue)
		{
			m_fadeAlpha = kFadeValue;
		}
	}
	
}


