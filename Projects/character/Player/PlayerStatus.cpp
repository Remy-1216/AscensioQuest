#include "Playerstatus.h"
#include "SoundManager.h"
#include "LoadCsv.h"
#include "Pad.h"
#include <string>
#include <fstream>
#include <sstream>

namespace
{
	//ステータスを上げる為に必要なポイント
	constexpr int kRequiredPoint2 = 2;
	constexpr int kRequiredPoint4 = 4;
	constexpr int kRequiredPoint6 = 6;
	constexpr int kRequiredPoint8 = 8;
	constexpr int kRequiredPoint10 = 10;
	constexpr int kRequiredPoint12 = 12;
	constexpr int kRequiredPoint14 = 14;
	constexpr int kRequiredPoint16 = 16;
	constexpr int kRequiredPoint18 = 18;
	constexpr int kRequiredPoint20 = 20;


	//HpUp
	constexpr int kHpUp = 5;

	constexpr int kHpUp0 = 0;
	constexpr int kHpUp1 = 5;
	constexpr int kHpUp2 = 10;
	constexpr int kHpUp3 = 15;
	constexpr int kHpUp4 = 20;
	constexpr int kHpUp5 = 25;
	constexpr int kHpUp6 = 30;
	constexpr int kHpUp7 = 35;
	constexpr int kHpUp8 = 40;
	constexpr int kHpUp9 = 45;
	constexpr int kHpUp10 = 50;


	//攻撃力Up
	constexpr int kAttackUp = 2;

	//魔法攻撃力Up
	constexpr int kMagicAttackUp = 2;

	//防御力Up
	constexpr int kDefensePowerUp = 2;

	//攻撃、魔法攻撃、防御力の段階ごとの数値
	constexpr int kStatusUp0 = 0;
	constexpr int kStatusUp1 = 2;
	constexpr int kStatusUp2 = 4;
	constexpr int kStatusUp3 = 6;
	constexpr int kStatusUp4 = 8;
	constexpr int kStatusUp5 = 10;
	constexpr int kStatusUp6 = 12;
	constexpr int kStatusUp7 = 14;
	constexpr int kStatusUp8 = 16;
	constexpr int kStatusUp9 = 18;

	//改造度段階数
	constexpr int kStatusUpTimes1 = 0;
	constexpr int kStatusUpTimes2 = 1;
	constexpr int kStatusUpTimes3 = 2;
	constexpr int kStatusUpTimes4 = 3;
	constexpr int kStatusUpTimes5 = 4;
	constexpr int kStatusUpTimes6 = 5;
	constexpr int kStatusUpTimes7 = 6;
	constexpr int kStatusUpTimes8 = 7;
	constexpr int kStatusUpTimes9 = 8;
	constexpr int kStatusUpTimes10 = 9;

	//カスタムボーナス
	constexpr int kSpecialMoveGauge = 50;

	//フルカスタムボーナス
	constexpr int kAttackBonus = 20;
	constexpr int kMagicAttackBonus = 10;

}

PlayerStatus::PlayerStatus(): m_statusPoint(0), m_hpUp(0), m_attackUp(0), m_attackUpTimes(0), m_magicAttackUp(0),
m_magicAttackUpTimes(0), m_defensePowerUp(0), m_defensePowerUpTimes(0), m_specialMoveGauge(0),m_customBonus(0), m_isCustomBonus(false), m_isFullCustomBonus(false)
{
}

PlayerStatus::~PlayerStatus()
{

}

void PlayerStatus::Init()
{
	LoadCsv::GetInstance().LoadStatusUp(m_statusUpValue);
	m_statusPoint = m_statusUpValue.StatusPoint;
	m_hpUp = m_statusUpValue.hpUp;
	m_hpUpTimes = m_statusUpValue.hpUpTimes;
	m_attackUp = m_statusUpValue.attackPowerUp;
	m_attackUpTimes = m_statusUpValue.attackPowerUpTimes;
	m_magicAttackUp = m_statusUpValue.magicAttackPowerUp;
	m_magicAttackUpTimes = m_statusUpValue.magicAttackPowerUpTimes;
	m_defensePowerUp = m_statusUpValue.defensePowerUp;
	m_defensePowerUpTimes = m_statusUpValue.defensePowerUpTimes;
	m_specialMoveGauge = m_statusUpValue.specialMoveGauge;
	m_customBonus = m_statusUpValue.customBonus;

	m_isCustomBonus = false;

	m_pSoundMamager = std::make_shared<SoundManager>();
}

