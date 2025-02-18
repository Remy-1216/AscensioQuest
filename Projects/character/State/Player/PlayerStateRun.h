#pragma once
#include "PlayerStateBase.h"
class PlayerStateRun :public PlayerStateBase
{
public:

	PlayerStateRun(std::shared_ptr<Player> player) : PlayerStateBase(player),m_runSpeed(0.0f){};
	void Init();

	virtual void Update(Stage& stage, const Pad& pad, const Camera& camera);
	virtual StateKind GetKind() override { return StateKind::kRun; }
	virtual void Draw();

#ifdef _DEBUG
	// 現在の状態名を取得
	virtual std::string GetStateName() override { return "走る"; }
#endif

private:
	//走るスピード
	float m_runSpeed;
};

