#pragma once
#include "SceneBase.h"
#include <memory>
class PlayerStatus;
class SoundManager;
class SceneStatus : public SceneBase
{

public:
	/// <summary>
	/// ステータスUP構造体
	/// </summary>
	struct  StatusUp
	{
		int StatusPoint;			//ステータスポイント
		int hpUp;					//HPUP数値
		int attackPowerUp;			//攻撃力UP数値
		int magicAttackPowerUp;		//魔法攻撃力UP数値
		int defensePowerUp;			//防御力UP数値
	};
public:
	SceneStatus();

	virtual ~SceneStatus();

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
	/// フェードイン、フェードアウト
	/// </summary>
	void Fade();

	/// <summary>
	/// カーソルの動き
	/// </summary>
	/// <param name="pad"></param>
	void CursorMotion(const Pad& pad);

	/// <summary>
	/// 強化されたゲージを描画する
	/// </summary>
	void ReleasedGaugeDraw();

	/// <summary>
	/// カーソルの位置によって背景を変更
	/// </summary>
	void BackgroundChange();

	/// <summary>
	/// スキルポイントがいくつあり、何ポイントでステータスを上げられるのかを描画する
	/// </summary>
	void DrawPoint();

	/// <summary>
	/// カスタムボーナスの説明画面を描画する
	/// </summary>
	void DrawCustomBonus();


	/// <summary>
	/// カーソル位置での行うこと
	/// </summary>
	std::shared_ptr<SceneBase>  UpdateCursorPos(const Pad& pad);


private:

	std::shared_ptr<PlayerStatus> m_pPlayerStatus;

	/// <summary>
	/// カーソルの位置が能力値UPの場所にある場合に表示する画像
	/// </summary>
	int m_statusUpBgHandle;

	/// <summary>
	/// カーソルのハンドル
	/// </summary>
	int m_cursorHandle;

	/// <summary>
	/// 解放済みのゲージのハンドル
	/// </summary>
	int m_releasedGaugeHandle1;
	int m_releasedGaugeHandle2;
	int m_releasedGaugeHandle3;
	int m_releasedGaugeHandle4;
	int m_releasedGaugeHandle5;
	int m_releasedGaugeHandle6;
	int m_releasedGaugeHandle7;
	int m_releasedGaugeHandle8;
	int m_releasedGaugeHandle9;
	int m_releasedGaugeHandle10;

	/// <summary>
	/// カスタムボーナスの説明ハンドル
	/// </summary>
	int m_customBonusHandle;

	/// <summary>
	/// フルカスタムボーナスの説明ハンドル
	/// </summary>
	int m_fullCustomBonusHandle;

	/// <summary>
	/// セレクト画面に戻るかどうか
	/// </summary>
	bool m_isSelect;

	/// <summary>
	/// カーソルの位置
	/// </summary>
	VECTOR m_cursorPos;
};

