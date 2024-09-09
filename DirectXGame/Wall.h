#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include <memory>

class Wall {
public:
	Wall();
	~Wall();

	void Initialize();
	void Update();
	void Draw(const ViewProjection& viewProjection);

public://関数

public://ゲッター

public://セッター

private://変数
	//ワールドトランスフォーム
	WorldTransform worldTransformWall_[2];
	WorldTransform worldTransformBlack_;
	WorldTransform worldTransformPhaseGround_;

	//モデル
	std::unique_ptr<Model> modelWall_[2];
	std::unique_ptr<Model> modelBlack_ = nullptr;
	std::unique_ptr<Model> modelPhaseGround_ = nullptr;


private:
	//壁の長さ
	const float kWallLength_ = 3200.0f;
	//落下速度
	Vector3 fallVelocity_;

private://フェーズ処理変数
	//各フェーズの時間
	const int kPhaseTime_ = 60 * 10;
	//フェーズタイマー
	int phaseTimer_ = 0;
	//プレイヤーの身長(カメラと地面の距離)
	const float kPlayerHeight_ = 50.0f;
	//休憩ゾーンが来るフラグ
	bool isComingPhaseGround_ = false;
	//着地したフラグ
	bool isLanding_ = false;
	

};
