//
// Game.cpp
//

#include "pch.h"
#include "Game.h"


extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;



Game::Game() :
    m_window(0),
    m_outputWidth(800),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
//初期化(ゲーム画面の横幅縦幅を引数)
void Game::Initialize(HWND window, int width, int height)
{
	//ウィンドウの設定
    m_window = window;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

	//システム的な初期化
    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

	////////////////////////////
	//各種初期化はこの辺に書く//
	////////////////////////////
	
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormal>>(m_d3dContext.Get());

	m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());

	//射影行列を作る
	m_effect->SetProjection(XMMatrixOrthographicOffCenterRH(0,
		m_outputWidth, m_outputHeight, 0, 0, 1));
	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());

	//汎用ステート設定を生成
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

	m_debugcamera = std::make_unique<DebugCamera>(m_outputWidth, m_outputHeight);

	//エフェクトファクトリ作成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	//テクスチャのパスを指定
	m_factory->SetDirectory(L"Resources");
	//モデル作成
	m_modelSkyDome = Model::CreateFromCMO(m_d3dDevice.Get(), (L"Resources/skydome.cmo"), *m_factory);
	m_modelGround = Model::CreateFromCMO(m_d3dDevice.Get(), (L"Resources/ground200m.cmo"), *m_factory);
	m_modelCaterpiller = Model::CreateFromCMO(m_d3dDevice.Get(), (L"Resources/caterpillar.cmo"), *m_factory);

	//for (int i = 0; i < 20; i++)
	//{
		//m_modelBall[i] = Model::CreateFromCMO(m_d3dDevice.Get(), (L"Resources/ball.cmo"), *m_factory);
	//}

	for (int i = 0; i < 20; i++)
	{
		m_modelTeapot[i] = Model::CreateFromCMO(m_d3dDevice.Get(), (L"Resources/teapot.cmo"), *m_factory);
	}

	//for (int i = 0; i < 10000; i++)
	//{
	//	m_modelGround2[i] = Model::CreateFromCMO(m_d3dDevice.Get(), (L"Resources/ground1m.cmo"), *m_factory);
	//}


	for (int i = 0; i < 20; i++)
	{
		m_dir[i] = XMConvertToRadians(rand() % 360);
		m_distance[i] = rand() % 100;
	}

	m_count = 0;

	//球の位置初期化
	//内側
	//for (int i = 0; i < 10; i++)
	//{
	//	//スケーリング
	//	Matrix scalemat = Matrix::CreateScale(2.0f);
	//	//平行移動
	//	Matrix transmat = Matrix::CreateTranslation(0.0f, 0.0f, 20.0f);
	//	//ロール
	//	Matrix rotmatz = Matrix::CreateRotationZ(XM_PIDIV4);
	//	//ピッチ(仰角)
	//	Matrix rotmatx = Matrix::CreateRotationX(XM_PIDIV4);
	//	//ヨー(方位角)
	//	Matrix rotmaty = Matrix::CreateRotationY(XM_2PI / 10 * (1 + i));
	//	//回転行列の合成
	//	Matrix rotmat = rotmaty;// *rotmatx * rotmaty;
	//							//ワールド行列の合成(SRT)このかけ順が安定
	//	m_worldBall[i] = scalemat * transmat * rotmat;
	//}
	////外側
	//for (int i = 10; i < 20; i++)
	//{
	//	//スケーリング
	//	Matrix scalemat = Matrix::CreateScale(2.0f);
	//	//平行移動
	//	Matrix transmat = Matrix::CreateTranslation(0.0f, 0.0f, 40.0f);
	//	//ロール
	//	Matrix rotmatz = Matrix::CreateRotationZ(XM_PIDIV4);
	//	//ピッチ(仰角)
	//	Matrix rotmatx = Matrix::CreateRotationX(XM_PIDIV4);
	//	//ヨー(方位角)
	//	Matrix rotmaty = Matrix::CreateRotationY(XM_2PI / 10 * (1 + i - 10));
	//	//回転行列の合成
	//	Matrix rotmat = rotmaty;// *rotmatx * rotmaty;
	//							//ワールド行列の合成(SRT)このかけ順が安定
	//	m_worldBall[i] = scalemat * transmat * rotmat;
	//}
	//
	////地面2
	//for (int i = 0; i < 100; i++)
	//{
	//	for (int j = 0; j < 100; j++)
	//	{
	//		//スケーリング
	//		Matrix scalemat = Matrix::CreateScale(1.0f);
	//		//平行移動
	//		Matrix transmat = Matrix::CreateTranslation(1.0f * j - 50, 0.0f, i - 50);
	//		//ロール
	//		Matrix rotmatz = Matrix::CreateRotationZ(0);
	//		//ピッチ(仰角)
	//		Matrix rotmatx = Matrix::CreateRotationX(0);
	//		//ヨー(方位角)
	//		Matrix rotmaty = Matrix::CreateRotationY(0);
	//		//回転行列の合成
	//		Matrix rotmat = rotmatz *rotmatx * rotmaty;
	//		//ワールド行列の合成(SRT)このかけ順が安定
	//		m_worldGround2[j+(i*100)] = scalemat * rotmat* transmat;
	//		//m_worldBall[i] = scalemat * transmat * rotmat;
	//		//m_modelGround2[j + (i * 400)] = transmat;
	//	}
	//}

	m_keyboard = std::make_unique<Keyboard>();

	tank_rot = 0.0f;
}

