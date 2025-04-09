#pragma once
#include "SceneStageBase.h"

class Boss;
class Stage;

class SceneSecondStage : public SceneStageBase
{
public:
	SceneSecondStage();

	virtual ~SceneSecondStage();

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

	std::shared_ptr<Boss> m_pBoss;

	std::shared_ptr<Stage>m_pStage;

	//ボスのモデルハンドル
	int m_bossHandle;

};