void PlayerStatus::Draw()
{
#ifdef _DEBUG
	DrawFormatString(0, 50, GetColor(0,0,0), "ステータスポイント:%4d", m_statusPoint);
	DrawFormatString(0, 100, GetColor(0,0,0), "HPUP:%4d", m_hpUp);
	DrawFormatString(0, 150, GetColor(0,0,0), "攻撃力UP:%4d", m_attackUp);
	DrawFormatString(0, 200, GetColor(0,0,0), "魔法攻撃力UP:%4d", m_magicAttackUp);
	DrawFormatString(0, 250, GetColor(0,0,0), "防御力UP:%4d", m_defensePowerUp);
#endif // _DEBUG

}

//データをCSVに出力
void PlayerStatus::StatOutput(int statasUpPoint)
{
	m_statusPoint = m_statusPoint + statasUpPoint;

	//出力したいファイル名を指定する
	std::ofstream outputfile("data/csv/StatusUP.csv");

	std::vector<std::tuple<const char*, const char*, const char*, const char*, const  char*,const char*, 
		const char*, const char*, const char*, const char*, const char*>>nameData;
	std::vector<std::tuple<int, int,int,int,int, int, int, int, int,int,int>>data;
	//出力したいデータを作る
	nameData.push_back(std::make_tuple("ステータスポイント", "HPUP","HPUP段階数", "攻撃力UP","攻撃力UP段階数 ","魔法攻撃力UP", 
		"魔法攻撃力UP段階数", "防御力UP","防御力UP段階数","必殺技ゲージ","カスタムボーナスの説明回数"));
	data.push_back(std::make_tuple(m_statusPoint, m_hpUp,m_attackUpTimes, m_attackUp,m_attackUpTimes, m_magicAttackUp,
		m_magicAttackUpTimes, m_defensePowerUp,m_defensePowerUpTimes, m_specialMoveGauge, m_customBonus));
	//データの出力

	for (auto&& a : nameData) {
		outputfile << std::get<0>(a);
		outputfile << ',';
		outputfile << std::get<1>(a);
		outputfile << ',';
		outputfile << std::get<2>(a);
		outputfile << ',';
		outputfile << std::get<3>(a);
		outputfile << ',';
		outputfile << std::get<4>(a);
		outputfile << ',';
		outputfile << std::get<5>(a);
		outputfile << ',';
		outputfile << std::get<6>(a);
		outputfile << ',';
		outputfile << std::get<7>(a);
		outputfile << ',';
		outputfile << std::get<8>(a);
		outputfile << '\n';
	}
	for (auto&& b : data) {
		outputfile << std::get<0>(b);
		outputfile << ',';
		outputfile << std::get<1>(b);
		outputfile << ',';
		outputfile << std::get<2>(b);
		outputfile << ',';
		outputfile << std::get<3>(b);
		outputfile << ',';
		outputfile << std::get<4>(b);
		outputfile << ',';
		outputfile << std::get<5>(b);
		outputfile << ',';
		outputfile << std::get<6>(b);
		outputfile << ',';
		outputfile << std::get<7>(b);
		outputfile << ',';
		outputfile << std::get<8>(b);
		outputfile << ',';
		outputfile << std::get<9>(b);
		outputfile << ',';
		outputfile << std::get<10>(b);
		outputfile << '\n';
	}
	//ファイルの出力
	outputfile.close();
}