// Executes the basic game loop.
//UpdateとRender呼び出し
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    elapsedTime;

	//////////////////////////
	//毎フレームの処理を書く//
	//////////////////////////
	m_debugcamera->Update();

	////ティーポットの回転
	//for (int i = 0; i < 20; i++)
	//{
	//	//平行移動
	//	Matrix transmat = Matrix::CreateTranslation(cosf(m_dir[i])*m_distance[i], 0.0f, sinf(m_dir[i])*m_distance[i]);
	//	//回転
	//	Matrix rotmaty = Matrix::CreateRotationY(XMConvertToRadians(-3.0f));
	//	m_worldTeapot[i] = transmat * rotmaty;
	//	//m_worldTeapot[i] *= ;
	//}




	m_count++;
	//ティーポット
	for (int i = 0; i < 20; i++)
	{
		//m_lerp[i] = Lerp(Vector3(cosf(m_dir[i])*m_distance[i], 0.0f, sinf(m_dir[i])*m_distance[i]), Vector3(0.0f,0.0f,0.0f), 10.0f, linearity(10.0f));
		float val = (sinf(m_count / 20.0f) + 1.0f) * 2.5f;
		//スケーリング
		Matrix scalemat = Matrix::CreateScale(val);
		//平行移動
		//Matrix transmat = Matrix::CreateTranslation(cosf(m_dir[i])*m_distance[i]+m_lerp[i].x, 0.0f+m_lerp[i].y, sinf(m_dir[i])*m_distance[i]+m_lerp[i].z);
		//Matrix transmat = Matrix::CreateTranslation(0.0f,0.0f,0.0f);
		Matrix transmat = Matrix::CreateTranslation(cosf(m_dir[i])*m_distance[i], 0.0f, sinf(m_dir[i])*m_distance[i]);
		//ロール
		Matrix rotmatz = Matrix::CreateRotationZ(0);
		//ピッチ(仰角)
		Matrix rotmatx = Matrix::CreateRotationX(0);
		//ヨー(方位角)
		Matrix rotmaty = Matrix::CreateRotationY(m_count / 10.0f);
		//回転行列の合成
		Matrix rotmat = rotmaty;// *rotmatx * rotmaty;
								//ワールド行列の合成(SRT)このかけ順が安定
		m_worldTeapot[i] = scalemat * rotmat* transmat;
	}

	//スケーリング
	Matrix scalemat = Matrix::CreateScale(1.0f);
	//平行移動
	Matrix transmat = Matrix::CreateTranslation(0.0f, 20.0f, 0.0f);
	//ロール
	Matrix rotmatz = Matrix::CreateRotationZ(0);
	//ピッチ(仰角)
	Matrix rotmatx = Matrix::CreateRotationX(0);
	//ヨー(方位角)
	Matrix rotmaty = Matrix::CreateRotationY(0);
	//回転行列の合成
	Matrix rotmat = rotmatz * rotmatx * rotmaty;
	//ワールド行列の合成(SRT)このかけ順が安定
	m_worldCaterpiller = rotmatx * scalemat * transmat;
	//m_worldCaterpiller.Identity;

	////内側
	//for (int i = 0; i < 10; i++)
	//{
	//	Matrix rotmaty = Matrix::CreateRotationY(XMConvertToRadians(3.0f));
	//	m_worldBall[i] *= rotmaty;
	//}
	////外側
	//for (int i = 10; i < 20; i++)
	//{
	//	Matrix rotmaty = Matrix::CreateRotationY(XMConvertToRadians(-3.0f));
	//	m_worldBall[i] *= rotmaty;
	//}
	////スケーリング
	//Matrix scalemat = Matrix::CreateScale(2.0f);
	////平行移動
	//Matrix transmat = Matrix::CreateTranslation(0.0f, 10.0f, 0.0f);
	////ロール
	//Matrix rotmatz = Matrix::CreateRotationZ(XM_PIDIV4);
	////ピッチ(仰角)
	//Matrix rotmatx = Matrix::CreateRotationX(XM_PIDIV4);
	////ヨー(方位角)
	//Matrix rotmaty = Matrix::CreateRotationY(XM_PIDIV4);
	//回転行列の合成
	//Matrix rotmat = rotmatz * rotmatx * rotmaty;
	//ワールド行列の合成(SRT)このかけ順が安定
	//m_worldBall[10] = rotmatx * scalemat * transmat;

	//キーボードの状態取得
	Keyboard::State kb = m_keyboard->GetState();

	//aキーを押している間左旋回
	if (kb.A)
	{
		//自機のベクトル
		float rot = -0.1;
		//自機の亜票を移動
		tank_rot += rot;
	}
	//dキーを押している間右旋回
	if (kb.D)
	{
		//自機のベクトル
		float rot = 0.1;
		//自機の亜票を移動
		tank_rot += rot;
	}

	//wキーを押している間全身
	if (kb.W)
	{
		//自機のベクトル
		Vector3 moveV(0.0f, 0.0f, -0.1f);
		//移動ベクトルを自機の角度分回転
		//Matrix rotmat = Matrix::CreateRotationY(tank_rot);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		//自機の亜票を移動
		tank_pos += moveV;
	}
	//sキーを押している間交代
	if (kb.S)
	{
		//自機のベクトル
		Vector3 moveV(0.0f, 0.0f, 0.1f);
		//移動ベクトルを自機の角度分回転
		moveV = Vector3::TransformNormal(moveV, m_worldCaterpiller);
		//自機の亜票を移動
		tank_pos += moveV;
	}



	{//自機のワールド行列を計算
		//平行移動
		Matrix transmat = Matrix::CreateTranslation(tank_pos);
		//Y軸回転
		Matrix rotmat = Matrix::CreateRotationY(tank_rot);
		//平行移動行列をワールド座標にコピー
		m_worldCaterpiller = rotmat * transmat ;
	}

}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

	//キャンバスの初期化
    Clear();

    // TODO: Add your rendering code here.

	//////////////////
	//描画処理を書く//
	//////////////////
	//m_d3dContextに描画処理を突っ込む
	//Opaque = 塗りつぶす（不透明)
	m_d3dContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	//奥行きを使わない
	m_d3dContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	//カリングしない(表裏どっちでも描画)
	m_d3dContext->RSSetState(m_states->Wireframe());

	////ビュー行列生成
	//m_view = Matrix::CreateLookAt(
	//	Vector3(0.f, 0.f, 20.f),	//カメラ視点
	//	Vector3(0,0,0),			//カメラ参照点
	//	Vector3(0,1,0));		//上方向ベクトル
	m_view = m_debugcamera->GetCameraMatrix();

	//射影行列生成
	m_proj = Matrix::CreatePerspectiveFieldOfView(
		XM_PI / 4.f,			//視野角(上下方向)
		float(m_outputWidth) / float(m_outputHeight),	//アスペクト比
		0.1f,	//ニアクリップ
		500.f);	//ファークリップ

	uint16_t indices[] =
	{
		0,1,2,
		2,1,3,
	};

	VertexPositionNormal vertices[] =
	{
		{ Vector3(-1.0f,+1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f) },
		{ Vector3(-1.0f,-1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f) },
		{ Vector3(+1.0f,+1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f) },
		{ Vector3(+1.0f,-1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f) },
	};

	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);

	//設定を反映
	m_effect->Apply(m_d3dContext.Get());
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	//モデルの描画
	m_modelSkyDome->Draw(m_d3dContext.Get(), *m_states, Matrix::Identity, m_view, m_proj);
	m_modelGround->Draw(m_d3dContext.Get(), *m_states, Matrix::Identity, m_view, m_proj);
	m_modelCaterpiller->Draw(m_d3dContext.Get(), *m_states, m_worldCaterpiller, m_view, m_proj);

	//for (int i = 0; i < 20; i++)
	//{
	//	m_modelTeapot[i]->Draw(m_d3dContext.Get(), *m_states, m_worldTeapot[i], m_view, m_proj);
	//}

	//for (int i = 0; i < 20; i++)
	//{
		//m_modelBall[i]->Draw(m_d3dContext.Get(), *m_states, m_worldBall[i], m_view, m_proj);
	//}

	//for (int i = 0; i < 10000; i++)
	//{
	//	m_modelGround2[i]->Draw(m_d3dContext.Get(), *m_states, m_worldBall[i], m_view, m_proj);
	//}
	
	//m_modelBall[0]->Draw(m_d3dContext.Get(), *m_states, m_worldBall[0], m_view, m_proj);


	//描画をする
	m_batch->Begin();
	VertexPositionColor v1(Vector3(0.0f, 0.5f, 0.5f), Colors::Yellow);
	VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::Yellow);
	VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::Yellow);

	//VertexPositionColor v1(Vector3(400.f, 150.f, 0.f), Colors::Cyan);
	//VertexPositionColor v2(Vector3(600.f, 450.f, 0.f), Colors::Magenta);
	//VertexPositionColor v3(Vector3(200.f, 450.f, 0.f), Colors::Yellow);

