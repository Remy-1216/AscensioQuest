#include "EnemyStateWalk.h"
#include "EnemyStateAttack.h"
#include "EnemyStateDamage.h"
#include "EnemyStateStatusUp.h"
#include "EnemyStateDie.h"
#include "BossAI.h"
#include "Player.h"
#include "Stage.h"

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

void EnemyStateWalk::Init(const int characterKinds)
{
	m_pEnemy->ChangeAnim("Walk");

	if (characterKinds == kBoss)
	{
		m_pBossAI = std::make_shared<BossAI>();
		m_pBossAI->Init();
	}
	
	m_coolTime = 0;
}

void EnemyStateWalk::Update(Stage& stage, const Player& player, const int characterKinds)
{
	//近距離攻撃を行う敵の処理
	if (characterKinds == kShortDistanceEnemy)
	{
		ShortDistanceEnemyWalk(stage, player, kShortDistanceEnemy);
	}

	//遠距離攻撃を行う敵の処理
	if (characterKinds == kLongDistanceEnemy)
	{
		LongDistanceEnemyWalk(stage, player, kLongDistanceEnemy);
	}

	//ボスの処理
	if (characterKinds == kBoss)
	{
		BossWalk(stage, player, kBoss);
	}

	//チュートリアル用の敵の処理
	if (characterKinds == kTutorialEnemy)
	{
		TutorialEnemyWalk(stage,player,kTutorialEnemy);
	}

}

void EnemyStateWalk::Draw(const int characterKinds)
{
	printfDx("歩いている状態\n");
}

void EnemyStateWalk::ShortDistanceEnemyWalk(Stage& stage, const Player& player, const int characterKinds)
{
	//攻撃のクールタイム
	m_coolTime++;

	//プレイヤーとの距離を計算する
	m_distance = VSize(VSub(m_pEnemy->GetPos(), player.GetPos()));

	//敵の初期位置からターゲット位置に向かうベクトルを生成する
	m_move = GoToPlayer(player.GetPos());

	//一定距離まで来ると動き始める
	if (m_distance <= kDistancePlayer)
	{
		m_pEnemy->ComingCharacter(stage, m_move);
	}
	//一定距離より離れるとその場で待機
	else
	{
		m_pEnemy->ComingCharacter(stage, VGet(0.0f,0.0f,0.0f));
	}
	
	//攻撃可能域に近づくと歩く状態から攻撃状態に遷移
	if (m_distance <= kShortDistanceEnemyMove && m_coolTime >= kCoolTime)
	{
		m_nextState = std::make_shared<EnemyStateAttack>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateAttack>(m_nextState);
		state->Init(characterKinds, kLightAttack);
		return;
	}

	//歩く状態からダメージを受けた状態に遷移
	if (m_pEnemy->GetHitCharacterAttack())
	{
		m_nextState = std::make_shared<EnemyStateDamage>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateDamage>(m_nextState);
		state->Init(characterKinds);
		return;
	}

	//歩く状態から死亡状態に遷移
	if (m_pEnemy->GetHp() <= 0)
	{
		m_nextState = std::make_shared<EnemyStateDie>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateDie>(m_nextState);
		state->Init(characterKinds);
		return;
	}	
}

void EnemyStateWalk::LongDistanceEnemyWalk(Stage& stage, const Player& player, const int characterKinds)
{
	//攻撃のクールタイム
	m_coolTime++;

	//エネミーとプレイヤーの距離を出します
	m_distance = VSize(VSub(m_pEnemy->GetPos(), player.GetPos()));

	m_move = GoToPlayer(player.GetPos());

	//一定距離まで来ると動き始める
	if (m_distance <= kDistancePlayer)
	{
		m_pEnemy->ComingCharacter(stage, m_move);
	}
	//一定距離より離れるとその場で待機
	else
	{
		m_pEnemy->ComingCharacter(stage, VGet(0.0f, 0.0f, 0.0f));
	}

	//攻撃可能域に近づくと歩く状態から攻撃状態に遷移
	if (m_distance <= kLongDistanceEnemyMove && m_coolTime >= kCoolTime)
	{
		m_nextState = std::make_shared<EnemyStateAttack>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateAttack>(m_nextState);
		state->Init(characterKinds, kLightAttack);
		return;
	}

	//歩く状態からダメージを受けた状態に遷移
	if (m_pEnemy->GetHitCharacterAttack())
	{
		m_nextState = std::make_shared<EnemyStateDamage>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateDamage>(m_nextState);
		state->Init(characterKinds);
		return;
	}
	//歩く状態から死亡状態に遷移
	if (m_pEnemy->GetHp() <= 0)
	{
		m_nextState = std::make_shared<EnemyStateDie>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateDie>(m_nextState);
		state->Init(characterKinds);
	}
}

