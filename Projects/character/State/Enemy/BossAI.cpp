#include "DxLib.h"
#include "BossAI.h"
#include "characterBase.h"
#include "Player.h"
#include "EnemyStateAttack.h"
#include "EnemyStateStatusUp.h"
#include "EnemyStateWalk.h"
namespace
{
	//ボスの行動切り替え時のＨＰ
	constexpr int kBossHp = 150;

	//ボスとプレイヤーの距離
	constexpr float kShortDistance = 200.0f;
	constexpr float kLongDistance = 600.0f;

	//ボスの残りHPが高く、プレイヤーが近い場所にいる場合の行動確率
	constexpr int kHpHighlyRangeNearLightAttack = 70;
	constexpr int kHpHighlyRangeNearStorongAttack = 90;
	constexpr int kHpHighlyRangeNearStatusUp = 100;

	//ボスの残りHPが高く、プレイヤーが遠い場所にいる場合の行動確率
	constexpr int kHpHighlyRangeFarThrowing = 60;
	constexpr int kHpHighlyRangeFarJump = 80;
	constexpr int kHpHighlyRangeFarStatusUp = 100;

	//ボスの残りHPが低く、プレイヤーが近い場合の行動確率
	constexpr int kHpLowRangeNearStorongAttack = 70;
	constexpr int kHpLowRangeNearLightAttack = 80;
	constexpr int kHpLowRangeNearStatusUp = 100;

	//ボスの残りHPが低く、プレイヤーが遠い場所の行動確率
	constexpr int kHpLowRangeFarThrowing = 70;	
	constexpr int kHpLowRangeFarStatusUp = 90;
	constexpr int kHpLowRangeFarJump = 100;
}

BossAI::BossAI()
{
}

BossAI::~BossAI()
{
}

void BossAI::Init()
{
	m_state = kWalk;
}

int BossAI::StateSet(CharacterBase& boss,const Player& player)
{
	m_distance = VSize(VSub(boss.GetPos(), player.GetPos()));
	if (boss.GetHp() >= kBossHp && m_distance <= kShortDistance)
	{
	 	 return HpHighlyRangeNear();
	}
	if (boss.GetHp() >= kBossHp && m_distance >= kLongDistance)
	{
		return HpHighlyRangeFar();
	}
	if (boss.GetHp() <= kBossHp && m_distance <= kShortDistance)
	{
		return HpLowRangeNear();
	}
	if (boss.GetHp() <= kBossHp && m_distance >= kLongDistance)
	{
		return HpLowRangeFar();
	}


	return kWalk;
}

int BossAI::HpHighlyRangeNear()
{
	m_probability = GetRand(100);

	if (m_probability < kHpHighlyRangeNearLightAttack)
	{
		m_state = kLightAttack;
	}
	//強攻撃を行う
	else if (m_probability < kHpHighlyRangeNearStorongAttack)
	{
		m_state = kStorongAttack;
	}
	//自身の強化を行う
	else if (m_probability < kHpHighlyRangeNearStatusUp)
	{
		m_state = kStatusUp;
	}

	return m_state;
}

int BossAI::HpHighlyRangeFar()
{
	m_probability = GetRand(100);
	if (m_probability < kHpHighlyRangeFarThrowing)
	{
		m_state = kThrowingAttack;
	}
	else if (m_probability < kHpHighlyRangeFarJump)
	{
		m_state = kJump;
	}
	else if (m_probability < kHpHighlyRangeFarStatusUp)
	{
		m_state = kStatusUp;
	}

	return m_state;
}

int BossAI::HpLowRangeNear()
{
	m_probability = GetRand(100);
	if (m_probability < kHpLowRangeNearStorongAttack)
	{
		m_state = kThrowingAttack;
	}
	else if (m_probability < kHpLowRangeNearLightAttack)
	{
		m_state = kLightAttack;
	}
	else if (m_probability < kHpLowRangeNearStatusUp)
	{
		m_state = kStatusUp;
	}

	return m_state;
}

int BossAI::HpLowRangeFar()
{
	m_probability = GetRand(100);
	if (m_probability < kHpLowRangeFarThrowing)
	{
		m_state = kThrowingAttack;
	}
	else if (m_probability < kHpLowRangeFarStatusUp)
	{
		m_state = kStatusUp;
	}
	else if (m_probability < kHpLowRangeFarJump)
	{
		m_state = kJump;
	}


	return m_state;
}
