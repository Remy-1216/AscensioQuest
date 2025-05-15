#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStatus.h"
#include "SoundManager.h"
#include "EffectManager.h"
#include "LoadCsv.h"
#include "Pad.h"
#include "Camera.h"
#include "Stage.h"
#include <cmath>
#include <memory>

namespace 
{
	const char* const kPlayerName = "player";

	//魔法攻撃を行った時に消費されるMP量
	constexpr int kMp = 20;

	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame = 4.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//球の半径
	constexpr float kSphereRadius = 45.0f;

	//球の位置調整
	constexpr float kSpherePosY = 25.0f;

	// 重力
	constexpr float kGravity = -5.0f; 

	//必殺技仕様ゲージ
	constexpr int kSuperMeter = 100;

	//攻撃アニメーション時、当たり判定を出す時間
	constexpr float kAttack1Time = 19.0f;

	constexpr float kAttack2StartTime = 3.0f;
	
	constexpr float kAttack2EndTime = 7.0f;

	constexpr float kAttack3StartTime = 5.0f;

	constexpr float kAttack3EndTime = 11.0f;

	//魔法攻撃の進むスピード
	constexpr float kMagicAttackSpeed = 5.0f;

	//魔法攻撃が進む距離
	constexpr float kMagicAttackDistance = 400.0f;

	//攻撃当たり判定の初期位置
	constexpr float kAttackPosY = -200.0f;

	//時間経過で回復するMPの量
	constexpr float kMpRecovery = 0.03f;

	//無敵時間
	constexpr int  kInvincibleTime = 120;

	//スペシャルゲージの増加
	constexpr int kSpecialMoveGauge = 5;

	//攻撃回数
	constexpr int kAttack1 = 1;
	constexpr int kAttack2 = 2;

	//落ちてしまうと死んでしまう位置
	constexpr float kMinimumAltitude = -500.0f;

	//無敵時間の長さ
	constexpr int kInvincibleTimeLimit = 120;

}

Player::Player() :CharacterBase(m_handle),m_hpUp(0), m_attackUp(0), m_magicAttackUp(0), m_defensePowerUp(0),
m_maxMp(0), m_damage(0),m_specialMoveGauge(0), m_guardSuccess(0),m_sphereRadius(0.0f), m_isPlayerGuard(false), m_isAttack(false), m_isAttackCollision(false),m_isMagicAttack(false),
m_isMagicAttackMove(false), m_isSpecialMoveAvailable(false), m_isSpecialMove(false), m_isWarpPoint(false),m_movementDirection(VGet(0.0f, 0.0f, 0.0f)), m_spherePos(VGet(0.0f,0.0f,0.0f)),
m_magicDirection(VGet(0.0f,0.0f,0.0f))
{
	//モデルをロード
	m_handle = MV1LoadModel("data/model/player/Player.mv1");

	//アニメーションのデータを読み込む
	LoadCsv::GetInstance().LoadPlayerAnimData(m_animData);

	// ステータスを読み込む
	LoadCsv::GetInstance().LoadStatus(m_status, kPlayerName);

	//胴体の当たり判定時に使うカプセルの情報を読み込む
	LoadCsv::GetInstance().LoadCollisionInfo(m_collisionInfo, kPlayerName);

	//プレイヤーの座標位置情報を読み込む
	LoadCsv::GetInstance().LoadPlayerPosData(m_characterPos, kPlayerName);

	//ステータスを受け取る
	m_hp = m_status.maxHp;
	m_mp = m_status.maxMp;
	m_attackPower = m_status.attackPower;
	m_magicAttackPower = m_status.magicAttackPower;
	m_defensePower = m_status.defensePower;
	m_walkSpeed = m_status.walkSpeed;
	m_runSpeed = m_status.runSpeed;

	//胴体の当たり判定用カプセルの座標を受け取る
	m_capsuleStartPoint = m_collisionInfo.capsuleStartPoint;
	m_capsuleEndPoint = m_collisionInfo.capsuleEndPoint;
	m_radius = m_collisionInfo.radius;
	
	//攻撃時に使用するカプセルの座標を受け取る
	m_attackCapsuleStartPoint = m_collisionInfo.attackCapsuleStartPoint;
	m_attackCapsuleEndPoint = m_collisionInfo.attackCapsuleEndPoint;
	m_attackRadius = m_collisionInfo.attackRadius;

	//魔法攻撃時に使用するカプセルの座標を受け取る
	m_magicCapsuleStartPoint = m_collisionInfo.magicCapsuleStartPoint;
	m_magicCapsuleEndPoint = m_collisionInfo.magicCapsuleEndPoint;
	m_magicRadius = m_collisionInfo.magicRadius;

	//必殺技時に使用するカプセルの座標を受け取る
	m_specialMoveStartPoint = m_collisionInfo.specialMoveStartPoint;
	m_specialMoveEndPoint = m_collisionInfo.specialMoveEndPoint;
	m_specialMoveRadius = m_collisionInfo.specialMoveRadius;


}

