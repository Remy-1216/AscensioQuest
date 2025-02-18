#include "SoundManager.h"
#include "DxLib.h"

SoundManager::SoundManager():m_titleBGM(-1), m_selectBGM(-1), m_statusBGM(-1),m_gamePlayBGM(-1), m_clearBGM(-1), m_gameOverBGM(-1),
m_moveCursorSE(-1),m_determinationSE(-1), m_failureSE(-1), m_rollSwordSE(-1), m_swordSE(-1), m_magicAttackSE(-1), m_guardSE(-1),
m_specialMoveSE(-1), m_hitShortDistanceAttackSE(-1),m_hitLongDistanceAttackSE(-1), m_statusUpSE(-1),m_warpSE(-1)
{
}

SoundManager::~SoundManager()
{
	//BGMやSEのデータを削除
	DeleteSoundMem(m_titleBGM);
	DeleteSoundMem(m_selectBGM);
	DeleteSoundMem(m_statusBGM);
	DeleteSoundMem(m_gamePlayBGM);
	DeleteSoundMem(m_clearBGM);
	DeleteSoundMem(m_gameOverBGM);
	DeleteSoundMem(m_moveCursorSE);
	DeleteSoundMem(m_determinationSE);
	DeleteSoundMem(m_failureSE);
	DeleteSoundMem(m_rollSwordSE);
	DeleteSoundMem(m_swordSE);
	DeleteSoundMem(m_magicAttackSE);
	DeleteSoundMem(m_guardSE);
	DeleteSoundMem(m_specialMoveSE);
	DeleteSoundMem(m_hitShortDistanceAttackSE);
	DeleteSoundMem(m_hitLongDistanceAttackSE);
	DeleteSoundMem(m_statusUpSE);
	DeleteSoundMem(m_warpSE);
}

void SoundManager::Init()
{
}

void SoundManager::TitleBGM()
{
	//ロード
	if (m_titleBGM <= -1)
	{
		m_titleBGM = LoadSoundMem("data/BGM/start.mp3");
	}
	

	//BGMの設定
	PlaySoundMem(m_titleBGM, DX_PLAYTYPE_LOOP, true);
}

void SoundManager::SelectBGM()
{
	//ロード
	if (m_selectBGM <= -1)
	{
		m_selectBGM = LoadSoundMem("data/BGM/GameSelect.mp3");
	}
	

	//BGMの設定
	PlaySoundMem(m_selectBGM, DX_PLAYTYPE_LOOP, true);
}

void SoundManager::StatusBGM()
{
	//ロード
	if (m_statusBGM <= -1)
	{
		m_statusBGM = LoadSoundMem("data/BGM/GameStatus.mp3");
	}
	
	//BGMの設定
	PlaySoundMem(m_statusBGM, DX_PLAYTYPE_LOOP, true);

}

void SoundManager::GamePlayBGM()
{
	//ロード
	if (m_gamePlayBGM <= -1)
	{
		m_gamePlayBGM = LoadSoundMem("data/BGM/GamePlay.mp3");
	}
	

	//BGMの設定
	PlaySoundMem(m_gamePlayBGM, DX_PLAYTYPE_LOOP, true);
}

void SoundManager::ClearBGM()
{
	//ロード
	if (m_clearBGM <= -1)
	{
		m_clearBGM = LoadSoundMem("data/BGM/GameClear.mp3");
	}
		

	//BGMの設定
	PlaySoundMem(m_clearBGM, DX_PLAYTYPE_LOOP, true);
}

void SoundManager::GameOverBGM()
{

	if (m_gameOverBGM <= -1)
	{
		//ロード
		m_gameOverBGM = LoadSoundMem("data/BGM/GameOver.mp3");
	}
	

	//BGMの設定
	PlaySoundMem(m_gameOverBGM, DX_PLAYTYPE_LOOP, true);
}

void SoundManager::MoveCursorSE()
{
	//ロード
	if (m_moveCursorSE <= -1)
	{
		m_moveCursorSE = LoadSoundMem("data/SE/cursor.mp3");
	}

	//SEの設定
	PlaySoundMem(m_moveCursorSE, DX_PLAYTYPE_BACK, true);

}

void SoundManager::DeterminationSE()
{
	//ロード
	if (m_determinationSE <= -1)
	{
		m_determinationSE = LoadSoundMem("data/SE/DeterminingSound.mp3");
	}
	

	//SEの設定
	PlaySoundMem(m_determinationSE, DX_PLAYTYPE_BACK, true);
}

void SoundManager::FailureSE()
{
	if (m_failureSE <= -1)
	{
		m_failureSE = LoadSoundMem("data/SE/cancel.mp3");
	}

	//SEの設定
	PlaySoundMem(m_failureSE, DX_PLAYTYPE_BACK, true);
}

void SoundManager::RollSwordSE()
{
	if (m_rollSwordSE <= -1)
	{
		m_rollSwordSE = LoadSoundMem("data/SE/Swing.mp3");
	}

	//SEの設定
	PlaySoundMem(m_rollSwordSE, DX_PLAYTYPE_BACK, true);
}

void SoundManager::SwordSE()
{
	//ロード
	if (m_swordSE <= -1)
	{
		m_swordSE = LoadSoundMem("data/SE/Cut.mp3");
	}

	//SEの設定
	PlaySoundMem(m_swordSE, DX_PLAYTYPE_BACK, true);
}

void SoundManager::MagicAttackSE()
{
	//ロード
	if (m_magicAttackSE <= -1)
	{
		m_magicAttackSE = LoadSoundMem("data/SE/MagicAttack.mp3");
	}

	PlaySoundMem(m_magicAttackSE, DX_PLAYTYPE_BACK, true);
}

void SoundManager::GuardSE()
{
	if (m_guardSE <= -1)
	{
		m_guardSE = LoadSoundMem("data/SE/Guard.mp3");
	}

	PlaySoundMem(m_guardSE, DX_PLAYTYPE_BACK, true);
}

void SoundManager::SpecialMoveSE()
{
	if (m_specialMoveSE <= -1)
	{
		m_specialMoveSE = LoadSoundMem("data/SE/SpecialMove.mp3");
	}

	PlaySoundMem(m_specialMoveSE, DX_PLAYTYPE_BACK, true);
}

void SoundManager::HitShortDistanceAttackSE()
{
	if (m_hitShortDistanceAttackSE <= -1)
	{
		m_hitShortDistanceAttackSE = LoadSoundMem("data/SE/ShotEnemyAttackHit.mp3");
	}

	PlaySoundMem(m_hitShortDistanceAttackSE, DX_PLAYTYPE_BACK, true);
}

void SoundManager::HitLongDistanceAttackSE()
{
	if (m_hitLongDistanceAttackSE <= -1)
	{
		m_hitLongDistanceAttackSE = LoadSoundMem("data/SE/LongEnemyAttackHIt.mp3");
	}

	PlaySoundMem(m_hitLongDistanceAttackSE, DX_PLAYTYPE_BACK, true);

}

void SoundManager::StatusUpSE()
{
	//ロード
	if (m_statusUpSE <= -1)
	{
		m_statusUpSE = LoadSoundMem("data/SE/StatusUp.mp3");
	}

	//SEの設定
	PlaySoundMem(m_statusUpSE, DX_PLAYTYPE_BACK, true);

}

void SoundManager::WarpSE()
{
	if (m_warpSE <= -1)
	{
		m_warpSE = LoadSoundMem("data/SE/warp.mp3");
	}

	//SEの設定
	PlaySoundMem(m_warpSE, DX_PLAYTYPE_BACK, true);
}
