#pragma once
#include <Input.h>
#include <algorithm>
#include "Model.h"
#include "WinApp.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "Sprite.h"
#include "MyTools.h"
#include "Matrix.h"
#include "BaseCharacter.h"

class Player : public BaseCharacter {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデル</param>
	void Initialize(const std::vector<Model*> models);

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

private:
	WorldTransform worldTransform3DReticle_;

	Sprite* sprite2DReticle_ = nullptr;

	// キーボード入力
	Input* input_ = nullptr;

	// 自キャラの速さ
	float kCharacterSpeed = 0.5f;

	// カーソルの移動量
	Vector2 mouseMove = {0.0f, 0.0f};
	Vector2 mousePosPre;
	const Vector2 mouseCenter = {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2};
	Vector2 mousePos;

	bool isDebug_ = false;


};
