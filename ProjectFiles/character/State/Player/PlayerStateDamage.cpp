#include "PlayerStateDamage.h"
#include "PlayerStateIdle.h"
#include "PlayerStateWalk.h"
#include "PlayerStateRun.h"
#include "PlayerStateAttack.h"
#include "PlayerStateMagicAttack.h"
#include "PlayerStateGuard.h"
#include "PlayerStateDie.h"
#include "DxLib.h"
#include "Player.h"
#include "Pad.h"

void PlayerStateDamage::Init()
{
	m_pPlayer->ChangeAnim("Damage");
}

void PlayerStateDamage::Update(Stage& stage, const Pad&pad,const Camera& camera)
{
	m_hp = m_pPlayer->GetHp();
	m_pPlayer->Move(stage, VGet(0.0f,0.0f,0.0f));

	//状態遷移
	//ダメージから待機状態に変更
	if (m_pPlayer->GetAnimLoopEndTime() <= m_pPlayer->GetCurrentAnimTime())
	{
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
	}

	//ダメージを受けた状態から死亡状態
	if (m_hp <= 0)
	{
		m_nextState = std::make_shared<PlayerStateDie>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateDie>(m_nextState);
		state->Init();
	}

}

void PlayerStateDamage::Draw()
{
	//現在の状態を表示
	DrawFormatString(0, 50, GetColor(0,0,0), "ダメージを受けている状態");
}
