#include "Boss.h"
#include "LoadCsv.h"
#include "EnemyStateBase.h"
#include "EnemyStateWalk.h"
#include "Player.h"
#include <memory>

namespace
{
	const char* const kBossName = "boss";

	// 重力
	constexpr float kGravity = -5.0f;

	//胴体に当たった時のダメージ
	constexpr int kDamage = 5;

	//フレームの番号
	constexpr int kRightShoulder = 51;	//右肩
	constexpr int kRightElbow = 53;		//右肘
	constexpr int kRightHand = 70;		//右手
	constexpr int kLeftShoulder = 18;	//左肩
	constexpr int kLeftElbow = 19;		//左肘
	constexpr int kLeftHand = 26;		//左手

	//攻撃の当たり判定用カプセルの半径
	constexpr float kAttackRadius = 20.0f;

	//ボスが倒れた時に渡すステータスポイント
	constexpr int  kStatusPoint = 100;

	//遠距離攻撃の進むスピード
	constexpr float kAttackSpeed =10.0f;

	//魔法攻撃が進む距離
	constexpr float kAttackDistance =2500.0f;

	//ボスのモデルの大きさを調整
	constexpr float kBossSize = 1.5f;

	//投擲物の大きさを調整
	constexpr float kThrowingSize = 0.5f;

	//攻撃後の当たり判定の位置
	constexpr float kAttackPosY = -200.0f;
}

Boss::Boss() :CharacterBase(m_handle), m_throwingHandle(-1), m_statusPoint(-1), m_isMove(false), m_isGameClear(false),
m_attackDirection(VGet(0.0f,0.0f,0.0f)),m_attackCapsuleIntermediate(VGet(0.0f,0.0f,0.0f)), m_rightShoulderPos(VGet(0.0f,0.0f,0.0f)),
m_rightElbowPos(VGet(0.0f,0.0f,0.0f)), m_rightHandPos(VGet(0.0f,0.0f,0.0f)), m_leftShoulderPos(VGet(0.0f,0.0f,0.0f)),
m_leftElbowPos(VGet(0.0f,0.0f,0.0f)), m_leftHandPos(VGet(0.0f,0.0f,0.0f))
{
	//ステータスを読み込む
	LoadCsv::GetInstance().LoadStatus(m_status, kBossName);

	//当たり判定の時に使うカプセルの情報を読み込む
	LoadCsv::GetInstance().LoadCollisionInfo(m_collisionInfo, kBossName);
	
	//ボスの座標位置情報を読み込む
	LoadCsv::GetInstance().LoadPlayerPosData(m_characterPos, kBossName);

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

}

Boss::~Boss()
{
	//モデルを削除
	MV1DeleteModel(m_handle);
	MV1DeleteModel(m_throwingHandle);

	m_pEffectManager->ClearEffect();
	m_pEffectManager.reset();

	m_pEffectManager = nullptr;
}

void Boss::Init(int handle)
{
	//モデルを受け取る
 // モデルを受け取る
	m_handle = handle;

	m_isHitAttack = false;

	// 初期位置設定
	m_pos = VGet(m_characterPos.posX, m_characterPos.posY, m_characterPos.posZ);

	// アニメーションを読み込む
	LoadCsv::GetInstance().LoadBossAnimData(m_animData);

	// 遠距離攻撃時に描画されるモデルをロードする
	m_throwingHandle = MV1LoadModel("data/model/item/Rock.mv1");

	// モデルのサイズを変更
	MV1SetScale(m_handle, VGet(kBossSize, kBossSize, kBossSize));
	MV1SetScale(m_throwingHandle, VGet(kThrowingSize, kThrowingSize, kThrowingSize));

	// キャラクターの種類の設定
	m_characterKind = kBoss;

	// 重力
	m_gravity = kGravity;

	// 初期化
	m_isLightAttack = false;
	m_isStorongAttack = false;
	m_isThrowing = false;
	m_isKnockedDown = false;
	m_isGameClear = false;
	m_isAttack = false;
	m_isAttackMove = false;

	// 最初は死んでいないので動けるためtrue
	m_isMove = true;

	// 初期化
	m_attackCapsuleStart = VGet(0.0f, kAttackPosY, 0.0f);
	m_attackCapsuleEnd = VGet(0.0f, kAttackPosY, 0.0f);

	m_capsuleStart = VGet(0.0f, 0.0f, 0.0f);
	m_capsuleEnd = VGet(0.0f, 0.0f, 0.0f);

	// ステートの作成
	m_pEnemyState = std::make_shared<EnemyStateWalk>(shared_from_this());
	m_pEnemyState->m_nextState = m_pEnemyState;

	auto state = std::dynamic_pointer_cast<EnemyStateWalk>(m_pEnemyState);
	state->Init(m_characterKind);
}

