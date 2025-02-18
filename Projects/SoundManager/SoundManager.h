#pragma once
class SoundManager
{
public:

	SoundManager();

	virtual ~SoundManager();


	void Init();

	/// <summary>
	/// タイトル画面でBGMを流す
	/// </summary>
	void TitleBGM();

	/// <summary>
	/// セレクト画面でBGMを流す
	/// </summary>
	void SelectBGM();

	/// <summary>
	/// ステータス画面でBGMを流す
	/// </summary>
	void StatusBGM();

	/// <summary>
	/// ゲーム画面でBGMを流す
	/// </summary>
	void GamePlayBGM();

	/// <summary>
	/// クリア画面でBGMを流す
	/// </summary>
	void ClearBGM();

	/// <summary>
	/// ゲームオーバー画面でBGMを流す
	/// </summary>
	void GameOverBGM();

	/// <summary>
	/// カーソル移動SEを流す
	/// </summary>
	void MoveCursorSE();

	/// <summary>
	/// 決定SEを流す
	/// </summary>
	void DeterminationSE();

	/// <summary>
	/// 失敗SEを流す
	/// </summary>
	void FailureSE();

	/// <summary>
	/// 剣を振った時のSEを流す
	/// </summary>
	void RollSwordSE();

	/// <summary>
	/// 剣で斬った時のSEを流す
	/// </summary>
	void SwordSE();

	/// <summary>
	/// 魔法攻撃を行った時のSEを流す
	/// </summary>
	void MagicAttackSE();

	/// <summary>
	/// ガードが成功した時のSEを流す
	/// </summary>
	void GuardSE();

	/// <summary>
	/// 必殺技を発動した時のSEを流す
	/// </summary>
	void SpecialMoveSE();

	/// <summary>
	/// 近距離の攻撃を受けた時のSEを流す
	/// </summary>
	void HitShortDistanceAttackSE();

	/// <summary>
	/// 遠距離の攻撃を受けた時のSEを流す
	/// </summary>
	void HitLongDistanceAttackSE();

	/// <summary>
	/// 能力を上げた時のSEを流す
	/// </summary>
	void StatusUpSE();

	/// <summary>
	/// ワープした時のSEを流す
	/// </summary>
	void WarpSE();

private:

	//タイトル画面BGM　
	int m_titleBGM;

	//セレクト画面BGM
	int m_selectBGM;

	//ステータス画面BGM
	int m_statusBGM;

	//ゲーム中画面BGM
	int m_gamePlayBGM;

	//クリア画面BGM
	int m_clearBGM;

	//ゲームオーバー画面BGM
	int m_gameOverBGM;

	//カーソル移動時のSE
	int m_moveCursorSE;

	//決定音SE
	int m_determinationSE;

	//失敗時のSE
	int m_failureSE;

	//剣を振った時のSE
	int m_rollSwordSE;

	//剣で切った時のSE
	int m_swordSE;

	//魔法攻撃した時のSE
	int m_magicAttackSE;

	//ガード成功時のSE
	int m_guardSE;

	//必殺技を発動した時のSE
	int m_specialMoveSE;

	//近距離の攻撃を受けた時のSE
	int m_hitShortDistanceAttackSE;

	//遠距離の攻撃を受けた時のSE
	int m_hitLongDistanceAttackSE;

	//能力を上げた時のSE
	int m_statusUpSE;

	//ワープ音
	int m_warpSE;


};

