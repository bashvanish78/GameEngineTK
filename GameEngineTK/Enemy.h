#include <SimpleMath.h>
#include"Obj3d.h"
#include <Keyboard.h>

#pragma once
class Enemy
{
public:

	//自機パーツ
	enum PARTS
	{
		PARTS_TANK,	//タンク
		PARTS_ENGINE,//エンジン
		PARTS_HEAD,	//頭1
		PARTS_HEAD2,	//頭2
		PARTS_CANNON,//キャノン1
		PARTS_CANNON2,//キャノン2
		PARTS_BULLET,//弾

		PARTS_NUM
	};

	//コンストラクタ
	Enemy(DirectX::Keyboard* key);
	//デストラクタ
	~Enemy();
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	//計算
	void Calc();
	
	//座標を習得
	const DirectX::SimpleMath::Vector3& GetTrans();
	//回転を取得
	const DirectX::SimpleMath::Vector3& GetRot();
	//スケールを取得
	const DirectX::SimpleMath::Vector3& GetScale();
	//ワールド座標を取得
	const DirectX::SimpleMath::Matrix& GetWorld();

	//座標を設定
	void SetTrans(DirectX::SimpleMath::Vector3& trans);
	//回転を設定
	void SetRot(DirectX::SimpleMath::Vector3& rot);
	//スケールを設定
	void SetScale(DirectX::SimpleMath::Vector3& scale);
	
	

private:
	DirectX::Keyboard* m_keyboard;
	Obj3d* m_parts;

	std::vector<Obj3d> m_Obj;

	float m_cycle;

	//発射してから消えるまで
	int shootcount;
	//消える時間
	int deletecount;
	//弾の角度
	float bulletangle;
	float cannonangle;

	float m_DistAngle;
	int m_timer;

};

