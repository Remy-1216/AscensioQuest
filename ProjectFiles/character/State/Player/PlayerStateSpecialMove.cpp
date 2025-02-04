#include "PlayerStateSpecialMove.h"
#include "PlayerStateIdle.h"
#include "Player.h"

namespace
{
	//アニメーション時間調整
	constexpr float kAnimTime = 10.0f;
}

void PlayerStateSpecialMove::Init()
{
	m_pPlayer->ChangeAnim("SpecialMove");
	m_isSpecialMoveTime = false;
}

void PlayerStateSpecialMove::Update(Stage& stage, const Pad& pad, const Camera& camera)
{
	m_pPlayer->Move(stage, VGet(0.0f, 0.0f, 0.0f));

	

	//状態遷移
	//攻撃から待機状態に変更
	if (m_pPlayer->GetAnimLoopEndTime() <= m_pPlayer->GetCurrentAnimTime())
	{
		m_isSpecialMoveTime = true;
		m_pPlayer->SpecialMove(pad, m_isSpecialMoveTime);

		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
	}
}

void PlayerStateSpecialMove::Draw()
{
	DrawFormatString(0, 50, GetColor(0,0,0), "プレイヤーが必殺技中");
}
