#pragma once
#include "Audio.h"
#include "BaseScene.h"
#include "WorldTransform.h"
#include "DirectXCommon.h"
#include "Model.h"
#include "Sprite.h"
#include "Input.h"
#include "ViewProjection.h"
#include "RestPlayer.h"
#include "RestCamera.h"
#include "memory"

using namespace std;

class RestScene : public BaseScene {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	RestScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~RestScene() override;

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
	// オブジェクト用ワールドトランスフォーム
	WorldTransform worldTransformBeforeObject_;
	WorldTransform worldTransformAfterObject_;
	// スプライト
	
	// モデル
	unique_ptr<Model> modelBeforeObject_ = nullptr;
	unique_ptr<Model> modelAfterObject_ = nullptr;


	// インスタンス
	unique_ptr<RestCamera> restCamera_ = nullptr;
	unique_ptr<RestPlayer> restPlayer_ = nullptr;


private:
	//強化済みフラグ
	bool isEnhanced = false;

};
