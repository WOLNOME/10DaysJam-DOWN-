#pragma once
#include "ColliderUshio.h"
#include <Input.h>
#include "Model.h"
#include "WinApp.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "Sprite.h"
#include "Script/MyTools.h"
#include "Script/Matrix.h"


class RestItem : public ColliderUshio {
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
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(ViewProjection& viewProjection);

public:
	void OnCollision() override;
	void OnSpecialCollision() override;
	Vector3 GetWorldPosition() override;
	WorldTransform& GetWorldTransform() { return worldTransform_; }
	float GetRadius() override { return radius_; }
	bool GetIsDead() { return isDead_; }

private:
	WorldTransform worldTransform_;
	//モデル
	std::unique_ptr<Model> modelHeart_ = nullptr;
	//消滅フラグ
	bool isDead_ = false;
	//初期位置
	const Vector3 prePos_ = {0.0f, 2.5f, 7.0f};
	//速度
	Vector3 velocity_ = {0.0f, 0.7f * (1.0f / 60.0f), 0.0f};
	


};