Player::~Player()
{
	//モデルの削除
	MV1DeleteModel(m_handle);

	m_pEffectManager->ClearEffect();
	m_pEffectManager.reset();
	m_pEffectManager = nullptr;
}

void Player::Init()
{
	//初期化
	m_isKnockedDown = false;

	m_isHitEnemy = false;

	m_isAttack = false;

	m_isMagicAttack = false;

	m_isMagicAttackMove = false;

	m_isSpecialMove = false;

	m_isSpecialMoveAvailable = false;

	m_characterAngle = 0.0f;

	m_sphereRadius = kSpherePosY;

	//無敵時間の長さ
	m_invincibleTimeLimit = kInvincibleTimeLimit;


	//座標の初期化
	m_pos = VGet(m_characterPos.posX, m_characterPos.posY, m_characterPos.posZ);

	//モデルの位置
	MV1SetPosition(m_handle, m_pos);

	//重力の設定
	m_gravity = kGravity;

	m_pPlayerStatus = std::make_shared<PlayerStatus>();
	m_pPlayerStatus->Init();

	//ステータスUPした値をステータスに足す
	m_hp += m_pPlayerStatus->GetHpUp();
	m_attackPower += m_pPlayerStatus->GetAttackUp();
	m_magicAttackPower += m_pPlayerStatus->GetMagicAttackUp();
	m_defensePower += m_pPlayerStatus->GetDefensePowerUp();
	m_specialMoveGauge += m_pPlayerStatus->GetspecialMoveGauge();

	//最大HPと最大MPの設定
	m_maxHp = m_hp;
	m_maxMp = m_mp;

	m_pState = std::make_shared<PlayerStateIdle>(shared_from_this());
	m_pState->m_nextState = m_pState;

	auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_pState);
	state->Init();

	m_pSoundManager = std::make_shared<SoundManager>();
}

void Player::Update(Stage& stage, const Pad&pad,const Camera& camera)
{
	m_isAttack = false;
	m_isMagicAttack = false;
	m_isWarpPoint = false;
	m_isAttackCollision = false;

	// 前のフレームと違うstateの場合
	if (m_pState->GetKind() != m_pState->m_nextState->GetKind())
	{
		// stateを変更する
		m_pState = m_pState->m_nextState;
		m_pState->m_nextState = m_pState;
	}

	// stateの更新
	m_pState->Update(stage, pad, camera);
	
	// 現在の状態を取得
	m_stateKinds = m_pState->GetKind();

	//エフェクトの更新
	m_pEffectManager->Update();

	// 重力を足す
	m_pos.y =m_pos.y += m_gravity;

	//ステージから落ちると、ゲームオーバー
	if (m_pos.y <= kMinimumAltitude)
	{
		m_isKnockedDown = true;
	}
	else
	{
		m_isKnockedDown = m_pState->GetIsDie();
	}

	//ワープ処理
	stage.WarpPoint(*this, pad);

	m_mp += kMpRecovery;

	if (m_mp >= m_maxMp)
	{
		m_mp = m_maxMp;
	}

	//必殺技を使えるかどうか
	if (m_specialMoveGauge >= kSuperMeter)
	{
		m_isSpecialMoveAvailable = true;
	}
	else
	{
		m_isSpecialMoveAvailable =false;
	}
	
	//アニメーションの更新
	UpdateAnim();

	//当たり判定の更新
	UpdateCol();

	//モデルの位置
	MV1SetPosition(m_handle, m_pos);

	//無敵時間関係
	InvincibleTime();

	//死んだときの処理
	Die();
}

