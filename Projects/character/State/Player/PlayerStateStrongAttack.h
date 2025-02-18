#pragma once
#include "PlayerStateBase.h"
class PlayerStateStrongAttack :  public PlayerStateBase
{
public:
	PlayerStateStrongAttack(std::shared_ptr<Player> player) : PlayerStateBase(player) {};

	void Init(const int  characterKinds);

	virtual void Update(const Pad& pad, const Camera& camera, const int  characterKinds);

	/// <summary>
   /// 現在の状態を渡す
   /// </summary>
   /// <returns></returns>
	virtual StateKind GetKind() override { return StateKind::kSpecialMove; }

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(const int  characterKinds);

#ifdef _DEBUG
	// 現在の状態名を取得
	virtual std::string GetStateName() override { return "必殺技"; }
#endif

	//強攻撃ゲージの消費

};

