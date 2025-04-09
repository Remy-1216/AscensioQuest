#include "PlayerStateGuard.h"
#include "PlayerStateIdle.h"
#include "PlayerStateWalk.h"
#include "PlayerStateRun.h"
#include "PlayerStateAttack.h"
#include "PlayerStateMagicAttack.h"
#include "DxLib.h"
#include "Player.h"
#include "Pad.h"

void PlayerStateGuard::Init()
{
	m_pPlayer->ChangeAnim("Guard");
}

void PlayerStateGuard::Update(Stage& stage, const Pad&pad,const Camera& camsra)
{
	m_pPlayer->Move(stage, VGet(0.0f, 0.0f, 0.0f));

	m_pPlayer->Guard(pad);

	//ガード状態から待機状態に変更
	if (pad.IsRelase("LB"))
	{
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		return;
	}
}

void PlayerStateGuard::Draw()
{
	DrawFormatString(0, 50, GetColor(0,0,0), "ガード中");
}
