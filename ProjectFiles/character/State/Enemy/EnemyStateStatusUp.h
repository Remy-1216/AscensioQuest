﻿#pragma once
#include "EnemyStateBase.h"

class EnemyStateStatusUp : public EnemyStateBase
{
public:

	EnemyStateStatusUp(std::shared_ptr<CharacterBase>enemy) :EnemyStateBase(enemy) {}

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 自分を強化しているときの動き
	/// </summary>
	/// <param name="stage"></param>
	/// <param name="player"></param>
	virtual void Update(Stage& stage, const Player& player, const int  characterKinds);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(const int  characterKinds);

	/// <summary>
	/// 現在の状態
	/// </summary>
	/// <returns></returns>
	virtual StateKind GetKind() override { return StateKind::kStatusUp; }

private:

};