//ゲームを初期から始めた時に行うCSVの出力
void PlayerStatus::ResetOutPut()
{
	m_statusPoint = 0;
	m_hpUp = 0;
	m_hpUpTimes = 0;
	m_attackUp = 0;
	m_attackUpTimes = 0;
	m_magicAttackUp = 0;
	m_magicAttackUpTimes = 0;
	m_defensePowerUp = 0;
	m_defensePowerUpTimes = 0;
	m_specialMoveGauge = 0;
	m_customBonus = 0;

	//出力したいファイル名を指定する
	std::ofstream outputfile("data/csv/StatusUP.csv");

	std::vector<std::tuple<const char*, const char*, const char*, const char*, const  char*, const char*, const char*, const char*, const char*, const char*, const char*>>nameData;
	std::vector<std::tuple<int, int, int, int, int, int, int, int, int, int, int>>data;
	//出力したいデータを作る
	nameData.push_back(std::make_tuple("ステータスポイント", "HPUP", "HPUP段階数", "攻撃力UP", "攻撃力UP段階数 ", "魔法攻撃力UP", "魔法攻撃力UP段階数", "防御力UP", "防御力UP段階数", "必殺技ゲージ", "カスタムボーナスの説明回数"));
	data.push_back(std::make_tuple(m_statusPoint, m_hpUp, m_attackUpTimes, m_attackUp, m_attackUpTimes, m_magicAttackUp, m_magicAttackUpTimes, m_defensePowerUp, m_defensePowerUpTimes, m_specialMoveGauge, m_customBonus));
	//データの出力

	for (auto&& a : nameData) {
		outputfile << std::get<0>(a);
		outputfile << ',';
		outputfile << std::get<1>(a);
		outputfile << ',';
		outputfile << std::get<2>(a);
		outputfile << ',';
		outputfile << std::get<3>(a);
		outputfile << ',';
		outputfile << std::get<4>(a);
		outputfile << ',';
		outputfile << std::get<5>(a);
		outputfile << ',';
		outputfile << std::get<6>(a);
		outputfile << ',';
		outputfile << std::get<7>(a);
		outputfile << ',';
		outputfile << std::get<8>(a);
		outputfile << '\n';
	}
	for (auto&& b : data) {
		outputfile << std::get<0>(b);
		outputfile << ',';
		outputfile << std::get<1>(b);
		outputfile << ',';
		outputfile << std::get<2>(b);
		outputfile << ',';
		outputfile << std::get<3>(b);
		outputfile << ',';
		outputfile << std::get<4>(b);
		outputfile << ',';
		outputfile << std::get<5>(b);
		outputfile << ',';
		outputfile << std::get<6>(b);
		outputfile << ',';
		outputfile << std::get<7>(b);
		outputfile << ',';
		outputfile << std::get<8>(b);
		outputfile << ',';
		outputfile << std::get<9>(b);
		outputfile << ',';
		outputfile << std::get<10>(b);
		outputfile << '\n';
	}
	//ファイルの出力
	outputfile.close();
}

//カスタムボーナス
void PlayerStatus::CustomBonus(const Pad&pad)
{
	if (m_hpUpTimes >= kStatusUpTimes6 && m_attackUpTimes >= kStatusUpTimes6 && m_defensePowerUpTimes >= kStatusUpTimes6 &&
		m_magicAttackUpTimes >= kStatusUpTimes6 && m_defensePowerUpTimes >= kStatusUpTimes6 && m_customBonus == 0)
	{
		m_isCustomBonus = true;

		m_specialMoveGauge = kSpecialMoveGauge;

		if (pad.IsTrigger("A"))
		{
			m_isCustomBonus = false;

			m_customBonus++;
		}
	}

	if (m_hpUpTimes >= kStatusUpTimes10 && m_attackUpTimes >= kStatusUpTimes10 && m_defensePowerUpTimes >= kStatusUpTimes10 &&
		m_magicAttackUpTimes >= kStatusUpTimes10 && m_defensePowerUpTimes >= kStatusUpTimes10 && m_customBonus == 1)
	{
		m_isFullCustomBonus = true;

		m_attackUp += kAttackBonus;

		m_magicAttackUp += kMagicAttackBonus;


		
		m_customBonus++;
		
	}
}

