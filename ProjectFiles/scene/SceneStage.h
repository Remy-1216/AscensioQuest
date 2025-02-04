#pragma once
#include <vector>
#include <memory>
#include "SceneBase.h"
#include "DxLib.h"

class Player;
class Camera;
class Boss;
class EnemyManager;
class Stage;
class PlayerStatus;
class UIBar;

class SceneStage :  public SceneBase
{
public:
	SceneStage(const int StageNumber);
	virtual ~SceneStage();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pad"></param>
	/// <returns></returns>
	virtual std::shared_ptr<SceneBase> Update(const Pad& pad);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();


	virtual void End();

private:

	/// <summary>
	/// ステージ1の動き
	/// </summary>
	/// <param name="pad"></param>
	void Stage1Update(const Pad& pad);


	/// <summary>
	/// ステージ2の動き
	/// </summary>
	void Stage2Update(const Pad& pad);

	/// <summary>
	/// プレイヤーの位置によって操作説明を変更する
	/// </summary>
	void OperationUI();

	/// <summary>
	/// フェードイン、フェードアウト
	/// </summary>
	void Fade();

private:

	//UI関係
	int m_operationHandle;				//通常時の操作方法のハンドル
	int m_specialMoveOperationHandle;	//必殺技ができる状態の操作方法のハンドル
	int m_warpPointOperationHandle;		//ワープポイントにいる時に表示する操作方法のハンドル
	int m_allOperatioHandle;			//ワープ地点にいる上で必殺技も撃てる状態の操作方法のハンドル


	std::shared_ptr<Player>m_pPlayer;

	std::shared_ptr<Camera>m_pCamera;

	std::shared_ptr<Stage>m_pStage;

	std::shared_ptr<EnemyManager>m_pEnemyManager;

	std::shared_ptr<Boss> m_pBoss;

	std::shared_ptr<PlayerStatus>m_pPlayerStatus;

	std::shared_ptr<UIBar>m_pUIBar;

	// シャドウマップのハンドル
	int m_shadowMapHandle;

	//ステージの種類
	int m_stageKinds;

	//ボスのモデルハンドル
	int m_bossHandle;

	//敵の人数
	int m_enemyNum;

	//プレイヤーが死んだかどうか
	bool m_isPlayerDie;

	//ワープポイントにいるかどうか
	bool m_isWarpPoint;

	//必殺技ゲージが100を超えているかどうか
	bool m_isSpecialMoveAvailable;

	//ゲームをクリアしたかどうか
	bool m_isGameClear;

	//カメラアングルを保持する
	VECTOR m_cameraAngle;
};