//描画
void Player::Draw()
{
	//モデルの表示
	MV1DrawModel(m_handle);

	//エフェクトの描画
	m_pEffectManager->Draw();

	

#ifdef _DEBUG

	//現在の状態
	m_pState->Draw();

	//プレイヤーの座標表示
	DrawFormatString(0, 400, GetColor(0, 0, 0), "playerの座標(%.2f,%.2f,%.2f)", m_pos.x, m_pos.y, m_pos.z);

	//足元のカプセルを表示
	DrawSphere3D(m_spherePos, m_sphereRadius, 5,0x000000, 0x000000, false);

	//胴体のカプセルの表示
	DrawCapsule3D(m_capsuleStart, m_capsuleEnd, m_radius, 40, GetColor(0, 255, 0), GetColor(0,0,0), false);
	
	//攻撃のカプセルの表示
	DrawCapsule3D(m_attackCapsuleStart, m_attackCapsuleEnd, m_attackRadius, 40, GetColor(255, 255, 0), GetColor(0,0,0), false);

	//魔法攻撃のカプセルの表示
	DrawCapsule3D(m_magicCapsuleStart, m_magicCapsuleEnd, m_magicRadius, 40, GetColor(255, 0, 0), GetColor(0,0,0), false);

	//必殺技時のカプセルの表示
	DrawCapsule3D(m_specialMoveStart, m_specialMoveEnd, m_specialMoveRadius, 40, GetColor(255, 0, 0), GetColor(0,0,0), false);

	DrawFormatString(0,450, GetColor(0,0,0), "playerのHP:%4d", m_hp);

	DrawFormatString(0, 500, GetColor(0,0,0), "playerの必殺技ゲージ:%4d", m_specialMoveGauge);

#endif
}

//動いた時の処理
void Player::Move( Stage& stage,const VECTOR& moveVec)
{
	// 移動中のみ向きを更新する
	if (VSize(moveVec) > 0.0f)
	{
		m_characterAngle = atan2f(-moveVec.x, -moveVec.z);

		m_attackAngle = atan2f(moveVec.x, moveVec.z);
	}

	m_pos = stage.CheckObjectCol(*this, moveVec);

	//カプセルの座標をプレイヤーと同じ位置に調整
	m_capsuleStart = VAdd(m_pos, m_capsuleStartPoint);
	m_capsuleEnd = VAdd(m_pos, m_capsuleEndPoint);
	m_spherePos = VAdd(m_pos, VGet(0.0f, kSpherePosY, 0.0f));

	//モデルの位置
	MV1SetPosition(m_handle, m_pos);

	//モデルの回転更新
	MV1SetRotationXYZ(m_handle, VGet(0.0f, m_characterAngle, 0.0f));
}

//攻撃をした時の処理
void Player::Attack(int attackNum,float animTime)
{
	m_isAttack = true; 


	//アニメーションの時間によって、当たり判定を出す。
	if (animTime >= kAttack1Time&& attackNum == 0)
	{
		m_isAttackCollision = true;
	}

	if (animTime >= kAttack2StartTime && attackNum == kAttack1)
	{
		m_isAttackCollision = true;
	}
	else if (animTime >= kAttack2EndTime && attackNum == kAttack1)
	{
		m_isAttackCollision = false;
	}

	if (animTime >= kAttack3StartTime && attackNum == kAttack2)
	{
		m_isAttackCollision = true;
	}
	else if (animTime >= kAttack3EndTime && attackNum == kAttack2)
	{
		m_isAttackCollision = false;
	}
}

