#pragma once
#include "collider.h"
#include <Input.h>
#include "Model.h"
#include "WinApp.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "Sprite.h"
#include "Script/MyTools.h"
#include "Script/Matrix.h"


class RestItem : public Collider {
public:
	RestItem();
	~RestItem() override;

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

public:
	void OnCollision([[maybe_unused]] Collider* other) override;
	WorldTransform& GetWorldTransform() { return worldTransform_; }
	
private:
	WorldTransform worldTransform_;
};
