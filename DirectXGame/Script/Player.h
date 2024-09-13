#pragma once
#include <Input.h>
#include <algorithm>
#include <memory>
#include "Model.h"
#include "WinApp.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "Sprite.h"
#include "MyTools.h"
#include "Matrix.h"
#include "BaseCharacter.h"
#include "collisionTypeIdDef.h"
#include "PlayerBullet.h"

using namespace std;

/// Wallの前方宣言
class Wall;

class Player : public BaseCharacter {
public:
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデル</param>
	void Initialize(const std::vector<Model*> models);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection& viewProjection);

	/// <summary>
	/// 移動処理
	/// </summary>
	void Translation();

	/// <summary>
	/// 旋回処理
	/// </summary>
	void Rotate();

	/// <summary>
	/// マウスでの視点移動
	/// </summary>
	void MouseMove();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();
	
	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	/// <summary>
	/// ワールド座標から3Dレティクルの座標を計算
	/// </summary>
	void WorldTo3DReticle();

	/// <summary>
	/// 3Dレティクルの座標から2Dレティクルの座標を計算
	/// </summary>
	void Reticle3Dto2D(ViewProjection& viewProjection);

	/// <summary>
	/// 中心座標の取得
	/// </summary>
	Vector3 GetCenter() const override;

	/// <summary>
	/// 3Dレティクルのワールド座標を取得
	/// </summary>
	Vector3 Get3DReticleWorldPosition();

	const list<PlayerBullet*> GetBullets() { return bullets_; }

	/// <summary>
	/// デスフラグ
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }

	/// <summary>
	/// 壁をセット
	/// </summary>
	/// <param name="wall"></param>
	void SetWall(Wall* wall) { wall_ = wall; }

	/// <summary>
	/// 弾の生成
	/// </summary>
	void CreateBullet(Model* model, const Vector3& pos, const Vector3& velocity);

	/// <summary>
	/// 衝突判定
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

private:
	WorldTransform worldTransform3DReticle_;

	unique_ptr<Model> bulletModel_;
	Sprite* sprite2DReticle_ = nullptr;

	// キーボード入力
	Input* input_ = nullptr;

	// 壁
	Wall* wall_ = nullptr;

	// 自キャラの速さ
	float kCharacterSpeed_;

	// 弾
	list<PlayerBullet*> bullets_;
	const float kReLoadTime_ = 10.0f;
	float reLoadTimer_;

	// カーソルの移動量
	Vector2 mouseMove_;
	Vector2 mousePosPre_;
	const Vector2 mouseCenter_ = {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2};
	Vector2 mousePos_;

	bool isDebug_ = false;

	// 落下速度
	Vector3 fallingVelocity_;
	Vector3 fallingVelocityJet_;

	// 体力
	const int kMaxHp_ = 5;
	int hp_;

	// デスフラグ
	bool isDead_;
};
