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

	////自機パーツ
	//enum PLAYER_PARTS
	//{
	//	PLAYER_PARTS_TANK,	//タンク
	//	PLAYER_PARTS_ENGINE,//エンジン
	//	PLAYER_PARTS_HEAD,	//頭1
	//	PLAYER_PARTS_HEAD2,	//頭2
	//	PLAYER_PARTS_CANNON,//キャノン1
	//	PLAYER_PARTS_CANNON2,//キャノン2
	//	PLAYER_PARTS_BULLET,//弾

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

	//変数追加が可能
	//プリミティブバッヂ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>> m_batch;
	//エフェクト
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	//入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//汎用ステート設定
	std::unique_ptr<DirectX::CommonStates> m_states;

	//ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	//ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	//射影行列
	DirectX::SimpleMath::Matrix m_proj;

	//デバッグカメラ
	std::unique_ptr<DebugCamera> m_debugcamera;

	//エフェクトファクトリ
	std::unique_ptr<DirectX::EffectFactory> m_factory;
	//天球モデル
	Obj3d m_ObjSkyDome;

	//モデル地面
	Obj3d m_ObjGround;

	//キャタピラのモデル
	std::unique_ptr<DirectX::Model> m_modelCaterpiller;
	//キャタピラのモデル
	std::unique_ptr<DirectX::Model> m_modelCaterpiller2;

	//ティーポットのワールド行列
	DirectX::SimpleMath::Matrix m_worldTeapot[20];

	//キャタピラのワールド行列
	DirectX::SimpleMath::Matrix m_worldCaterpiller;
	//キャタピラのワールド行列
	DirectX::SimpleMath::Matrix m_worldCaterpiller2;

	float m_distance[20];
	int m_count;

	std::unique_ptr<DirectX::Keyboard> m_keyboard;

	//自機の座標
	float tank_rot;
	DirectX::SimpleMath::Vector3 tank_pos;

	/*DirectX::SimpleMath::Vector3 m_lerp[20];

	DirectX::SimpleMath::Vector3 Lerp(DirectX::SimpleMath::Vector3 startPosition, DirectX::SimpleMath::Vector3 targetPosition, float t, float v);*/
	//float linearity(float time);

	//自機パーツ1(親パーツ)
	//Obj3d m_ObjPlayer1;
	//自機パーツ2(子パーツ)
	//Obj3d m_ObjPlayer2;



	//カメラのオブジェクト
	std::unique_ptr<FollowCamera>m_camera;

	std::unique_ptr<Player>m_Player;

	std::vector<std::unique_ptr<Enemy>>m_Enemies;
};