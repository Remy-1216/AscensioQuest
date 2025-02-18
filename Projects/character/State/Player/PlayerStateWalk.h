#pragma once
#include "PlayerStateBase.h"
class PlayerStateWalk :public PlayerStateBase
{
public:

	PlayerStateWalk(std::shared_ptr<Player> player) : PlayerStateBase(player),m_walkSpeed(0.0f){}
	void Init();

	/// <summary>
	/// 歩く状態の動き
	/// </summary>
	/// <param name="pad">コントローラー</param>
	/// <param name="camera">カメラ</param>
	virtual void Update(Stage& stage,const Pad& pad, const Camera& camera);

	/// <summary>
	/// 現在の状態を渡す
	/// </summary>
	/// <returns></returns>
	virtual StateKind GetKind() override { return StateKind::kWalk; }


	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

#ifdef _DEBUG
	/// <summary>
	/// 今の状態
	/// </summary>
	/// <returns></returns>
	virtual std::string GetStateName() override { return "歩く"; }
#endif

private:

	/// <summary>
	/// 歩く処理
	/// </summary>
	void WalkingProcess(Stage& stage);

private:


	//走るスピード
	float m_walkSpeed;
};

