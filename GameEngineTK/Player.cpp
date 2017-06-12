#include "pch.h"
#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Player::Player(DirectX::Keyboard* key)
{
	m_keyboard = key;
}

void Player::Initialize()
{
	//���@�p�[�c�̓ǂݍ���
	m_Obj.resize(PARTS_NUM);
	m_Obj[PARTS_TANK].LoadModel(L"Resources/caterpillar.cmo");
	m_Obj[PARTS_ENGINE].LoadModel(L"Resources/engine.cmo");
	m_Obj[PARTS_HEAD].LoadModel(L"Resources/Head.cmo");
	m_Obj[PARTS_HEAD2].LoadModel(L"Resources/Head2.cmo");
	m_Obj[PARTS_CANNON].LoadModel(L"Resources/Cannon.cmo");
	m_Obj[PARTS_CANNON2].LoadModel(L"Resources/Cannon.cmo");
	m_Obj[PARTS_BULLET].LoadModel(L"Resources/Head.cmo");

	//�e�q�֌W�̍\�z(�q�p�[���ɐe��ݒ�)
	m_Obj[PARTS_ENGINE].SetObjParent(&m_Obj[PARTS_TANK]);
	m_Obj[PARTS_HEAD].SetObjParent(&m_Obj[PARTS_TANK]);
	m_Obj[PARTS_HEAD2].SetObjParent(&m_Obj[PARTS_ENGINE]);
	m_Obj[PARTS_CANNON].SetObjParent(&m_Obj[PARTS_HEAD2]);
	m_Obj[PARTS_CANNON2].SetObjParent(&m_Obj[PARTS_HEAD2]);

	//�q�p�[�c�̐e����̃I�t�Z�b�g(���W�̂���)��ݒ�
	m_Obj[PARTS_ENGINE].SetTranslation(Vector3(0.0f, 0.2f, 0.0f));
	m_Obj[PARTS_HEAD].SetTranslation(Vector3(0.0f, 0.6f, -0.45f));
	m_Obj[PARTS_HEAD2].SetTranslation(Vector3(0.0f, 1.0f, 0.2f));
	m_Obj[PARTS_CANNON].SetTranslation(Vector3(0.0f, 0.4f, 0.0f));
	m_Obj[PARTS_CANNON2].SetTranslation(Vector3(0.0f, 0.6f, 0.0f));

	m_Obj[PARTS_BULLET].SetTranslation(m_Obj[PARTS_CANNON].GetTranslation());

	m_Obj[PARTS_CANNON2].SetScale(m_Obj[PARTS_CANNON2].GetScale() * -1);

	m_cycle = 0.0f;

	shootcount = 0;

	deletecount = 60;
	deletecount += 1;
}


Player::~Player()
{
}

