﻿#define NOMINMAX
#include "LoadCsv.h"
#include "CharacterBase.h"
#include "Camera.h"
#include "Stage.h"
#include "EffectManager.h"
#include "UIBar.h"
#include <windows.h>	
#include <algorithm>
// 定数
namespace
{
	// アニメーション情報
	constexpr float kAnimBlendMax = 1.0f;	 // アニメーションブレンドの最大値
	constexpr float kAnimBlendSpeed = 0.1f;	 // アニメーションブレンドの変化速度

	//ノックバック
	constexpr float kKnockBack = 0.5f;

	
}

CharacterBase::CharacterBase(int handle):m_status(),m_animData(),m_currentPlayAnim(0),m_prevPlayAnim(0.0f),m_animBlendRate(0.0f), m_currentAnimTime(0), m_prevAnimTime(0.0f), m_totalAnimTime(0.0f),
m_animPlaySpeed(0.0f),m_animLoopStartTime(0.0f),m_animLoopEndTime(0.0f),m_isLoopAnim(false), m_isAttack(false), m_isAttackMove(false),m_hp(0),m_mp(0),m_attackPower(0),m_magicAttackPower(0), m_defensePower(0),m_walkSpeed(0.0f), m_runSpeed(0.0f), m_handle(-1),
m_modelPolygon(0),m_pos(VGet(0.0f,0.0f,0.0f)),m_moveDir(VGet(0.0f, 0.0f, 0.0f)), m_knockBack(VGet(0.0f,0.0f,0.0f))
{
	//エフェクト
	m_pEffectManager = std::make_shared<EffectManager>();
	m_pEffectManager->Init();

    m_pUIBar = std::make_shared<UIBar>();
    m_pUIBar->Init();
}

CharacterBase::~CharacterBase()
{
	//モデルの削除
	MV1DrawModel(m_handle);

	m_pEffectManager->ClearEffect();
	m_pEffectManager.reset();
	m_pEffectManager = nullptr;
}

void CharacterBase::DrawShadowModel()
{
    //シャドウマップにモデルを描画する
    MV1DrawModel(m_handle);

  
}

//アニメーションの更新
void CharacterBase::UpdateAnim()
{
    // ブレンド率が1以下の場合
    if (m_animBlendRate < kAnimBlendMax)
    {
        m_animBlendRate += kAnimBlendSpeed;
        m_animBlendRate = std::min(m_animBlendRate, kAnimBlendMax);
    }

    // アニメーションの再生時間を進める
    m_currentAnimTime += m_animPlaySpeed;

    // アニメーションが繰り返し行われる場合
    if (m_isLoopAnim && !m_isPlayerGuard)
    {
        if (m_currentAnimTime > m_animLoopEndTime)
        {
            m_currentAnimTime = m_animLoopStartTime;
        }
    }

    if (m_isLoopAnim && m_isPlayerGuard)
    {
        if (m_currentAnimTime > m_animLoopEndTime)
        {
            m_currentAnimTime = m_animLoopEndTime;
        }
    }

    // アニメーションの総再生時間を超えた場合
    if (m_currentAnimTime > m_totalAnimTime)
    {
        m_currentAnimTime = 0.0f;
    }

    // 再生時間を更新
    MV1SetAttachAnimTime(m_handle, m_currentPlayAnim, m_currentAnimTime);

    // アニメーションのブレンド率を設定する

    // m_prevPlayAnimが有効なインデックスか確認
    if (m_prevPlayAnim != -1) 
    {
        MV1SetAttachAnimBlendRate(m_handle, m_prevPlayAnim, kAnimBlendMax - m_animBlendRate);
    }
    MV1SetAttachAnimBlendRate(m_handle, m_currentPlayAnim, m_animBlendRate);
}



