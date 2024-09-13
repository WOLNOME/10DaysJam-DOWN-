#pragma once
#include "ColliderUshio.h"
#include "Model.h"
#include "Script/Matrix.h"
#include "Script/MyTools.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "WorldTransform.h"
#include <Input.h>
#include <algorithm>

class RestPlayer : public ColliderUshio {
public:
	RestPlayer();
	~RestPlayer() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 移動処理
	/// </summary>
	void Translation();

	void GoHole();

	/// <summary>
	/// 旋回処理
	/// </summary>
	void Rotate();

	/// <summary>
	/// マウスでの視点移動
	/// </summary>
	void MouseMove();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(ViewProjection& viewProjection);

	void DrawUI();

public:
	void OnCollision() override;
	void OnSpecialCollision() override;
	Vector3 GetWorldPosition() override;
	WorldTransform& GetWorldTransform() { return worldTransform_; }
	bool GetIsGetItem() { return isGetItem_; }
	float GetRadius() override { return radius_; }
	bool GetIsTransition() { return isTransition_; }

private:
	WorldTransform worldTransform_;

	//スプライト
	uint32_t textureHandleSpaceUI_;
	std::unique_ptr<Sprite> spriteSpaceUI_ = nullptr;

	// キーボード入力
	Input* input_ = nullptr;

	// 自キャラの速さ
	float kCharacterSpeed = 0.1f;

	// カーソルの移動量
	Vector2 mouseMove = {0.0f, 0.0f};
	Vector2 mousePosPre;
	const Vector2 mouseCenter = {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2};
	Vector2 mousePos;

	bool isDebug_ = false;

	// プレイヤーの身長
	const float playerHeight_ = 2.0f;
	// アイテム獲得フラグ
	bool isGetItem_ = false;

private: // 遷移関係
	// 遷移待機状態フラグ
	bool isReady = false;
	// 穴の中に入るフラグ
	bool isGoHole = false;

	// 穴の中を見るアニメーション
	bool isLookHole_ = false;
	const int kLookHoleTime_ = 60;
	int lookHoleTimer_ = 0;

	// 穴に入るアニメーション
	bool isInHole_ = false;
	const int kInHoleTime_ = 60;
	int inHoleTimer_ = 0;
	const float kJumpHeight_ = 0.5f;
	const float forwardSpeed_ = 6.0f * (1.0f / 60.0f);
	Vector3 velocity_ = {0.0f, 0.0f, 0.0f};
	const float kGravity_ = 0.023f;

	// 遷移中処理
	bool isTransition_ = false;

	// 向き関係
	Vector3 startRotation_ = {0.0f, 0.0f, 0.0f};
	Vector3 goalRotation_ = {0.0f, 0.0f, 0.0f};
	// 座標関係
	Vector3 startTranslation_ = {0.0f, 0.0f, 0.0f};
	Vector3 goalTranslation_ = {0.0f, 0.0f, 0.0f};
};
