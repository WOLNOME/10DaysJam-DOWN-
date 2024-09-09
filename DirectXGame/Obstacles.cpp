#include "Obstacles.h"
#include "GameScene.h"

Obstacles::Obstacles() {}

Obstacles::~Obstacles() {}

void Obstacles::Initialize() {
	//ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	//座標初期化
	worldTransform_.translation_ = {0.0f, -gameScene_->GetWall()->GetKWallLength(), 0.0f};
	//モデル生成
	modelObstacles_.reset(Model::CreateFromOBJ("obstacles"));


}

void Obstacles::Update() {
	//生成
	if (gameScene_->GetWall()->GetPhaseTimer() >= (gameScene_->GetWall()->GetKPhaseTime() / 2)) {
		worldTransform_.translation_.y += gameScene_->GetWall()->GetFallingSpeed();
	}


}

void Obstacles::Draw(const ViewProjection& viewProjection) { 
	modelObstacles_->Draw(worldTransform_, viewProjection); 
}