//HPの上限を上げる処理
void PlayerStatus::HpUp()
{
	if (!m_isCustomBonus)
	{
		if (m_hpUp == kHpUp0 && m_statusPoint >= kRequiredPoint2)
		{
			m_pSoundMamager->StatusUpSE();

			m_hpUp += kHpUp;
			m_statusPoint -= kRequiredPoint2;
			m_hpUpTimes++;
		}
		else if (m_hpUp == kHpUp1 && m_statusPoint >= kRequiredPoint4)
		{
			m_pSoundMamager->StatusUpSE();

			m_hpUp += kHpUp;
			m_statusPoint -= kRequiredPoint4;
			m_hpUpTimes++;
		}
		else if (m_hpUp == kHpUp2 && m_statusPoint >= kRequiredPoint6)
		{
			m_pSoundMamager->StatusUpSE();

			m_hpUp += kHpUp;
			m_statusPoint -= kRequiredPoint6;
			m_hpUpTimes++;
		}
		else if (m_hpUp == kHpUp3 && m_statusPoint >= kRequiredPoint8)
		{
			m_pSoundMamager->StatusUpSE();

			m_hpUp += kHpUp;
			m_statusPoint -= kRequiredPoint8;
			m_hpUpTimes++;
		}
		else if (m_hpUp == kHpUp4 && m_statusPoint >= kRequiredPoint10)
		{
			m_pSoundMamager->StatusUpSE();

			m_hpUp += kHpUp;
			m_statusPoint -= kRequiredPoint10;
			m_hpUpTimes++;
		}
		else if (m_hpUp == kHpUp5 && m_statusPoint >= kRequiredPoint12)
		{
			m_pSoundMamager->StatusUpSE();

			m_hpUp += kHpUp;
			m_statusPoint -= kRequiredPoint12;
			m_hpUpTimes++;
		}
		else if (m_hpUp == kHpUp6 && m_statusPoint >= kRequiredPoint14)
		{
			m_pSoundMamager->StatusUpSE();

			m_hpUp += kHpUp;
			m_statusPoint -= kRequiredPoint14;
			m_hpUpTimes++;
		}
		else if (m_hpUp == kHpUp7 && m_statusPoint >= kRequiredPoint16)
		{
			m_pSoundMamager->StatusUpSE();

			m_hpUp += kHpUp;
			m_statusPoint -= kRequiredPoint16;
			m_hpUpTimes++;
		}
		else if (m_hpUp == kHpUp8 && m_statusPoint >= kRequiredPoint18)
		{
			m_pSoundMamager->StatusUpSE();

			m_hpUp += kHpUp;
			m_statusPoint -= kRequiredPoint18;
			m_hpUpTimes++;
		}
		else if (m_hpUp == kHpUp9 && m_statusPoint >= kRequiredPoint20)
		{
			m_pSoundMamager->StatusUpSE();

			m_hpUp += kHpUp;
			m_statusPoint -= kRequiredPoint20;
			m_hpUpTimes++;
		}
	}
	
}

