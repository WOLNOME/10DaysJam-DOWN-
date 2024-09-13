#pragma once
#include "Audio.h"
#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Model.h"
#include "Sprite.h"
#include "Input.h"
#include "ViewProjection.h"
#include "TitleCamera.h"
#include "TitlePlayer.h"
#include <memory>

using namespace std;

/// <summary>
/// ゲームシーン
/// </summary>
class TitleScene : public BaseScene {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Input* input, Audio* audio) override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

public: // ゲッター
	SCENE GetNextScene() override { return NextScene; }
	
private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	// 入力
	Input* input_ = nullptr;
	// 音
	Audio* audio_ = nullptr;
	//ビューポート
	ViewProjection viewProjection_;
	// オブジェクト用ワールドトランスフォーム
	WorldTransform worldTransformObject_;
	// スプライト
	uint32_t textureHandleTitleRogo_;
	unique_ptr<Sprite> spriteTitleRogo_ = nullptr;

	// モデル
	unique_ptr<Model> modelObject_ = nullptr;
	// インスタンス
	unique_ptr<TitleCamera> titleCamera_ = nullptr;
	unique_ptr<TitlePlayer> titlePlayer_ = nullptr;

private:
	///アニメーション
	//カメラ移動
	bool isCameraTransition_ = false;
	int cameraTransitionTimer_ = 0;
	const int kCameraTransitionTime_ = 100;
	//深淵のぞく
	bool isLookHole_ = false;
	int lookHoleTimer_ = 0;
	const int kLookHoleTime_ = 50;

	//ジャンプイン
	bool isJumpIn_ = false;
	int jumpInTimer_ = 0;
	const int kJumpInTime_ = 60;

	//遷移中
	bool isMoveScene = false;

};
