﻿#pragma once
#include "DxLib.h"
#include <memory>

class CharacterBase;
class Player;
class Pad;
class EffectManager;

class Stage
{

public:

	//ステージの種類
	enum Stagekinds
	{
		Stage1,
		Stage2,
	};

public:

	/// <summary>
	/// ワープできる位置とワープした先
	/// </summary>
	struct WarpPointPos
	{
		float warpSourceX;		//ワープできる位置のX座標
		float warpSourceY;		//ワープできる位置のY座標
		float warpSourceZ;		//ワープできる位置のZ座標
		float warpTargetX;		//ワープした先の位置X座標
		float warpTargetY;		//ワープした先の位置Y座標
		float warpTargetZ;		//ワープした先の位置Z座標
	};

	/// <summary>
	/// ステージの座標データ
	/// </summary>
	struct StagePos
	{
		float posX;				// X座標
		float posY;				// Y座標
		float posZ;				// Z座標
		float size;				// ステージのサイズ
	};
public:

	Stage(int stageKinds);

	virtual ~Stage();

	void Draw();


	/// <summary>
	/// ワープポイントの情報を読み取る
	/// </summary>
	void LoadWarpPointPos();

	/// <summary>
	/// ワープ関係の処理
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標位置</param>
	void WarpPoint(Player&player, const Pad& pad);

	/// <summary>
	/// オブジェクトと当たり判定を行う
	/// </summary>
	/// <param name="obj">オブジェクト参照</param>
	/// <param name="moveVec">移動ベクトル</param>
	/// <returns>補正した移動先のポジション</returns>
	VECTOR CheckObjectCol(CharacterBase& character, const VECTOR& moveVec);

	/// <summary>
	/// ステージモデルを取得
	/// </summary>
	/// <returns>3Dモデルのハンドル</returns>
	int GetStageHandle() const { return m_stageHandle; }

private:
	/// <summary>
	/// 検出されたポリゴンが壁ポリゴンか床ポリゴンかを判断する
	/// </summary>
	/// <param name="hitDim">ヒットしたコリジョン情報</param>
	/// <param name="checkPosition">移動後の座標</param>
	void AnalyzeWallAndFloor(MV1_COLL_RESULT_POLY_DIM hitDim, const VECTOR& checkPosition);

	/// <summary>
	/// オブジェクトと壁ポリゴンとの当たりをチェックする
	/// </summary>
	/// <param name="obj">オブジェクト参照</param>
	/// <param name="checkPosition">移動後の座標</param>
	/// <returns>補正すべきベクトル</returns>
	VECTOR CheckHitPlayerWithWall( const VECTOR& checkPosition);

	/// <summary>
	/// オブジェクトと床ポリゴンとの当たりをチェックする
	/// </summary>
	/// <param name="obj">オブジェクト参照</param>
	/// <param name="checkPosition">移動後の座標</param>
	/// <returns>補正すべきベクトル</returns>
	VECTOR CheckHitPlayerWithFloor(const VECTOR& checkPosition);

private:

	static constexpr int kWarpPointNum = 8;

private:

	std::shared_ptr<EffectManager> m_pEffectManager;

	//ステージのハンドル
	int m_stageHandle;

	//背景のハンドル
	int m_bgHandle;

	//ステージの種類
	int m_stageKinds;

	//ポリゴン関係
	int m_wallNum;			// 壁ポリゴンの数
	int	m_floorNum;			// 床ポリゴンの数
	static const int MaxHitColl = 2048;			// 処理するコリジョンポリゴンの最大数
	MV1_COLL_RESULT_POLY* m_wall[MaxHitColl];	// 壁ポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	MV1_COLL_RESULT_POLY* m_floor[MaxHitColl];	// 床ポリゴンの構造体のアドレスを保存しておくためのポインタ配列

	//2つのステージに共通する変数
	StagePos m_stage;					//ステージの情報
	float m_posX;					//X座標
	float m_posY;					//Y座標
	float m_posZ;					//Z座標
	float m_size;					//ステージのサイズ
	VECTOR m_stagePos;				//ステージの座標位置


	//プレイヤーの移動前の座標
	VECTOR m_oldPos;

	//プレイヤーの移動後の座標
	VECTOR m_nextPos;

	//プレイヤーの位置
	VECTOR m_playerPos;


	//ワープ関係の情報を読み込む
	WarpPointPos m_warpPointPos;

	//ワープできる座標
	VECTOR m_warpSource[kWarpPointNum];

	//ワープした先の座標
	VECTOR m_warpTarger[kWarpPointNum];

	//プレイヤーとワープ地点との距離
	VECTOR m_distance;


	////ワープできる座標			合計八か所
	//VECTOR m_warpSource1;
	//VECTOR m_warpSource2;
	//VECTOR m_warpSource3;
	//VECTOR m_warpSource4;
	//VECTOR m_warpSource5;
	//VECTOR m_warpSource6;
	//VECTOR m_warpSource7;
	//VECTOR m_warpSource8;

	////ワープした先の座標		合計八か所
	//VECTOR m_warpTarger1;
	//VECTOR m_warpTarger2;
	//VECTOR m_warpTarger3;
	//VECTOR m_warpTarger4;
	//VECTOR m_warpTarger5;
	//VECTOR m_warpTarger6;
	//VECTOR m_warpTarger7;
	//VECTOR m_warpTarger8;
};

