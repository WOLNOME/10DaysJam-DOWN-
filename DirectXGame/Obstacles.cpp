#include "Obstacles.h"
#include "GameScene.h"
#include "ImGuiManager.h"

Obstacles::Obstacles() {}

Obstacles::~Obstacles() {}

void Obstacles::Initialize() {
	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	// 座標初期化
	worldTransform_.translation_ = {0.0f, -gameScene_->GetWall()->GetKWallLength(), 0.0f};
	// モデル生成
	modelObstacles_.reset(Model::CreateFromOBJ("obstacles"));
}

void Obstacles::Update() {
	// 出現
	if (gameScene_->GetWall()->GetPhaseTimer() >= 60 * 3) {
		// 速度加算
		worldTransform_.translation_.y += gameScene_->GetWall()->GetFallingSpeed();
		if (worldTransform_.translation_.y >= 50) {
			worldTransform_.translation_ = {0.0f, -gameScene_->GetWall()->GetKWallLength(), 0.0f};
			worldTransform_.rotation_.y += 0.5f;
		}
	}

	worldTransform_.UpdateMatrix();

#ifdef _DEBUG
	ImGui::Begin(" ");
	ImGui::Text("height : %f", worldTransform_.translation_.y);
	ImGui::End();

#endif // _DEBUG
}

void Obstacles::Draw(const ViewProjection& viewProjection) { modelObstacles_->Draw(worldTransform_, viewProjection); }

void Obstacles::OnCollision(Collider* other) { other; }

Vector3 Obstacles::GetCenter() const {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
