#pragma once
#include <Input.h>
#include "Model.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "MyTools.h"
#include "BaseCharacter.h"

class Player : public BaseCharacter {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(const std::vector<Model*> models, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 移動処理
	/// </summary>
	void Translation();

	/// <summary>
	/// 旋回処理
	/// </summary>
	void Rotate();

	/// <summary>
	/// 中心座標の取得
	/// </summary>
	Vector3 GetCenter() const override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(ViewProjection& viewProjection);

private:
	// テクスチャハンドル
	uint32_t textureHandle_;
	// キーボード入力
	Input* input_ = nullptr;

	// 自キャラの速さ
	float kCharacterSpeed = 0.2f;

};
