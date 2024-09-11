#pragma once
#include "Audio.h"
#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Model.h"
#include "Sprite.h"
#include "Input.h"
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

	// スプライト
	uint32_t textureHandleTitle_;
	std::unique_ptr<Sprite> spriteTitle_ = nullptr;

	// モデル

	// インスタンス
};
