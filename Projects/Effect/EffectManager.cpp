#include "EffectManager.h"
#include "LoadCsv.h"
#include "EffekseerForDXLib.h"

namespace
{
	// CSVに書かれているエフェクトの名前
	const char* const kPlayerDamageEffect = "playerdamage";
	const char* const kEnemyDamageEffect = "enemydamage";
	const char* const kMagicAttackEffect = "magicattack";
	const char* const kWarpPointEffect = "warppoint";
	const char* const kItemEffect = "item";
	const char* const kPlayerGuardEffect = "playerguard";

}

EffectManager::EffectManager():m_playingEffectHandle(-1),m_warpPointEffectHandle(-1), m_magicAttackEffectHandle(-1),
m_playerDamageEffectHandle(-1),m_enemyDamageEffectHandle(-1),m_playerGuardEffectHandle(-1),m_itemEffectHandle(-1),
m_magicAttackEffectTime(0), m_playerDamageEffectTime(0), m_enemyDamageEffectTime(0), m_playerGuardEffectTime(0),
m_itemEffectTime(0), m_isMagicAttackEffect(false), m_isPlayerDamageEffect(false), m_isEnemyDamageEffect(false),
m_isPlayerGuardEffect(false), m_isItemEffect(false)
{
	m_effectData.EffectSize = 0;
	m_effectData.EffectAdjPosY = 0;
	m_effectData.EffectTime = 0;

	for (int i = 0; i < kWarpPointNum; i++)
	{
		m_warpPointEffectTime[i] = 0.0f;
		m_isWarpPointEffect[i] = false;
	}
	
}

EffectManager::~EffectManager()
{
	//エフェクトの削除
	DeleteEffekseerEffect(m_playingEffectHandle);
	DeleteEffekseerEffect(m_warpPointEffectHandle);
	DeleteEffekseerEffect(m_magicAttackEffectHandle);
	DeleteEffekseerEffect(m_playerDamageEffectHandle);
	DeleteEffekseerEffect(m_enemyDamageEffectHandle);
	DeleteEffekseerEffect(m_playerGuardEffectHandle);
	DeleteEffekseerEffect(m_itemEffectHandle);
}

void EffectManager::Init()
{
	ClearEffect();
}

void EffectManager::Update()
{
	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();
}

void EffectManager::Draw()
{
	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();
}

void EffectManager::ClearEffect()
{
	StopEffekseer3DEffect(m_playingEffectHandle);
}

void EffectManager::DrawMagicAttackEffect(VECTOR& magicAttackPos)
{
	LoadCsv::GetInstance().LoadEffectData(m_effectData, kMagicAttackEffect);

	if (m_magicAttackEffectHandle <= -1)
	{
		m_magicAttackEffectHandle = LoadEffekseerEffect("data/Effect/MagicAttack.efkefc", 2.0f);
	}

	m_magicAttackEffectTime++;

	// エフェクトが1回のみ表示されるようにする
	if (m_magicAttackEffectTime >= m_effectData.EffectTime)
	{
		m_magicAttackEffectTime = 0;
		m_isMagicAttackEffect = false;

		return;
	}

	//エフェクトが再生されていない場合
	if (!m_isMagicAttackEffect)
	{
		m_playingEffectHandle = PlayEffekseer3DEffect(m_magicAttackEffectHandle);

		
		SetScalePlayingEffekseer3DEffect(m_playingEffectHandle, m_effectData.EffectSize, m_effectData.EffectSize, m_effectData.EffectSize);

		m_isMagicAttackEffect = true;
	}

	// エフェクトの表示位置を設定
	SetPosPlayingEffekseer3DEffect(m_playingEffectHandle, magicAttackPos.x, magicAttackPos.y, magicAttackPos.z);
	
}

void EffectManager::DrawWarpPointEffect(VECTOR worpPos, int warpPointNum)
{
	LoadCsv::GetInstance().LoadEffectData(m_effectData, kWarpPointEffect);
	

	// エフェクトが1回のみ表示されるようにする
	if (m_warpPointEffectTime[warpPointNum] >= m_effectData.EffectTime)
	{
		m_warpPointEffectTime[warpPointNum] = 0;
		m_isWarpPointEffect[warpPointNum] = false;

		return;
	}

	//エフェクトを読み込む
	if (m_warpPointEffectHandle <= -1)
	{
		m_warpPointEffectHandle = LoadEffekseerEffect("data/Effect/WarpPoint.efkefc", 2.0f);
	}

	m_warpPointEffectTime[warpPointNum]++;

	//エフェクトが再生されていない場合
	if (!m_isWarpPointEffect[warpPointNum])
	{
		m_playingEffectHandle = PlayEffekseer3DEffect(m_warpPointEffectHandle);

		SetScalePlayingEffekseer3DEffect(m_playingEffectHandle, m_effectData.EffectSize, m_effectData.EffectSize, m_effectData.EffectSize);

		// エフェクトの表示位置を設定
		SetPosPlayingEffekseer3DEffect(m_playingEffectHandle, worpPos.x, worpPos.y + m_effectData.EffectSize, worpPos.z);

		m_isWarpPointEffect[warpPointNum] = true;
	}

	
}

