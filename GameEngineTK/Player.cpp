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
	//自機パーツの読み込み
	m_Obj.resize(PARTS_NUM);
	m_Obj[PARTS_TANK].LoadModel(L"Resources/caterpillar.cmo");
	m_Obj[PARTS_ENGINE].LoadModel(L"Resources/engine.cmo");
	m_Obj[PARTS_HEAD].LoadModel(L"Resources/Head.cmo");
	m_Obj[PARTS_HEAD2].LoadModel(L"Resources/Head2.cmo");
	m_Obj[PARTS_CANNON].LoadModel(L"Resources/Cannon.cmo");
	m_Obj[PARTS_CANNON2].LoadModel(L"Resources/Cannon.cmo");
	m_Obj[PARTS_BULLET].LoadModel(L"Resources/Head.cmo");

	//親子関係の構築(子パールに親を設定)
	m_Obj[PARTS_ENGINE].SetObjParent(&m_Obj[PARTS_TANK]);
	m_Obj[PARTS_HEAD].SetObjParent(&m_Obj[PARTS_TANK]);
	m_Obj[PARTS_HEAD2].SetObjParent(&m_Obj[PARTS_ENGINE]);
	m_Obj[PARTS_CANNON].SetObjParent(&m_Obj[PARTS_HEAD2]);
	m_Obj[PARTS_CANNON2].SetObjParent(&m_Obj[PARTS_HEAD2]);

	//子パーツの親からのオフセット(座標のずれ)を設定
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

	//パーツギミック
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


	//キーボードの状態取得
	Keyboard::State kb = m_keyboard->GetState();

	//aキーを押している間左旋回
	if (kb.A)
	{
		//自機の座標を移動
		float angle = m_Obj[0].GetRotation().y;
		float angle2 = m_Obj[PARTS_BULLET].GetRotation().y;
		m_Obj[0].SetRotation(Vector3(0.0f, angle + 0.03f, 0.0f));
		m_Obj[PARTS_BULLET].SetRotation(Vector3(0.0f, angle + 0.03f, 0.0f));
	}
	//dキーを押している間右旋回
	if (kb.D)
	{
		//自機の座標を移動
		float angle = m_Obj[0].GetRotation().y;
		float angle2 = m_Obj[PARTS_BULLET].GetRotation().y;
		m_Obj[0].SetRotation(Vector3(0.0f, angle - 0.03f, 0.0f));
		m_Obj[PARTS_BULLET].SetRotation(Vector3(0.0f, angle - 0.03f, 0.0f));
	}

	//wキーを押している間前進
	if (kb.W)
	{
		//自機のベクトル
		Vector3 moveV(0.0f, 0.0f, -0.1f);
		float angle = m_Obj[0].GetRotation().y;
		//移動ベクトルを自機の角度分回転
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		//自機の座標を移動
		Vector3 pos = m_Obj[0].GetTranslation();
		m_Obj[0].SetTranslation(pos + moveV);

	}
	//sキーを押している間後退
	if (kb.S)
	{
		//自機のベクトル
		Vector3 moveV(0.0f, 0.0f, 0.1f);
		float angle = m_Obj[0].GetRotation().y;
		//移動ベクトルを自機の角度分回転
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		//自機の座標を移動
		Vector3 pos = m_Obj[0].GetTranslation();
		m_Obj[0].SetTranslation(pos + moveV);
	}
	//発射
	if (kb.Space)
	{
		if (shootcount == 0)
		{
			//自機のベクトル
			bulletangle = m_Obj[PARTS_ENGINE].GetRotation().x * -1;
			cannonangle = m_Obj[PARTS_CANNON].GetRotation().y * -1;
			shootcount = 1;
		}
	}

	//常時キャノンの場所
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
		//発射した場合
		if (shootcount >= deletecount)
		{
			shootcount = 0;
		}
		else
		{
			//移動させ続ける
			Vector3 moveV(0.0f, 0.0f, 0.5f);
			//移動ベクトルを自機の角度分回転
			Matrix rotmatx = Matrix::CreateRotationX(bulletangle);
			Matrix rotmaty = Matrix::CreateRotationY(cannonangle);
			Matrix rotmat = rotmatx * rotmaty;
			moveV = Vector3::TransformNormal(moveV, rotmat);
			//弾の座標を移動
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
	//プレイヤーの描画
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
