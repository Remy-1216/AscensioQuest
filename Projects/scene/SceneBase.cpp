#include "SceneBase.h"
#include "SoundManager.h"

SceneBase::SceneBase()
{
	m_pSoundManager = std::make_shared<SoundManager>();
}

SceneBase::~SceneBase()
{
}
