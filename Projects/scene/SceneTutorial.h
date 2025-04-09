#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include <vector>
#include <memory>

class Player;
class TutorialEnemy;
class Stage;
class Camera;
class UIBar;
class SceneTutorial : public SceneBase
{
public:

	enum TutorialKinds
	{
		kTurorialStart,
		kFirstTutorial,		//第一チュートリアル
		kSecondTutorial,	//第二チュートリアル
		kThirdTutorial,		//第三チュートリアル
		kFourTutorial,		//第四チュートリアル
		kFifthTutorial,		//第五チュートリアル
		kSixTutorial,		//第六チュートリアル
	};

	//チュートリアル時に必要なプレイヤーの状態
	enum StateKind
	{
		kWait,			//待機状態
		kWalk,			//歩いてる状態
		kRun,			//走っている状態
		kAttack,		//攻撃している状態
		kMagicAttack,	//魔法攻撃をしている状態
		kSpecialMove,	//必殺技を打っている状態
		kGuard,			//ガードをしている状態
	};

public:
	SceneTutorial();
	~SceneTutorial();

	virtual void Init();
	
	virtual std::shared_ptr<SceneBase> Update(const Pad& pad);

	void Draw() override;
	
	virtual void End();

private:
	/// <summary>
	/// 全体のチュートリアル処理
	/// </summary>
	/// <param name="pad"></param>
	void TutorialUpdate();

	/// <summary>
	/// 第一チュートリアルの処理
	/// </summary>
	/// <param name="pad"></param>
	void FirstTutorial();

	/// <summary>
	/// 第二チュートリアルの処理
	/// </summary>
	void SecondTutorial();

	/// <summary>
	/// 第三チュートリアルの処理
	/// </summary>
	void ThirdTutorial();

	/// <summary>
	/// 第四チュートリアルの処理
	/// </summary>
	void FourTutorial();

	/// <summary>
	/// 操作説明中の処理
	/// </summary>
	void ManualUpdate(const Pad  & pad);

	/// <summary>
	/// 操作説明中に動画や画像を描画する
	/// </summary>
	void DrawManual();

	/// <summary>
	/// 動画を繰り返し流す
	/// </summary>
	void RepeatVideo();

	/// <summary>
	///	操作説明中テキストを表示する処理
	/// </summary>
	/// <param name="pad"></param>
	void DrawManualText();

	/// <summary>
	/// チュートリアル中に描画する画像
	/// </summary>
	void DrawTutorial();

	/// <summary>
	/// チュートリアル中テキストを表示する処理
	/// </summary>
	void DrawTutorialText();

	/// <summary>
	/// フェード関係
	/// </summary>
	void Fade();

private:

	std::shared_ptr<Player> m_pPlayer;

	std::shared_ptr<TutorialEnemy> m_pTutorialEnemy;

	std::shared_ptr<Stage> m_pStage;

	std::shared_ptr<Camera> m_pCamera;

	std::shared_ptr<UIBar> m_pUIBar;


	/// <summary>
	/// 敵モデルのハンドル
	/// </summary>
	int m_tutorialEnemyHandle;

	/// <summary>
	/// 文字を表示する背景のハンドル
	/// </summary>
	int m_textBackHandle;

	/// <summary>
	/// 操作説明中に表示するUIのハンドル
	/// </summary>
	int m_manualUIHandle;

	/// <summary>
	/// 操作説明1についての画像ハンドル
	/// </summary>
	int m_firstManualHandle;

	/// <summary>
	/// 操作説明2についての画像ハンドル
	/// </summary>
	int m_secondManualHandle;

	/// <summary>
	/// 操作説明3についての画像ハンドル
	/// </summary>
	int m_thirdManualHandle;

	/// <summary>
	/// 操作説明4の画像ハンドル
	/// </summary>
	int m_fourManualHandle;

	/// <summary>
	/// 操作説明5の画像ハンドル
	/// </summary>
	int m_fifthManualHandle;

	/// <summary>
	/// 操作説明6の画像ハンドル
	/// </summary>
	int m_sixManualHandle;

	/// <summary>
	/// チュートリアル1の画像ハンドル
	/// </summary>
	int m_firstTutorialHandle;

