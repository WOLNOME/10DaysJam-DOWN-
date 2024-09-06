#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "memory"
#include "vector"
#include "cassert"
#include "collider.h"	
class BaseCharacter : public Collider{
protected:
	std::vector<Model*> models_;
	WorldTransform worldTransform_;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデル</param>
	virtual void Initialize(const std::vector<Model*> models);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	virtual void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// Getter
	/// </summary>
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	virtual Vector3 GetCenter() const override;
};
