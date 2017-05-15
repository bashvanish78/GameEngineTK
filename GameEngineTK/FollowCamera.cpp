#include "FollowCamera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//�ÓI�����o�ϐ��̏�����
const float FollowCamera::CAMERA_DISTANCE = 5.0f;

FollowCamera::FollowCamera(int width, int height)
	:Camera(width,height)
{
	m_targetPos = Vector3(0.0f, 0.0f, 0.0f);
	m_targetAngle = 0.0f;
}

void FollowCamera::Update()
{
	//�J�������_���W�A�Q�Ɠ_���W
	Vector3 eyepos, refpos;

	//�^�[�Q�b�g�̍��W�͎��@�̍��W�ɒǏ]
	refpos = m_targetPos + Vector3(0.0f, 2.0f, 0.0f);
	//�^�[�Q�b�g���W����J�������W�ւ̍���
	Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);

	//���@�̌��ɉ�荞�ނ��߂̉�]�s��
	Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
	//�J�����ւ̃x�N�g�����J�X
	cameraV = Vector3::TransformNormal(cameraV, rotmat);

	//�J�������W���v�Z
	eyepos = refpos + cameraV;

	//�����Z�b�g
	this->SetEyePos(eyepos);
	this->SetRefPos(refpos);

	//Camera��Update���Ăяo��(���N���X�̍X�V�j
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
