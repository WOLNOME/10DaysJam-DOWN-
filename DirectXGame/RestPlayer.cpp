#define _USE_MATH_DEFINES
#include "RestPlayer.h"
#include "CollisionConfigUshio.h"
#include "ImGuiManager.h"
#include "WinApp.h"
#include <cassert>
#include <cmath>

RestPlayer::RestPlayer() {}

RestPlayer::~RestPlayer() {}

void RestPlayer::Initialize() {
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.f, playerHeight_, 0.f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.UpdateMatrix();

	//スプライト
	textureHandleSpaceUI_ = TextureManager::Load("Title/SpaceUI.png");
	spriteSpaceUI_.reset(Sprite::Create(textureHandleSpaceUI_, { WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f + 200}, {1, 1, 1, 1}, {0.5f, 0.5f}));

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	// 半径設定
	radius_ = 0.3f;

	SetCursorPos(int(mouseCenter.x), int(mouseCenter.y));
	SetCollisionAttribute(kCollisionAttributePlayer);
}

void RestPlayer::Update() {
	if (!isGoHole) {
		if (!isDebug_) {
			// マウスによる視点移動
			MouseMove();
		}

		// 移動処理
		Translation();
	} else {
		GoHole();
	}

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_P)) {
		isDebug_ = !isDebug_;
	}
	ShowCursor(isDebug_);

	ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiCond_Once);    // ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_Once); // ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("Player");

	ImGui::DragFloat3("Translation", &worldTransform_.translation_.x, 0.05f);

	const float kMoveLimitX = 9.5f;
	const float kMoveLimitZ = 9.5f;

	// 制限
	worldTransform_.translation_.x = MyTools::Clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.z = MyTools::Clamp(worldTransform_.translation_.z, -kMoveLimitZ, kMoveLimitZ);

	ImGui::DragFloat3("Rotation", &worldTransform_.rotation_.x, 0.01f);

	// 制限
	worldTransform_.rotation_.x = MyTools::Clamp(worldTransform_.rotation_.x, 0.0f, float(M_PI_2));
	// 回転を消す
	worldTransform_.rotation_.z = 0.0f;

	ImGui::DragFloat2("mouseMove", &mouseMove.x, 0.05f);
	ImGui::DragFloat2("mousePos", &mousePos.x, 0.05f);

	ImGui::Text("Push P Key : mouseLock switching ");
	ImGui::Checkbox("Debug", &isDebug_);

	ImGui::End();

#endif // _DEBUG

	// 行列の更新・転送
	worldTransform_.UpdateMatrix();
}

void RestPlayer::Translation() {
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キーボード操作
	// 押した方向で移動ベクトルを変更(左右)
	if (!isGoHole) {
		if (input_->PushKey(DIK_A)) {
			move.x -= kCharacterSpeed;
		} else if (input_->PushKey(DIK_D)) {
			move.x += kCharacterSpeed;
		}

		// 押した方向で移動ベクトルを変更(上下)
		if (input_->PushKey(DIK_W)) {
			move.z += kCharacterSpeed;
		} else if (input_->PushKey(DIK_S)) {
			move.z -= kCharacterSpeed;
		}
	}

	// アイテム獲得後に範囲内なら準備完了
	if (isGetItem_) {
		if (!isGoHole&&worldTransform_.translation_.x < 5.5f && worldTransform_.translation_.x > -5.5f && worldTransform_.translation_.z < 5.5f && worldTransform_.translation_.z > -5.5f) {
			isReady = true;
		} else {
			isReady = false;
		}
	}

	// 準備完了時にスペースキーで穴の中に入る処理
	if (isReady) {
		if (input_->TriggerKey(DIK_SPACE)) {
			isGoHole = true;
			isReady = false;
			isLookHole_ = true;
		}
	}

	// 移動ベクトルの向きを自キャラの向きに合わせる
	Matrix4x4 rotationMatrix = Matrix::MakeRotateMatrix4x4(0.0f, worldTransform_.rotation_.y, worldTransform_.rotation_.z);
	move = Matrix::Multiply(move, rotationMatrix);

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ = MyTools::Add(worldTransform_.translation_, move);

	const float kMoveLimitX = 9.5f;
	const float kMoveLimitZ = 9.5f;

	// 制限
	worldTransform_.translation_.x = MyTools::Clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.z = MyTools::Clamp(worldTransform_.translation_.z, -kMoveLimitZ, kMoveLimitZ);
	if (isGetItem_) {
		if (worldTransform_.translation_.z < 4.5f && worldTransform_.translation_.z > -4.5f) {
			if (worldTransform_.translation_.x < 4.5f && worldTransform_.translation_.x > -4.5f) {
				worldTransform_.translation_.x -= move.x;
			}
		}
		if (worldTransform_.translation_.x < 4.5f && worldTransform_.translation_.x > -4.5f) {
			if (worldTransform_.translation_.z < 4.5f && worldTransform_.translation_.z > -4.5f) {
				worldTransform_.translation_.z -= move.z;
			}
		}
	}

	// 移動を消す
	worldTransform_.translation_.y = playerHeight_;
}

