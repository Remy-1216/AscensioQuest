#include "Rect.h"
#include "DxLib.h"
#include <cassert>

Rect::Rect() :
	m_x(0.0f),
	m_y(0.0f),
	m_z(0.0f),
	m_xWidth(0.0f),
	m_yHeight(0.0f),
	m_zDepth(0.0f)
{
	//左上
	pos1 = VGet(0.0f, 0.0f, 0.0f);
	//左下
	pos2 = VGet(0.0f, 0.0f, 0.0f);
	//右上
	pos3 = VGet(0.0f, 0.0f, 0.0f);
	//右下
	pos4 = VGet(0.0f, 0.0f, 0.0f);
	//後ろの左上
	pos5 = VGet(0.0f, 0.0f, 0.0f);
	//後ろの左下
	pos6 = VGet(0.0f, 0.0f, 0.0f);
	//後ろの右上
	pos7 = VGet(0.0f, 0.0f, 0.0f);
	//後ろの右下
	pos8 = VGet(0.0f, 0.0f, 0.0f);

}

Rect::~Rect()
{
}

void Rect::Draw(unsigned int Color, bool isFill)
{
	//左上から右上
	DrawLine3D(pos1, pos3, Color);
	//左上から左下
	DrawLine3D(pos1, pos2, Color);
	//左上から後ろの左上
	DrawLine3D(pos1, pos5, Color);
	//右上から右した
	DrawLine3D(pos3, pos4, Color);
	//右上から後ろの右上
	DrawLine3D(pos3, pos7, Color);
	//左下から後ろの左下
	DrawLine3D(pos2, pos6, Color);
	//左下から右下
	DrawLine3D(pos2, pos4, Color);
	//右下から後ろの右下
	DrawLine3D(pos4, pos8, Color);
	//後ろの左上から後ろの右上
	DrawLine3D(pos5, pos7, Color);
	//後ろの左上から後ろの左下
	DrawLine3D(pos5, pos6, Color);
	//後ろの右下から後ろの右上
	DrawLine3D(pos8, pos7, Color);
	//後ろの右下から後ろの左下
	DrawLine3D(pos8, pos6, Color);
}

void Rect::SetCenter(float x, float y, float z,float width, float height, float depth)
{
	//ワールド座標からスクリーン座標に変換
	//VECTOR screenPos = ConvWorldPosToScreenPos(VGet(x, y,z));

	//代入する
	m_x = x;
	m_y = y;
	m_z = z;
	m_xWidth = x + width;
	m_yHeight = y + height;
	m_zDepth = z + depth;

	//左上座標
	pos1 = VGet(m_x, m_y, m_z);
	//左下座標
	pos2 = VGet(m_x, m_yHeight, m_z);
	//右上座標
	pos3 = VGet(m_xWidth, m_y, m_z);
	//右下座標
	pos4 = VGet(m_xWidth, m_yHeight, m_z);
	//後ろの左上座標
	pos5 = VGet(m_x, m_y, m_zDepth);
	//後ろの左下座標
	pos6 = VGet(m_x, m_yHeight, m_zDepth);
	//後ろの右上座標
	pos7 = VGet(m_xWidth, m_y, m_zDepth);
	//後ろの右下座標
	pos8 = VGet(m_xWidth, m_yHeight, m_zDepth);
}

bool Rect::IsCollsion(const Rect& rect)
{
	//絶対当たらないパターン以外は当たっている
	if (m_x > rect.m_xWidth) return false;
	if (m_y > rect.m_yHeight) return false;
	if (m_xWidth < rect.m_x) return false;
	if (m_yHeight < rect.m_y) return false;
	if (m_z > rect.m_zDepth) return false;
	if (m_zDepth < rect.m_z) return false;

	//当たらないパターン以外は当たっている
	return true;
}