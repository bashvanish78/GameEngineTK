//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "DebugCamera.h"
#include "FollowCamera.h"
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>
#include <Keyboard.h>
#include "Obj3d.h"
#include <vector>
#include "Player.h"
#include "Enemy.h"


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

	////���@�p�[�c
	//enum PLAYER_PARTS
	//{
	//	PLAYER_PARTS_TANK,	//�^���N
	//	PLAYER_PARTS_ENGINE,//�G���W��
	//	PLAYER_PARTS_HEAD,	//��1
	//	PLAYER_PARTS_HEAD2,	//��2
	//	PLAYER_PARTS_CANNON,//�L���m��1
	//	PLAYER_PARTS_CANNON2,//�L���m��2
	//	PLAYER_PARTS_BULLET,//�e

	//	PLAYER_PARTS_NUM
	//};

    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;

    Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

	//�ϐ��ǉ����\
	//�v���~�e�B�u�o�b�a
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>> m_batch;
	//�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	//���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//�ėp�X�e�[�g�ݒ�
	std::unique_ptr<DirectX::CommonStates> m_states;

	//���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	//�r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	//�ˉe�s��
	DirectX::SimpleMath::Matrix m_proj;

	//�f�o�b�O�J����
	std::unique_ptr<DebugCamera> m_debugcamera;

	//�G�t�F�N�g�t�@�N�g��
	std::unique_ptr<DirectX::EffectFactory> m_factory;
	//�V�����f��
	Obj3d m_ObjSkyDome;

	//���f���n��
	Obj3d m_ObjGround;

	//�L���^�s���̃��f��
	std::unique_ptr<DirectX::Model> m_modelCaterpiller;
	//�L���^�s���̃��f��
	std::unique_ptr<DirectX::Model> m_modelCaterpiller2;

	//�e�B�[�|�b�g�̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldTeapot[20];

	//�L���^�s���̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldCaterpiller;
	//�L���^�s���̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldCaterpiller2;

	float m_distance[20];
	int m_count;

	std::unique_ptr<DirectX::Keyboard> m_keyboard;

	//���@�̍��W
	float tank_rot;
	DirectX::SimpleMath::Vector3 tank_pos;

	/*DirectX::SimpleMath::Vector3 m_lerp[20];

	DirectX::SimpleMath::Vector3 Lerp(DirectX::SimpleMath::Vector3 startPosition, DirectX::SimpleMath::Vector3 targetPosition, float t, float v);*/
	//float linearity(float time);

	//���@�p�[�c1(�e�p�[�c)
	//Obj3d m_ObjPlayer1;
	//���@�p�[�c2(�q�p�[�c)
	//Obj3d m_ObjPlayer2;



	//�J�����̃I�u�W�F�N�g
	std::unique_ptr<FollowCamera>m_camera;

	std::unique_ptr<Player>m_Player;

	std::vector<std::unique_ptr<Enemy>>m_Enemies;
};