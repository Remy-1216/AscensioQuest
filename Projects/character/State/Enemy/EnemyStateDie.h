#pragma once
#include "EnemyStateBase.h"
class EnemyStateDie :public EnemyStateBase
{
public:
	EnemyStateDie(std::shared_ptr<CharacterBase>enemy) :EnemyStateBase(enemy) {}

	void Init(const int  characterKinds);

	virtual void Update(Stage& stage, const Player& player, const int  characterKinds);

	virtual void Draw(const int  characterKinds);

	virtual StateKind GetKind() override { return StateKind::kDie; }

private:
	/// <summary>
	/// 近距離型エネミーが死んだ時の動き
	/// </summary>
	void ShortDistanveEnemyDie(Stage& stage, const Player& player, const int  characterKinds);

	/// <summary>
	/// 遠距離型エネミーが死んだときの動き
	/// </summary>
	void LongDistanveEnemyDie(Stage& stage, const Player& player, const int  characterKinds);

	/// <summary>
	/// ボスが死んだときの動き
	/// </summary>
	void BossDie(Stage& stage, const Player& player, const int  characterKinds);
};