void Player::Update()
{

	//�p�[�c�M�~�b�N
	{
		m_cycle += 0.1f;
		float scale = 1.6f + sinf(m_cycle);
		m_Obj[PARTS_HEAD2].SetScale(Vector3(scale, scale, scale));

		float angle = m_Obj[PARTS_CANNON].GetRotation().y;
		m_Obj[PARTS_CANNON].SetRotation(Vector3(0.0f, angle + 0.03f, 0.0f));
		float angle2 = m_Obj[PARTS_CANNON2].GetRotation().y;
		m_Obj[PARTS_CANNON2].SetRotation(Vector3(0.0f, angle2 + 0.03f, 0.0f));

		//float angle = m_Obj[PARTS_CANNON].GetRotation().x;
		//m_Obj[PARTS_HEAD].SetRotation(Vector3(angle + 0.03f, 0.0f, 0.0f));
		m_Obj[PARTS_HEAD].SetScale(Vector3(1, scale, 1));

		float angleengine = m_Obj[PARTS_CANNON].GetRotation().x + sinf(m_cycle);
		m_Obj[PARTS_ENGINE].SetRotation(Vector3(angleengine, 0.0f, 0.0f));

	}


	//�L�[�{�[�h�̏�Ԏ擾
	Keyboard::State kb = m_keyboard->GetState();

	//a�L�[�������Ă���ԍ�����
	if (kb.A)
	{
		//���@�̍��W���ړ�
		float angle = m_Obj[0].GetRotation().y;
		float angle2 = m_Obj[PARTS_BULLET].GetRotation().y;
		m_Obj[0].SetRotation(Vector3(0.0f, angle + 0.03f, 0.0f));
		m_Obj[PARTS_BULLET].SetRotation(Vector3(0.0f, angle + 0.03f, 0.0f));
	}
	//d�L�[�������Ă���ԉE����
	if (kb.D)
	{
		//���@�̍��W���ړ�
		float angle = m_Obj[0].GetRotation().y;
		float angle2 = m_Obj[PARTS_BULLET].GetRotation().y;
		m_Obj[0].SetRotation(Vector3(0.0f, angle - 0.03f, 0.0f));
		m_Obj[PARTS_BULLET].SetRotation(Vector3(0.0f, angle - 0.03f, 0.0f));
	}

	//w�L�[�������Ă���ԑO�i
	if (kb.W)
	{
		//���@�̃x�N�g��
		Vector3 moveV(0.0f, 0.0f, -0.1f);
		float angle = m_Obj[0].GetRotation().y;
		//�ړ��x�N�g�������@�̊p�x����]
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		//���@�̍��W���ړ�
		Vector3 pos = m_Obj[0].GetTranslation();
		m_Obj[0].SetTranslation(pos + moveV);

	}
	//s�L�[�������Ă���Ԍ��
	if (kb.S)
	{
		//���@�̃x�N�g��
		Vector3 moveV(0.0f, 0.0f, 0.1f);
		float angle = m_Obj[0].GetRotation().y;
		//�ړ��x�N�g�������@�̊p�x����]
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		//���@�̍��W���ړ�
		Vector3 pos = m_Obj[0].GetTranslation();
		m_Obj[0].SetTranslation(pos + moveV);
	}
	//����
	if (kb.Space)
	{
		if (shootcount == 0)
		{
			//���@�̃x�N�g��
			bulletangle = m_Obj[PARTS_ENGINE].GetRotation().x * -1;
			cannonangle = m_Obj[PARTS_CANNON].GetRotation().y * -1;
			shootcount = 1;
		}
	}

	//�펞�L���m���̏ꏊ
	if (shootcount == 0)
	{
		Vector3 cannonpos = m_Obj[PARTS_TANK].GetTranslation() + m_Obj[PARTS_ENGINE].GetTranslation() + m_Obj[PARTS_HEAD2].GetTranslation() + m_Obj[PARTS_CANNON].GetTranslation();
		Matrix rotmatx = Matrix::CreateRotationX(m_Obj[PARTS_ENGINE].GetRotation().x);
		Matrix rotmaty = Matrix::CreateRotationY(m_Obj[PARTS_ENGINE].GetRotation().y);
		Matrix rotmat = rotmatx * rotmaty;
		//cannonpos = Vector3::TransformNormal(cannonpos, rotmat);
		m_Obj[PARTS_BULLET].SetTranslation(cannonpos);
	}
	else
	{
		//���˂����ꍇ
		if (shootcount >= deletecount)
		{
			shootcount = 0;
		}
		else
		{
			//�ړ�����������
			Vector3 moveV(0.0f, 0.0f, 0.5f);
			//�ړ��x�N�g�������@�̊p�x����]
			Matrix rotmatx = Matrix::CreateRotationX(bulletangle);
			Matrix rotmaty = Matrix::CreateRotationY(cannonangle);
			Matrix rotmat = rotmatx * rotmaty;
			moveV = Vector3::TransformNormal(moveV, rotmat);
			//�e�̍��W���ړ�
			Vector3 pos = m_Obj[PARTS_BULLET].GetTranslation();
			m_Obj[PARTS_BULLET].SetTranslation(pos + moveV);
			shootcount++;
		}

	}
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Update();
	}

}

void Player::Draw()
{
	//�v���C���[�̕`��
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Draw();
	}
}

DirectX::SimpleMath::Vector3 Player::GetTrans()
{
	return m_Obj[0].GetTranslation();
}

DirectX::SimpleMath::Matrix Player::GetWorld()
{
	return m_Obj[0].GetWorld();
}

DirectX::SimpleMath::Vector3 Player::GetScale()
{
	return m_Obj[0].GetScale();
}

DirectX::SimpleMath::Vector3 Player::GetRot()
{
	return m_Obj[0].GetRotation();
}

void Player::SetScale(DirectX::SimpleMath::Vector3 scale)
{
	m_Obj[0].SetScale(scale);
}

void Player::SetRot(DirectX::SimpleMath::Vector3 rot)
{
	m_Obj[0].SetRotation(rot);
}

void Player::SetTrans(DirectX::SimpleMath::Vector3 trans)
{
	m_Obj[0].SetTranslation(trans);
}
