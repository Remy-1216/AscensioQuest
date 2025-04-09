#include "SceneBase.h"
#include "SoundManager.h"

SceneBase::SceneBase():m_bgHandle(-1), m_playVideo(0), m_shadowMapHandle(-1), m_fadeAlpha(0), m_isOpenMenu(false), m_isBackToTheTitle(false)
{
	//音声管理クラスのインスタンスを生成
	m_pSoundManager = std::make_shared<SoundManager>();
}

SceneBase::~SceneBase()
{
	m_pSoundManager.reset();
	m_pSoundManager = nullptr;
}
