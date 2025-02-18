#pragma once
#include "PlayerStateBase.h"
class PlayerStateDamage :
    public PlayerStateBase
{
public:
    PlayerStateDamage(std::shared_ptr<Player> player) : PlayerStateBase(player) {};

    /// <summary>
    /// 初期化
    /// </summary>
    void Init();

    /// <summary>
    /// ダメージを受けた時の動き
    /// </summary>
    /// <param name="pad">コントローラー</param>
    /// <param name="camera">カメラ</param>
    virtual void Update(Stage& stage, const Pad& pad, const Camera& camera);
    
    /// <summary>
    /// 現在の状態を与える
    /// </summary>
    /// <returns></returns>
    virtual StateKind GetKind() override { return StateKind::kDamage; }

    /// <summary>
    /// 描画
    /// </summary>
    virtual void Draw();

#ifdef _DEBUG
    // 現在の状態名を取得
    virtual std::string GetStateName() override { return "ダメージが入った"; }
#endif
};

