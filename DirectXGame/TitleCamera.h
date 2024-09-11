#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

class TitleCamera {
public:
	TitleCamera();
	~TitleCamera();

	void Initialize();
	void Update();

public://関数
	void Transition(WorldTransform& worldTransform,int timer,int kTime);
	void LookHole(int timer, int kTime);
	void JumpIn(int timer);
	void MoveScene();

public: // ゲッター
	const ViewProjection& GetViewProjection() { return viewProjection_; }

public://セッター

private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//座標
	Vector3 position_ = {0.0f,0.0f,0.0f};
	Vector3 startPosition_;
	Vector3 goalPosition_;
	//角度
	Vector3 rotation_ = {0.0f,0.0f,0.0f};
	Vector3 startRotation_;
	Vector3 goalRotation_;

	//重力
	const float kGravity_ = 0.04f;
	//ジャンプ量
	const float kJumpHeight_ = 1.0f;
	Vector3 velocity_{0.0f, 0.0f, 0.0f};
	//平行移動量
	const float kSpeed = 2.0f * (1.0f / 60.0f);
	//下向きタイマー
	int lookUnderTimer_ = 0;
	//下向き時間
	const int kLookUnderTimer_ = 30;


};