void EffectManager::DrawPlayerDamageEffect(VECTOR playerPos)
{
	LoadCsv::GetInstance().LoadEffectData(m_effectData, kPlayerDamageEffect);

	//エフェクトを一回のみ表示されるようにする
	if (m_playerDamageEffectTime >= m_effectData.EffectTime)
	{
		m_playerDamageEffectTime = 0;
		m_isPlayerDamageEffect = false;

		return;
	}

	//エフェクトを読み込む
	if (m_playerDamageEffectHandle <= -1)
	{
		m_playerDamageEffectHandle = LoadEffekseerEffect("data/Effect/MagicAttack.efkefc", 2.0f);
	}

	m_playerDamageEffectTime++;

	if (!m_isPlayerDamageEffect)
	{
		m_playingEffectHandle = PlayEffekseer3DEffect(m_playerDamageEffectHandle);

		// エフェクトの表示位置を設定
		SetPosPlayingEffekseer3DEffect(m_playingEffectHandle, playerPos.x, playerPos.y + m_effectData.EffectAdjPosY, playerPos.z);
		SetScalePlayingEffekseer3DEffect(m_playingEffectHandle, m_effectData.EffectSize, m_effectData.EffectSize, m_effectData.EffectSize);

		m_isPlayerDamageEffect = true;
	}
}

void EffectManager::DrawEnemyDamageEffect(VECTOR enemyPos)
{
	LoadCsv::GetInstance().LoadEffectData(m_effectData, kEnemyDamageEffect);

	//エフェクトを一回のみ表示されるようにする
	if (m_enemyDamageEffectTime >= m_effectData.EffectTime)
	{
		m_enemyDamageEffectTime = 0;
		m_isEnemyDamageEffect = false;

		return;
	}

	//エフェクトを読み込む
	if (m_enemyDamageEffectHandle <= -1)
	{
		m_enemyDamageEffectHandle = LoadEffekseerEffect("data/Effect/EnemyHit.efkefc", 2.0f);
	}

	m_enemyDamageEffectTime++;

	if (!m_isEnemyDamageEffect)
	{
		m_playingEffectHandle = PlayEffekseer3DEffect(m_enemyDamageEffectHandle);

		// エフェクトの表示位置を設定
		SetPosPlayingEffekseer3DEffect(m_playingEffectHandle, enemyPos.x, enemyPos.y + m_effectData.EffectAdjPosY, enemyPos.z);
		SetScalePlayingEffekseer3DEffect(m_playingEffectHandle, m_effectData.EffectSize, m_effectData.EffectSize, m_effectData.EffectSize);

		m_isEnemyDamageEffect = true;
	}
}

void EffectManager::DrawPlayerGuardEffect(VECTOR playerPos)
{
	LoadCsv::GetInstance().LoadEffectData(m_effectData, kPlayerGuardEffect);

	//エフェクトを一回のみ表示されるようにする
	if (m_playerGuardEffectTime >= m_effectData.EffectTime)
	{
		m_playerGuardEffectTime = 0;
		m_isPlayerGuardEffect = false;

		return;
	}

	//エフェクトを読み込む
	if (m_playerGuardEffectHandle <= -1)
	{
		m_playerGuardEffectHandle = LoadEffekseerEffect("data/Effect/GuardHit.efkefc", 2.0f);
	}

	m_playerGuardEffectTime++;

	if (!m_isPlayerGuardEffect)
	{
		m_playingEffectHandle = PlayEffekseer3DEffect(m_playerGuardEffectHandle);

		// エフェクトの表示位置を設定
		SetPosPlayingEffekseer3DEffect(m_playingEffectHandle, playerPos.x, playerPos.y + m_effectData.EffectAdjPosY, playerPos.z);
		SetScalePlayingEffekseer3DEffect(m_playingEffectHandle, m_effectData.EffectSize, m_effectData.EffectSize, m_effectData.EffectSize);

		m_isPlayerGuardEffect = true;
	}
}

void EffectManager::DrawItemEffect(VECTOR itemPos)
{
	LoadCsv::GetInstance().LoadEffectData(m_effectData, kItemEffect);

	//エフェクトを一回のみ表示されるようにする
	if (m_itemEffectTime >= m_effectData.EffectTime)
	{
		m_itemEffectTime = 0;
		m_isItemEffect = false;

		return;
	}

	//エフェクトを読み込む
	if (m_itemEffectHandle <= -1)
	{
		m_itemEffectHandle = LoadEffekseerEffect("data/Effect/Item.efkefc", 2.0f);
	}

	m_itemEffectTime++;

	if (!m_isItemEffect)
	{
		m_playingEffectHandle = PlayEffekseer3DEffect(m_itemEffectHandle);

		// エフェクトの表示位置を設定
		SetPosPlayingEffekseer3DEffect(m_playingEffectHandle, itemPos.x,itemPos.y + m_effectData.EffectAdjPosY,itemPos.z);
		SetScalePlayingEffekseer3DEffect(m_playingEffectHandle, m_effectData.EffectSize, m_effectData.EffectSize, m_effectData.EffectSize);

		m_isItemEffect = true;
	}
}
