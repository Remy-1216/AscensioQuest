#include "PlayerStateIdle.h"
#include "PlayerStateWalk.h"
#include "PlayerStateRun.h"
#include "PlayerStateAttack.h"
#include "PlayerStateMagicAttack.h"
#include "PlayerStateGuard.h"
#include "PlayerStateDamage.h"
#include "PlayerStateSpecialMove.h"
#include "PlayerStateDie.h"
#include "DxLib.h"
#include "Player.h"
#include "Pad.h"

void PlayerStateIdle::Init()
{
	m_pPlayer->ChangeAnim("Idle");
}


void PlayerStateIdle::Update(Stage& stage,const Pad&pad, const Camera& camera)
{
	
	m_pPlayer->Move(stage, VGet(0.0f, 0.0f, 0.0f));

	PlayerIdle(pad, camera);

	m_isHitCharacter = false;

	m_isSpecialMove = false;

	m_isSpecialMoveTime = false;
}

void PlayerStateIdle::Draw()
{
	DrawFormatString(0, 50, GetColor(0,0,0), "待機中");	
}

void PlayerStateIdle::PlayerIdle(const Pad& pad, const Camera& camera)
{
	m_isHitCharacter = m_pPlayer->GetIsHitEnemy();
	m_isSpecialMove = m_pPlayer->GetIsSpecialMove();

	m_pPlayer->SpecialMove(pad,false);

	//待機状態から走る状態に変更
	if (pad.IsPress("RB") && (pad.IsPress("left") || pad.IsPress("right") || pad.IsPress("up") || pad.IsPress("down")))
	{
		m_nextState = std::make_shared<PlayerStateRun>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateRun>(m_nextState);
		state->Init();
		return;
	}

	// 待機状態から歩く状態に変更
	else if (pad.IsPress("left") || pad.IsPress("right") || pad.IsPress("up") || pad.IsPress("down"))
	{
		m_nextState = std::make_shared<PlayerStateWalk>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateWalk>(m_nextState);
		state->Init();
		return;
	}

	//待機状態から魔法攻撃状態に変更
	if (pad.IsTrigger("B"))
	{
		m_nextState = std::make_shared<PlayerStateMagicAttack>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateMagicAttack>(m_nextState);
		state->Init();
		return;
	}

	//待機状態から攻撃状態に変更
	if (pad.IsPress("A"))
	{
		m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
		state->Init();
		return;
	}

	//待機状態からガード状態
	if (pad.IsPress("LB"))
	{
		m_nextState = std::make_shared<PlayerStateGuard>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateGuard>(m_nextState);
		state->Init();
		return;
	}

	//待機状態からダメージを受ける状態
	if (m_isHitCharacter)
	{
		m_nextState = std::make_shared<PlayerStateDamage>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateDamage>(m_nextState);
		state->Init();
		return;
	}

	//待機状態から必殺技をうつ状態
	if (pad.IsPress("Y")&&m_isSpecialMove)
	{
		m_nextState = std::make_shared<PlayerStateSpecialMove>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateSpecialMove>(m_nextState);
		state->Init();
		return;
	}

	//ダメージを受けた状態から死亡状態
	if (m_pPlayer->GetHp() <= 0)
	{
		m_nextState = std::make_shared<PlayerStateDie>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateDie>(m_nextState);
		state->Init();
	}
}
