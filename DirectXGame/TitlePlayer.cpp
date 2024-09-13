#include "TitlePlayer.h"
#include "ImGuiManager.h"
#include "Script/Matrix.h"
#include "Script/MyTools.h"
#include "TextureManager.h"
#include "WinApp.h"

TitlePlayer::TitlePlayer() {}

TitlePlayer::~TitlePlayer() {}

void TitlePlayer::Initialize(Input* input) {
	// インプット
	input_ = input;
	// ワールドトランスフォーム親子付け
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformLeftArm_.parent_ = &worldTransformBody_;
	worldTransformRightArm_.parent_ = &worldTransformBody_;

	// ワールドトランスフォーム初期化
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformLeftArm_.Initialize();
	worldTransformRightArm_.Initialize();

	worldTransformBody_.translation_ = positionBody_;
	worldTransformHead_.translation_ = positionHead_;
	worldTransformLeftArm_.translation_ = positionLeftArm_;
	worldTransformRightArm_.translation_ = positionRightArm_;

	// スプライト
	textureHandleSpaceUI_ = TextureManager::Load("Title/SpaceUI.png");
	spriteSpaceUI_.reset(Sprite::Create(textureHandleSpaceUI_, Vector2(0.0f, 0.0f), Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	// モデル
	modelBody_.reset(Model::CreateFromOBJ("player/float_Body"));
	modelHead_.reset(Model::CreateFromOBJ("player/float_Head"));
	modelLeftArm_.reset(Model::CreateFromOBJ("player/float_L_arm"));
	modelRightArm_.reset(Model::CreateFromOBJ("player/float_R_arm"));
}

void TitlePlayer::Update(ViewProjection& viewProjection) {
	// 移動処理
	velocity_ = {0.0f, 0.0f, 0.0f};
	if (!isStart_) {
		if (input_->PushKey(DIK_W) && input_->PushKey(DIK_A) && input_->PushKey(DIK_S) && input_->PushKey(DIK_D)) {
		} else if (input_->PushKey(DIK_W) && input_->PushKey(DIK_D) && input_->PushKey(DIK_S)) {
			velocity_.x = playerSpeed_;
		} else if (input_->PushKey(DIK_D) && input_->PushKey(DIK_S) && input_->PushKey(DIK_W)) {
			velocity_.z = -playerSpeed_;
		} else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_A) && input_->PushKey(DIK_W)) {
			velocity_.x = -playerSpeed_;
		} else if (input_->PushKey(DIK_A) && input_->PushKey(DIK_W) && input_->PushKey(DIK_D)) {
			velocity_.z = playerSpeed_;
		} else if (input_->PushKey(DIK_W) && input_->PushKey(DIK_D)) {
			velocity_.z = playerSpeed_;
			velocity_.x = playerSpeed_;
		} else if (input_->PushKey(DIK_D) && input_->PushKey(DIK_S)) {
			velocity_.z = -playerSpeed_;
			velocity_.x = playerSpeed_;
		} else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_A)) {
			velocity_.z = -playerSpeed_;
			velocity_.x = -playerSpeed_;
		} else if (input_->PushKey(DIK_A) && input_->PushKey(DIK_W)) {
			velocity_.z = playerSpeed_;
			velocity_.x = -playerSpeed_;
		} else if (input_->PushKey(DIK_W)) {
			velocity_.z = playerSpeed_;
		} else if (input_->PushKey(DIK_A)) {
			velocity_.x = -playerSpeed_;
		} else if (input_->PushKey(DIK_S)) {
			velocity_.z = -playerSpeed_;
		} else if (input_->PushKey(DIK_D)) {
			velocity_.x = playerSpeed_;
		} else {
			isNotInput_ = true;
		}
	}

	// 落下可能範囲判断
	if (worldTransformBody_.translation_.x < 5.5f && worldTransformBody_.translation_.x > -5.5f && worldTransformBody_.translation_.z < 5.5f && worldTransformBody_.translation_.z > -5.5f) {
		isReady_ = true;
	} else {
		isReady_ = false;
	}

	// 落下アニメーション始動
	if (isReady_) {
		if (!isStart_) {
			if (input_->TriggerKey(DIK_SPACE)) {
				isStart_ = true;
				isReady_ = false;
			}
		}
	}

	// 速度加算
	worldTransformBody_.translation_ += velocity_;
	// 移動制限
	worldTransformBody_.translation_.x = max(worldTransformBody_.translation_.x, -9.0f);
	worldTransformBody_.translation_.z = max(worldTransformBody_.translation_.z, -9.0f);
	worldTransformBody_.translation_.x = min(worldTransformBody_.translation_.x, 9.0f);
	worldTransformBody_.translation_.z = min(worldTransformBody_.translation_.z, 9.0f);
	if (worldTransformBody_.translation_.x < 4.5f && worldTransformBody_.translation_.x > -4.5f && worldTransformBody_.translation_.z < 4.5f && worldTransformBody_.translation_.z > -4.5f) {
		worldTransformBody_.translation_ -= velocity_;
	}

	// プレイヤーの向きによって方向転換
	if (isNotInput_) {
		isNotInput_ = false;
	} else {
		worldTransformBody_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
		Matrix4x4 rotateYMatrix = Matrix::MakeRotateYMatrix4x4(-worldTransformBody_.rotation_.y);
		Vector3 velocityZ = Matrix::Transform(velocity_, rotateYMatrix);
		worldTransformBody_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);
	}
	if (isStart_) {
		Vector3 direction_;
		direction_.x = 0.0f - worldTransformBody_.translation_.x;
		direction_.y = 0.05f - worldTransformBody_.translation_.y;
		direction_.z = 0.0f - worldTransformBody_.translation_.z;
		worldTransformBody_.rotation_.y = std::atan2(direction_.x, direction_.z);
		Matrix4x4 rotateYMatrix = Matrix::MakeRotateYMatrix4x4(-worldTransformBody_.rotation_.y);
		Vector3 velocityZ = Matrix::Transform(direction_, rotateYMatrix);
		worldTransformBody_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);
	}

	// ワールドトランスフォーム転送
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformLeftArm_.UpdateMatrix();
	worldTransformRightArm_.UpdateMatrix();
	// 頭のスクリーン座標を獲得
	if (!isStart_) {
		if (isReady_) {
			Matrix4x4 matViewport = Matrix::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
			Matrix4x4 matViewProjectionViewport = Matrix::Multiply(Matrix::Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
			Vector3 playerHeadScreenPos3D = Matrix::Transform(GetWorldPositionHead(), matViewProjectionViewport);
			Vector2 playerHeadScreenPos2D = {playerHeadScreenPos3D.x, playerHeadScreenPos3D.y};

			// SpaceUIの表示位置を決定
			spriteSpaceUI_->SetPosition({playerHeadScreenPos2D.x, playerHeadScreenPos2D.y - 70});
		}
	}

#ifdef _DEBUG
	ImGui::Begin("titlePlayer");
	ImGui::DragFloat3("posBody", &positionBody_.x, 0.01f);
	ImGui::DragFloat3("posHead", &positionHead_.x, 0.01f);
	ImGui::DragFloat3("posLeftArm", &positionLeftArm_.x, 0.01f);
	ImGui::DragFloat3("posRightArm", &positionRightArm_.x, 0.01f);
	ImGui::End();
#endif // _DEBUG
}

void TitlePlayer::Draw(const ViewProjection& viewProjection) {
	modelBody_->Draw(worldTransformBody_, viewProjection);
	modelHead_->Draw(worldTransformHead_, viewProjection);
	modelLeftArm_->Draw(worldTransformLeftArm_, viewProjection);
	modelRightArm_->Draw(worldTransformRightArm_, viewProjection);
}

void TitlePlayer::DrawUI() {
	if (isReady_) {
		spriteSpaceUI_->Draw();
	}
}

Vector3 TitlePlayer::GetWorldPositionHead() {
	Vector3 result;

	result.x = worldTransformHead_.matWorld_.m[3][0];
	result.y = worldTransformHead_.matWorld_.m[3][1];
	result.z = worldTransformHead_.matWorld_.m[3][2];

	return result;
}
