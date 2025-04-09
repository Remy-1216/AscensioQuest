#pragma once
#include <memory>
#include "DxLib.h"

class SoundManager;
class Pad;

class SceneBase : public std::enable_shared_from_this<SceneBase>
{
public:
	//ステージの種類
	enum Stagekinds
	{
		Title,
		Tutorial,
		Select,
		Stage1,
		Stage2,
	};
public:
	SceneBase();
	virtual ~SceneBase();

	virtual void Init() = 0;	//シーンに入るときの初期化処理

	virtual std::shared_ptr<SceneBase>Update(const Pad& pad) = 0;	//シーンを変更する場合は遷移先のシーンのポインタ
	//シーン変更しない場合は自身のポインタ

	virtual void Draw() = 0;	//毎フレーム行う描画処理
	virtual void End() = 0;		//シーンを抜けるときの処理

protected:

	std::shared_ptr<SoundManager> m_pSoundManager;

	//画像のハンドル
	int m_bgHandle;

	/// <summary>
	/// 動画を再生した回数
	/// </summary>
	int m_playVideo;

	/// <summary>
	/// シャドウマップのハンドル
	/// </summary>
	int m_shadowMapHandle;

	//フェードイン、アウト
	int m_fadeAlpha;

	/// <summary>
	/// メニュー画面を開くかどうか
	/// </summary>
	bool m_isOpenMenu;

	/// <summary>
	/// タイトル画面にもどるか
	/// </summary>
	bool m_isBackToTheTitle;

	//プレイヤーの座標
	VECTOR m_playerPos;

	/// <summary>
	/// カメラの座標
	/// </summary>
	VECTOR m_cameraPos;
	VECTOR m_cameraTarget;
};