//アニメーションの変更
void CharacterBase::ChangeAnim(std::string animName)
{
    // 前のアニメーションをデタッチする
    if (m_prevPlayAnim != -1)
    {
        MV1DetachAnim(m_handle, m_prevPlayAnim);
        m_prevPlayAnim = -1;
    }

    // 再生中のアニメーションを1つ前に移動する
    m_prevPlayAnim = m_currentPlayAnim;
    m_prevAnimTime = m_currentAnimTime;

    // アニメーションを設定
    m_animPlaySpeed = m_animData[animName].playSpeed;
    m_animLoopStartTime = m_animData[animName].loopFrame;
    m_animLoopEndTime = m_animData[animName].endFrame;

    // ループ終了フレームが0でない場合、ループフラグを立てる
    if (m_animLoopEndTime > 0)
    {
        m_isLoopAnim = true;
    }
    else
    {
        m_isLoopAnim = false;
    }

    // 新たにアニメーションをアタッチする
    m_currentPlayAnim = MV1AttachAnim(m_handle, m_animData[animName].number);
    m_currentAnimTime = 0.0f;

    //アニメーションの総再生時間を設定
    m_totalAnimTime = MV1GetAnimTotalTime(m_handle, m_animData[animName].number);

    // ブレンド率はprevが有効でない場合、1.0にする
    if (m_prevPlayAnim == -1)
    {
        m_animBlendRate = kAnimBlendMax;
    }
    else
    {
        m_animBlendRate = 0.0f;
    }

    //ガードのアニメーション時の処理
    if (animName == "Guard" || animName == "Die" || animName == "Death")
    {
        //ガードを行っているのでtrue
        m_isPlayerGuard = true;
    }
    else
    {
        //ガードを行っていないのでfalse
        m_isPlayerGuard = false;
    }
}


float CharacterBase::GetAnimTotalTime(std::string animName)
{

	int animIndex = m_animData[animName].number;
	float totalTime = MV1GetAnimTotalTime(m_handle, animIndex);

	return totalTime;
}

//プレイヤーに向かう処理
void CharacterBase::ComingCharacter(Stage& stage, VECTOR moveVec)
{
	// 移動中のみ向きを更新する
	if (VSize(moveVec) > 0.0f)
	{
		m_characterAngle = atan2f(-moveVec.x, -moveVec.z);

		m_attackAngle = atan2f(moveVec.x, moveVec.z);
	}

	m_pos = stage.CheckObjectCol(*this, moveVec);
}

//近距離攻撃をする敵が攻撃をした時の処理
void CharacterBase::ShortDistanceEnemyAttack(float animTime)
{
	if (animTime <= 30.0f)
	{
		m_isShortDistanceEnemyAttack = true;
	}

	if (animTime >= 38.0f)
	{
		m_isShortDistanceEnemyAttack = false;
	}
}

//遠距離攻撃をする敵が攻撃を行ったときの処理
void CharacterBase::LongDistanceEnemyAttack()
{
	m_isAttack = true;
}

//ボスの攻撃時の処理
void CharacterBase::BossAttack(int attackKinds)
{
	if (attackKinds == kLightAttack)
	{
		m_isLightAttack = true;
	}
	if (attackKinds == kStorongAttack)
	{
		m_isStorongAttack = true;
	}
	if (attackKinds == kThrowingAttack)
	{
		m_isThrowing = true;
	}
}

//ノックバックの処理
void CharacterBase::KnockBack(VECTOR m_vector)
{
	m_knockBack = VScale(m_vector, -0.5f);
	m_pos = VAdd(m_pos, VScale(m_knockBack, kKnockBack));
}

//キャラクターの向いている方向
void CharacterBase::UpdateAngle(VECTOR playerpos)
{
	// ３Ｄモデル２から３Ｄモデル１に向かうベクトルを算出
	m_subVector = VSub(playerpos, m_pos);

	m_characterAngle = atan2f(m_subVector.x, m_subVector.z);

	//プレイヤーの方向を向く
	MV1SetRotationXYZ(m_handle, VGet(0.0f, m_characterAngle + DX_PI_F, 0.0f));
}


//プレイヤーの攻撃を受けた時の処理
void CharacterBase::HitPlayerAttack(VECTOR attackStart, VECTOR attackEnd, float radius, int attackPower)
{
	if (HitCheck_Capsule_Capsule(m_capsuleStart, m_capsuleEnd, m_radius, attackStart, attackEnd, radius) && m_isMove)
	{
		m_hp -= attackPower;
		m_isHitAttack = true;
		m_pEffectManager->DrawEnemyDamageEffect(m_pos);
		KnockBack(m_subVector);
	}
	else
	{
		m_isHitAttack = false;
	}

	if (m_hp <= 0)
	{
		m_hp = 0;
		m_isMove = false;
	}
}

//死んだときの処理
void CharacterBase::Die()
{
	if (!m_isMove)
	{
		m_capsuleStart = VGet(0.0f, -100.0f, 0.0f);
		m_capsuleEnd = VGet(0.0f, -100.0f, 0.0f);
	}
}

void CharacterBase::End()
{
}