void Boss::Update(Stage& stage, Player& player, VECTOR playerPos)
{
	m_isLightAttack = false;
	m_isStorongAttack = false;

	// 前のフレームと違うstateの場合
	if (m_pEnemyState->GetKind() != m_pEnemyState->m_nextState->GetKind())
	{
		// stateを変更する
		m_pEnemyState = m_pEnemyState->m_nextState;
		m_pEnemyState->m_nextState = m_pEnemyState;
	}

	// ステートの更新
	m_pEnemyState->Update(stage, player, m_characterKind);

	// 重力を足す
	m_pos = VAdd(m_pos, VGet(0.0f, m_gravity, 0.0f));

	// カプセルの座標をエネミーと同じ位置に調整
	m_capsuleStart = VAdd(m_pos, m_capsuleStartPoint);
	m_capsuleEnd = VAdd(m_pos, m_capsuleEndPoint);

	// プレイヤーと当たったかどうか
	HitPlayer(player);

	// プレイヤーの攻撃が当たったかどうかを渡す
	player.HitAttack(m_isHitAttack);

	// アニメーション
	UpdateAnim();

	// 攻撃の方向
	UpdateCol();

	// 向いている方向
	UpdateAngle(player.GetPos());

	//死んだ後の処理
	Died();
}

//描画
void Boss::Draw()
{
	//モデルの位置
	MV1SetPosition(m_handle, m_pos);

	//モデル描画
	if (!m_isKnockedDown)
	{
		MV1DrawModel(m_handle);
	}

	//投擲物が動いているかどうか
	if (m_isAttackMove)
	{
		MV1DrawModel(m_throwingHandle);

		MV1SetPosition(m_throwingHandle, m_attackCapsuleEnd);
	}

#ifdef _DEBUG

	//ボスの座標表示
	DrawFormatString(0, 400, GetColor(0, 0, 0), "ボスの座標(%.2f,%.2f,%.2f)", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(0, 450, GetColor(0, 0, 0), "ボスの当たり判定の始点(%.2f,%.2f,%.2f)", m_capsuleStart.x, m_capsuleStart.y, m_capsuleStart.z);
	DrawFormatString(0, 500, GetColor(0, 0, 0), "ボスの当たり判定の終点(%.2f,%.2f,%.2f)", m_capsuleEnd.x, m_capsuleEnd.y, m_capsuleEnd.z);

	////胴体の当たり判定のカプセルを表示
	DrawCapsule3D(m_capsuleStart, m_capsuleEnd, m_radius, 40, GetColor(255, 255, 255), GetColor(255, 255, 255), false);

	//弱攻撃の当たり判定カプセルを表示
	DrawCapsule3D(m_rightShoulderPos, m_rightElbowPos, kAttackRadius, 40, GetColor(255, 255, 0), GetColor(255, 255, 255), false);
	DrawCapsule3D(m_rightElbowPos, m_rightHandPos, kAttackRadius, 40, GetColor(255, 255, 0), GetColor(255, 255, 255), false);
	
	//強攻撃の当たり判定のカプセルを表示
	DrawCapsule3D(m_leftShoulderPos, m_leftElbowPos, kAttackRadius, 40, GetColor(255, 255, 0), GetColor(255, 255, 255), false);
	DrawCapsule3D(m_leftElbowPos, m_leftHandPos, kAttackRadius, 40, GetColor(255, 255, 0), GetColor(255, 255, 255), false);

	//遠距離攻撃の当たり判定のカプセルを表示
	DrawCapsule3D(m_attackCapsuleStart, m_attackCapsuleEnd, m_attackRadius, 40, GetColor(255, 255, 0), GetColor(0, 0, 0), false);
#endif
}

//プレイヤーとの当たり判定
void Boss::HitPlayer(Player& player)
{
	//プレイヤーと胴体が当たったかどうか
	if (player.HitEnemyBody(m_pos, m_capsuleStart, m_capsuleEnd, m_radius))
	{
		player.Damage(kDamage, kShortDistanceEnemy);
	}

	//プレイヤーに攻撃が当たったかどうか
	if (player.HitEnemyAttack(m_pos, m_attackCapsuleStart, m_attackCapsuleEnd, m_attackRadius))
	{
		player.Damage(m_attackPower, kShortDistanceEnemy);
	}

	//プレイヤーに自分の攻撃が当たったかどうか
	if (player.HitEnemyAttack(m_pos, m_attackCapsuleStart, m_attackCapsuleEnd, m_attackRadius))
	{
		player.Damage(static_cast<float>(m_attackPower), kLongDistanceEnemy);

		m_attackCapsuleStart = VGet(0.0f, kAttackPosY, 0.0f);
		m_attackCapsuleEnd = VGet(0.0f, kAttackPosY, 0.0f);

		m_isAttack = false;
		m_isAttackMove = false;
		m_isThrowing = false;
	}

	HitAnyPlayerAttack(player);
}

//プレイヤーの度の攻撃に当たったのかの確認
void Boss::HitAnyPlayerAttack(Player& player)
{
	// プレイヤーの攻撃が当たったかどうか
	HitPlayerAttack(player.GetAttackCapsuleStart(), player.GetAttackCapsuleEnd(),
					player.GetAttackRadius(), player.GetAttackPower());

	// プレイヤーの魔法攻撃が当たったかどうか
	HitPlayerAttack(player.GetMagicCapsuleStart(), player.GetMagicCapsuleEnd(),
					player.GetMagicCapsuleRadius(), player.GetMagicPower());

	// プレイヤーの必殺技に当たったかどうか
	HitPlayerAttack(player.GetSpecialMoveStart(), player.GetSpecialMoveEnd(),
				player.GetSpecialMoveRadius(), player.GetAttackPower());
}

//攻撃の当たり判定
void Boss::UpdateCol()
{
	//弱攻撃の当たり判定の更新
	LightAttackCol();

	//強攻撃の当たり判定の更新
	StorongAttackCol();

	//遠距離攻撃の当たり判定の更新
	ThrowingCol();

	if (!m_isAttack && !m_isAttackMove)
	{
		m_attackCapsuleStart = VGet(0.0f, kAttackPosY, 0.0f);
		m_attackCapsuleEnd = VGet(0.0f, kAttackPosY, 0.0f);
	}

}

//弱攻撃の当たり判定の更新
void Boss::LightAttackCol()
{
	//弱攻撃を行った時
	if (m_isLightAttack)
	{
		m_rightShoulderPos = MV1GetFramePosition(m_handle, kRightShoulder);
		m_rightElbowPos = MV1GetFramePosition(m_handle, kRightElbow);
		m_rightHandPos = MV1GetFramePosition(m_handle, kRightHand);
	}
	else
	{
		//攻撃の当たり判定を更新
		m_rightShoulderPos = VGet(0.0f, kAttackPosY, 0.0f);
		m_rightElbowPos = VGet(0.0f, kAttackPosY, 0.0f);
		m_rightHandPos = VGet(0.0f, kAttackPosY, 0.0f);
	}
}

//強攻撃の当たり判定の更新
void Boss::StorongAttackCol()
{
	//強攻撃を行った時
	if (m_isStorongAttack)
	{
		m_leftShoulderPos = MV1GetFramePosition(m_handle, kLeftShoulder);
		m_leftElbowPos = MV1GetFramePosition(m_handle, kLeftElbow);
		m_leftHandPos = MV1GetFramePosition(m_handle, kLeftHand);
	}
	else
	{
		m_leftShoulderPos = VGet(0.0f, kAttackPosY, 0.0f);
		m_leftElbowPos = VGet(0.0f, kAttackPosY, 0.0f);
		m_leftHandPos = VGet(0.0f, kAttackPosY, 0.0f);
	}
}

//投擲攻撃の当たり判定の更新
void Boss::ThrowingCol()
{
	//投擲攻撃を行った時
	if (m_isThrowing)
	{
		// プレイヤーの向きをもとに当たり判定の位置を調整する
		MATRIX rotationMatrix = MGetRotY(m_attackAngle);
		m_isAttack = true;

		//遠距離攻撃の当たり判定を更新
		if (m_isAttack && !m_isAttackMove)
		{
			m_attackCapsuleStart = VAdd(m_pos, (VTransform(m_attackCapsuleStartPoint, rotationMatrix)));

			m_attackDirection = VTransform({ 0.0f, 0.0f, 1.0f }, rotationMatrix);

			m_attackCapsuleEnd = VAdd(m_attackCapsuleStart, (VTransform(m_attackCapsuleEndPoint, rotationMatrix)));

			m_isAttackMove = true;
			m_isAttack = false;

		}
		if (m_isAttackMove)
		{
			m_attackCapsuleStart = VAdd(m_attackCapsuleStart, VScale(m_attackDirection, kAttackSpeed));
			m_attackCapsuleEnd = VAdd(m_attackCapsuleEnd, VScale(m_attackDirection, kAttackSpeed));

			// 最大距離に達したら停止
			if (VSize(VSub(m_attackCapsuleStart, m_pos)) >= kAttackDistance)
			{
				m_isAttack = false;
				m_isAttackMove = false;
				m_isThrowing = false;

				m_attackCapsuleStart = VGet(0.0f, kAttackPosY, 0.0f);
				m_attackCapsuleEnd = VGet(0.0f, kAttackPosY, 0.0f);

			}
		}
	}
}

//死んだ後の処理
void Boss::Died()
{

	m_isKnockedDown = m_pEnemyState->GetIsDie();

	// 死んだ瞬間の動き
	Die();

	//倒れるモーションが終わったかどうか
	if (m_isKnockedDown)
	{
		m_isGameClear = true;
	}
}
