#include "PlayerStateIdle.h"
#include "PlayerStateAttack.h"
#include "PlayerStateWalk.h"
#include "PlayerStateRun.h"
#include "Player.h"
#include "Pad.h"

void PlayerStateAttack::Init()
{
	m_pPlayer->ChangeAnim("Attack1");
	m_isAttacking = false;
	
	m_aButtonCount = 0;

    m_attacks = 0;
}

void PlayerStateAttack::Update(Stage& stage, const Pad& pad, const Camera& camera)
{
    //アニメーションのスピードを受け取る
    m_animTime = m_pPlayer->GetAnimSpeed();

    //ステージとの当たり判定
    m_pPlayer->Move(stage, VGet(0.0f, 0.0f, 0.0f));

    //アニメーションがそれだけ進んでいるか
    m_time += m_animTime;

    // 攻撃処理
    if (pad.IsTrigger("A"))
    {
        m_aButtonCount++;  // Aボタンを押した回数を記録
    }
    
    //攻撃の処理
    Attack();

    // 状態遷移
   // 攻撃から待機状態に変更
    if (m_pPlayer->GetAnimLoopEndTime() <= m_time)
    {
        m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
        auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
        state->Init();

        return;
    }

    //攻撃状態から走る状態に変更
    if (pad.IsPress("RB") && (pad.IsPress("left") || pad.IsPress("right") || pad.IsPress("up") || pad.IsPress("down")))
    {
        m_nextState = std::make_shared<PlayerStateRun>(m_pPlayer);
        auto state = std::dynamic_pointer_cast<PlayerStateRun>(m_nextState);
        state->Init();
        return;
    }

    // 攻撃状態から歩く状態に変更
    else if (pad.IsPress("left") || pad.IsPress("right") || pad.IsPress("up") || pad.IsPress("down"))
    {
        m_nextState = std::make_shared<PlayerStateWalk>(m_pPlayer);
        auto state = std::dynamic_pointer_cast<PlayerStateWalk>(m_nextState);
        state->Init();
        return;
    }

}
void PlayerStateAttack::Draw()
{
	DrawFormatString(0, 50, GetColor(0,0,0), "プレイヤーが攻撃中");
}

void PlayerStateAttack::Attack()
{
    // 初回攻撃の開始
    if (!m_isAttacking)
    {
        m_pPlayer->Attack(m_aButtonCount, m_pPlayer->GetCurrentAnimTime());
        m_isAttacking = true;  // 攻撃中フラグを立てる
    }
    else
    {
        m_pPlayer->Attack(m_attacks, m_pPlayer->GetCurrentAnimTime());
    }

    if (m_aButtonCount >= 1 && m_attacks == 0)
    {
        m_pPlayer->ChangeAnim("Attack2");
        m_time = 0;
        m_attacks++;
    }
    else if (m_aButtonCount >= 2 && m_attacks == 1)
    {
        m_pPlayer->ChangeAnim("Attack3");
        m_time = 0;
        m_attacks++;
    }
    else if (m_aButtonCount >= 3 && m_attacks == 2)
    {
        // 攻撃終了
        m_attacks = 0;
        m_aButtonCount = 0;
        m_isAttacking = false;  // 攻撃終了
    }
}

