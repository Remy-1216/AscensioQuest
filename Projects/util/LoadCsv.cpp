#include "LoadCsv.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>


LoadCsv* LoadCsv::m_instance = nullptr;

namespace
{
	const char* const kCharaStatusFileName = "data/csv/CharaStatus.csv";
	const char* const kCharaAnimDataFileName = "data/csv/AnimData.csv";
	const char* const kCharacterPosDataFileName = "data/csv/CharacterPos.csv";
	const char* const kStageInformationName = "data/csv/StageInformation.csv";
	const char* const kShortDistanceEnemyAnimFileName = "data/csv/ShortDistanceEnemyAnimData.csv";
	const char* const kLongDistanceEnemyAnimFileName = "data/csv/LongDistanceEnemyAnimData.csv";
	const char* const kBossAnimFileName = "data/csv/BossAnimData.csv";
	const char* const kCollisionInfoFileName = "data/csv/CollisionInfo.csv";
	const char* const kEnemyPosOnStageFileName = "data/csv/EnemyPosOnStage.csv";
	const char* const kWarpPointPosFileName = "data/csv/WarpPoint.csv";
	const char* const kStatusUpFileName = "data/csv/StatusUP.csv";
	const char* const kEffectDataFileName = "data/csv/EffectData.csv";
	const char* const kFontDataFileName = "data/csv/FontData.csv";
	const char* const kMapPosFileName = "data/csv/MapPos.csv";
	const char* const kBossAIFileName = "data/csv/BossAI.csv";

	/// <summary>
	/// 文字列を分割する
	/// </summary>
	/// <param name="input">文字列</param>
	/// <param name="delimiter">区切る文字(,)</param>
	/// <returns>分割した文字列</returns>
	std::vector<std::string> split(std::string& input, char delimiter)
	{
		std::istringstream stream(input);
		std::string field;
		std::vector<std::string> result;
		while (getline(stream, field, delimiter))
		{
			result.push_back(field);
		}
		return result;
	}
}

//キャラクターのステータスを読み込む
void LoadCsv::LoadStatus(CharacterBase::Status& data, const char* charaName)
{
	std::ifstream ifs(kCharaStatusFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		// 参照したいキャラが見つかっていたら処理をやめる
		// MEMO:strcmp 文字列を比較する 第1引数 = 第2引数の場合0
		if (strcmp(str, charaName) == 0)
		{
			break;
		}
		else
		{
			strvec.erase(strvec.begin(), strvec.end());
		}
	}

	// ステータス情報を代入する
	data.maxHp = std::stof(strvec[1]);
	data.maxMp = std::stof(strvec[2]);
	data.attackPower = std::stof(strvec[3]);
	data.magicAttackPower = std::stof(strvec[4]);
	data.defensePower = std::stof(strvec[5]);
	data.walkSpeed = std::stof(strvec[6]);
	data.runSpeed = std::stof(strvec[7]);
}

//キャラクターの当たり判定の時に使うカプセルの座標位置を読み込む
void LoadCsv::LoadCollisionInfo(CharacterBase::CollisionInfo& collisionInfo, const char* charaName)
{
	std::ifstream ifs(kCollisionInfoFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		// 参照したいキャラが見つかっていたら処理をやめる
		// MEMO:strcmp 文字列を比較する 第1引数 = 第2引数の場合0
		if (strcmp(str, charaName) == 0)
		{
			break;
		}
		else
		{
			strvec.erase(strvec.begin(), strvec.end());
		}
	}

	// カプセルの情報を代入する
	collisionInfo.capsuleStartPoint.x = std::stof(strvec[1]);
	collisionInfo.capsuleStartPoint.y = std::stof(strvec[2]);
	collisionInfo.capsuleStartPoint.z = std::stof(strvec[3]);
	collisionInfo.capsuleEndPoint.x = std::stof(strvec[4]);
	collisionInfo.capsuleEndPoint.y = std::stof(strvec[5]);
	collisionInfo.capsuleEndPoint.z = std::stof(strvec[6]);
	collisionInfo.radius = std::stof(strvec[7]);
	collisionInfo.attackCapsuleStartPoint.x = std::stof(strvec[8]);
	collisionInfo.attackCapsuleStartPoint.y = std::stof(strvec[9]);
	collisionInfo.attackCapsuleStartPoint.z = std::stof(strvec[10]);
	collisionInfo.attackCapsuleEndPoint.x = std::stof(strvec[11]);
	collisionInfo.attackCapsuleEndPoint.y = std::stof(strvec[12]);
	collisionInfo.attackCapsuleEndPoint.z = std::stof(strvec[13]);
	collisionInfo.attackRadius = std::stof(strvec[14]);
	collisionInfo.magicCapsuleStartPoint.x = std::stof(strvec[15]);
	collisionInfo.magicCapsuleStartPoint.y = std::stof(strvec[16]);
	collisionInfo.magicCapsuleStartPoint.z = std::stof(strvec[17]);
	collisionInfo.magicCapsuleEndPoint.x = std::stof(strvec[18]);
	collisionInfo.magicCapsuleEndPoint.y = std::stof(strvec[19]);
	collisionInfo.magicCapsuleEndPoint.z = std::stof(strvec[20]);
	collisionInfo.magicRadius = std::stof(strvec[21]);
	collisionInfo.specialMoveStartPoint.x = std::stof(strvec[22]);
	collisionInfo.specialMoveStartPoint.y = std::stof(strvec[23]);
	collisionInfo.specialMoveStartPoint.z = std::stof(strvec[24]);
	collisionInfo.specialMoveEndPoint.x = std::stof(strvec[25]);
	collisionInfo.specialMoveEndPoint.y = std::stof(strvec[26]);
	collisionInfo.specialMoveEndPoint.z = std::stof(strvec[27]);
	collisionInfo.specialMoveRadius = std::stof(strvec[28]);
}

