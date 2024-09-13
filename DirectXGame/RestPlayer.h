#pragma once
#include "collider.h"
#include <Input.h>
#include <algorithm>
#include "Model.h"
#include "WinApp.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "Sprite.h"
#include "Script/MyTools.h"
#include "Script/Matrix.h"

class RestPlayer : public Collider {
public:
	RestPlayer();
	~RestPlayer()override;

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

	/// <summary>
	/// 旋回処理
	/// </summary>
	void Rotate();

	/// <summary>
	/// マウスでの視点移動
	/// </summary>
	void MouseMove();

	/// <summary>
	/// 中心座標の取得
	/// </summary>
	Vector3 GetCenter() const override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(ViewProjection& viewProjection);

	void DrawUI();

public:
	void OnCollision([[maybe_unused]] Collider* other) override;
	WorldTransform& GetWorldTransform() { return worldTransform_; }
	void Item();
	bool GetIsGetItem() { return isGetItem_; }

private:
	WorldTransform worldTransform_;

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

	//プレイヤーの身長
	const float playerHeight_ = 1.0f;
	//アイテム獲得フラグ
	bool isGetItem_ = false;

};