//攻撃力を上げる時の処理
void PlayerStatus::AttackPowerUp()
{
	if (!m_isCustomBonus)
	{
		if (m_attackUp == kStatusUp0 && m_statusPoint >= kRequiredPoint2)
		{
			m_pSoundMamager->StatusUpSE();

			m_attackUp += kAttackUp;
			m_statusPoint -= kRequiredPoint2;
			m_attackUpTimes++;
		}
		else if (m_attackUp == kStatusUp1 && m_statusPoint >= kRequiredPoint4)
		{
			m_pSoundMamager->StatusUpSE();

			m_attackUp += kAttackUp;
			m_statusPoint -= kRequiredPoint4;
			m_attackUpTimes++;
		}
		else if (m_attackUp == kStatusUp2 && m_statusPoint >= kRequiredPoint6)
		{
			m_pSoundMamager->StatusUpSE();

			m_attackUp += kAttackUp;
			m_statusPoint -= kRequiredPoint6;
			m_attackUpTimes++;
		}
		else if (m_attackUp == kStatusUp3 && m_statusPoint >= kRequiredPoint8)
		{
			m_pSoundMamager->StatusUpSE();

			m_attackUp += kAttackUp;
			m_statusPoint -= kRequiredPoint8;
			m_attackUpTimes++;
		}
		else if (m_attackUp == kStatusUp4 && m_statusPoint >= kRequiredPoint10)
		{
			m_pSoundMamager->StatusUpSE();

			m_attackUp += kAttackUp;
			m_statusPoint -= kRequiredPoint10;
			m_attackUpTimes++;
		}
		else if (m_attackUp == kStatusUp5 && m_statusPoint >= kRequiredPoint12)
		{
			m_pSoundMamager->StatusUpSE();

			m_attackUp += kAttackUp;
			m_statusPoint -= kRequiredPoint12;
			m_attackUpTimes++;
		}
		else if (m_attackUp == kStatusUp6 && m_statusPoint >= kRequiredPoint14)
		{
			m_pSoundMamager->StatusUpSE();

			m_attackUp += kAttackUp;
			m_statusPoint -= kRequiredPoint14;
			m_attackUpTimes++;
		}
		else if (m_attackUp == kStatusUp7 && m_statusPoint >= kRequiredPoint16)
		{
			m_pSoundMamager->StatusUpSE();

			m_attackUp += kAttackUp;
			m_statusPoint -= kRequiredPoint16;
			m_attackUpTimes++;
		}
		else if (m_attackUp == kStatusUp8 && m_statusPoint >= kRequiredPoint18)
		{
			m_pSoundMamager->StatusUpSE();

			m_attackUp += kAttackUp;
			m_statusPoint -= kRequiredPoint18;
			m_attackUpTimes++;
		}
		else if (m_attackUp == kStatusUp9 && m_statusPoint >= kRequiredPoint20)
		{
			m_pSoundMamager->StatusUpSE();

			m_attackUp += kAttackUp;
			m_statusPoint -= kRequiredPoint20;
			m_attackUpTimes++;
		}
	}
	
}

//魔法攻撃力を上げる時の処理
void PlayerStatus::MagicAttackPowerUp()
{
	if (!m_isCustomBonus)
	{
		if (m_magicAttackUp == kStatusUp0 && m_statusPoint >= kRequiredPoint2)
		{
			m_pSoundMamager->StatusUpSE();

			m_magicAttackUp += kMagicAttackUp;
			m_statusPoint -= kRequiredPoint2;
			m_magicAttackUpTimes++;
		}
		else if (m_magicAttackUp == kStatusUp1 && m_statusPoint >= kRequiredPoint4)
		{
			m_pSoundMamager->StatusUpSE();

			m_magicAttackUp += kMagicAttackUp;
			m_statusPoint -= kRequiredPoint4;
			m_magicAttackUpTimes++;
		}
		else if (m_magicAttackUp == kStatusUp2 && m_statusPoint >= kRequiredPoint6)
		{
			m_pSoundMamager->StatusUpSE();

			m_magicAttackUp += kMagicAttackUp;
			m_statusPoint -= kRequiredPoint6;
			m_magicAttackUpTimes++;
		}
		else if (m_magicAttackUp == kStatusUp3 && m_statusPoint >= kRequiredPoint8)
		{
			m_pSoundMamager->StatusUpSE();

			m_magicAttackUp += kMagicAttackUp;
			m_statusPoint -= kRequiredPoint8;
			m_magicAttackUpTimes++;
		}
		else if (m_magicAttackUp == kStatusUp4 && m_statusPoint >= kRequiredPoint10)
		{
			m_pSoundMamager->StatusUpSE();

			m_magicAttackUp += kMagicAttackUp;
			m_statusPoint -= kRequiredPoint10;
			m_magicAttackUpTimes++;
		}
		else if (m_magicAttackUp == kStatusUp5 && m_statusPoint >= kRequiredPoint12)
		{
			m_pSoundMamager->StatusUpSE();

			m_magicAttackUp += kMagicAttackUp;
			m_statusPoint -= kRequiredPoint12;
			m_magicAttackUpTimes++;
		}
		else if (m_magicAttackUp == kStatusUp6 && m_statusPoint >= kRequiredPoint14)
		{
			m_pSoundMamager->StatusUpSE();

			m_magicAttackUp += kMagicAttackUp;
			m_statusPoint -= kRequiredPoint14;
			m_magicAttackUpTimes++;
		}
		else if (m_magicAttackUp == kStatusUp7 && m_statusPoint >= kRequiredPoint16)
		{
			m_pSoundMamager->StatusUpSE();

			m_magicAttackUp += kMagicAttackUp;
			m_statusPoint -= kRequiredPoint16;
			m_magicAttackUpTimes++;
		}
		else if (m_magicAttackUp == kStatusUp8 && m_statusPoint >= kRequiredPoint18)
		{
			m_pSoundMamager->StatusUpSE();

			m_magicAttackUp += kMagicAttackUp;
			m_statusPoint -= kRequiredPoint18;
			m_magicAttackUpTimes++;
		}
		else if (m_magicAttackUp == kStatusUp9 && m_statusPoint >= kRequiredPoint20)
		{
			m_pSoundMamager->StatusUpSE();

			m_magicAttackUp += kMagicAttackUp;
			m_statusPoint -= kRequiredPoint20;
			m_magicAttackUpTimes++;
		}
	}
	
}

