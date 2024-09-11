#pragma once
#include "collider.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include <memory>

class GameScene;

class Obstacles : public Collider {
public:
	Obstacles();
	~Obstacles()override;

	void Initialize();
	void Update();
	void Draw(const ViewProjection& viewProjection);

public://関数
	void OnCollision([[maybe_unused]] Collider* other) override;

public://ゲッター
	Vector3 GetCenter() const override;

public://セッター
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//3Dモデル
	std::unique_ptr<Model> modelObstacles_ = nullptr;
	//シーンデータ
	GameScene* gameScene_ = nullptr;
	


};
