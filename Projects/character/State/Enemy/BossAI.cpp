#include "DxLib.h"
#include "BossAI.h"
#include "characterBase.h"
#include "Player.h"
#include "EnemyStateAttack.h"
#include "EnemyStateStatusUp.h"
#include "EnemyStateWalk.h"
#include "LoadCsv.h"
namespace
{
	//ボスの行動切り替え時のＨＰ
	constexpr int kBossHp = 150;

	//ボスとプレイヤーの距離
	constexpr float kShortDistance = 200.0f;
	constexpr float kLongDistance = 600.0f;

	//HPが高くプレイヤーが近い
	const  char* const kHpHighlyRangeNear = "HpHighlyRangeNear";

	//HPが高くプレイヤーが遠い
	const  char* const kHpHighlyRangeFar = "HpHighlyRangeFar";

	//HPが低くプレイヤーが近い
	const  char* const kHpLowRangeNear = "HpLowRangeNear";

	//HPは低くプレイヤーが遠い
	const  char* const kHpLowRangeFar = "HpLowRangeFar";

	//ランダム数値
	constexpr int  kRandom = 100;
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

//どのような状況下か
int BossAI::StateSet(CharacterBase& boss,const Player& player)
{
	//敵とプレイヤーの距離
	m_distance = VSize(VSub(boss.GetPos(), player.GetPos()));
	
	if (boss.GetHp() >= kBossHp && m_distance <= kShortDistance)
	{
		LoadCsv::GetInstance().LoadBossAIData(m_bossAI, kHpHighlyRangeNear);
	 	 return AIMotion(m_bossAI);
	}
	if (boss.GetHp() >= kBossHp && m_distance >= kLongDistance)
	{
		LoadCsv::GetInstance().LoadBossAIData(m_bossAI, kHpHighlyRangeFar);
		return AIMotion(m_bossAI);
	}
	if (boss.GetHp() <= kBossHp && m_distance <= kShortDistance)
	{
		LoadCsv::GetInstance().LoadBossAIData(m_bossAI, kHpLowRangeNear);
		return AIMotion(m_bossAI);
	}
	if (boss.GetHp() <= kBossHp && m_distance >= kLongDistance)
	{
		LoadCsv::GetInstance().LoadBossAIData(m_bossAI, kHpLowRangeFar);
		return AIMotion(m_bossAI);
	}

	return kWalk;
}

int BossAI::AIMotion(AI  bossAI)
{
	m_probability = GetRand(100);
	if (m_probability < bossAI.rushAttackProbability)
	{
		return  kRushAttack;
	}

	if (m_probability <= bossAI.lightAttackProbability)
	{
		return  kLightAttack;
	}

	if (m_probability <= bossAI.storongAttackProbability)
	{
		return kStorongAttack;
	}

	if (m_probability <= bossAI.statusUpProbability)
	{
		return kStatusUp;
	}

	if (m_probability <= bossAI.throwingAttackProbability)
	{
		return kThrowingAttack;
	}


}
