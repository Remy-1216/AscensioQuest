#include "EnemyStateDie.h"
#include "Stage.h"

namespace
{
	//倒れた瞬間に消えるようにするためアニメーションの時間を少し短くする
	constexpr float kCutTime = 5.0f;
}

void EnemyStateDie::Init(const int characterKinds)
{
	m_pEnemy->ChangeAnim("Die");
	m_isDie = false;
}

void EnemyStateDie::Update(Stage& stage, const Player& player, const int characterKinds)
{
	if (characterKinds == kShortDistanceEnemy)
	{
		ShortDistanveEnemyDie(stage, player, kShortDistanceEnemy);
	}

	if (characterKinds == kLongDistanceEnemy)
	{
		LongDistanveEnemyDie(stage, player, kLongDistanceEnemy);
	}
	if (characterKinds == kBoss)
	{
		BossDie(stage, player, kBoss);
	}

}

void EnemyStateDie::Draw(const int characterKinds)
{
	printfDx("死んだ状態\n");
}

void EnemyStateDie::ShortDistanveEnemyDie(Stage& stage, const Player& player, const int characterKinds)
{
	m_pEnemy->ComingCharacter(stage, VGet(0.0f, 0.0f, 0.0f));

	//死んだアニメーションが終わったらシーンを遷移させる
	if (m_pEnemy->GetAnimLoopEndTime() <= m_pEnemy->GetCurrentAnimTime())
	{
		m_isDie = true;
	}
}

void EnemyStateDie::LongDistanveEnemyDie(Stage& stage, const Player& player, const int characterKinds)
{
	m_pEnemy->ComingCharacter(stage, VGet(0.0f, 0.0f, 0.0f));

	//死んだアニメーションが終わったらシーンを遷移させる
	if (m_pEnemy->GetAnimLoopEndTime() <= m_pEnemy->GetCurrentAnimTime())
	{
		m_isDie = true;
	}
}

void EnemyStateDie::BossDie(Stage& stage, const Player& player, const int characterKinds)
{
	m_pEnemy->ComingCharacter(stage, VGet(0.0f, 0.0f, 0.0f));

	//死んだアニメーションが終わったらシーンを遷移させる
	if (m_pEnemy->GetAnimLoopEndTime() <= m_pEnemy->GetCurrentAnimTime())
	{
		m_isDie = true;
	}
}
