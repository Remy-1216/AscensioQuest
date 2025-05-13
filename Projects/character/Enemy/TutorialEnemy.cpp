#include "TutorialEnemy.h"
#include "EnemyStateIdle.h"
#include "LoadCsv.h"
#include "Player.h"

namespace
{
	//確率
	const int kProbability = 100;

	// 重力
	constexpr float kGravity = -5.0f;

	//胴体に当たった時のダメージ
	constexpr float kDamage = 5;

	//攻撃後の当たり判定の位置
	constexpr float kAttackPosY = -200.0f;

	//チュートリアル中の立ち位置
	constexpr float kPosX = 610.0f;
	constexpr float kPosY = 19.0f;
	constexpr float kPosZ = -463.0f;

	//無敵時間
	constexpr int kInvincibleTime =80;

	//モデルを回転させる
	constexpr float kRotate = 160.0f;
}

TutorialEnemy::TutorialEnemy() :CharacterBase(m_handle), m_attackHitTimes(0), m_magicAttackHitTimes(0), m_specialMoveHitTimes(0)
{
	//ステータスを読み込む
	LoadCsv::GetInstance().LoadStatus(m_status, "tutorialenemy");

	//当たり判定の時に使うカプセルの情報を読み込む
	LoadCsv::GetInstance().LoadCollisionInfo(m_collisionInfo, "tutorialenemy");

	// ステータスを与える
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

	m_characterKind = kTutorialEnemy;

	m_isKnockedDown = false;

	m_invincibleTimeLimit = kInvincibleTime;
}

TutorialEnemy::~TutorialEnemy()
{
	//モデルの削除
	MV1DeleteModel(m_handle);

	m_pEffectManager->ClearEffect();
	m_pEffectManager.reset();
	m_pEffectManager = nullptr;
}

void TutorialEnemy::Init(int handle, VECTOR enemyPos)
{
	//モデルを受け取る
	m_handle = handle;

	//初期位置設定
	m_pos = enemyPos;

	//モデルを回転させる
	MV1SetRotationXYZ(m_handle, VGet(0.0f, kRotate, 0.0f));

	//アニメーションを読み込む
	LoadCsv::GetInstance().LoadShortDistanceEnemyAnimData(m_animData);

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

	//無敵時間
	m_invincibleTime = kInvincibleTime;

	//プレイヤーの攻撃に当たったか
	m_isHitAttack = false;

	//ステート
	m_pEnemyState = std::make_shared<EnemyStateIdle>(shared_from_this());
	m_pEnemyState->m_nextState = m_pEnemyState;

	auto state = std::dynamic_pointer_cast<EnemyStateIdle>(m_pEnemyState);
	state->Init(m_characterKind);
}

void TutorialEnemy::Update(Stage& stage, Player& player, int tutorialKinds)
{
	//同じ位置に立つため
	m_pos = VGet(kPosX, kPosY, kPosZ);

	// 前のフレームと違うstateの場合
	if (m_pEnemyState->GetKind() != m_pEnemyState->m_nextState->GetKind())
	{
		// stateを変更する
		m_pEnemyState = m_pEnemyState->m_nextState;
		m_pEnemyState->m_nextState = m_pEnemyState;
	}

	//無敵時間関係
	InvincibleTime();

	//特定のチュートリアル中処理
	TutorialUpdate(tutorialKinds);

	//プレイヤーに攻撃が当たったかどうか
	player.HitAttack(m_isHitAttack);

	HitPlayer(player);

	//カプセルの座標をエネミーと同じ位置に調整
	m_capsuleStart = VAdd(m_pos, m_capsuleStartPoint);
	m_capsuleEnd = VAdd(m_pos, m_capsuleEndPoint);

	// 重力を足す
	m_pos = VAdd(m_pos, VGet(0.0f, m_gravity, 0.0f));

	// stateの更新
	m_pEnemyState->Update(stage, player, m_characterKind);

	//アニメーション
	UpdateAnim();
	

	//当たり判定の更新
	UpdateCol();

	//プレイヤーの攻撃に当たったかどうか	
	HitAnyPlayerAttack(player);
}

void TutorialEnemy::Draw()
{
	//モデルの位置
	MV1SetPosition(m_handle, m_pos);

	//モデルの表示
	MV1DrawModel(m_handle);

#ifdef _DEBUG

	//当たり判定のカプセルの表示
	DrawCapsule3D(m_capsuleStart, m_capsuleEnd, m_radius, 40, GetColor(0, 255, 255), GetColor(255, 255, 255), false);

	//攻撃のカプセルの表示
	DrawCapsule3D(m_attackCapsuleStart, m_attackCapsuleEnd, m_attackRadius, 40, GetColor(255, 255, 0), GetColor(255, 255, 255), false);

#endif
}

void TutorialEnemy::TutorialUpdate(int tutorialKinds)
{
	//チュートリアル4限定の処理
	if (tutorialKinds == kFourTutorial)
	{
		m_isFourTutorial = true;
	}

}

//プレイヤーに攻撃が当たったかどうか
void TutorialEnemy::HitPlayer(Player& player)
{
	
	//攻撃がプレイヤーに当たったか
	if (player.HitEnemyAttack(m_pos, m_attackCapsuleStart, m_attackCapsuleEnd, m_attackRadius))
	{
		player.Damage(static_cast<float>(m_attackPower), kShortDistanceEnemy);

		m_attackCapsuleStart = VGet(0.0f, kAttackPosY, 0.0f);
		m_attackCapsuleEnd = VGet(0.0f, kAttackPosY, 0.0f);
	}
	
}

//プレイヤーの攻撃に当たったかどうか
void TutorialEnemy::HitAnyPlayerAttack(Player & player)
{

	if (HitPlayerAttack(player.GetAttackCapsuleStart(), player.GetAttackCapsuleEnd(),
		player.GetAttackRadius(), 0) && !m_isInvincible)
	{
		m_attackHitTimes++;
	}
	else if (HitPlayerAttack(player.GetMagicCapsuleStart(), player.GetMagicCapsuleEnd(),
		player.GetMagicCapsuleRadius(), 0) && !m_isInvincible)
	{
		m_magicAttackHitTimes++;
	}
	else if (HitPlayerAttack(player.GetSpecialMoveStart(), player.GetSpecialMoveEnd(),
		player.GetSpecialMoveRadius(), 0) && !m_isInvincible)
	{
		m_specialMoveHitTimes++;
	}
}

//当たり判定の更新
void TutorialEnemy::UpdateCol()
{
	// プレイヤーの向きをもとに当たり判定の位置を調整する
	MATRIX rotationMatrix = MGetRotY(m_attackAngle);

	if (m_isShortDistanceEnemyAttack)
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
