#include "Gradation.h"
#include "TextureManager.h"
#include "WinApp.h"

Gradation::Gradation() {}

Gradation::~Gradation() {}

void Gradation::Initialize(KindAni which) {
	// テクスチャハンドル
	textureHandle_ = TextureManager::Load("black.png");
	// スプライト生成
	sprite_.reset(Sprite::Create(textureHandle_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	// インかアウトか
	which_ = which;
	// 透明度
	switch (which_) {
	case ADMISSION:
		alpha_ = 0.0f;
		break;
	case EXIT:
		alpha_ = 1.0f;
		break;
	default:
		break;
	}
}

void Gradation::Update(int AnimationFrame, int kAnimationFrame) {
	switch (which_) {
	case ADMISSION:
		// 徐々に暗く
		alpha_ = 1.0f * ((float)AnimationFrame / kAnimationFrame);
		// 最後の5fは一番暗く
		if (AnimationFrame >= kAnimationFrame - 5) {
			alpha_ = 1.0f;
		}
		// スプライトの透明度再設定
		sprite_->SetColor({1.0f, 1.0f, 1.0f, alpha_});
		break;
	case EXIT:
		// 徐々に明るく
		alpha_ = 1.0f - 1.0f * ((float)AnimationFrame / kAnimationFrame);
		// 最初の5fは暗く
		if (AnimationFrame <= 5) {
			alpha_ = 1.0f;
		}
		// スプライトの透明度再設定
		sprite_->SetColor({1.0f, 1.0f, 1.0f, alpha_});
		break;
	default:
		break;
	}
}

void Gradation::Draw(ID3D12GraphicsCommandList* commandList) {
	if (isDraw_) {
#pragma region 前景スプライト描画
		// 前景スプライト描画前処理
		Sprite::PreDraw(commandList);

		/// <summary>
		/// ここに前景スプライトの描画処理を追加できる
		/// </summary>
		sprite_->Draw();

		// スプライト描画後処理
		Sprite::PostDraw();

#pragma endregion
	}
}