//魔法攻撃をした時の処理
void Player::MagicAttack()
{
	if (m_mp >= kMp)
	{
		m_mp -= kMp;
		m_pSoundManager->MagicAttackSE();
		m_isMagicAttack = true;
	}
}

//必殺技を行った時の処理
void Player::SpecialMove(const Pad& pad,bool isSpecialMove)
{
	if (m_specialMoveGauge == kSuperMeter && pad.IsTrigger("Y"))
	{
		m_isSpecialMove = true;

		m_specialMoveGauge = 0;
	}

	if (isSpecialMove)
	{
		m_isSpecialMove = false;
	}
}

void Player::Guard(const Pad& pad)
{
	m_isPlayerGuard = true;

	if (pad.IsRelase("LB") || m_isHitAttack)
	{
		m_isPlayerGuard = false;
	}
}

//ワープ移動時の処理
void Player::Warp(const Pad& pad,VECTOR warpTarget)
{
	m_isWarpPoint = true;

	if (pad.IsTrigger("X"))
	{
		m_pos = warpTarget;

		m_pSoundManager->WarpSE();
	}

#ifdef _DEBUG

	DrawFormatString(0, 150, GetColor(0,0,0), "ワープできます");

#endif
}

//敵の胴体か敵の攻撃によって変わるダメージ処理
void Player::Damage(float damage,int enemyType)
{
	//近距離攻撃が当たった時の処理
	if (m_isHitAttack && !m_isPlayerGuard && enemyType == kShortDistanceEnemy)
	{
		m_pSoundManager->HitShortDistanceAttackSE();
		m_damage = damage;
		m_hp -= damage;
		m_pEffectManager->DrawPlayerDamageEffect(m_pos);
		m_isMove = false;
		KnockBack(m_subVector);
	}
	//ガード中に近距離攻撃が当たった時の処理
	else if (m_isHitAttack && m_isPlayerGuard && enemyType == kShortDistanceEnemy)
	{
		m_pSoundManager->HitShortDistanceAttackSE();
		m_damage = damage * 0.5;
		m_hp -= damage * 0.5;
		m_pEffectManager->DrawPlayerGuardEffect(m_pos);
		m_pSoundManager->GuardSE();
		m_guardSuccess++;
		KnockBack(m_subVector);
	}
	
	//遠距離攻撃が当たった時の処理
	if (m_isHitAttack && !m_isPlayerGuard &&enemyType == kLongDistanceEnemy)
	{
		m_pSoundManager->HitLongDistanceAttackSE();
		m_damage = damage;
		m_hp -= damage;
		m_pEffectManager->DrawPlayerDamageEffect(m_pos);
		m_isMove = false;
		KnockBack(m_subVector);
	}
	//ガード中に遠距離攻撃に当たった時の処理
	else if (m_isHitAttack && m_isPlayerGuard && enemyType == kLongDistanceEnemy)
	{
		m_pSoundManager->HitLongDistanceAttackSE();
		m_hp -= damage*0.5f;
		m_pEffectManager->DrawPlayerGuardEffect(m_pos);
		m_pSoundManager->GuardSE();
		KnockBack(m_subVector);
	}

	//敵に当たった時の処理
	if (m_isHitEnemy && !m_isPlayerGuard)
	{
		m_pSoundManager->HitShortDistanceAttackSE();
		m_hp -= damage;
		m_isMove = false;

		KnockBack(m_subVector);
	}

	//ガード中に敵に当たった時の処理
	else if (m_isHitEnemy && m_isPlayerGuard)
	{
		m_pSoundManager->HitShortDistanceAttackSE();
		m_damage = damage * 0.5f;
		m_hp -= damage*0.5f;
		KnockBack(m_subVector);
	}

	if (!m_isHitEnemy && !m_isHitAttack && m_hp > 0)
	{
		m_damage = 0;
		m_isMove = true;
	}
}

//回復処理
void Player::RecoverHp(int recoveryQuantity)
{
	m_hp += recoveryQuantity;

	if (m_hp >= m_maxHp)
	{
		m_hp = m_maxHp;
	}
}

