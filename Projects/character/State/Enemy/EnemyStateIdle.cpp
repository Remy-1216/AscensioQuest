#include "EnemyStateIdle.h"
#include "EnemyStateDamage.h"
#include "EnemyStateAttack.h"
#include "Player.h"

namespace
{
	//プレイヤーとの距離
	constexpr float kDistancePlayer = 3000.0f;

	//プレイヤーに一定距離近づくと攻撃する時の距離
	constexpr float kShortDistanceEnemyMove = 150.0f;

	//プレイヤーに一定距離近づくと攻撃する時の距離
	constexpr float kLongDistanceEnemyMove = 500.0f;

	//クールタイム
	constexpr int kCoolTime = 180;
}
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

	//敵の初期位置からターゲット位置に向かうベクトルを生成する
	m_move = GoToPlayer(player.GetPos());

	m_pEnemy->ComingCharacter(stage, m_move);

	//攻撃可能域に近づくと歩く状態から攻撃状態に遷移
	if (m_distance <= kLongDistanceEnemyMove  && m_pEnemy->GetFourTutorial())
	{
		m_nextState = std::make_shared<EnemyStateAttack>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateAttack>(m_nextState);
		state->Init(characterKinds, kLightAttack);
		return;
	}

}

void EnemyStateIdle::Draw(const int characterKinds)
{
}

VECTOR EnemyStateIdle::GoToPlayer(VECTOR playerPos)
{
	//始点から終点に向かうベクトルを求める場合、終点の座標-始点の座標で求める
	VECTOR toTarget = VSub(playerPos, m_pEnemy->GetPos());

	//ベクトルの長さをkSpeedにしてやる
	//ベクトルの正規化　長さを１にする
	toTarget = VNorm(toTarget);

	//歩く速さをかける
	m_move.x = toTarget.x * m_pEnemy->GetWalkSpeed();
	m_move.y = toTarget.y;
	m_move.z = toTarget.z * m_pEnemy->GetWalkSpeed();

	return m_move;
}
