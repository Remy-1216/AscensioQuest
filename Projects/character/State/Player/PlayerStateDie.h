#pragma once
#include "PlayerStateBase.h"
class PlayerStateDie:public PlayerStateBase
{
public:
	PlayerStateDie(std::shared_ptr<Player> player) : PlayerStateBase(player) {};

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// キャラクターの死んだ時の動き
	/// </summary>
	/// <param name="pad">コントローラー</param>
	/// <param name="camera">カメラ</param>
	/// <param name="characterKinds">キャラクターの種類</param>
	virtual void Update(Stage& stage, const Pad& pad, const Camera& camera);
	
	/// <summary>
	/// 現在の状態
	/// </summary>
	/// <returns></returns>
	virtual StateKind GetKind() override { return StateKind::kDie; }
	
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="characterKinds"></param>
	virtual void Draw();

#ifdef _DEBUG
	// 現在の状態名を取得
	virtual std::string GetStateName() override { return "死亡"; }
#endif
};

