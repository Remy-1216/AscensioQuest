#pragma once
#include "PlayerStateBase.h"
class PlayerStateMagicAttack :
    public PlayerStateBase
{
public:
    PlayerStateMagicAttack(std::shared_ptr<Player> player) : PlayerStateBase(player) ,m_animTime(0.0f),m_time(0.0f){};
    
    void Init();
    
    virtual void Update(Stage& stage, const Pad& pad, const Camera& camera);


    virtual StateKind GetKind() override { return StateKind::kMagicAttack; }

    virtual void Draw();

#ifdef _DEBUG
    // 現在の状態名を取得
    virtual std::string GetStateName() override { return "魔法攻撃"; }
#endif
private:

    //アニメーションの進む時間
    float m_animTime;

    //時間
    float m_time;
};

