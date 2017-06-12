
#include "Obj3d.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//静的メンバ変数
// デバイス
Microsoft::WRL::ComPtr<ID3D11Device>            Obj3d::m_d3dDevice;
// デバイスコンテキスト
Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;
// カメラ
Camera* Obj3d::m_camera;
// 汎用ステート設定
std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;
// エフェクトファクトリ
std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;

void Obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Camera * camera)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_camera = camera;

	//汎用ステート設定を生成
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

	//エフェクトファクトリ作成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	//テクスチャのパスを指定
	m_factory->SetDirectory(L"Resources");
}

Obj3d::Obj3d()
{

	//メンバ変数を初期化
	m_ObjParent = nullptr;
	m_scale = Vector3(1, 1, 1);

}

//モデルを読み込む
void Obj3d::LoadModel(const wchar_t * filename)
{
	m_model = Model::CreateFromCMO(m_d3dDevice.Get(), filename, *m_factory);
}

//ワールド行列の計算
void Obj3d::Update()
{
	Matrix scalemat = Matrix::CreateScale(m_scale);

	//Z軸回転
	Matrix rotmatZ = Matrix::CreateRotationZ(m_rotation.z);
	//X軸回転
	Matrix rotmatX = Matrix::CreateRotationX(m_rotation.x);
	//Y軸回転
	Matrix rotmatY = Matrix::CreateRotationY(m_rotation.y);

	Matrix rotmat = rotmatZ * rotmatX * rotmatY;

	//平行移動
	Matrix transmat = Matrix::CreateTranslation(m_translation);

	//ワールド行列を合成
	m_world = scalemat * rotmat * transmat;

	if (m_ObjParent != NULL)
	{
		//親がある場合は親の掛け算(子の行列*親の行列)
		m_world *= m_ObjParent->GetWorld();
	}


}

//描画
void Obj3d::Draw()
{
	if (m_model != NULL)
	{
		m_model->Draw(m_d3dContext.Get(),
			*m_states, 
			m_world,
			m_camera->GetView(), 
			m_camera->GetProjection());
	}	
}
