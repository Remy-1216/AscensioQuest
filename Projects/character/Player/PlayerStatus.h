#pragma once
#include "DxLib.h"
#include <memory>
class SoundManager;
class Pad;

class PlayerStatus
{

public:
	/// <summary>
	/// ステータスUP構造体
	/// </summary>
	struct  StatusUpValue
	{
		int StatusPoint;				//ステータスポイント
		int hpUp;						//HPUP数値
		int hpUpTimes;					//HPUP段階数
		int attackPowerUp;				//攻撃力UP数値
		int attackPowerUpTimes;			//攻撃力UP段階数
		int magicAttackPowerUp;			//魔法攻撃力UP数値
		int magicAttackPowerUpTimes;	//魔法攻撃力UP段階数
		int defensePowerUp;				//防御力UP数値
		int defensePowerUpTimes;		//防御力UP段階数
		int specialMoveGauge;			//必殺技ゲージ
		int customBonus;				//カスタムボーナスを表示した回数
	};

public:
	PlayerStatus();

	virtual ~PlayerStatus();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ステータスをUPして値を出力する
	/// </summary>
	void StatOutput(int statasUpPoint = 0.0f);

	/// <summary>
	/// ステータスをリセットし、出力する
	/// </summary>
	void ResetOutPut();

	//カスタムボーナスを受け取れるかどうか　
	void CustomBonus(const Pad& Pad);

	/// <summary>
	/// ステータスポイントの数値を与える
	/// </summary>
	/// <returns></returns>
	int GetStatusPoint()const {return m_statusPoint;}

	/// <summary>
	/// 強化したHPの数値を与える
	/// </summary>
	/// <returns></returns>
	int GetHpUp()const { return m_hpUp; }

	/// <summary>
	/// HPの強化回数を与える
	/// </summary>
	/// <returns></returns>
	int GetHpUpTimes() const { return m_hpUpTimes; }

	/// <summary>
	/// 強化した攻撃力の数値を与える
	/// </summary>
	/// <returns></returns>
	int GetAttackUp() const { return m_attackUp; }

	/// <summary>
	/// 攻撃力の強化回数を与える
	/// </summary>
	/// <returns></returns>
	int GetAttackUpTimes() const { return m_attackUpTimes; }

	/// <summary>
	/// 強化された魔法攻撃力の数値を与える
	/// </summary>
	/// <returns></returns>
	int GetMagicAttackUp()const { return m_magicAttackUp; }

	/// <summary>
	/// 魔法攻撃の強化回数
	/// </summary>
	/// <returns></returns>
	int GetMagicAttackUpTimes()const { return m_magicAttackUpTimes; }

	/// <summary>
	/// 強化された防御力の数値を与える
	/// </summary>
	/// <returns></returns>
	int GetDefensePowerUp() const { return m_defensePowerUp; }

	/// <summary>
	/// 防御力の強化回数
	/// </summary>
	/// <returns></returns>
	int GetDefensePowerUpTimes() const { return m_defensePowerUpTimes; }

	/// <summary>
	/// 必殺技ゲージ
	/// </summary>
	/// <returns></returns>
	int GetspecialMoveGauge() const { return m_specialMoveGauge; }

	/// <summary>
	/// カスタムボーナス
	/// </summary>
	/// <returns></returns>
	bool GetIsCustomBonus()const { return m_isCustomBonus; }

	/// <summary>
	/// フルカスタムボーナス
	/// </summary>
	/// <returns></returns>
	bool GetIsFullCustomBonus() const { return m_isFullCustomBonus; }

public:

	//HPをUPさせる
	void HpUp();

	//攻撃力UPする
	void AttackPowerUp();

	//魔法攻撃力UPする
	void MagicAttackPowerUp();

	//防御力UPする
	void DefensePowerUp();

private:

	std::shared_ptr<SoundManager> m_pSoundMamager;

	StatusUpValue m_statusUpValue;

	//ステータスポイント
	int m_statusPoint;

	//HPUP数値
	int m_hpUp;

	//HPUPした回数
	int m_hpUpTimes;

	//攻撃力UP数値
	int m_attackUp;

	//攻撃力UP回数
	int m_attackUpTimes;

	//魔法攻撃力UP数値
	int m_magicAttackUp;

	//魔法攻撃力UP回数
	int m_magicAttackUpTimes;

	//防御力UP数値
	int m_defensePowerUp;

	//防御力UP回数
	int m_defensePowerUpTimes;

	//必殺技ゲージ
	int m_specialMoveGauge;

	//カスタムボーナスを表示した回数
	int m_customBonus;

	//カスタムボーナス取得できるか
	bool m_isCustomBonus;

	//フルカスタムボーナス取得できるか
	bool m_isFullCustomBonus;


};