//	m_batch->DrawTriangle(v1, v2, v3);

	m_batch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices, 6, vertices, 4);
	//m_batch->DrawLine(
	//	VertexPositionColor(
	//		SimpleMath::Vector3(0,0,0),
	//		SimpleMath::Color(1,1,1)), 
	//	VertexPositionColor(
	//		SimpleMath::Vector3(800, 600, 0),
	//		SimpleMath::Color(0, 0, 0))
	//);
	m_batch->End();

	
	//画面に反映
    Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    HRESULT hr = D3D11CreateDevice(
        nullptr,                                // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        m_d3dDevice.ReleaseAndGetAddressOf(),   // returns the Direct3D device created
        &m_featureLevel,                        // returns feature level of device created
        m_d3dContext.ReleaseAndGetAddressOf()   // returns the device immediate context
        );

    if (hr == E_INVALIDARG)
    {
        // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
        hr = D3D11CreateDevice(nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            creationFlags,
            &featureLevels[1],
            _countof(featureLevels) - 1,
            D3D11_SDK_VERSION,
            m_d3dDevice.ReleaseAndGetAddressOf(),
            &m_featureLevel,
            m_d3dContext.ReleaseAndGetAddressOf()
            );
    }

    DX::ThrowIfFailed(hr);

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    // DirectX 11.1 if present
    if (SUCCEEDED(m_d3dDevice.As(&m_d3dDevice1)))
        (void)m_d3dContext.As(&m_d3dContext1);

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory1> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        ComPtr<IDXGIFactory2> dxgiFactory2;
        if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
        {
            // DirectX 11.1 or later

            // Create a descriptor for the swap chain.
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
            swapChainDesc.Width = backBufferWidth;
            swapChainDesc.Height = backBufferHeight;
            swapChainDesc.Format = backBufferFormat;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = backBufferCount;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
            fsSwapChainDesc.Windowed = TRUE;

            // Create a SwapChain from a Win32 window.
            DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
                m_d3dDevice.Get(),
                m_window,
                &swapChainDesc,
                &fsSwapChainDesc,
                nullptr,
                m_swapChain1.ReleaseAndGetAddressOf()
                ));

            DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
        }
        else
        {
            DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
            swapChainDesc.BufferCount = backBufferCount;
            swapChainDesc.BufferDesc.Width = backBufferWidth;
            swapChainDesc.BufferDesc.Height = backBufferHeight;
            swapChainDesc.BufferDesc.Format = backBufferFormat;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.OutputWindow = m_window;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.Windowed = TRUE;

            DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf()));
        }

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain1.Reset();
    m_swapChain.Reset();
    m_d3dContext1.Reset();
    m_d3dContext.Reset();
    m_d3dDevice1.Reset();
    m_d3dDevice.Reset();

    CreateDevice();

    CreateResources();
}

//Vector3 Game::Lerp(Vector3 startPosition, Vector3 targetPosition, float t, float v)
//{
//	Vector3 lerpPosition = Vector3(0.0f,0.0f,0.0f);
//
//	lerpPosition = (1 - (v*t)) * startPosition + (v*t) * targetPosition;
//
//	return lerpPosition;
//}
//
//float Game::linearity(float time)
//{
//	float vt = 0.0f;
//
//	vt = time;
//
//	return vt;
//}