//プレイヤーのアニメーション情報を読み込む
void LoadCsv::LoadPlayerAnimData(std::map<std::string, CharacterBase::AnimInfo>& data)
{
	std::ifstream ifs(kCharaAnimDataFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		// アニメーション情報を設定
		std::string animName = strvec[0];
		try
		{
			data[animName].number = std::stoi(strvec[1]);
			data[animName].loopFrame = std::stof(strvec[2]);
			data[animName].endFrame = std::stof(strvec[3]);
			data[animName].playSpeed = std::stof(strvec[4]);
		}
		catch (const std::invalid_argument& e)
		{
			
		}
	}
}

//プレイヤーの座標位置を読み込む
void LoadCsv::LoadPlayerPosData(CharacterBase::CharacterPos& characterPos, const char* charaName)
{
	std::ifstream ifs(kCharacterPosDataFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		// 参照したいキャラが見つかっていたら処理をやめる
		// MEMO:strcmp 文字列を比較する 第1引数 = 第2引数の場合0
		if (strcmp(str, charaName) == 0)
		{
			break;
		}
		else
		{
			strvec.erase(strvec.begin(), strvec.end());
		}
	}

	characterPos.posX = std::stoi(strvec[1]);
	characterPos.posY = std::stoi(strvec[2]);
	characterPos.posZ = std::stoi(strvec[3]);
}

//近距離型エネミーのアニメーション情報を読み込む
void LoadCsv::LoadShortDistanceEnemyAnimData(std::map<std::string, CharacterBase::AnimInfo>& data)
{
	std::ifstream ifs(kShortDistanceEnemyAnimFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		// アニメーション情報を設定
		std::string animName = strvec[0];
		try
		{
			data[animName].number = std::stoi(strvec[1]);
			data[animName].loopFrame = std::stof(strvec[2]);
			data[animName].endFrame = std::stof(strvec[3]);
			data[animName].playSpeed = std::stof(strvec[4]);
		}
		catch (const std::invalid_argument& e)
		{
		}

	
	}
}

//遠距離型エネミーのアニメーション情報を読み込む
void LoadCsv::LoadLongDistanceEnemyAnimData(std::map<std::string, CharacterBase::AnimInfo>& data)
{
	std::ifstream ifs(kLongDistanceEnemyAnimFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		// アニメーション情報を設定
		std::string animName = strvec[0];
		try
		{
			data[animName].number = std::stoi(strvec[1]);
			data[animName].loopFrame = std::stof(strvec[2]);
			data[animName].endFrame = std::stof(strvec[3]);
			data[animName].playSpeed = std::stof(strvec[4]);
		}
		catch (const std::invalid_argument& e)
		{
		}
	}
}

//ボスのアニメーションを読み込む
void LoadCsv::LoadBossAnimData(std::map<std::string, CharacterBase::AnimInfo>& data)
{
	std::ifstream ifs(kBossAnimFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		// アニメーション情報を設定
		std::string animName = strvec[0];
		try
		{
			data[animName].number = std::stoi(strvec[1]);
			data[animName].loopFrame = std::stof(strvec[2]);
			data[animName].endFrame = std::stof(strvec[3]);
			data[animName].playSpeed = std::stof(strvec[4]);
		}
		catch (const std::invalid_argument& e)
		{
		}
	}
}

