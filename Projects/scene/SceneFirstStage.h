#pragma once
#include "SceneStageBase.h"

class EnemyManager;
class Stage;

class SceneFirstStage :public SceneStageBase
{
public:
	SceneFirstStage();

	virtual ~SceneFirstStage();

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

	std::shared_ptr<EnemyManager> m_pEnemyManager;

	std::shared_ptr<Stage>m_pStage;


	//UI背景のハンドル
	int m_stageUIBGHandle;
};

