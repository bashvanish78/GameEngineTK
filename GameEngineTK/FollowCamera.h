/// <summary>
/// ���@�ɒǏ]����J�����̃N���X
/// </summary>

#pragma once
#include "Camera.h"

class FollowCamera : public Camera
{
public:
	//���@�ƃJ�����̋���
	static const float CAMERA_DISTANCE;
	FollowCamera(int width,int height);
	//�X�V
	void Update() override;
	//�Ǐ]�Ώۂ̍��W���Z�b�g
	void SetTargetPos(const DirectX::SimpleMath::Vector3 targetPos);
	//�Ǐ]�Ώۂ̉�]�p���Z�b�g
	void SetTargetAngle(const float targetAngle);

protected:
	//�Ǐ]�Ώۂ̍��W
	DirectX::SimpleMath::Vector3 m_targetPos;
	//�Ǐ]�Ώۂ̉�]�p
	float m_targetAngle;

};

