#include "DxLib.h"
#include "Apple.h"
#include "Player.h"
#include "EffectManager.h"


namespace
{
	//当たり判定
	constexpr float  kSphereRadius = 20.0f;

	//回復量
	constexpr int kRecoveryQuantity = 10;

	//サイズ調整
	constexpr float kReduction = 0.55f;
}


Apple::Apple():m_handle(-1),m_isHitPlayer(false),m_pos(VGet(0.0f,0.0f,0.0f)),m_distance(VGet(0.0f,0.0f,0.0f))
{
}

Apple::~Apple()
{
	m_pEffectManager->ClearEffect();
	m_pEffectManager.reset();
	m_pEffectManager = nullptr;

	//モデルの削除
	MV1DeleteModel(m_handle);

}

void Apple::Init(VECTOR enemyPos)
{
	m_pEffectManager = std::make_shared<EffectManager>();
	m_pEffectManager->Init();

	//モデルのロード
	m_handle = MV1LoadModel("data/model/item/Fruit.mv1");

	m_pos = enemyPos;

	m_isHitPlayer = false;

	//モデルのサイズ変更
	MV1SetScale(m_handle, VGet(kReduction, kReduction, kReduction));
}

void Apple::Update()
{
	//エフェクトの動き
	m_pEffectManager->Update();

	//モデルの位置
	MV1SetPosition(m_handle, m_pos);
}

void Apple::Draw()
{
	//モデルの表示
	MV1DrawModel(m_handle);

	//エフェクトの描画
	m_pEffectManager->Draw();
	m_pEffectManager->DrawItemEffect(m_pos);

#ifdef  _DEBUG

	//足元のカプセルを表示
	DrawSphere3D(m_pos, kSphereRadius, 5, 0x000000, 0x000000, false);

#endif

}

void Apple::HitPlayer(Player& player)
{
	m_distance = VSub(player.GetPos(), m_pos);

	if (VSize(m_distance) < kSphereRadius + player.GetSphereRadius())
	{
		player.RecoverHp(kRecoveryQuantity);
		m_isHitPlayer = true;
	}
}
