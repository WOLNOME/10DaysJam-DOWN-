#pragma once
#include "Input.h"
#include "Sprite.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <memory>

class TitlePlayer {
public:
	TitlePlayer();
	~TitlePlayer();

	void Initialize(Input* input);
	void Update(ViewProjection& viewProjection);
	void Draw(const ViewProjection& viewProjection);
	void DrawUI();

public://関数
	Vector3 GetWorldPositionHead();

public://ゲッター
	bool GetIsStart() { return isStart_; }
	WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }

public://セッター
private:
	// インプット
	Input* input_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformLeftArm_;
	WorldTransform worldTransformRightArm_;

	//スプライト
	uint32_t textureHandleSpaceUI_;
	std::unique_ptr<Sprite> spriteSpaceUI_ = nullptr;
	// モデル
	std::unique_ptr<Model> modelBody_ = nullptr;
	std::unique_ptr<Model> modelHead_ = nullptr;
	std::unique_ptr<Model> modelLeftArm_ = nullptr;
	std::unique_ptr<Model> modelRightArm_ = nullptr;

	Vector3 positionBody_ = {0.19f, 0.05f, -7.0f};
	Vector3 positionHead_ = {0.0f, 1.5f, 0.0f};
	Vector3 positionLeftArm_ = {-0.65f, 1.1f, 0.0f};
	Vector3 positionRightArm_ = {0.65f, 1.1f, 0.0f};

	Vector3 velocity_ = {0.0f, 0.0f, 0.0f};
	bool isNotInput_ = false;
	bool isReady_ = false;
	bool isStart_ = false;
	const float playerSpeed_ = 5.0f * (1.0f / 60.0f);


};
