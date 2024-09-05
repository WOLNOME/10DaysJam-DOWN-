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
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

private: // 変数
	// 座標
	Vector3 pos_;
};
