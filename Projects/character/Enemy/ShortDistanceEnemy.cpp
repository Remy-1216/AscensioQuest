#include "ShortDistanceEnemy.h"
#include "EnemyStateWalk.h"
#include "LoadCsv.h"
#include "Player.h"
#include "UIBar.h"
#include <memory>

namespace
{
	//アイテムドロップの確率
	const int kDropProbability = 40;

	//アイテムドロップの回数
	const int kDorps = 1;

	//確率
	const int kProbability = 100;

	// 重力
	constexpr float kGravity = -5.0f;

	//胴体に当たった時のダメージ
	constexpr float kDamage = 5;

	//攻撃後の当たり判定の位置
	constexpr float kAttackPosY = -200.0f;

	//無敵時間の長さ
	constexpr int kInvincibleTime = 30;
}

ShortDistanceEnemy::ShortDistanceEnemy():CharacterBase(m_handle)
{
	// ステータスを読み込む
	LoadCsv::GetInstance().LoadStatus(m_status, "shortdistanceenemy");

	//当たり判定の時に使うカプセルの情報を読み込む
	LoadCsv::GetInstance().LoadCollisionInfo(m_collisionInfo, "shortdistanceenemy");

	//ステータスを与える
	m_hp = m_status.maxHp;
	m_mp = m_status.maxMp;
	m_attackPower = m_status.attackPower;
	m_magicAttackPower = m_status.magicAttackPower;
	m_defensePower = m_status.defensePower;
	m_walkSpeed = m_status.walkSpeed;
	m_runSpeed = m_status.runSpeed;

	//カプセルの座標を与える
	m_capsuleStartPoint = m_collisionInfo.capsuleStartPoint;
	m_capsuleEndPoint = m_collisionInfo.capsuleEndPoint;
	m_radius = m_collisionInfo.radius;

	//攻撃時に使用するカプセルの座標を受け取る
	m_attackCapsuleStartPoint = m_collisionInfo.attackCapsuleStartPoint;
	m_attackCapsuleEndPoint = m_collisionInfo.attackCapsuleEndPoint;
	m_attackRadius = m_collisionInfo.attackRadius;

	m_isKnockedDown = false;

	//無敵時間の長さ
	m_invincibleTimeLimit = kInvincibleTime;
}

ShortDistanceEnemy::~ShortDistanceEnemy()
{
	//モデルの削除
	MV1DeleteModel(m_handle);

	m_pEffectManager->ClearEffect();
	m_pEffectManager.reset();
	m_pEffectManager = nullptr;
}

void ShortDistanceEnemy::Init(int handle, VECTOR enemyPos)
{
	//モデルを受け取る
	m_handle = handle;

	//初期位置設定
	m_pos = enemyPos;

	//アニメーションを読み込む
	LoadCsv::GetInstance().LoadShortDistanceEnemyAnimData(m_animData);

	//キャラクターの種類を設定
	m_characterKind = kShortDistanceEnemy;

	//初期化
	//最初は攻撃をしていないのでflase
	m_isShortDistanceEnemyAttack = false;

	//最初は当たっていないのでfalse
	m_isHitAttack = false;

	//最初は死んでいないのでfalse
	m_isKnockedDown = false;

	//最初は死んでいないので動けるためtrue
	m_isMove = true;

	m_maxHp = m_hp;

	//当たり判定の初期化
	m_attackCapsuleStart = VGet(0.0f, kAttackPosY, 0.0f);
	m_attackCapsuleEnd = VGet(0.0f, kAttackPosY, 0.0f);

	m_capsuleStart = VGet(0.0f, 0.0f, 0.0f);
	m_capsuleEnd = VGet(0.0f, 0.0f, 0.0f);

	//エフェクト
	m_pEffectManager = std::make_shared<EffectManager>();
	m_pEffectManager->Init();

	//重力
	m_gravity = kGravity;

	m_subVector = VGet(0.0f, 0.0f, 0.0f);

	//ステート
	m_pEnemyState = std::make_shared<EnemyStateWalk>(shared_from_this());
	m_pEnemyState->m_nextState = m_pEnemyState;

	auto state = std::dynamic_pointer_cast<EnemyStateWalk>(m_pEnemyState);
	state->Init(m_characterKind);	
}

