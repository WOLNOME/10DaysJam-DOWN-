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
	//現在の時間
	int GetPhaseTimer() { return phaseTimer_; }
	//1フェーズの時間
	int GetKPhaseTime() { return kPhaseTime_; }
	//壁の長さ
	float GetKWallLength() { return kWallLength_; }
	//落下速度
	float GetFallingSpeed() { return fallVelocity_.y; }
	//着地判定
	bool GetIsLanding() { return isLanding_; }

public://セッター
	void SetFallVelocity(const Vector3& velocity) { fallVelocity_ = velocity; }

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
	Vector3 fallVelocity_ = {0.0f, 5.0f, 0.0f};

private://フェーズ処理変数
	//各フェーズの時間
	const int kPhaseTime_ = 60 * 30;
	//フェーズタイマー
	int phaseTimer_ = 0;
	//プレイヤーの身長(カメラと地面の距離)
	const float kPlayerHeight_ = 10.0f;
	//休憩ゾーンが来るフラグ
	bool isComingPhaseGround_ = false;
	//着地したフラグ
	bool isLanding_ = false;
	

};