//エネミーの座標を読み込む
void LoadCsv::LoadEnemyPos(EnemyManager::Pos& enemyPos, const char* enemyname)
{
	std::ifstream ifs(kEnemyPosOnStageFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		// 参照したいキャラが見つかっていたら処理をやめる
		// MEMO:strcmp 文字列を比較する 第1引数 = 第2引数の場合0
		if (strcmp(str, enemyname) == 0)
		{
			break;
		}
		else
		{
			strvec.erase(strvec.begin(), strvec.end());
		}
	}

	// ステータス情報を代入する
	enemyPos.enemyPosX1 = std::stof(strvec[1]);
	enemyPos.enemyPosY1 = std::stof(strvec[2]);
	enemyPos.enemyPosZ1 = std::stof(strvec[3]);
	enemyPos.enemyPosX2 = std::stof(strvec[4]);
	enemyPos.enemyPosY2 = std::stof(strvec[5]);
	enemyPos.enemyPosZ2 = std::stof(strvec[6]);
	enemyPos.enemyPosX3 = std::stof(strvec[7]);
	enemyPos.enemyPosY3 = std::stof(strvec[8]);
	enemyPos.enemyPosZ3 = std::stof(strvec[9]);
	enemyPos.enemyPosX4 = std::stof(strvec[10]);
	enemyPos.enemyPosY4 = std::stof(strvec[11]);
	enemyPos.enemyPosZ4 = std::stof(strvec[12]);
	enemyPos.enemyPosX5 = std::stof(strvec[13]);
	enemyPos.enemyPosY5 = std::stof(strvec[14]);
	enemyPos.enemyPosZ5 = std::stof(strvec[15]);
	enemyPos.enemyPosX6 = std::stof(strvec[16]);
	enemyPos.enemyPosY6 = std::stof(strvec[17]);
	enemyPos.enemyPosZ6 = std::stof(strvec[18]);
	enemyPos.enemyPosX7 = std::stof(strvec[19]);
	enemyPos.enemyPosY7 = std::stof(strvec[20]);
	enemyPos.enemyPosZ7 = std::stof(strvec[21]);
	enemyPos.enemyPosX8 = std::stof(strvec[22]);
	enemyPos.enemyPosY8 = std::stof(strvec[23]);
	enemyPos.enemyPosZ8 = std::stof(strvec[24]);
}

//ステージの位置をロードする
void LoadCsv::LoadStageInfo(Stage::StagePos& data, const char* charaName)
{
	std::ifstream ifs(kStageInformationName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		// 参照したい名前が見つかっていたら処理をやめる
		// MEMO:strcmp 文字列を比較する 第1引数 = 第2引数の場合0
		if (strcmp(str, charaName) == 0)
		{
			break;
		}
		else
		{
			strvec.erase(strvec.begin(), strvec.end());
		}
	}

	// ステージ情報を代入する
	data.posX = std::stof(strvec[1]);
	data.posY = std::stof(strvec[2]);
	data.posZ = std::stof(strvec[3]);
	data.size = std::stof(strvec[4]);
}

//ワープ位置を読み込む
void LoadCsv::LoadWarpPointPos(Stage::WarpPointPos& data, const char* warpName)
{
	std::ifstream ifs(kWarpPointPosFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		// 参照したい名前が見つかっていたら処理をやめる
		// MEMO:strcmp 文字列を比較する 第1引数 = 第2引数の場合0
		if (strcmp(str, warpName) == 0)
		{
			break;
		}
		else
		{
			strvec.erase(strvec.begin(), strvec.end());
		}
	}

	//ワープ関連情報を代入する
	data.warpSourceX = std::stof(strvec[1]);
	data.warpSourceY = std::stof(strvec[2]);
	data.warpSourceZ = std::stof(strvec[3]);
	data.warpTargetX = std::stof(strvec[4]);
	data.warpTargetY = std::stof(strvec[5]);
	data.warpTargetZ = std::stof(strvec[6]);
	

}

//強化ステータスデータを読み込む
void LoadCsv::LoadStatusUp(PlayerStatus::StatusUpValue& data)
{
	std::ifstream ifs(kStatusUpFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		// アニメーション情報を設定
		std::string animName = strvec[0];
		try
		{
			data.StatusPoint = std::stoi(strvec[0]);
			data.hpUp= std::stoi(strvec[1]);
			data.hpUpTimes = std::stoi(strvec[2]);
			data.attackPowerUp =  std::stoi(strvec[3]);
			data.attackPowerUpTimes = std::stoi(strvec[4]);
			data.magicAttackPowerUp = std::stoi(strvec[5]);
			data.magicAttackPowerUpTimes = std::stoi(strvec[6]);
			data.defensePowerUp = std::stoi(strvec[7]);
			data.defensePowerUpTimes = std::stoi(strvec[8]);
			data.specialMoveGauge = std::stoi(strvec[9]);
			data.customBonus = std::stoi(strvec[10]);
		

		}
		catch (const std::invalid_argument& e)
		{
		}
	}
}

