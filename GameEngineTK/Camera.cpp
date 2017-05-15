#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//コンストラクタ
//引数width(int),height(int)
Camera::Camera(int width, int height)
{
	//メンバ変数の初期化
	//カメラ視点
	m_eyepos = Vector3(0.0f, 0.0f, 5.0f);
	//カメラ参照点
	m_refpos = Vector3(0.0f, 0.0f, 0.0f);
	//上方向ベクトル
	m_upvec = Vector3(0, 1, 0);

	//m_upvec.Normalize();

	//垂直方向視野角
	m_fovY = XMConvertToRadians(60.0f);
	//アスペクト比(横縦の比率)
	m_aspect = (float)width / height;
	//ニアクリップ(手前の表示限界)
	m_nearclip = 0.1f;
	//ファークリップ(奥の表示限界)
	m_farclip = 1000.0f;

	//ビュー行列
	m_view = Matrix::CreateLookAt(
		m_eyepos,	//カメラ視点
		m_refpos,			//カメラ参照点
		m_upvec);		//上方向ベクトル

	//射影行列生成
	m_proj = Matrix::CreatePerspectiveFieldOfView(
		m_fovY,			//視野角(上下方向)
		m_aspect,	//アスペクト比
		m_nearclip,	//ニアクリップ
		m_farclip);	//ファークリップ

}

//デストラクタ
Camera::~Camera()
{
}

//更新
void Camera::Update()
{
	//ビュー行列
	m_view = Matrix::CreateLookAt(
		m_eyepos,	//カメラ視点
		m_refpos,			//カメラ参照点
		m_upvec);		//上方向ベクトル

	//射影行列生成
	m_proj = Matrix::CreatePerspectiveFieldOfView(
		m_fovY,			//視野角(上下方向)
		m_aspect,	//アスペクト比
		m_nearclip,	//ニアクリップ
		m_farclip);	//ファークリップ

}

const Matrix& Camera::GetView()
{
	//ビュー行列をリターン
	return m_view;
}

const Matrix& Camera::GetProjection()
{
	//射影行列をリターン
	return m_proj;
}

//視点座標のメンバ変数にeyepos代入
void Camera::SetEyePos(const Vector3 eyepos)
{	
	m_eyepos = eyepos;
}

//注視点のメンバ変数にrefpos代入
void Camera::SetRefPos(const Vector3 refpos)
{
	m_refpos = refpos;
}

//カメラの上方向ベクトルのメンバ変数にupvec代入
void Camera::SetUpvec(const Vector3 upvec)
{
	m_upvec = upvec;
}

//垂直方向視野角のメンバ変数にfovY代入
void Camera::SetFovY(const float fovY)
{
	m_fovY = fovY;
}

//アスペクト比のメンバ変数にaspect代入
void Camera::SetAspect(const float aspect)
{
	m_aspect = aspect;
}

//ニアクリップのメンバ変数にnearclip代入
void Camera::SetNearClip(const float nearclip)
{
	m_nearclip = nearclip;
}

//ファークリップのメンバ変数にfarclip代入
void Camera::SetFarClip(const float farclip)
{
	m_farclip = farclip;
}