	/// <summary>
	/// チュートリアル2の画像ハンドル
	/// </summary>
	int m_secondTutorialHandle;

	/// <summary>
	/// チュートリアル3の画像ハンドル
	/// </summary>
	int m_thirdTutorialHandle;

	/// <summary>
	/// チュートリアル4の画像ハンドル
	/// </summary>
	int m_fourTutorialHandle;

	/// <summary>
	/// チュートリアル5の画像ハンドル
	/// </summary>
	int m_fifthTutorialHandle;

	/// <summary>
	/// EXゲージをわかりやすくした画像ハンドル
	/// </summary>
	int m_exGaugeHandle;

	/// <summary>
	/// ステータス画面の画像ハンドル
	/// </summary>
	int m_statusScreenHandle;

	/// <summary>
	/// プレイヤーが攻撃を行っているときの動画ハンドル
	/// </summary>
	int m_attackVideoHandle;

	/// <summary>
	/// プレイヤーが魔法攻撃を行っているときの動画ハンドル
	/// </summary>
	int m_magicAttackVideoideoHandle;

	/// <summary>
	/// MPが消費されている時の動画ハンドル
	/// </summary>
	int m_consumeMPVideoHandle;

	/// <summary>
	/// MPが回復している時の動画ハンドル
	/// </summary>
	int m_mpRecoveryVideoHandle;

	/// <summary>
	/// EXゲージが上昇している動画ハンドル
	/// </summary>
	int m_exGaugeRiseVideoHandle;

	/// <summary>
	/// 必殺技をしているときの動画ハンドル
	/// </summary>
	int m_specialMoveVideoHandle;

	/// <summary>
	/// ガードを行っているときの動画ハンドル
	/// </summary>
	int m_guardVideoHandle;

	/// <summary>
	/// ガードが成功した時の動画ハンドル
	/// </summary>
	int m_guardSuccessVideoHandle;

	/// <summary>
	/// ワープエフェクトの動画ハンドル
	/// </summary>
	int m_warpEffectHandle;

	/// <summary>
	/// ワープ移動の動画ハンドル
	/// </summary>
	int m_warpMovementHandle;

	/// <summary>
	/// ステータスを上げた時の動画ハンドル
	/// </summary>
	int m_statusUpVideoHandle;

	/// <summary>
	/// 現在のチュートリアルの種類
	/// </summary>
	int m_tutorialKinds;

	/// <summary>
	/// 歩いている時間
	/// </summary>
	int m_walkTimes;

	/// <summary>
	/// 走っている時間
	/// </summary>
	int m_runTimes;

	/// <summary>
	/// 通常攻撃を行った回数
	/// </summary>
	int m_attackCount;

	/// <summary>
	/// 魔法攻撃を行った回数
	/// </summary>
	int m_magicAttackCount;

	/// <summary>
	/// 必殺技を行った回数
	/// </summary>
	int m_specialMoveCount;

	/// <summary>
	/// ガードが成功した回数
	/// </summary>
	int m_guardCount;

	/// <summary>
	/// 操作説明の説明回数
	/// </summary>Play video
	int m_manualCount;

	/// <summary>
	/// スタートボタン押した回数
	/// </summary>
	int m_startPush;

	/// <summary>
	/// 第一のチュートリアルかどうか
	/// </summary>
	bool m_isFirstTutorial;

	/// <summary>
	/// 第二のチュートリアルかどうか
	/// </summary>
	bool m_isSecondTutorial;

	/// <summary>
	/// 第三のチュートリアルかどうか
	/// </summary>
	bool m_isThirdTutorial;

	/// <summary>
	/// 第四のチュートリアルかどうか
	/// </summary>
	bool m_isFourtutorial;

	/// <summary>
	/// 第五のチュートリアルかどうか
	/// </summary>
	bool m_isFifthManual;

	/// <summary>
	/// 第六のチュートリアルかどうか
	/// </summary>
	bool m_isSixTutorial;

	/// <summary>
	/// 操作説明中かどうか
	/// </summary>
	bool m_isManual;

	/// <summary>
	/// チュートリアルクリアしたかどうか
	/// </summary>
	bool m_isTutorialClear;

	/// <summary>
	/// すべてのチュートリアルクリアしたかどうか
	/// </summary>
	bool m_isClear;
};

	