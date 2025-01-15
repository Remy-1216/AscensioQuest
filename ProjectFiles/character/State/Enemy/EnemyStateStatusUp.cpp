﻿#include "EnemyStateStatusUp.h"
#include "EnemyStateWalk.h"

void EnemyStateStatusUp::Init()
{
	m_pEnemy->ChangeAnim("StatusUp");
}

void EnemyStateStatusUp::Update(Stage& stage, const Player& playerconst,const int  characterKinds)
{
	//エネミーの動き
	m_pEnemy->ComingCharacter(stage, VGet(0.0f, 0.0f, 0.0f));

	if (m_pEnemy->GetAnimLoopEndTime() <= m_pEnemy->GetCurrentAnimTime())
	{
		m_nextState = std::make_shared<EnemyStateWalk>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateWalk>(m_nextState);
		state->Init(kShortDistanceEnemy);
	}
}

void EnemyStateStatusUp::Draw(const int  characterKinds)
{
	printfDx("自己強化中\n");
}