//敵の胴体に当たったかそうか
bool Player::HitEnemyBody(VECTOR enemyPos, VECTOR HitEnemyCollisionStart, VECTOR HitEnemyCollisionEnd, float HitEnemyRadius)
{
	m_subVector = VSub(enemyPos, m_pos);

	if (HitCheck_Capsule_Capsule(m_capsuleStart, m_capsuleEnd, m_radius, HitEnemyCollisionStart, HitEnemyCollisionEnd, HitEnemyRadius))
	{
		PushAway(HitEnemyCollisionStart, HitEnemyCollisionEnd, HitEnemyRadius);
	}
	if (HitCheck_Capsule_Capsule(m_capsuleStart, m_capsuleEnd, m_radius, HitEnemyCollisionStart, HitEnemyCollisionEnd, HitEnemyRadius) && !m_isInvincible)
	{
		m_isHitEnemy = true;
		m_isMove = false;
	}
	else if (!HitCheck_Capsule_Capsule(m_capsuleStart, m_capsuleEnd, m_radius, HitEnemyCollisionStart, HitEnemyCollisionEnd, HitEnemyRadius))
	{
		m_isHitEnemy = false;
	}
	

	return m_isHitEnemy;
}

//敵の攻撃が当たったかどうか
bool Player::HitEnemyAttack(VECTOR enemyPos, VECTOR HitEnemyAttackCollisionStart, VECTOR HitEnemyAttackCollisionEnd, float HitAttackEnemyRadius)
{
	m_subVector = VSub(enemyPos, m_pos);

	if (HitCheck_Capsule_Capsule(m_capsuleStart, m_capsuleEnd, m_radius, HitEnemyAttackCollisionStart, HitEnemyAttackCollisionEnd, HitAttackEnemyRadius) && !m_isInvincible)
	{
		m_isHitAttack = true;
	}
	else if(!HitCheck_Capsule_Capsule(m_capsuleStart, m_capsuleEnd, m_radius, HitEnemyAttackCollisionStart, HitEnemyAttackCollisionEnd, HitAttackEnemyRadius))
	{
		m_isHitAttack = false;
	}

	return m_isHitAttack;
}

//攻撃が当たった時の処理
void Player::HitAttack(bool hitAttack)
{
	//当たった時の処理
	if (hitAttack&& m_isAttack)
	{
		//攻撃が当たったのでスペシャルゲージがたまる
		m_specialMoveGauge += kSpecialMoveGauge;

		//スペシャルゲージが一定値を超えた時の処理
		if (m_specialMoveGauge >= kSuperMeter)
		{
			m_specialMoveGauge = kSuperMeter;
		}

		m_pSoundManager->SwordSE();

		return;
	}

	//攻撃が当たっていない時の処理
	else if(!hitAttack && m_isAttack)
	{
		m_pSoundManager->RollSwordSE();

		return;
	}
}

//敵の胴体に当たった時に押しのけあう処理
void Player::PushAway(VECTOR HitEnemyCollisionStart, VECTOR HitEnemyCollisionEnd, float HitEnemyRadius)
{
	//敵の胴体に当たった時の処理
	VECTOR centerA = VScale(VAdd(m_capsuleStart, HitEnemyCollisionStart), 0.5f);
	VECTOR centerB = VScale(VAdd(m_capsuleEnd, HitEnemyCollisionEnd), 0.5f);


	VECTOR pushVec = VSub(centerA, centerB);
	float len = VSize(pushVec);

	if (len > 0.0001f) 
	{
		pushVec = VGet(0.5f, 0.0f, 0.0f);
		len = 1.0f;
	}

	float overlap = (m_radius + HitEnemyRadius) - len;
	pushVec = VNorm(pushVec); // 単位ベクトル化
	pushVec = VScale(pushVec, overlap); // 押し出し量を掛ける

	//位置をずらす
	m_pos.x += pushVec.x;
	//m_pos.y += pushVec.y;
	m_pos.z +=pushVec.z;

}



