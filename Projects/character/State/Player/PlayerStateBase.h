#pragma once
#include <memory>
#include <string>
#include "DxLib.h"
#include "CharacterBase.h"

class Player;
class TitlePlayer;
class Camera;
class Stage;
class Pad;

class PlayerStateBase: public std::enable_shared_from_this<PlayerStateBase>
{
public:

	//プレイヤーの状態
	enum StateKind
	{
		kWait,			//待機状態
		kWalk,			//歩いてる状態
		kRun,			//走っている状態
		kAttack,		//攻撃している状態
		kMagicAttack,	//魔法攻撃をしている状態
		kSpecialMove,	//必殺技を打っている状態
		kGuard,			//ガードをしている状態
		kDamage,		//敵からダメージを受けている状態
		kDie,			//死んでしまった状態
		kFistPump,		//ガッツポーズ状態
		kWaveHand,		//手を振っている状態
	};

	PlayerStateBase(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer, m_characterKind = 0, m_hp = 0, m_cameraAngle = 0.0f, m_playerAngle = 0.0f,m_analogX = 0.0f,
		m_analogZ = 0.0f, m_isDie = false, m_isSpecialMove = false, m_isSpecialMoveTime = false, m_move = VGet(0.0f,0.0f,0.0f), m_playerPos = VGet(0.0f,0.0f,0.0f), m_movementDirection = VGet(0.0f,0.0f,0.0f); }

	virtual void Update(Stage& stage,const Pad& pad, const Camera& camera) = 0;

	virtual void Draw() = 0;

	// 現在の状態を取得
	virtual StateKind GetKind() = 0;

	/// <summary>
	/// 死んだかどうかを渡す
	/// </summary>
	/// <returns></returns>
	bool GetIsDie() const { return m_isDie; }



#ifdef _DEBUG
	// 現在の状態名を取得
	virtual std::string GetStateName() = 0;
#endif

public:
	std::shared_ptr<PlayerStateBase> m_nextState;	// 次のStateを保存する

protected:
	std::shared_ptr<Player> m_pPlayer;

	//ジョイパッドの出力
	DINPUT_JOYSTATE m_input;

	//キャラクターの種類
	int m_characterKind;

	//プレイヤーのHP
	int m_hp;

	//カメラのアングル
	float m_cameraAngle;

	//プレイヤーのアングル
	float m_playerAngle;

	//アナログスティックの数値
	float m_analogX;
	float m_analogZ;

	//キャラクターとキャラクターが当たったかどうか
	bool m_isHitCharacter;

	//死んだかどうか
	bool m_isDie;

	//必殺技を打てるかどうか
	bool m_isSpecialMove;

	//必殺技のアニメーションが最後まで終わったかどうか
	bool m_isSpecialMoveTime;

	/// <summary>
	/// プレイヤーがどこまで動いたか
	/// </summary>
	VECTOR m_move;

	//プレイヤーの座標位置
	VECTOR m_playerPos;

	//プレイヤーの移動した位置
	VECTOR m_playerMove;

	//カメラの回転を方向に変換した値を保存する
	VECTOR m_movementDirection;

	//カメラのマトリックス
	MATRIX m_rotMatrix;

	// モデルの回転行列を作成
	MATRIX m_modelRotMatrix;
};

