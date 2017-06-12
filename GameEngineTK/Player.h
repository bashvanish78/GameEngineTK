#include <SimpleMath.h>
#include"Obj3d.h"
#include <Keyboard.h>

#pragma once
class Player
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

	//発射してから消えるまで
	int shootcount;
	//消える時間
	int deletecount;
	//弾の角度
	float bulletangle;
	float cannonangle;

};