void ShortDistanceEnemy::Update(Stage& stage, const Player& player,VECTOR playerPos)
{
	// 前のフレームと違うstateの場合
	if (m_pEnemyState->GetKind() != m_pEnemyState->m_nextState->GetKind())
	{
		// stateを変更する
		m_pEnemyState = m_pEnemyState->m_nextState;
		m_pEnemyState->m_nextState = m_pEnemyState;
	}
	
	//カプセルの座標をエネミーと同じ位置に調整
	m_capsuleStart = VAdd(m_pos, m_capsuleStartPoint);
	m_capsuleEnd = VAdd(m_pos, m_capsuleEndPoint);

	// stateの更新
	m_pEnemyState->Update(stage,player, m_characterKind);

	// 重力を足す
	m_pos = VAdd(m_pos, VGet(0.0f, m_gravity, 0.0f));

	//アニメーション
	UpdateAnim();

	//当たり判定の更新
	UpdateCol();

	//無敵時間関係
	InvincibleTime();

	m_isKnockedDown = m_pEnemyState->GetIsDie();

	//死んだ瞬間の動き
	Die();

	//向いている方向
	if (m_isMove)
	{
		UpdateAngle(playerPos);
	}

}

void ShortDistanceEnemy::Draw()
{
	//モデルの位置
	MV1SetPosition(m_handle, m_pos);

	//HPバーの描画
	//m_pUIBar->DrawEnemyGaugeBar(*this);

	//死んでいないときは表示し、死んだ後には表示しない(EnemyManagerが完成次第なくなる)
	if (!m_isKnockedDown)
	{
		//モデルの表示
		MV1DrawModel(m_handle);
	}

#ifdef _DEBUG

	//本体のカプセルの表示
	DrawCapsule3D(m_capsuleStart, m_capsuleEnd, m_radius, 40, GetColor(0, 255, 255), GetColor(255, 255, 255), false);

	//攻撃のカプセルの表示
	DrawCapsule3D(m_attackCapsuleStart, m_attackCapsuleEnd, m_attackRadius, 40, GetColor(255, 255, 0), GetColor(255, 255, 255), false);

	DrawFormatString(0, 600, GetColor(255, 255, 255), "enemy_blackのHP:%4d", m_hp);

	DrawFormatString(0, 700, GetColor(255, 255, 255), "enemy_blackの座標(%.2f,%.2f,%.2f)", m_pos.x, m_pos.y, m_pos.z);

#endif
}

//プレイヤーの攻撃が当たったかどうか
void ShortDistanceEnemy::HitAnyPlayerAttack(Player& player)
{
	//プレイヤーの攻撃が当たったかどうか
	HitPlayerAttack(player.GetAttackCapsuleStart(), player.GetAttackCapsuleEnd(),
		player.GetAttackRadius(), player.GetAttackPower());
	if (!m_isHitAttack)
	{
		//プレイヤーの魔法攻撃が当たったかどうか
		HitPlayerAttack(player.GetMagicCapsuleStart(), player.GetMagicCapsuleEnd(),
			player.GetMagicCapsuleRadius(), player.GetMagicPower());
		if (!m_isHitAttack)
		{
			//プレイヤー必殺技が当たったかどうか
			HitPlayerAttack(player.GetSpecialMoveStart(), player.GetSpecialMoveEnd(),
				player.GetSpecialMoveRadius(), player.GetAttackPower());
		}
	}
}

void ShortDistanceEnemy::HitPlayer(Player& player)
{
	//胴体がプレイヤーに当たったか
	if (player.HitEnemyBody(m_pos,m_capsuleStart, m_capsuleEnd, m_radius))
	{
		player.Damage(kDamage,kShortDistanceEnemy);
	}

	//攻撃がプレイヤーに当たったか
	if (player.HitEnemyAttack(m_pos, m_attackCapsuleStart, m_attackCapsuleEnd, m_attackRadius))
	{
		player.Damage(static_cast<float>(m_attackPower), kShortDistanceEnemy);

		m_attackCapsuleStart = VGet(0.0f, kAttackPosY, 0.0f);
		m_attackCapsuleEnd = VGet(0.0f, kAttackPosY, 0.0f);

	}
}

//当たり判定の更新
void ShortDistanceEnemy::UpdateCol()
{
	// プレイヤーの向きをもとに当たり判定の位置を調整する
	MATRIX rotationMatrix = MGetRotY(m_attackAngle);

	if(m_isShortDistanceEnemyAttack)
	{ 
		//攻撃の当たり判定を更新
		m_attackCapsuleStart = VAdd(m_pos, (VTransform(m_attackCapsuleStartPoint, rotationMatrix)));
		m_attackCapsuleEnd = VAdd(m_attackCapsuleStart, (VTransform(m_attackCapsuleEndPoint, rotationMatrix)));
	}
	else
	{
		//攻撃の当たり判定を更新
		m_attackCapsuleStart = VGet(0.0f, kAttackPosY, 0.0f);
		m_attackCapsuleEnd = VGet(0.0f, kAttackPosY, 0.0f);
	}
	
}