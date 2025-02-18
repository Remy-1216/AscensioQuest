#include "PlayerStateDie.h"
#include "Player.h"

void PlayerStateDie::Init()
{
	m_pPlayer->ChangeAnim("Death");
	m_isDie = false;
}

void PlayerStateDie::Update(Stage& stage, const Pad&pad,const Camera& camera)
{
	m_pPlayer->Move(stage, VGet(0.0f, 0.0f, 0.0f));

	//死んだアニメーションが終わったらシーンを遷移させる
	if (m_pPlayer->GetAnimLoopEndTime() <= m_pPlayer->GetCurrentAnimTime())
	{
		m_isDie = true;
	}
}

void PlayerStateDie::Draw()
{
	//現在の状態を表示
	DrawFormatString(0, 50, GetColor(0,0,0), "死亡した状態");
}
