#pragma once

#include "BaseCharacter.h"
#include "Matrix4x4Function.h"
#include "Vector3Function.h"
#include "collisionTypeIdDef.h"
#include "myFunction.h"


using namespace std;

class Player;

class GameScene;

class Enemy : public BaseCharacter {
public:
	/// <summary>
	/// コンストラクタ
	///</summary>
	Enemy();

	/// <summary>
	/// デストラクタ
	///
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*> models, const int& enemyType, const Vector3& position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;

	/// <summary>
	/// 弾の発射処理
	/// </summary>
	void Fire();

	/// <summary>
	/// 衝突判定
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;

	/// <summary>
	/// 中心座標の取得
	/// </summary>
	Vector3 GetCenter() const override;

	/// <summary>
	///	敵生成
	/// </summary>
	// void CreateEnemy(const int& enemyType, const Vector3& position);

	/// <summary>
	///	Setter
	/// </summary>
	void SetIsDead(const bool& isDead) { isDead_ = isDead; }
	void SetType(const int& type) { enemyType_ = static_cast<EnemyType>(type); }
	void SetPlayer(Player* player) { player_ = player; }
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	/// <summary>
	///	Getter
	/// </summary>
	bool IsDead() const { return isDead_; }

private:
	enum EnemyType {
		kFollow,
		kFront,
		kBack,
		kRight,
		kLeft,
	};

	EnemyType enemyType_;

	Player* player_ = nullptr;

	GameScene* gameScene_ = nullptr;

	Vector3 velocity_ = {0.0f, 0.0f, 0.0f};

	float speed_ = 0.055f;

	float rand_ = 0.0f;
	int randIndex_ = 0;

	int randTimer_ = 0;

	bool isDead_ = false;
};