/// <summary>
/// 3D�I�u�W�F�N�g�̃N���X
/// </summary>

#pragma once

#include <windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>

#include "Camera.h"

class Obj3d
{
public:
	//�ÓI�����o�֐�
	static void InitializeStatic(
		Microsoft::WRL::ComPtr<ID3D11Device>            d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>     d3dContext,
		Camera* camera);
private:
	//�ÓI�����o�ϐ�

	//�f�o�C�X
	static Microsoft::WRL::ComPtr<ID3D11Device>            Obj3d::m_d3dDevice;

	//�f�o�C�X�R���e�L�X�g
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;

	//�J�����̃I�u�W�F�N�g
	static Camera* Obj3d::m_camera;

	//�ėp�X�e�[�g�ݒ�
	static std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;

	//�G�t�F�N�g�t�@�N�g��
	static std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;

public:
	//�����o�֐�
	Obj3d();
	//CMO�t�@�C����ǂݍ���(���f���ǂݍ���)
	void LoadModel(const wchar_t* filename);
	//�X�V
	void Update();
	//�`��
	void Draw();

	//SetMAU
	//�X�P�[�����O(XYZ)
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; }
	//��]�p(XYZ)
	void SetRotation(const DirectX::SimpleMath::Vector3& rotation) { m_rotation = rotation; }
	//���s�ړ�(XYZ)
	void SetTranslation(const DirectX::SimpleMath::Vector3& translation) { m_translation = translation; }
	//�e��Obj3d�ւ̃|�C���^
	void SetObjParent(Obj3d* ObjParent) { m_ObjParent = ObjParent; }
	
	//GetMAU
	//�X�P�[�����O(XYZ)
	const DirectX::SimpleMath::Vector3& GetScale() { return m_scale; }
	//��]�p(XYZ)
	const DirectX::SimpleMath::Vector3& GetRotation() { return m_rotation; }
	//���s�ړ�(XYZ)
	const DirectX::SimpleMath::Vector3& GetTranslation() { return m_translation; }
	//���[���h�s��
	const DirectX::SimpleMath::Matrix& GetWorld() { return m_world; }


private:
	//�����o�ϐ�

	//���f��
	std::unique_ptr<DirectX::Model> m_model;
	//�X�P�[�����O(XYZ)
	DirectX::SimpleMath::Vector3 m_scale;
	//��]�p(XYZ)
	DirectX::SimpleMath::Vector3 m_rotation;
	//���s�ړ�(XYZ)
	DirectX::SimpleMath::Vector3 m_translation;
	//���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	//�e��Obj3d�ւ̃|�C���^
	Obj3d* m_ObjParent;
};
