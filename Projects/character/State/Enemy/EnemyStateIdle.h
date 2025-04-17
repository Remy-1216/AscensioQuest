#pragma once
#include "EnemyStateBase.h"
class Player;

class EnemyStateIdle : public EnemyStateBase
{
public:
	EnemyStateIdle(std::shared_ptr<CharacterBase>enemy):EnemyStateBase(enemy){}

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="characterKinds"></param>
	void Init(const int characterKinds);

	/// <summary>
	/// 待機中の動き
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="characterKinds">敵の種類</param>
	virtual void Update(Stage& stage, const Player& player, const int  characterKinds);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="characterKinds"></param>
	virtual void Draw(const int  characterKinds);

	/// <summary>
	/// 現在の状態
	/// </summary>
	/// <returns></returns>
	virtual StateKind GetKind() override { return StateKind::kIdle; }

private:
	VECTOR GoToPlayer(VECTOR playerPos);

private:
	VECTOR m_move;
};

