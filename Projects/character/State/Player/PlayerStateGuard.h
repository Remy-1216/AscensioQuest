#pragma once
#include "PlayerStateBase.h"
class PlayerStateGuard :public PlayerStateBase
{
public:
	PlayerStateGuard(std::shared_ptr<Player> player) : PlayerStateBase(player) {};

	void Init();

	virtual void Update(Stage& stage, const Pad& pad, const Camera& camera);

	virtual StateKind GetKind() override { return StateKind::kGuard; }

	virtual void Draw();

#ifdef _DEBUG
	// 現在の状態名を取得
	virtual std::string GetStateName() override { return "ガード中"; }
#endif

	//Guardをしている状態かどうか
	bool m_isGurad;


};