void PlayerStatus::DefensePowerUp()
{
	if (!m_isCustomBonus)
	{
		if (m_defensePowerUp == kStatusUp0 && m_statusPoint >= kRequiredPoint2)
		{
			m_pSoundMamager->StatusUpSE();

			m_defensePowerUp += kDefensePowerUp;
			m_statusPoint -= kRequiredPoint2;
			m_defensePowerUpTimes++;
		}
		else if (m_defensePowerUp == kStatusUp1 && m_statusPoint >= kRequiredPoint4)
		{
			m_pSoundMamager->StatusUpSE();

			m_defensePowerUp += kDefensePowerUp;
			m_statusPoint -= kRequiredPoint4;
			m_defensePowerUpTimes++;
		}
		else if (m_defensePowerUp == kStatusUp2 && m_statusPoint >= kRequiredPoint6)
		{
			m_pSoundMamager->StatusUpSE();

			m_defensePowerUp += kDefensePowerUp;
			m_statusPoint -= kRequiredPoint6;
			m_defensePowerUpTimes++;
		}
		else if (m_defensePowerUp == kStatusUp3 && m_statusPoint >= kRequiredPoint8)
		{
			m_pSoundMamager->StatusUpSE();

			m_defensePowerUp += kDefensePowerUp;
			m_statusPoint -= kRequiredPoint8;
			m_defensePowerUpTimes++;
		}
		else if (m_defensePowerUp == kStatusUp4 && m_statusPoint >= kRequiredPoint10)
		{
			m_pSoundMamager->StatusUpSE();

			m_defensePowerUp += kDefensePowerUp;
			m_statusPoint -= kRequiredPoint10;
			m_defensePowerUpTimes++;
		}
		else if (m_defensePowerUp == kStatusUp5 && m_statusPoint >= kRequiredPoint12)
		{
			m_pSoundMamager->StatusUpSE();

			m_defensePowerUp += kDefensePowerUp;
			m_statusPoint -= kRequiredPoint12;
			m_defensePowerUpTimes++;
		}
		else if (m_defensePowerUp == kStatusUp6 && m_statusPoint >= kRequiredPoint14)
		{
			m_pSoundMamager->StatusUpSE();

			m_defensePowerUp += kDefensePowerUp;
			m_statusPoint -= kRequiredPoint14;
			m_defensePowerUpTimes++;
		}
		else if (m_defensePowerUp == kStatusUp7 && m_statusPoint >= kRequiredPoint16)
		{
			m_pSoundMamager->StatusUpSE();

			m_defensePowerUp += kDefensePowerUp;
			m_statusPoint -= kRequiredPoint16;
			m_defensePowerUpTimes++;
		}
		else if (m_defensePowerUp == kStatusUp8 && m_statusPoint >= kRequiredPoint18)
		{
			m_pSoundMamager->StatusUpSE();

			m_defensePowerUp += kDefensePowerUp;
			m_statusPoint -= kRequiredPoint18;
			m_defensePowerUpTimes++;
		}
		else if (m_defensePowerUp == kStatusUp9 && m_statusPoint >= kRequiredPoint20)
		{
			m_pSoundMamager->StatusUpSE();

			m_defensePowerUp += kDefensePowerUp;
			m_statusPoint -= kRequiredPoint20;
			m_defensePowerUpTimes++;
		}
	}

	
}
