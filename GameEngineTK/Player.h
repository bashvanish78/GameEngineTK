#include <SimpleMath.h>
#include"Obj3d.h"
#include <Keyboard.h>

#pragma once
class Player
{
public:

	//���@�p�[�c
	enum PARTS
	{
		PARTS_TANK,	//�^���N
		PARTS_ENGINE,//�G���W��
		PARTS_HEAD,	//��1
		PARTS_HEAD2,	//��2
		PARTS_CANNON,//�L���m��1
		PARTS_CANNON2,//�L���m��2
		PARTS_BULLET,//�e

		PARTS_NUM
	};

	Player(DirectX::Keyboard* key);
	void Initialize();
	~Player();
	void Update();
	void Draw();

	DirectX::SimpleMath::Vector3 GetScale();
	DirectX::SimpleMath::Vector3 GetRot();
	DirectX::SimpleMath::Vector3 GetTrans();
	DirectX::SimpleMath::Matrix GetWorld();

	void SetScale(DirectX::SimpleMath::Vector3 scale);
	void SetRot(DirectX::SimpleMath::Vector3 rot);
	void SetTrans(DirectX::SimpleMath::Vector3 trans);

private:
	DirectX::Keyboard* m_keyboard;
	Obj3d* m_parts;

	std::vector<Obj3d> m_Obj;

	float m_cycle;

	//���˂��Ă��������܂�
	int shootcount;
	//�����鎞��
	int deletecount;
	//�e�̊p�x
	float bulletangle;
	float cannonangle;

};