void RestPlayer::GoHole() {
	// のぞくアニメーション
	if (isLookHole_) {
		lookHoleTimer_++;
		if (lookHoleTimer_ == 1) {
			// 最初の向きを保存
			startRotation_ = worldTransform_.rotation_;
			// 最後の向きを保存
			Vector3 direction_ = MyTools::Normalize(MyTools::Subtract({0.0f, -10.0f, 0.0f}, worldTransform_.translation_));
			goalRotation_.y = std::atan2(direction_.x, direction_.z);
			Matrix4x4 rotateYMatrix = Matrix::MakeRotateYMatrix4x4(-goalRotation_.y);
			Vector3 velocityZ = Matrix::Transform(direction_, rotateYMatrix);
			goalRotation_.x = std::atan2(-velocityZ.y, velocityZ.z);
			//
			if (goalRotation_.y >= 0.0f) {
				if (startRotation_.y >= 0.0f) {
					
				} else {
					if (startRotation_.y - goalRotation_.y < (float)M_PI) {
						startRotation_.y = ((float)M_PI * 2.0f) + startRotation_.y;
					}
				}
			} else {
				if (startRotation_.y >= 0.0f) {
					if (startRotation_.y - goalRotation_.y > (float)M_PI) {
						startRotation_.y = (-(float)M_PI * 2.0f) + startRotation_.y;
					}
				} else {

				}
			}


			
		}
		//ラープ処理
		worldTransform_.rotation_ = MyTools::Lerp(startRotation_, goalRotation_, (float)lookHoleTimer_ / kLookHoleTime_);
		//制限時間に達したら
		if (lookHoleTimer_ >= kLookHoleTime_) {
			worldTransform_.rotation_ = goalRotation_;
			lookHoleTimer_ = 0;
			isLookHole_ = false;
			isInHole_ = true;
		}
	}
	//穴に入るアニメーション
	if (isInHole_) {
		inHoleTimer_++;
		if (inHoleTimer_ == 1) {
			//速度を自機の向きに合わせる
			Vector3 direction = {0.0f, 0.0f, 0.0f};
			direction.z = forwardSpeed_;
			Matrix4x4 rotationMatrix = Matrix::MakeRotateMatrix4x4(0.0f, worldTransform_.rotation_.y, 0.0f);
			direction = Matrix::Multiply(direction, rotationMatrix);
			velocity_ = {direction.x, kJumpHeight_, direction.z};

		}
		//重力加算
		velocity_.y += -kGravity_;
		//移動処理
		worldTransform_.translation_ = MyTools::Add(worldTransform_.translation_, velocity_);
		//制限時間に達したら
		if (inHoleTimer_ >= kInHoleTime_) {
			isInHole_ = false;
			isTransition_ = true;
			inHoleTimer_ = 0;
		}
	}
	//遷移中処理
	if (isTransition_) {

	}
}

void RestPlayer::Rotate() {
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	// 押した方向でベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.z += kRotSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.z -= kRotSpeed;
	}
}

void RestPlayer::MouseMove() {
	// マウス座標（スクリーン座標）を取得
	POINT mouse;
	GetCursorPos(&mouse);
	mousePos = {float(mouse.x), float(mouse.y)};

	mouseMove = {0.0f, 0.0f};
	mouseMove = MyTools::Normalize(float(mousePos.x) - WinApp::kWindowWidth / 2, float(mousePos.y) - WinApp::kWindowHeight / 2) * (3.0f / 60.0f);

	SetCursorPos(int(mouseCenter.x), int(mouseCenter.y));

	worldTransform_.rotation_.x += mouseMove.y;
	worldTransform_.rotation_.y += mouseMove.x;
	if (worldTransform_.rotation_.y > (float)M_PI * 2) {
		worldTransform_.rotation_.y -= (float)M_PI * 2;
	}
	if (worldTransform_.rotation_.y < -(float)M_PI * 2) {
		worldTransform_.rotation_.y += (float)M_PI * 2;
	}

	// 制限
	worldTransform_.rotation_.x = MyTools::Clamp(worldTransform_.rotation_.x, 0.0f, float(M_PI_2));
}

void RestPlayer::Draw(ViewProjection& viewProjection) { viewProjection; }

void RestPlayer::DrawUI() { 
	if (isReady) {
		spriteSpaceUI_->Draw();
	}
}

void RestPlayer::OnCollision() {}

void RestPlayer::OnSpecialCollision() { isGetItem_ = true; }

Vector3 RestPlayer::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
