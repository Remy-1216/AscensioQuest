#pragma once
#include "PlayerStateBase.h"
class PlayerStateAttack :
    public PlayerStateBase
{
public:
    PlayerStateAttack(std::shared_ptr<Player> player) : PlayerStateBase(player), m_aButtonCount(0), m_isAttacking(true) { m_animTime = 0.0f; m_time = 0.0f; m_aButtonCount = 0; m_attacks = 0; m_isAttacking = false; m_isPush = false; };

    void Init();

    /// <summary>
    /// 攻撃時の動き
    /// </summary>
    /// <param name="pad">コントローラー</param>
    /// <param name="camera">カメラ</param>
    virtual void Update(Stage& stage, const Pad& pad, const Camera& camera);
    
    /// <summary>
    /// 現在の状態を渡す
    /// </summary>
    /// <returns></returns>
    virtual StateKind GetKind() override { return StateKind::kAttack; }
    

    /// <summary>
    /// 描画
    /// </summary>
    virtual void Draw();

#ifdef _DEBUG
    // 現在の状態名を取得
    virtual std::string GetStateName() override { return "攻撃"; }
#endif
private:

    /// <summary>
    /// 攻撃を
    /// </summary>
    void Attack();
private:

    //アニメーションの進む時間
    float m_animTime;

    //時間
    float m_time;

    //Aボタンを押した回数
    int m_aButtonCount;

    //攻撃を行った回数
    int m_attacks;

    //攻撃中かどうか
    bool m_isAttacking;

    //ボタンを押しているかどうか
    bool m_isPush;
};