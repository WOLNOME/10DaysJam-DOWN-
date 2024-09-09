#include "Wall.h"
#include "Script/MyTools.h"

Wall::Wall() {}

Wall::~Wall() {}

void Wall::Initialize() {
	for (int i = 0; i < 2; i++) {
		// ワールドトランスフォーム初期化
		worldTransformWall_[i].Initialize();
		// モデル生成
		modelWall_[i].reset(Model::CreateFromOBJ("wall"));
	}
	// ワールドトランスフォーム初期化
	worldTransformBlack_.Initialize();
	worldTransformPhaseGround_.Initialize();
	// モデル生成
	modelBlack_.reset(Model::CreateFromOBJ("plane"));
	modelPhaseGround_.reset(Model::CreateFromOBJ("phaseGround"));
	// 初期位置
	worldTransformWall_[0].translation_ = {0.0f, 0.0f, 0.0f};
	worldTransformWall_[1].translation_ = {0.0f, -kWallLength_ + 1, 0.0f};
	worldTransformBlack_.translation_ = {0.0f, -(kWallLength_ - 50), 0.0f};
	worldTransformPhaseGround_.translation_ = {0.0f, -kWallLength_, 0.0f};
	// 変数の初期化
	fallVelocity_ = {0.0f, 8.0f, 0.0f};
}

void Wall::Update() {
	// タイマーインクリメント
	phaseTimer_++;
	if (phaseTimer_ >= kPhaseTime_) {
		isComingPhaseGround_ = true;
	}
	// 休憩ゾーン処理
	if (isComingPhaseGround_) {
		// 速度加算処理
		worldTransformPhaseGround_.translation_ = MyTools::Add(worldTransformPhaseGround_.translation_, fallVelocity_);
		if (worldTransformPhaseGround_.translation_.y >= -kPlayerHeight_) {
			isComingPhaseGround_ = false;
			isLanding_ = true;
			worldTransformPhaseGround_.translation_.y = -kPlayerHeight_;
		}
	}
	// 壁の処理
	for (int i = 0; i < 2; i++) {
		// 速度加算処理
		if (!isLanding_) {
			worldTransformWall_[i].translation_ = MyTools::Add(worldTransformWall_[i].translation_, fallVelocity_);
		}
		// 座標が450を超えたらループする
		if (worldTransformWall_[i].translation_.y > (kWallLength_ / 2) + 50) {
			worldTransformWall_[i].translation_.y = -((kWallLength_ - 50) + (kWallLength_ / 2)) + fallVelocity_.y;
		}
	}

	// ワールドトランスフォーム更新
	for (int i = 0; i < 2; i++) {
		worldTransformWall_[i].UpdateMatrix();
	}
	worldTransformBlack_.UpdateMatrix();
	worldTransformPhaseGround_.UpdateMatrix();
}

void Wall::Draw(const ViewProjection& viewProjection) {
	for (int i = 0; i < 2; i++) {
		modelWall_[i]->Draw(worldTransformWall_[i], viewProjection);
	}
	modelBlack_->Draw(worldTransformBlack_, viewProjection);
	modelPhaseGround_->Draw(worldTransformPhaseGround_, viewProjection);
}