void EnemyStateWalk::BossWalk(Stage& stage, const Player& player, const int characterKinds)
{
	//攻撃のクールタイム
	m_coolTime++;
	
	//エネミーとプレイヤーの距離を出します
	m_distance = VSize(VSub(m_pEnemy->GetPos(), player.GetPos()));

	//敵の初期位置からターゲット位置に向かうベクトルを生成する
	m_move = GoToPlayer(player.GetPos());

	//プレイヤーに向かって歩く
	m_pEnemy->ComingCharacter(stage, m_move);

	//歩く状態から死んだ状態に遷移する
	if (m_pEnemy->GetHp() <= 0)
	{
		m_nextState = std::make_shared<EnemyStateDie>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateDie>(m_nextState);
		state->Init(characterKinds);
	}

	if (m_pEnemy->GetHitCharacterAttack())
	{
		m_nextState = std::make_shared<EnemyStateDamage>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateDamage>(m_nextState);
		state->Init(characterKinds);
		return;
	}

	//歩く状態から弱攻撃状態に遷移する
	if (m_pBossAI->StateSet(*m_pEnemy, player) == kLightAttack && m_coolTime >= kCoolTime)
	{
		m_nextState = std::make_shared<EnemyStateAttack>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateAttack>(m_nextState);
		state->Init(characterKinds, kLightAttack);
		return;
	}
	//歩く状態から強攻撃状態に遷移する
	if (m_pBossAI->StateSet(*m_pEnemy, player) == kStorongAttack && m_coolTime >= kCoolTime)
	{
		m_nextState = std::make_shared<EnemyStateAttack>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateAttack>(m_nextState);
		state->Init(characterKinds, kStorongAttack);
		return;
	}
	//歩く状態から遠距離攻撃状態に遷移する
	if (m_pBossAI->StateSet(*m_pEnemy, player) == kThrowingAttack && m_coolTime >= kCoolTime)
	{
		m_nextState = std::make_shared<EnemyStateAttack>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateAttack>(m_nextState);
		state->Init(characterKinds, kThrowingAttack);
		return;
	}
	//歩く状態からステータスをアップする状態に遷移する
	if (m_pBossAI->StateSet(*m_pEnemy, player) == kStatusUp && m_coolTime >= kCoolTime)
	{
		m_nextState = std::make_shared<EnemyStateStatusUp>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateStatusUp>(m_nextState);
		state->Init();
		return;
	}

}

void EnemyStateWalk::TutorialEnemyWalk(Stage& stage, const Player& player, const int characterKinds)
{
	//プレイヤーとの距離を計算する
	m_distance = VSize(VSub(m_pEnemy->GetPos(), player.GetPos()));

	//攻撃可能域に近づくと歩く状態から攻撃状態に遷移
	if (m_distance <= kShortDistanceEnemyMove)
	{
		m_nextState = std::make_shared<EnemyStateAttack>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateAttack>(m_nextState);
		state->Init(characterKinds, kLightAttack);
		return;
	}

	//歩く状態からダメージを受けた状態に遷移
	if (m_pEnemy->GetHitCharacterAttack())
	{
		m_nextState = std::make_shared<EnemyStateDamage>(m_pEnemy);
		auto state = std::dynamic_pointer_cast<EnemyStateDamage>(m_nextState);
		state->Init(characterKinds);
		return;
	}
}

//敵の初期位置からターゲット位置に向かうベクトルを生成する
VECTOR EnemyStateWalk::GoToPlayer(VECTOR playerPos)
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