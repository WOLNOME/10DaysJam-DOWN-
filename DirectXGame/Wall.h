#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Function.h"
#include <memory>

class Wall {
public:
	Wall();
	~Wall();

	void Initialize();
	void Update();
	void Draw(const ViewProjection& viewProjection);

public://関数
public://ゲッター
public://セッター

private://変数
	//ワールドトランスフォーム
	WorldTransform worldTransformWall_[2];
	WorldTransform worldTransformBlack_;
	//モデル
	std::unique_ptr<Model> modelWall_[2];
	std::unique_ptr<Model> modelBlack_ = nullptr;

private:
	//壁の長さ
	const float kWallLength_ = 3200.0f;
	//落下速度
	Vector3 fallVelocity_; 

};
