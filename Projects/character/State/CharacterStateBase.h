#pragma once
#include <memory>
#include <string>
#include "DxLib.h"
#include "CharacterBase.h"

class Player;
class Pad;
class Camera;

class CharacterStateBase :public std::enable_shared_from_this<CharacterStateBase>
{
public:

	//キャラクターの状態
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
		kDie			//死んでしまった状態
	};

	CharacterStateBase(std::shared_ptr<CharacterBase> pcharacter) : m_pCharacter(pcharacter) {};


	virtual ~CharacterStateBase() = default;

	/// <summary>
	/// エネミーのUpdateの処理
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="characterKins">キャラの種類</param>
	virtual void Update(const Player& player, const int characterKinds) = 0;

	/// <summary>
	/// プレイヤーのUpdateの処理
	/// </summary>
	/// <param name="pad"></param>
	/// <param name="camera"></param>
	/// <param name="characterKinds"></param>
	virtual void Update(const Pad& pad, const Camera& camera, const int  characterKinds) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="characterKinds"></param>
	virtual void Draw(const int characterKinds) = 0;

	/// <summary>
	/// 現在の状態
	/// </summary>
	/// <returns></returns>
	virtual StateKind GetKind() = 0;

	/// <summary>
	/// 死んだかどうかを渡す
	/// </summary>
	/// <returns></returns>
	bool GetIsDie() const { return m_isDie; }

public:
	std::shared_ptr<CharacterStateBase>m_nextState;

	std::shared_ptr<CharacterBase> m_pCharacter;

protected:

	

	/// <summary>
	/// 死んだかどうか
	/// </summary>
	bool m_isDie;
};