void LoadCsv::LoadEffectData(EffectManager::EffectData& data, const char* effectName)
{
	std::ifstream ifs(kEffectDataFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		// 参照したい名前が見つかっていたら処理をやめる
		// MEMO:strcmp 文字列を比較する 第1引数 = 第2引数の場合0
		if (strcmp(str, effectName) == 0)
		{
			break;
		}
		else
		{
			strvec.erase(strvec.begin(), strvec.end());
		}
	}

	//ワープ関連情報を代入する
	data.EffectTime = std::stof(strvec[1]);
	data.EffectSize = std::stof(strvec[2]);
	data.EffectAdjPosY = std::stof(strvec[3]);
}

//フォントデータを読み込む
void LoadCsv::LoadFontData(Font::FontId& data, const char* fontDate)
{
	std::ifstream ifs(kFontDataFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		// 参照したいキャラが見つかっていたら処理をやめる
		// MEMO:strcmp 文字列を比較する 第1引数 = 第2引数の場合0
		if (strcmp(str, fontDate) == 0)
		{
			break;
		}
		else
		{
			strvec.erase(strvec.begin(), strvec.end());
		}

	}

	// ステータス情報を代入する
}

//マップの端の位置を読み込む
void LoadCsv::LoadMapPosData(Stage::MapPos& data, const char* direction)
{
	std::ifstream ifs(kMapPosFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		// 参照したいキャラが見つかっていたら処理をやめる
		// MEMO:strcmp 文字列を比較する 第1引数 = 第2引数の場合0
		if (strcmp(str, direction) == 0)
		{
			break;
		}
		else
		{
			strvec.erase(strvec.begin(), strvec.end());
		}
	}
	data.map1PosX = std::stoi(strvec[1]);
	data.map1PosZ = std::stoi(strvec[2]);
	data.map2PosX = std::stoi(strvec[3]);
	data.map2PosZ = std::stoi(strvec[4]);
	data.map3PosX = std::stoi(strvec[5]);
	data.map3PosZ = std::stoi(strvec[6]);
	data.map4PosX = std::stoi(strvec[7]);
	data.map4PosZ = std::stoi(strvec[8]);
	data.map5PosX = std::stoi(strvec[9]);
	data.map5PosZ = std::stoi(strvec[10]);
}

void LoadCsv::LoadMapPosData(EnemyManager::AreaPos& data, const char* direction)
{
	std::ifstream ifs(kMapPosFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		// 参照したいキャラが見つかっていたら処理をやめる
		// MEMO:strcmp 文字列を比較する 第1引数 = 第2引数の場合0
		if (strcmp(str, direction) == 0)
		{
			break;
		}
		else
		{
			strvec.erase(strvec.begin(), strvec.end());
		}
	}
	data.map1PosX = std::stoi(strvec[1]);
	data.map1PosZ = std::stoi(strvec[2]);
	data.map2PosX = std::stoi(strvec[3]);
	data.map2PosZ = std::stoi(strvec[4]);
	data.map3PosX = std::stoi(strvec[5]);
	data.map3PosZ = std::stoi(strvec[6]);
	data.map4PosX = std::stoi(strvec[7]);
	data.map4PosZ = std::stoi(strvec[8]);
	data.map5PosX = std::stoi(strvec[9]);
	data.map5PosZ = std::stoi(strvec[10]);
}

//ボスAIの上方を読み込む
void LoadCsv::LoadBossAIData(BossAI::AI& data, const char* status)
{
	std::ifstream ifs(kBossAIFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		// 参照したいキャラが見つかっていたら処理をやめる
		// MEMO:strcmp 文字列を比較する 第1引数 = 第2引数の場合0
		if (strcmp(str, status) == 0)
		{
			break;
		}
		else
		{
			strvec.erase(strvec.begin(), strvec.end());
		}
	}
	data.rushAttackProbability = std::stoi(strvec[1]);
	data.lightAttackProbability = std::stoi(strvec[2]);
	data.storongAttackProbability = std::stoi(strvec[3]);
	data.throwingAttackProbability = std::stoi(strvec[4]);
	data.jumpProbability = std::stoi(strvec[5]);
	data.statusUpProbability = std::stoi(strvec[6]);

}


