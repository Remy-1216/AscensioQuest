#pragma once
#include "PlayerStateBase.h"
class PlayerStateSpecialMove : public PlayerStateBase
{
public:
    PlayerStateSpecialMove(std::shared_ptr<Player> player) : PlayerStateBase(player) {}

	void Init();

    /// <summary>
    /// 必殺技をうっている時の動き
    /// </summary>
    /// <param name="pad">コントローラー</param>
    /// <param name="camera">カメラ</param>
    virtual void Update(Stage& stage, const Pad& pad, const Camera& camera);

    /// <summary>
    /// 現在の状態を渡す
    /// </summary>
    /// <returns></returns>
    virtual StateKind GetKind() override { return StateKind::kSpecialMove; }


    /// <summary>
    /// 描画
    /// </summary>
    virtual void Draw();

#ifdef _DEBUG
    // 現在の状態名を取得
    virtual std::string GetStateName() override { return "必殺技"; }
#endif
};

