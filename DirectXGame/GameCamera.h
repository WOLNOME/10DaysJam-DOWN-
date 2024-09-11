#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "memory"

class GameCamera {
public:
	GameCamera();
	~GameCamera();

	void Initialize();
	void Update();

public: // 関数

public: // ゲッター
	const ViewProjection& GetViewProjection() { return viewProjection_; }

public: // セッター
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

private: // 変数
	// 座標
	Vector3 pos_;
};
