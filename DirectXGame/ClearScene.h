#pragma once
#include "BaseScene.h"
#include "Audio.h"
#include "WorldTransform.h"
#include "DirectXCommon.h"
#include "Model.h"
#include "Sprite.h"
#include "Input.h"
#include "ViewProjection.h"
#include "memory"

using namespace std;

class ClearScene : public BaseScene {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	ClearScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ClearScene() override;

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

public:

public: // ゲッター
	SCENE GetNextScene() override { return NextScene; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	// 入力
	Input* input_ = nullptr;
	// 音
	Audio* audio_ = nullptr;
	// ビューポート
	
	// オブジェクト用ワールドトランスフォーム
	
	// スプライト
	uint32_t textureHandleGameClear_;
	unique_ptr<Sprite> spriteGameClear_ = nullptr;

	// モデル
	
	// インスタンス
	

	// コリジョンマネージャー
	

private:
};
