#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"
#include "PlayerStateWalk.h"
#include "PlayerStateAttack.h"
#include "PlayerStateMagicAttack.h"
#include "PlayerStateGuard.h"
#include "PlayerStateDamage.h"
#include "PlayerStateDie.h"
#include "DxLib.h"
#include "Player.h"
#include "Camera.h"
#include "Pad.h"
#include<cmath>

namespace
{
	//アナログスティックによる移動関連
	constexpr float kMaxSpeed = 0.5f;		//プレイヤーの最大移動速度
	constexpr float kAnalogRangeMin = 0.1;	//アナログスティックの入力判定範囲
	constexpr float kAnalogRangeMax = 0.8;
	constexpr float kAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大値
}
void PlayerStateRun::Init()
{
	m_pPlayer->ChangeAnim("Run");
	m_runSpeed = 0.0f;
	m_playerPos = VGet(0.0f, 0.0f, 0.0f);
	m_playerMove = VGet(0.0f, 0.0f, 0.0f);
	m_analogX = 0.0f;
	m_analogZ = 0.0f;

	m_isHitCharacter = false;
}

void PlayerStateRun::Update(Stage&stage,const Pad& pad, const Camera& camera)
{
	m_hp = m_pPlayer->GetHp();
	m_runSpeed = m_pPlayer->GetStatus().runSpeed;
	m_playerPos = m_pPlayer->GetPos();
	m_cameraAngle = camera.GetCameraAngle();
	m_isHitCharacter = m_pPlayer->GetIsHitEnemy();

	//走っている状態から歩く状態に変更
	//RBボタンを離すことで歩く状態に変更
	if (pad.IsRelase("RB") && (pad.IsPress("left") || pad.IsPress("right") || pad.IsPress("up") || pad.IsPress("down")))
	{
		m_nextState = std::make_shared<PlayerStateWalk>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateWalk>(m_nextState);
		state->Init();
		return;
	}


	//走っている状態から待機状態に変更
	if (pad.IsRelase("left") || (pad.IsRelase("right") || pad.IsRelase("up") || pad.IsRelase("down")))
	{
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		return;
	}

	//走っている状態から攻撃攻撃状態に変更
	if (pad.IsTrigger("A"))
	{
		m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
		state->Init();
		return;
	}


	//走っている状態から魔法攻撃状態に変更
	if (pad.IsTrigger("B"))
	{
		m_nextState = std::make_shared<PlayerStateMagicAttack>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateMagicAttack>(m_nextState);
		state->Init();
		return;
	}

	//走っている状態からガード状態
	if (pad.IsPress("LB"))
	{
		m_nextState = std::make_shared<PlayerStateGuard>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateGuard>(m_nextState);
		state->Init();
		return;
	}

	//走っている状態からダメージを受ける状態
	if (m_isHitCharacter)
	{
		m_nextState = std::make_shared<PlayerStateDamage>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateDamage>(m_nextState);
		state->Init();
		return;
	}

	//ダメージを受けた状態から死亡状態
	if (m_hp <= 0)
	{
		m_nextState = std::make_shared<PlayerStateDie>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateDie>(m_nextState);
		state->Init();
	}

	//走っているときの処理
	m_move = VGet(0.0f, 0.0f, 0.0f);


	//アナログスティックを使って移動
	int analogX = 0;
	int analogZ = 0;

	GetJoypadAnalogInput(&analogX, &analogZ, DX_INPUT_PAD1);
	//アナログスティックの入力の10% ~ 80%の範囲を使用する

	//ベクトルの長さが最大で1000になる
	//プレイヤーの最大移動速度は0.01f / frame

	m_move = VGet(analogX, 0.0f, -analogZ);	//ベクトルの長さは0～1000

	m_analogX = analogX;
	m_analogZ = analogZ;

	//0.00 ~ 0.01の長さにする
	//ベクトルの長さを取得する
	float len = VSize(m_move);

	//ベクトルの長さを0.0~1.0の割合に変換する
	float rate = len / kAnalogInputMax;

	//アナログスティック無効な範囲を除外する
	rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
	rate = std::min(rate, 1.0f);
	rate = std::max(rate, 0.0f);
	//速度が決定できるので移動ベクトルに反映する
	m_move = VNorm(m_move);
	float speed = m_runSpeed * rate;
	m_move = VScale(m_move, speed);

	
	// カメラの回転を基準に方向を変換
	m_rotMatrix = MGetRotY(m_cameraAngle);
	m_movementDirection = VTransform(m_move, m_rotMatrix);

	m_pPlayer->Move(stage,  m_movementDirection);

}

void PlayerStateRun::Draw()
{
	DrawFormatString(0, 50, GetColor(0,0,0), "走っている状態");
}
