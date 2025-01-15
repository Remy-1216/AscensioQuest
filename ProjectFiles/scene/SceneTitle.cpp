#include "SceneTitle.h"
#include "SceneSelect.h"
#include"SoundManager.h"
#include "Pad.h"
#include "DxLib.h"
#include "Game.h"

namespace
{
	//フェードイン、フェードアウトの数値
	constexpr int kFadeValue = 255;

	//フェード値の増減
	constexpr int kFadeUpDown = 8;
}

SceneTitle::SceneTitle():m_isSelect(false)
{
	
}

SceneTitle::~SceneTitle()
{
	m_pSoundManager.reset();
	m_pSoundManager = nullptr;

	//画像の削除
	DeleteGraph(m_bgHandle);
}

void SceneTitle::Init()
{
	//画像のロード
	m_bgHandle = LoadGraph("data/BG/title.png");

	//BGMの再生
	m_pSoundManager->TitleBGM();

	//フェード値の初期設定
	m_fadeAlpha = kFadeValue;

	m_isSelect = false;
}

std::shared_ptr<SceneBase> SceneTitle::Update(const Pad&pad)
{
	//フェード関係
	Fade();

	//Aボタンを押すと移行する
	if (pad.IsTrigger("A"))
	{
		//SEの再生
		m_pSoundManager->DeterminationSE();

		m_isSelect = true;
	}
	if (m_isSelect && m_fadeAlpha >= kFadeValue)
	{
		return std::make_shared<SceneSelect>();
	}

	return shared_from_this();
}

void SceneTitle::Draw()
{

	DrawGraph(0, 0, m_bgHandle, false);

	//フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
	DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく

#ifdef _DEBUG
	//DrawFormatString(0, 0, GetColor(0,0,0), "SceneTitle");

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
