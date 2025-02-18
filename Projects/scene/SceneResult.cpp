#include "SceneBase.h"
#include "SceneResult.h"
#include "SoundManager.h"
#include "DxLib.h"
#include "Pad.h"
#include "SceneSelect.h"
#include "Game.h"

namespace
{
	//フェードイン、フェードアウトの数値
	constexpr int kFadeValue = 255;

	//フェード値の増減
	constexpr int kFadeUpDown = 8;
}

SceneResult::SceneResult():m_isTitle(false)
{
}

SceneResult::~SceneResult()
{
	m_pSoundManager.reset();
	m_pSoundManager = nullptr;

	//画像の削除
	DeleteGraph(m_bgHandle);
}

void SceneResult::Init()
{
	//画像のロード
	m_bgHandle = LoadGraph("data/BG/GameOver.png");

	//BGMの再生
	m_pSoundManager->GameOverBGM();

	//フェード値の初期設定
	m_fadeAlpha = kFadeValue;

	m_isTitle = false;
}

std::shared_ptr<SceneBase> SceneResult::Update(const Pad& pad)
{

	//フェード関係
	Fade();

	//Aボタンを押すと移行する
	if (pad.IsTrigger("A"))
	{
		//SEの再生
		m_pSoundManager->DeterminationSE();

		m_isTitle = true;
	}
	if (pad.IsTrigger("B"))
	{
		DxLib_End();
	}
	if (m_isTitle && m_fadeAlpha >= kFadeValue)
	{
		return std::make_shared<SceneSelect>();
	}

	return shared_from_this();
}

void SceneResult::Draw()
{
	DrawGraph(0, 0, m_bgHandle,true);

	//フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
	DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく

}

void SceneResult::End()
{
}

void SceneResult::Fade()
{
	//フェードイン、フェードアウト
	if (m_isTitle)
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
