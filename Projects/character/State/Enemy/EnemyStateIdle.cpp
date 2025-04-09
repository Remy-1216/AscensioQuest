#include "EnemyStateIdle.h"
#include "EnemyStateDamage.h"

void EnemyStateIdle::Init(const int characterKinds)
{
	m_pEnemy->ChangeAnim("Idle");
	m_isDie = false;
}

void EnemyStateIdle::Update(Stage& stage, const Player& player, const int characterKinds)
{
	m_pEnemy->ComingCharacter(stage, VGet(0.0f, 0.0f, 0.0f));

	if (m_pEnemy->GetHitCharacterAttack())
	{
		m_nextState = std::make_shared<EnemyStateDamage>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateDamage>(m_nextState);
		state->Init(characterKinds);
		return;
	}

}

void EnemyStateIdle::Draw(const int characterKinds)
{
}
