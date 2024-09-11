#pragma once
#include "Sprite.h"
#include "memory"

enum KindAni {
	ADMISSION,
	EXIT,
};

class Gradation {
public:
	Gradation();
	~Gradation();

	void Initialize(KindAni which);
	void Update(int AnimationFrame, int kAnimationFrame);
	void Draw(ID3D12GraphicsCommandList* commandList);

public: // セッター
	void SetIsDraw(bool isDraw) { isDraw_ = isDraw; }

private:
	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;
	// テクスチャ
	uint32_t textureHandle_;
	// 透明度
	float alpha_;
	// インかアウトか
	KindAni which_;
	// 描画していいか判断
	bool isDraw_ = false;
};
