#include "WorldTransform.h"

void WorldTransform::UpdateMatrix() {
	// スケール、回転、平行移動を合成して行列を計算する
	matWorld_ = Matrix::MakeAffineMatrix(scale_, rotation_, translation_);

	// 親があれば親のワールド行列を掛ける
	if (parent_) {
		// 掛ける
		matWorld_ = Matrix::Multiply(matWorld_, parent_->matWorld_);
	}

	// 定数バッファに転送
	TransferMatrix();
}
