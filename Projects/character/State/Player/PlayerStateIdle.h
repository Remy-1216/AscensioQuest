#pragma once
#include "PlayerStateBase.h"
#include "DxLib.h"


class PlayerStateIdle :public PlayerStateBase
{
public:
	PlayerStateIdle(std::shared_ptr<Player> player) : PlayerStateBase(player) {};

	void Init();
	virtual void Update(Stage& stage, const Pad&pad,const Camera& camera);
	virtual StateKind GetKind() override { return StateKind::kWait; }
	virtual void Draw();

#ifdef _DEBUG
	// 現在の状態名を取得
	virtual std::string GetStateName() override { return "待機"; }
#endif
private:
	/// <summary>
	/// プレイヤーだった場合の待機状態の動き
	/// </summary>
	void PlayerIdle(const Pad& pad, const Camera& camera);
	
};

