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
	m_keyboard = nullptr;
	m_Player = nullptr;
	m_fpsFlag = false;
}

void FollowCamera::Update()
{
	//カメラ視点座標、参照点座標
	Vector3 eyepos, refpos;

	//キーボードの状態を取得
	Keyboard::State keystate = m_keyboard->GetState();
	m_keyboardTracker.Update(keystate);

	//カメラのフラグ
	//static bool m_fpsFlag = true;

	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::C))
	{
		//TPS,FPS切替
		//フラグ切り替え
		m_fpsFlag = !m_fpsFlag;
	}

	//プレイヤーの座標、角度をセット
	if (m_Player)
	{
		this->SetTargetPos(m_Player->GetTrans());
		this->SetTargetAngle(m_Player->GetRot().y);
	}


	//FPSのカメラ処理
	if(m_fpsFlag == true)
	{

		Vector3 position;
		//ターゲットの座標は自機の座標に追従
		position = m_targetPos + Vector3(0.0f, 0.2f, 0.0f);
		//ターゲット座標からカメラ座標への差分
		Vector3 cameraV(0.0f, 0.0f, -CAMERA_DISTANCE);

		//自機の後ろに回り込むための回転行列
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
		//カメラへのベクトルを開店
		cameraV = Vector3::TransformNormal(cameraV, rotmat);

		//カメラ座標を計算
		//ちょっと進んだ位置が視線座標
		refpos = position + cameraV * 1.1f;
		//がっつり進んだ位置が参照点座標
		eyepos = position + cameraV;

	}
	else
	//TPSのカメラ処理
	{
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

		//視点を現在位置から補間する処理
		eyepos = m_eyepos + (eyepos - m_eyepos) * 0.05;
		//参照点を現在位置から補間する処理
		refpos = m_refpos + (refpos - m_refpos) * 0.05;

	}



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

void FollowCamera::SetKeyBoard(DirectX::Keyboard * keyboard)
{
	m_keyboard = keyboard;
}

void FollowCamera::SetPlayer(Player * player)
{
	m_Player = player;
}
