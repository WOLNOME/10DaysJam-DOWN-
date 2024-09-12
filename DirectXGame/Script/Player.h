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
#include "collisionTypeIdDef.h"

/// Wallの前方宣言
class Wall;

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
	/// 壁をセット
	/// </summary>
	/// <param name="wall"></param>
	void SetWall(Wall* wall) { wall_ = wall; }

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

	// 壁
	Wall* wall_ = nullptr;

	// 自キャラの速さ
	float kCharacterSpeed_;

	// カーソルの移動量
	Vector2 mouseMove_;
	Vector2 mousePosPre_;
	const Vector2 mouseCenter_ = {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2};
	Vector2 mousePos_;

	bool isDebug_ = false;

	// 落下速度
	Vector3 fallingVelocity_;
	Vector3 fallingVelocityJet_;
};
