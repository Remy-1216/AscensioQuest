#pragma once
#include "CharacterBase.h"
#include "EnemyManager.h"
#include "EffectManager.h"
#include "Player.h"
#include "BossAI.h"
#include "PlayerStatus.h"
#include "Stage.h"
#include "Font.h"
#include <string>
#include <map>	
class LoadCsv
{
public:
	// コピーコンストラクタの禁止
	LoadCsv(const LoadCsv&) = delete;
	// ムーブコンストラクタの禁止
	LoadCsv(LoadCsv&&) = delete;
	// 代入演算子の禁止
	LoadCsv& operator= (const LoadCsv&) = delete;
	LoadCsv& operator= (LoadCsv&&) = delete;

	// インスタンスを取得する
	static LoadCsv& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new LoadCsv;
		}
		return *m_instance;
	}

	/// <summary>
	/// キャラクターのステータス情報を読み込む
	/// </summary>
	/// <param name="data">キャラクターのステータス</param>
	/// <param name="charaName">キャラクターの名前</param>
	void LoadStatus(CharacterBase::Status& data, const char* charaName);

	/// <summary>
	/// キャラクターの当たり判定の時に使うカプセルの始点と終点の座標位置を読み込む
	/// </summary>
	/// <param name="collisionInfo">当たり判定</param>
	/// <param name="charaName">キャラクターの名前</param>
	void LoadCollisionInfo(CharacterBase::CollisionInfo& collisionInfo, const char* charaName);

	/// <summary>
	///  プレイヤーのアニメーション情報を読み込む
	/// </summary>
	/// <param name="data">アニメーションに関しての情報</param>
	void LoadPlayerAnimData(std::map < std::string, CharacterBase::AnimInfo>& data);

	/// <summary>
	/// キャラクターの座標情報を読み込む
	/// </summary>
	/// <param name="playerPos">キャラクターの座標情報</param>
	void LoadPlayerPosData(CharacterBase::CharacterPos & characterPos,const char* charaName);

	/// <summary>
	///　近距離エネミーのアニメーション情報を読み込む
	/// </summary>
	/// <param name="data"></param>
	void LoadShortDistanceEnemyAnimData(std::map < std::string, CharacterBase::AnimInfo>& data);

	/// <summary>
	/// 遠距離エネミーのアニメーション情報を読み込む
	/// </summary>
	/// <param name="data"></param>
	void LoadLongDistanceEnemyAnimData(std::map < std::string, CharacterBase::AnimInfo>& data);

	/// <summary>
	/// ボスのアニメーション情報を読み込む
	/// </summary>
	/// <param name="data"></param>
	void LoadBossAnimData(std::map < std::string, CharacterBase::AnimInfo>& data);

	/// <summary>
	/// エネミーの種類によって湧く位置を読み込む
	/// </summary>
	/// <param name="enemyPos">エネミーの座標位置</param>
	/// <param name="enemyname">エネミーの名前</param>
	void LoadEnemyPos(EnemyManager::Pos& enemyPos, const char* enemyname);

	/// <summary>
	/// ステージの情報を読み込む
	/// </summary>
	/// <param name="data">ステージに関しての情報</param>
	void LoadStageInfo(Stage::StagePos& data, const char* charaName);

	/// <summary>
	/// ワープできる場所とワープ先の場所を読み込む
	/// </summary>
	/// <param name="data">ワープ関係の座標位置</param>
	/// <param name="charName">ワープの番号</param>
	void LoadWarpPointPos(Stage::WarpPointPos& data, const char* warpName);

	/// <summary>
	/// ステータスUPできる数値を読み込む
	/// </summary>
	/// <param name="data"></param>
	void LoadStatusUp(PlayerStatus::StatusUpValue& data);

	/// <summary>
	/// エフェクトのデータを読み込む
	/// </summary>
	/// <param name="data"></param>
	void LoadEffectData(EffectManager::EffectData& data, const char* effectName);

	/// <summary>
	/// フォントのデータを読み込む
	/// </summary>
	/// <param name="data">フォントデータ</param>
	/// <param name="fontDate">読み込むデータの名前</param>
	void LoadFontData(Font::FontId& data,const char* fontDate);

	/// <summary>
	/// マップの左上,右下の座標位置を読み込む
	/// </summary>
	/// <param name="data">マップの座標位置</param>
	/// <param name="map">方向</param>
	void LoadMapPosData(Stage::MapPos& data, const char* direction);

	/// <summary>
	/// マップの左上,右下の座標位置を読み込む
	/// </summary>
	/// <param name="data">マップの座標位置</param>
	/// <param name="map">方向</param>
	void LoadMapPosData(EnemyManager::AreaPos& data, const char* direction);

	/// <summary>
	/// ボスの状態によって変わる、行動確率を読み込む
	/// </summary>
	/// <param name="data">行動確率データ</param>
	/// <param name="status">どのような状態なのか</param>
	void LoadBossAIData(BossAI::AI& data, const char* status);

private:
	LoadCsv() = default;
	virtual ~LoadCsv() = default;

private:
	static LoadCsv* m_instance;				   // メッセージのインスタンス
	std::map<std::string, std::string> m_data; // メッセージのデータ
};

