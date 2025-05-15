#include "PlayerStateWalk.h"
#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAttack.h"
#include "PlayerStateMagicAttack.h"
#include "PlayerStateGuard.h"
#include "PlayerStateDamage.h"
#include "PlayerStateSpecialMove.h"
#include "PlayerStateDie.h"
#include "DxLib.h"
#include "Player.h"
#include "Camera.h"
#include "Pad.h"

namespace
{
	//アナログスティックによる移動関連
	constexpr float kMaxSpeed = 0.5f;		//プレイヤーの最大移動速度
	constexpr float kAnalogRangeMin = 0.1;	//アナログスティックの入力判定範囲
	constexpr float kAnalogRangeMax = 0.8;
	constexpr float kAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大値
}

void PlayerStateWalk::Init()
{
	m_pPlayer->ChangeAnim("Walk");
	m_playerPos = VGet(0.0f, 0.0f, 0.0f);
	m_playerMove = VGet(0.0f, 0.0f, 0.0f);
	m_walkSpeed = 0.0f;
	m_analogX = 0.0f;
	m_analogZ = 0.0f;

	m_isSpecialMove = false;
}

void PlayerStateWalk::Update(Stage& stage,const Pad& pad, const Camera& camera)
{
	m_walkSpeed = m_pPlayer->GetStatus().walkSpeed;
	m_playerPos = m_pPlayer->GetPos();
	m_isSpecialMove = m_pPlayer->GetIsSpecialMove();
	m_cameraAngle = camera.GetCameraAngle();
	m_isHitCharacter = m_pPlayer->GetIsHitEnemy();

	//歩く処理
	WalkingProcess(stage);


	//走っている状態から歩く状態に変更
	//RBボタンを押すことで歩く状態に変更
	if (pad.IsPress("RB") && (pad.IsPress("left") || pad.IsPress("right") || pad.IsPress("up") || pad.IsPress("down")))
	{
		m_nextState = std::make_shared<PlayerStateRun>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateRun>(m_nextState);
		state->Init();
		return;
	}

	//歩いている状態から待機状態に変更
	else if (pad.IsRelase("left") || (pad.IsRelase("right") || pad.IsRelase("up") || pad.IsRelase("down")))
	{
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		return;
	}

	//歩いている状態から攻撃攻撃状態に変更
	if (pad.IsTrigger("A"))
	{
		m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
		state->Init();
		return;
	}

	//歩いている状態から魔法攻撃状態に変更
	if (pad.IsTrigger("B"))
	{
		m_nextState = std::make_shared<PlayerStateMagicAttack>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateMagicAttack>(m_nextState);
		state->Init();
		return;
	}

	//歩いている状態からガード状態
	if (pad.IsPress("LB"))
	{
		m_nextState = std::make_shared<PlayerStateGuard>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateGuard>(m_nextState);
		state->Init();
		return;
	}

	//歩いている状態からダメージを受ける状態
	if (m_isHitCharacter)
	{
		m_nextState = std::make_shared<PlayerStateDamage>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateDamage>(m_nextState);
		state->Init();
		return;
	}

	//歩いている状態から必殺技を打つ状態
	if (pad.IsPress("Y") && m_isSpecialMove)
	{
		m_nextState = std::make_shared<PlayerStateSpecialMove>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateSpecialMove>(m_nextState);
		state->Init();
		return;
	}

	//ダメージを受けた状態から死亡状態
	if (m_pPlayer->GetHp() <= 0)
	{
		m_nextState = std::make_shared<PlayerStateDie>(m_pPlayer);
		auto state = std::dynamic_pointer_cast<PlayerStateDie>(m_nextState);
		state->Init();
	}

	
}

void PlayerStateWalk::Draw()
{
	DrawFormatString(0, 50, GetColor(0,0,0), "歩いている状態");
}

void PlayerStateWalk::WalkingProcess(Stage& stage)
{
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
	float speed = m_walkSpeed * rate;
	m_move = VScale(m_move, speed);

	// カメラの回転を基準に方向を変換
	m_rotMatrix = MGetRotY(m_cameraAngle);
	m_movementDirection = VTransform(m_move, m_rotMatrix);

	m_pPlayer->Move(stage, m_movementDirection);
}