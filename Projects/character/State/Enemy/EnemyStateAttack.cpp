#include "EnemyStateAttack.h"
#include "EnemyStateDamage.h"
#include "EnemyStateWalk.h"
#include "Stage.h"
#include "Player.h"

namespace
{
	//プレイヤーとの距離
	constexpr float kShortDistanceEnemydistance = 150.0f;

	constexpr float kLongDistanceEnemydistance = 300.0f;

	//攻撃の処理
	constexpr int kLight = 3;
	constexpr int kStorong = 4;
	constexpr int kThrowing = 5;

	//攻撃回数
	constexpr int kAttackNum = 1;

}

void EnemyStateAttack::Init(const int characterKinds, int attackKinds)
{
	m_attackKinds = attackKinds;
	if (m_attackKinds == kLightAttack)
	{
		m_pEnemy->ChangeAnim("Attack");
	}
	if (m_attackKinds == kStorongAttack)
	{
		m_pEnemy->ChangeAnim("StrongAttack");
	}
	if (m_attackKinds == kThrowing)
	{
		m_pEnemy->ChangeAnim("Throwing");
	}

	m_attackNum = 0;
}

void EnemyStateAttack::Update(Stage& stage, const Player& player, const int characterKinds)
{
	if (characterKinds == kShortDistanceEnemy)
	{
		ShortDistanveEnemyAttack(stage, player, kShortDistanceEnemy);
	}

	if (characterKinds == kLongDistanceEnemy)
	{
		LongDistanveEnemyAttack(stage, player,kLongDistanceEnemy);
	}
	if (characterKinds == kBoss)
	{
		BossAttack(stage, player, kBoss);
	}

	if (characterKinds == kTutorialEnemy)
	{
		TutorialEnemyAttack(stage, player, kTutorialEnemy);
	}


}

void EnemyStateAttack::Draw(const int characterKinds)
{
	printfDx("攻撃状態\n");
}

void EnemyStateAttack::ShortDistanveEnemyAttack(Stage& stage, const Player& player, const int  characterKinds)
{
	//エネミーの動き
	m_pEnemy->ComingCharacter(stage,VGet(0.0f,0.0f,0.0f));

	//エネミーが攻撃を行う
	m_pEnemy->ShortDistanceEnemyAttack(m_pEnemy->GetCurrentAnimTime());

	//状態遷移
	//攻撃から待機状態に変更
	if (m_pEnemy->GetAnimLoopEndTime() <= m_pEnemy->GetCurrentAnimTime())
	{
		m_nextState = std::make_shared<EnemyStateWalk>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateWalk>(m_nextState);
		state->Init(kShortDistanceEnemy);

		return;
	}

	if (m_pEnemy->GetHitCharacterAttack())
	{
		m_nextState = std::make_shared<EnemyStateDamage>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateDamage>(m_nextState);
		state->Init(characterKinds);
		return;
	}
}

void EnemyStateAttack::LongDistanveEnemyAttack(Stage& stage, const Player& player, const int  characterKinds)
{
	//エネミーの動き
	m_pEnemy->ComingCharacter(stage, VGet(0.0f, 0.0f, 0.0f));

	//エネミーが攻撃を行う
	if (m_attackNum <= kAttackNum)
	{
		m_pEnemy->LongDistanceEnemyAttack();
		m_attackNum++;
	}


	//状態遷移
	//攻撃から待機状態に変更
	if (m_pEnemy->GetAnimLoopEndTime() <= m_pEnemy->GetCurrentAnimTime())
	{
		m_nextState = std::make_shared<EnemyStateWalk>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateWalk>(m_nextState);
		state->Init(kLongDistanceEnemy);

		return;
	}

	if (m_pEnemy->GetHitCharacterAttack())
	{
		m_nextState = std::make_shared<EnemyStateDamage>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateDamage>(m_nextState);
		state->Init(characterKinds);
		return;
	}
}

void EnemyStateAttack::BossAttack(Stage& stage, const Player& player, const int characterKinds)
{
	//弱攻撃時の処理
	if (m_attackKinds == kLightAttack)
	{
		//エネミーの動き
		m_pEnemy->ComingCharacter(stage, VGet(0.0f, 0.0f, 0.0f));

		//ボスが攻撃を行う
		m_pEnemy->BossAttack(kLight);

		//状態遷移
		//攻撃から待機状態に変更
		if (m_pEnemy->GetAnimLoopEndTime() <= m_pEnemy->GetCurrentAnimTime())
		{
			m_nextState = std::make_shared<EnemyStateWalk>(m_pEnemy);
			auto state = std::dynamic_pointer_cast<EnemyStateWalk>(m_nextState);
			state->Init(characterKinds);
		}
	}

	//強攻撃時の処理
	if (m_attackKinds == kStorongAttack)
	{
		//エネミーの動き
		m_pEnemy->ComingCharacter(stage, VGet(0.0f, 0.0f, 0.0f));

		//ボスが攻撃を行う
		m_pEnemy->BossAttack(kStorong);

		//状態遷移
		//攻撃から待機状態に変更
		if (m_pEnemy->GetAnimLoopEndTime() <= m_pEnemy->GetCurrentAnimTime())
		{
			m_nextState = std::make_shared<EnemyStateWalk>(m_pEnemy);
			auto state = std::dynamic_pointer_cast<EnemyStateWalk>(m_nextState);
			state->Init(kShortDistanceEnemy);

			return;
		}
	}

	//投擲攻撃時の処理
	if(m_attackKinds == kThrowingAttack)
	{ 
		//エネミーの動き
		m_pEnemy->ComingCharacter(stage, VGet(0.0f, 0.0f, 0.0f));

		if (m_attackNum <= kAttackNum)
		{
			//ボスが攻撃を行う
			m_pEnemy->BossAttack(kThrowing);
			m_attackNum++;
		}
	
		//状態遷移
		//攻撃から待機状態に変更
		if (m_pEnemy->GetAnimLoopEndTime() <= m_pEnemy->GetCurrentAnimTime())
		{
			m_nextState = std::make_shared<EnemyStateWalk>(m_pEnemy);
			auto state = std::dynamic_pointer_cast<EnemyStateWalk>(m_nextState);
			state->Init(kShortDistanceEnemy);
		}
	}

}

void EnemyStateAttack::TutorialEnemyAttack(Stage& stage, const Player& player, const int characterKinds)
{
	//エネミーの動き
	m_pEnemy->ComingCharacter(stage, VGet(0.0f, 0.0f, 0.0f));

	//エネミーが攻撃を行う
	m_pEnemy->ShortDistanceEnemyAttack(m_pEnemy->GetCurrentAnimTime());

	//状態遷移
	//攻撃から待機状態に変更
	if (m_pEnemy->GetAnimLoopEndTime() <= m_pEnemy->GetCurrentAnimTime())
	{
		m_nextState = std::make_shared<EnemyStateWalk>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateWalk>(m_nextState);
		state->Init(kShortDistanceEnemy);

		return;
	}

	if (m_pEnemy->GetHitCharacterAttack())
	{
		m_nextState = std::make_shared<EnemyStateDamage>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateDamage>(m_nextState);
		state->Init(characterKinds);
		return;
	}
}