//向いている方向を変更する処理
void Player::UpdateCol()
{
	// プレイヤーの向きをもとに当たり判定の位置を調整する
	MATRIX rotationMatrixY = MGetRotY(m_attackAngle);

	AttackCol(rotationMatrixY);
	
	MagicAttackCol(rotationMatrixY);

	SpecialMoveCol(rotationMatrixY);

	//敵の胴体にも攻撃にも当たっていないときの処理
	if (!m_isHitEnemy && !m_isHitAttack && m_hp > 0)
	{
		m_isMove = true;
	}

	//プレイヤーが死んだときの処理
	if (m_hp <= 0 && !m_isKnockedDown)
	{
		m_hp = 0;
		m_isMove = false;
	}
}

//攻撃の当たり判定の更新
void Player::AttackCol(MATRIX rotationMatrixY)
{
	// カプセルの移動方向を計算
	if (m_isAttackCollision)
	{
		//剣の当たり判定を更新
		m_attackCapsuleStart = VAdd(m_pos, (VTransform(m_attackCapsuleStartPoint, rotationMatrixY)));

		m_attackCapsuleEnd = VAdd(m_attackCapsuleStart, (VTransform(m_attackCapsuleEndPoint, rotationMatrixY)));
	}

	else
	{
		//剣の当たり判定を更新
		m_attackCapsuleStart = VGet(0.0f, kAttackPosY, 0.0f);
		m_attackCapsuleEnd = VGet(0.0f, kAttackPosY, 0.0f);
	}
}

//魔法攻撃の当たり判定の更新
void Player::MagicAttackCol(MATRIX rotationMatrixY)
{
	//魔法攻撃の当たり判定を更新
	//魔法攻撃を行っているが、魔法攻撃がまだ動いていない場合
	if (m_isMagicAttack && !m_isMagicAttackMove)
	{
		//カプセルの二点のうちひとつを設定
		m_magicCapsuleStart = VAdd(m_pos, (VTransform(m_magicCapsuleStartPoint, rotationMatrixY)));

		//カプセルの二点の残り一つを設定
		m_magicCapsuleEnd = VAdd(m_magicCapsuleStart, (VTransform(m_magicCapsuleEndPoint, rotationMatrixY)));

		//カプセルの進行方向を計算で出す
		m_magicDirection = VTransform({ 0.0f, 0.0f, 1.0f }, rotationMatrixY);

		//進行方向などの処理が終わったので魔法攻撃を動かす
		m_isMagicAttackMove = true;
	}

	//魔法攻撃が動いているときの処理
	if (m_isMagicAttackMove)
	{
		//魔法攻撃の進む方向とスピードを計算して出す
		m_magicCapsuleStart = VAdd(m_magicCapsuleStart, VScale(m_magicDirection, kMagicAttackSpeed));
		m_magicCapsuleEnd = VAdd(m_magicCapsuleEnd, VScale(m_magicDirection, kMagicAttackSpeed));

		//エフェクトの演出を描画する
		m_pEffectManager->DrawMagicAttackEffect(m_magicCapsuleEnd);

		// 最大距離に達したら停止
		if (VSize(VSub(m_magicCapsuleStart, m_pos)) > kMagicAttackDistance)
		{
			//最大距離まで来たのですべてfalseにする
			m_isMagicAttackMove = false;
			m_isMagicAttack = false;

			m_magicCapsuleStart = VGet(0.0f, kAttackPosY, 0.0f);
			m_magicCapsuleEnd = VGet(0.0f, kAttackPosY, 0.0f);
		}
	}
}

//必殺技の当たり判定の更新
void Player::SpecialMoveCol(MATRIX rotationMatrixY)
{
	//必殺技の当たり判定を更新
	if (m_isSpecialMove)
	{
		m_specialMoveStart = VAdd(m_pos, (VTransform(m_specialMoveStartPoint, rotationMatrixY)));
		m_specialMoveEnd = VAdd(m_specialMoveStart, (VTransform(m_specialMoveEndPoint, rotationMatrixY)));
	}
	else
	{
		m_specialMoveStart = VGet(0.0f, kAttackPosY, 0.0f);
		m_specialMoveEnd = VGet(0.0f, kAttackPosY, 0.0f);
	}
}
