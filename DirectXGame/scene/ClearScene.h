#pragma once
#include "BaseScene.h"
#include "Audio.h"
#include "DirectXCommon.h"
#include "Model.h"
#include "Sprite.h"
#include "Input.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <memory>

using namespace std;

/// <summary>
/// クリアシーン
/// </summary>
class ClearScene : public BaseScene {
public: // メンバ関数
	ClearScene();
	~ClearScene();

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
	// ビューポート
	ViewProjection viewProjection_;
	// テクスチャ
	uint32_t textureHandleClearRogo_;
	uint32_t textureHandleSpaceUI_;
	uint32_t textureHandleBack_;
	// スプライト
	unique_ptr<Sprite> spriteClearRogo_ = nullptr;
	unique_ptr<Sprite> spriteSpaceUI_ = nullptr;
	unique_ptr<Sprite> spriteBack_ = nullptr;

};
