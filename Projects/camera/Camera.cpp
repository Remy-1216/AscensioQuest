#include "Camera.h"
#include "Pad.h"
#include <cmath>
#include "EffekseerForDXLib.h"

namespace
{
	//カメラの描画範囲
	constexpr float kMinimumDrawingrange = 10.0f;
	constexpr float kMaximumDrawingrange =8000.0f;

	//カメラの座標
	constexpr float kCameraPosY = 200.0f;
	constexpr float kCameraPosZ = -400.0f;

	// 旋回速度
	constexpr float kAngleSpeed = 0.05f;	

	//注視点の座標
	constexpr float kCameraTargetY = 100.0f;

	//カメラの距離
	constexpr float kCameraDist = 1000.0f;

	// 視野角
	constexpr float kFov = (DX_PI_F / 180.0f) * 60.0f;
	// 手前クリップ距離
	constexpr float kNear = 0.1f;
	// 奥クリップ距離
	constexpr float kFar = 1000.0f;
	// カメラの初期注視点
	constexpr float kTargetX = 00.0f;        // X軸
	constexpr float kTargetY = 100.0f;    // Y軸
	constexpr float kTargetZ = 0.0f;    // Z軸
	// カメラの中心座標
	constexpr float kScreenCenterX = 50.0f;    // X座標
	constexpr float kScreenCenterY = 1000.0f;    // Y座標

	constexpr float AngleSpeed = 0.05f;	// 旋回速度
}


Camera::Camera():m_cameraAngle(0.0f)
{
		

}


Camera::~Camera()
{
	// 処理なし.
}

void Camera::Init()
{
	//奥行100～5000までをカメラの描画範囲とする
	SetCameraNearFar(kMinimumDrawingrange, kMaximumDrawingrange);


	//初期化
	m_cameraPos = VGet(0.0f, 0.0f, 0.0f);
	
	m_cameraTarget = VGet(0.0f, 0.0f, 0.0f);

	m_cameraAngle = 0.0f;

	m_pad = 0;
}


void Camera::Update(VECTOR mplayerPos)
{
	m_pad = GetJoypadDirectInputState(DX_INPUT_PAD1,&m_input);
		
	//右にスティックを傾けている
	if (m_input.Rx > 0)
	{
		m_cameraAngle += AngleSpeed;
	}
	//右にスティックを傾けている
		//左にスティックを傾けている
	if (m_input.Rx < 0)
	{
		m_cameraAngle -= kAngleSpeed;
	}
	
	// カメラ座標
	m_cameraPos = VAdd(mplayerPos, VTransform({ 0.0f,  kCameraPosY,kCameraPosZ } ,MGetRotY(m_cameraAngle)));

	// 注視点
	m_cameraTarget = VAdd(mplayerPos, VGet(0.0f, kCameraTargetY, 0.0f));

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);
}

void Camera::Draw()
{

#ifdef _DEBUG
	DrawFormatString(160, 200, GetColor(0,0,0), "カメラの座標(%.2f,%.2f,%.2f)", m_cameraPos.x, m_cameraPos.y, m_cameraPos.z);

	DrawFormatString(250, 250, GetColor(0,0,0), "%d", m_input.Rx);
#endif
}
