#pragma once
#include "PlayerStateBase.h"
class PlayerStateAttack :
    public PlayerStateBase
{
public:
    PlayerStateAttack(std::shared_ptr<Player> player) : PlayerStateBase(player), m_aButtonCount(0), m_isAttacking(true){};

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

    //Aボタンを押した回数
    int m_aButtonCount;

    //攻撃を行った回数
    int m_attacks;

    //ボタンを押して、経過した時間
    int m_aPushTime;

    //攻撃2の再生時間
    float m_attack2Time;

    //攻撃3の再生時間
    float m_attack3Time;

    //攻撃中かどうか
    bool m_isAttacking;

    //ボタンを押しているかどうか
    bool m_isPush;
};

