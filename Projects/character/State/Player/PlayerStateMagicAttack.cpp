#include "PlayerStateIdle.h"
#include "PlayerStateWalk.h"
#include "PlayerStateRun.h"
#include "PlayerStateAttack.h"
#include "PlayerStateMagicAttack.h"
#include "PlayerStateGuard.h"
#include "DxLib.h"
#include "Player.h"
#include "Pad.h"

void PlayerStateMagicAttack::Init()
{
	m_pPlayer->ChangeAnim("MagicAttack");

	//魔法攻撃時の処理
	m_pPlayer->MagicAttack();
}

void PlayerStateMagicAttack::Update(Stage& stage, const Pad&pad,const Camera& camera)
{
	//ステージとの当たり判定
	m_pPlayer->Move(stage, VGet(0.0f, 0.0f, 0.0f));

	//アニメーションのスピードを受け取る
	m_animTime = m_pPlayer->GetAnimSpeed();

	//アニメーションがそれだけ進んでいるか
	m_time += m_animTime;

	//魔法攻撃状態から待機状態に変更
	if (m_pPlayer->GetAnimLoopEndTime() <= m_time)
	{
		m_time = 0.0f;

		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		return;
	}
	
}

void PlayerStateMagicAttack::Draw()
{
	DrawFormatString(0, 50, GetColor(0,0,0), "魔法攻撃中");
}
