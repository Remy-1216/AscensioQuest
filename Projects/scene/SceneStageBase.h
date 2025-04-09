#pragma once
#include <vector>
#include <memory>
#include "SceneBase.h"
#include "DxLib.h"

class Player;
class Camera;
class UIBar;
class PlayerStatus;

class SceneStageBase :public SceneBase
{
public:
	SceneStageBase();

	virtual ~SceneStageBase();

	virtual void Init() = 0;	//シーンに入るときの初期化処理

	virtual std::shared_ptr<SceneBase>Update(const Pad& pad) = 0;	//シーンを変更する場合は遷移先のシーンのポインタ
	//シーン変更しない場合は自身のポインタ

	virtual void Draw() = 0;	//毎フレーム行う描画処理
	virtual void End() = 0;		//シーンを抜けるときの処理

	/// <summary>
	/// タイトル画面に戻る
	/// </summary>
	void BackToTitle(const Pad& pad);

	/// <summary>
	/// プレイヤーの状態によって操作説明を変更する
	/// </summary>
	void OperationUI();

	/// <summary>
	/// メニュー画面を描画する
	/// </summary>
	void DrawMenu();

	/// <summary>
	/// メニュー画面に文字を描画する
	/// </summary>
	void DrawMenuText();

	/// <summary>
	/// フェード関係
	/// </summary>
	void Fade();

protected:

	std::shared_ptr<Player> m_pPlayer;

	std::shared_ptr<Camera> m_pCamera;

	std::shared_ptr<UIBar>m_pUIBar;

	std::shared_ptr<PlayerStatus>m_pPlayerStatus;

	//UI関係
	int m_operationHandle;				//通常時の操作方法のハンドル
	int m_specialMoveOperationHandle;	//必殺技ができる状態の操作方法のハンドル
	int m_warpPointOperationHandle;		//ワープポイントにいる時に表示する操作方法のハンドル
	int m_allOperatioHandle;			//ワープ地点にいる上で必殺技も撃てる状態の操作方法のハンドル

	//敵の人数
	int m_enemyNum;

	//メニュー画面のハンドル
	int m_menuHandle;

	//カーソル位置
	float m_cursorPosY;

	//ワープポイントにいるかどうか
	bool m_isWarpPoint;

	//必殺技ゲージが100を超えているかどうか
	bool m_isSpecialMoveAvailable;

	//ゲームをクリアしたかどうか
	bool m_isGameClear;

	//ゲームオーバーかどうか
	bool m_isGameOver;

	//カメラアングルを保持する
	VECTOR m_cameraAngle;

};

