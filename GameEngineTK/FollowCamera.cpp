#include "FollowCamera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//静的メンバ変数の初期化
const float FollowCamera::CAMERA_DISTANCE = 5.0f;

FollowCamera::FollowCamera(int width, int height)
	:Camera(width,height)
{
	m_targetPos = Vector3(0.0f, 0.0f, 0.0f);
	m_targetAngle = 0.0f;
}

void FollowCamera::Update()
{
	//カメラ視点座標、参照点座標
	Vector3 eyepos, refpos;

	//ターゲットの座標は自機の座標に追従
	refpos = m_targetPos + Vector3(0.0f, 2.0f, 0.0f);
	//ターゲット座標からカメラ座標への差分
	Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);

	//自機の後ろに回り込むための回転行列
	Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
	//カメラへのベクトルを開店
	cameraV = Vector3::TransformNormal(cameraV, rotmat);

	//カメラ座標を計算
	eyepos = refpos + cameraV;

	//情報をセット
	this->SetEyePos(eyepos);
	this->SetRefPos(refpos);

	//CameraのUpdateを呼び出す(基底クラスの更新）
	Camera::Update();

}

void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3 targetPos)
{
	m_targetPos = targetPos;
}

void FollowCamera::SetTargetAngle(const float targetAngle)
{
	m_targetAngle = targetAngle;
}
