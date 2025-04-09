#include "EnemyStateDamage.h"
#include "EnemyStateWalk.h"
#include "EnemyStateIdle.h"
#include "EnemyStateDie.h"
#include "Player.h"

void EnemyStateDamage::Init(const int characterKinds)
{
	m_pEnemy->ChangeAnim("Damage");
}

void EnemyStateDamage::Update(Stage& stage,const Player& player, const int characterKinds)
{
	if (characterKinds == kShortDistanceEnemy)
	{
		ShortDistanveEnemyDamage(stage, player, kShortDistanceEnemy);
	}

	if (characterKinds == kLongDistanceEnemy)
	{
		LongDistanveEnemyDamage(stage, player, kLongDistanceEnemy);
	}
	if (characterKinds == kBoss)
	{
		BossDamage(stage, player, kBoss);
	}
	if (characterKinds == kTutorialEnemy)
	{
		TutorialEnemyDamage(stage, player, kTutorialEnemy);
	}
}

void EnemyStateDamage::Draw(const int characterKinds)
{
	printfDx( "ダメージを受けている状態\n");
}

void EnemyStateDamage::ShortDistanveEnemyDamage(Stage& stage, const Player& player, const int characterKinds)
{
	//エネミーの動き
	m_pEnemy->ComingCharacter(stage, VGet(0.0f, 0.0f, 0.0f));


	//状態遷移
	//ダメージ受けていた状態から歩く状態に遷移
	if (m_pEnemy->GetAnimLoopEndTime() <= m_pEnemy->GetCurrentAnimTime())
	{
		m_nextState = std::make_shared<EnemyStateWalk>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateWalk>(m_nextState);
		state->Init(characterKinds);
	}

	//ダメージを受けていた状態から死亡状態に遷移
	if (m_pEnemy->GetHp()<= 0)
	{
		m_nextState = std::make_shared<EnemyStateDie>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateDie>(m_nextState);
		state->Init(characterKinds);
	}
}

void EnemyStateDamage::LongDistanveEnemyDamage(Stage& stage, const Player& player, const int characterKinds)
{
	//エネミーの動き
	m_pEnemy->ComingCharacter(stage, VGet(0.0f, 0.0f, 0.0f));


	//状態遷移
	//ダメージ受けていた状態から歩く状態に遷移
	if (m_pEnemy->GetAnimLoopEndTime() <= m_pEnemy->GetCurrentAnimTime())
	{
		m_nextState = std::make_shared<EnemyStateWalk>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateWalk>(m_nextState);
		state->Init(characterKinds);
	}

	//ダメージを受けていた状態から死亡状態に遷移
	if (m_pEnemy->GetHp() <= 0)
	{
		m_nextState = std::make_shared<EnemyStateDie>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateDie>(m_nextState);
		state->Init(characterKinds);
	}
}

void EnemyStateDamage::BossDamage(Stage& stage, const Player& player, const int characterKinds)
{
	//エネミーの動き
	m_pEnemy->ComingCharacter(stage, VGet(0.0f, 0.0f, 0.0f));


	//ダメージを受けていた状態から死亡状態に遷移
	if (m_pEnemy->GetHp() <= 0)
	{
		m_nextState = std::make_shared<EnemyStateDie>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateDie>(m_nextState);
		state->Init(characterKinds);
	}

	//状態遷移
	//ダメージ受けていた状態から歩く状態に遷移
	if (m_pEnemy->GetAnimLoopEndTime() <= m_pEnemy->GetCurrentAnimTime())
	{
		m_nextState = std::make_shared<EnemyStateWalk>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateWalk>(m_nextState);
		state->Init(characterKinds);
	}

	
}

void EnemyStateDamage::TutorialEnemyDamage(Stage& stage, const Player& player, const int characterKinds)
{
	//エネミーの動き
	m_pEnemy->ComingCharacter(stage, VGet(0.0f, 0.0f, 0.0f));


	//ダメージを受けていた状態から死亡状態に遷移
	if (m_pEnemy->GetHp() <= 0)
	{
		m_nextState = std::make_shared<EnemyStateDie>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateDie>(m_nextState);
		state->Init(characterKinds);
	}

	//状態遷移
	//ダメージ受けていた状態から歩く状態に遷移
	if (m_pEnemy->GetAnimLoopEndTime() <= m_pEnemy->GetCurrentAnimTime())
	{
		m_nextState = std::make_shared<EnemyStateIdle>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateIdle>(m_nextState);
		state->Init(characterKinds);
	}
}
