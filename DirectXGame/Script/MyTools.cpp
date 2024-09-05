#include "MyTools.h"
#include "Matrix.h"
#include <algorithm>
#include <iostream>

using namespace std;

/// 
/// ツール関数 ここから
/// 

/// マウスカーソルの座標を取得する関数
Vector3 MyTools::GetMousePosition(const float& kWindowWidth, const float& kWindowHeight, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix)
{
	int x = 0, y = 0;

	// クリップ座標に変換
	float clipX = (2.0f * float(x)) / kWindowWidth - 1.0f;
	float clipY = 1.0f - (2.0f * float(y)) / kWindowHeight;
	float clipZ = 0.0f; // スクリーン座標からは深度情報が得られないので適当な値を指定します

	// クリップ座標をカメラ座標に逆変換
	Vector3 eyeCoords = Matrix::Transform(Vector3(clipX, clipY, clipZ), Matrix::Inverse(projectionMatrix));

	// カメラ座標をワールド座標に逆変換
	Vector3 worldCoords = Matrix::Transform(eyeCoords, Matrix::Inverse(viewMatrix));

	return worldCoords;
}

/// 範囲内の値を返す関数
float MyTools::Clamp(const float& num, const float& min, const float& max)
{
	if (num < min)
	{
		return min;
	}
	else if (num > max)
	{
		return max;
	}
	else
	{
		return num;
	}
}

/// 線形補間
float MyTools::Lerp(const float& num1, const float& num2, const float& t)
{ 
	return num1 * (1 - t) + num2 * t;
}

/// 三角形の存在する平面情報を求める関数
MyTools::Plane MyTools::TriangleToPlane(const Triangle& triangle)
{
	// ベクトルv1,v2を求める
	Vector3 v1 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v2 = Subtract(triangle.vertices[2], triangle.vertices[1]);

	// 法線nを算出
	Vector3 n = Normalize(Cross(v1, v2));

	// 距離を求める
	float d = Dot(triangle.vertices[0], n);

	return Plane{ n, d };
}

/// 球と球の衝突判定を返す関数
bool MyTools::IsCollision(const Sphere& sphere1, const Sphere& sphere2)
{
	// 2つの球の中心点間の距離を求める
	float distance = Length(Subtract(sphere2.center, sphere1.center));

	// 半径の合計よりも短ければ衝突
	if (distance <= sphere1.radius + sphere2.radius) {
		return true;
	}

	return false;
}

/// 球と平面の衝突判定を返す関数
bool MyTools::IsCollision(const Sphere& sphere, const Plane& plane)
{
	// 平面と球の中心点との距離
	float k = Dot(plane.normal, sphere.center) - plane.distance;
	// 絶対値
	if (k < 0) { k *= -1.f; };

	if (k <= sphere.radius)
	{
		return true;
	}

	return false;
}

/// 直線と平面の衝突判定を返す関数
bool MyTools::IsCollision(const Line& line, const Plane& plane)
{
	// まず垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, line.diff);

	// 垂直=並行であるので、衝突しているはずがない
	if (dot == 0)
	{
		return false;
	}

	// tを求める
	//float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;

	// tの値と線の種類によって衝突しているかを判断する
	return true;
}

/// 半直線と平面の衝突判定を返す関数
bool MyTools::IsCollision(const Ray& ray, const Plane& plane)
{
	// まず垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, ray.diff);

	// 垂直=並行であるので、衝突しているはずがない
	if (dot == 0)
	{
		return false;
	}

	// tを求める
	float t = (plane.distance - Dot(ray.origin, plane.normal)) / dot;

	// tの値と線の種類によって衝突しているかを判断する
	if (t > 0.f)
	{
		return true;
	}
	return false;
}

/// 線分と平面の衝突判定を返す関数
bool MyTools::IsCollision(const Segment& segment, const Plane& plane)
{
	// まず垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, segment.diff);

	// 垂直=並行であるので、衝突しているはずがない
	if (dot == 0)
	{
		return false;
	}

	// tを求める
	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;

	// tの値と線の種類によって衝突しているかを判断する
	if (t >= 0.f && t <= 1.f)
	{
		return true;
	}
	return false;
}

/// 三角形と直線の衝突判定を返す関数
bool MyTools::IsCollision(const Triangle& triangle, const Line& line)
{
	// 三角形の存在する平面を求める
	Plane plane = TriangleToPlane(triangle);

	// 線と平面との衝突判定を行う
	if (IsCollision(line, plane))
	{
		// 衝突点を求める
		float dot = Dot(plane.normal, line.diff);
		float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;
		Vector3 p = Add(line.origin, Multiply(t, line.diff));

		// 各辺を結んだベクトルと、頂点と衝突点pを結んだベクトルのクロス積を取る
		Vector3 cross01 = Cross(Subtract(triangle.vertices[1], triangle.vertices[0]), Subtract(p, triangle.vertices[1]));
		Vector3 cross12 = Cross(Subtract(triangle.vertices[2], triangle.vertices[1]), Subtract(p, triangle.vertices[2]));
		Vector3 cross20 = Cross(Subtract(triangle.vertices[0], triangle.vertices[2]), Subtract(p, triangle.vertices[0]));

		// すべての小三角形のクロス積と法線が同じ方向を向いていたら衝突
		if (Dot(cross01, plane.normal) >= 0.0f &&
			Dot(cross12, plane.normal) >= 0.0f &&
			Dot(cross20, plane.normal) >= 0.0f)
		{
			return true;
		}
	}
	return false;
}

/// 三角形と半直線の衝突判定を返す関数
bool MyTools::IsCollision(const Triangle& triangle, const Ray& ray)
{
	// 三角形の存在する平面を求める
	Plane plane = TriangleToPlane(triangle);

	// 線と平面との衝突判定を行う
	if (IsCollision(ray, plane))
	{
		// 衝突点を求める
		float dot = Dot(plane.normal, ray.diff);
		float t = (plane.distance - Dot(ray.origin, plane.normal)) / dot;
		Vector3 p = Add(ray.origin, Multiply(t, ray.diff));

		// 各辺を結んだベクトルと、頂点と衝突点pを結んだベクトルのクロス積を取る
		Vector3 cross01 = Cross(Subtract(triangle.vertices[1], triangle.vertices[0]), Subtract(p, triangle.vertices[1]));
		Vector3 cross12 = Cross(Subtract(triangle.vertices[2], triangle.vertices[1]), Subtract(p, triangle.vertices[2]));
		Vector3 cross20 = Cross(Subtract(triangle.vertices[0], triangle.vertices[2]), Subtract(p, triangle.vertices[0]));

		// すべての小三角形のクロス積と法線が同じ方向を向いていたら衝突
		if (Dot(cross01, plane.normal) >= 0.0f &&
			Dot(cross12, plane.normal) >= 0.0f &&
			Dot(cross20, plane.normal) >= 0.0f)
		{
			return true;
		}
	}
	return false;
}

/// 三角形と線分の衝突判定を返す関数
bool MyTools::IsCollision(const Triangle& triangle, const Segment& segment)
{
	// 三角形の存在する平面を求める
	Plane plane = TriangleToPlane(triangle);

	// 線と平面との衝突判定を行う
	if (IsCollision(segment, plane))
	{
		// 衝突点を求める
		float dot = Dot(plane.normal, segment.diff);
		float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;
		Vector3 p = Add(segment.origin, Multiply(t, segment.diff));

		// 各辺を結んだベクトルと、頂点と衝突点pを結んだベクトルのクロス積を取る
		Vector3 cross01 = Cross(Subtract(triangle.vertices[1], triangle.vertices[0]), Subtract(p, triangle.vertices[1]));
		Vector3 cross12 = Cross(Subtract(triangle.vertices[2], triangle.vertices[1]), Subtract(p, triangle.vertices[2]));
		Vector3 cross20 = Cross(Subtract(triangle.vertices[0], triangle.vertices[2]), Subtract(p, triangle.vertices[0]));

		// すべての小三角形のクロス積と法線が同じ方向を向いていたら衝突
		if (Dot(cross01, plane.normal) >= 0.0f &&
			Dot(cross12, plane.normal) >= 0.0f &&
			Dot(cross20, plane.normal) >= 0.0f)
		{
			return true;
		}
	}
	return false;
}

/// AABB同士の衝突判定を返す関数
bool MyTools::IsCollision(const AABB& aabb1, const AABB& aabb2)
{
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && 
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && 
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z))
	{
		return true;
	}
	return false;
}

/// AABBと球の衝突判定を返す関数
bool MyTools::IsCollision(const AABB& aabb, const Sphere& sphere)
{
	// 最近接点を求める
	Vector3 closestPoint{ 
		std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
		std::clamp(sphere.center.y, aabb.min.y, aabb.max.y), 
		std::clamp(sphere.center.z, aabb.min.z, aabb.max.z) };

	// 最近接点と球の中心との距離を求める
	float distance = Length(Subtract(closestPoint, sphere.center));

	// 距離が半径より小さければ衝突
	if (distance <= sphere.radius)
	{
		// 衝突
		return true;
	}
	return false;
}

/// AABBと直線の衝突判定を返す関数
bool MyTools::IsCollision(const AABB& aabb, const Line& line)
{
	// 近い方(tが小さい)
	float tNearX = min((aabb.min.x - line.origin.x) / line.diff.x, (aabb.max.x - line.origin.x) / line.diff.x);
	float tNearY = min((aabb.min.y - line.origin.y) / line.diff.y, (aabb.max.y - line.origin.y) / line.diff.y);
	float tNearZ = min((aabb.min.z - line.origin.z) / line.diff.z, (aabb.max.z - line.origin.z) / line.diff.z);

	// 遠い方(tが大きい)
	float tFarX = max((aabb.min.x - line.origin.x) / line.diff.x, (aabb.max.x - line.origin.x) / line.diff.x);
	float tFarY = max((aabb.min.y - line.origin.y) / line.diff.y, (aabb.max.y - line.origin.y) / line.diff.y);
	float tFarZ = max((aabb.min.z - line.origin.z) / line.diff.z, (aabb.max.z - line.origin.z) / line.diff.z);

	// AABBとの衝突点(貫通点)のtが小さい方
	float tmin = max(max(tNearX, tNearY), tNearZ);
	// AABBとの衝突点(貫通点)のtが大きい方
	float tmax = min(min(tFarX, tFarY), tFarZ);

	if (tmin <= tmax)
	{
		// 衝突
		return true;
	}
	return false;
}

/// AABBと半直線の衝突判定を返す関数
bool MyTools::IsCollision(const AABB& aabb, const Ray& ray)
{
	// 近い方(tが小さい)
	float tNearX = min((aabb.min.x - ray.origin.x) / ray.diff.x, (aabb.max.x - ray.origin.x) / ray.diff.x);
	float tNearY = min((aabb.min.y - ray.origin.y) / ray.diff.y, (aabb.max.y - ray.origin.y) / ray.diff.y);
	float tNearZ = min((aabb.min.z - ray.origin.z) / ray.diff.z, (aabb.max.z - ray.origin.z) / ray.diff.z);

	// 遠い方(tが大きい)
	float tFarX = max((aabb.min.x - ray.origin.x) / ray.diff.x, (aabb.max.x - ray.origin.x) / ray.diff.x);
	float tFarY = max((aabb.min.y - ray.origin.y) / ray.diff.y, (aabb.max.y - ray.origin.y) / ray.diff.y);
	float tFarZ = max((aabb.min.z - ray.origin.z) / ray.diff.z, (aabb.max.z - ray.origin.z) / ray.diff.z);

	// AABBとの衝突点(貫通点)のtが小さい方
	float tmin = max(max(tNearX, tNearY), tNearZ);
	// AABBとの衝突点(貫通点)のtが大きい方
	float tmax = min(min(tFarX, tFarY), tFarZ);

	if (tmin <= tmax)
	{
		// 衝突
		return true;
	}
	return false;
}

/// AABBと線分の衝突判定を返す関数
bool MyTools::IsCollision(const AABB& aabb, const Segment& segment)
{
	// 近い方(tが小さい)
	float tNearX = min((aabb.min.x - segment.origin.x) / segment.diff.x, (aabb.max.x - segment.origin.x) / segment.diff.x);
	float tNearY = min((aabb.min.y - segment.origin.y) / segment.diff.y, (aabb.max.y - segment.origin.y) / segment.diff.y);
	float tNearZ = min((aabb.min.z - segment.origin.z) / segment.diff.z, (aabb.max.z - segment.origin.z) / segment.diff.z);

	// 遠い方(tが大きい)
	float tFarX = max((aabb.min.x - segment.origin.x) / segment.diff.x, (aabb.max.x - segment.origin.x) / segment.diff.x);
	float tFarY = max((aabb.min.y - segment.origin.y) / segment.diff.y, (aabb.max.y - segment.origin.y) / segment.diff.y);
	float tFarZ = max((aabb.min.z - segment.origin.z) / segment.diff.z, (aabb.max.z - segment.origin.z) / segment.diff.z);

	// AABBとの衝突点(貫通点)のtが小さい方
	float tmin = max(max(tNearX, tNearY), tNearZ);
	// AABBとの衝突点(貫通点)のtが大きい方
	float tmax = min(min(tFarX, tFarY), tFarZ);

	if (tmin <= tmax)
	{
		// 衝突
		return true;
	}
	return false;
}

/// 
/// ツール関数 ここまで
/// 

///
/// 2次元ベクトル ここから
///

/// 2次元ベクトルの内積を返す関数
float MyTools::Dot(const Vector2& v1, const Vector2& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

/// 2次元ベクトルのクロス積(外積)を返す関数
float MyTools::Cross(const Vector2& v1, const Vector2& v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}

/// 2次元ベクトルの長さ(ノルム)を返す関数
float MyTools::Length(float x, float y)
{
	return sqrtf(x * x + y * y);
}

/// 2次元ベクトルの正規化した値を返す関数
Vector2 MyTools::Normalize(float x, float y)
{
	float length = Length(x, y);
	if (length != 0) {
		x = x / length;
		y = y / length;
	}
	return Vector2{ x,y };
}

/// 2次元ベクトルの方向を求める関数
Vector2 MyTools::Direction(float x, float y)
{
	return Normalize(x, y);
}

///
/// 2次元ベクトル ここまで
/// 

///
/// 3次元ベクトル ここから
/// 

/// 3次元ベクトルの加算を返す関数
Vector3 MyTools::Add(const Vector3& v1, const Vector3& v2)
{
	return Vector3{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

/// 3次元ベクトルの減算を返す関数
Vector3 MyTools::Subtract(const Vector3& v1, const Vector3& v2)
{
	return Vector3{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

/// 3次元ベクトルのスカラー倍を返す関数
Vector3 MyTools::Multiply(float scalar, const Vector3& v)
{
	return Vector3{ scalar * v.x, scalar * v.y, scalar * v.z };
}

/// 3次元ベクトルの内積を返す関数
float MyTools::Dot(const Vector3& v1, const Vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/// 3次元ベクトルのクロス積(外積)を返す関数
Vector3 MyTools::Cross(const Vector3& v1, const Vector3& v2)
{
	return Vector3{ v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
}

/// 3次元ベクトル長さ(ノルム)を返す関数
float MyTools::Length(const Vector3& v)
{
	return sqrtf(Dot(v, v));
}

/// 3次元ベクトルの正規化した値を返す関数
Vector3 MyTools::Normalize(const Vector3& v)
{
	float length = Length(v);
	float x = v.x, y = v.y, z = v.z;
	if (length != 0)
	{
		x /= length;
		y /= length;
		z /= length;
	}

	return Vector3{ x, y, z };
}

/// 線形補間
Vector3 MyTools::Lerp(const Vector3& vector1, const Vector3& vector2, float t)
{ 
	return Add(vector1, Multiply(t, Subtract(vector2, vector1)));
}

/// 球面線形補間
Vector3 MyTools::Slerp(const Vector3& vector1, const Vector3& vector2, float t) 
{ 
	// 正規化ベクトルを求める
	Vector3 start = Normalize(vector1);
	Vector3 end = Normalize(vector2);

	// 内積を求める
	float dot = Dot(start, end);
	// 誤差により1.0fを超えるのを防ぐ
	dot = Clamp(dot, dot, 1.0f);

	// s－九コサインでθの角度を求める
	float theta = std::acosf(dot); 

	// θの角度からsinθを求める
	float sinTheta = std::sin(theta);

	// サイン(θ(1-t))を求める
	float sinThetaFrom = std::sin((1 - t) * theta);
	// サインθtを求める
	float sinThetaTo = std::sin(t * theta);

	Vector3 normalizeVector;
	// ゼロ除算を防ぐ
	if (sinTheta < 1.0e-5)
	{
		normalizeVector = start;
	}
	else
	{
		// 球面線形補間したベクトル(単位ベクトル)
		normalizeVector = Add(Multiply(sinThetaFrom / sinTheta, start), Multiply(sinThetaTo / sinTheta, end));
	}
	
	// ベクトルの長さはstartとendの長さを線形補間
	float length1 = Length(start);
	float length2 = Length(end);
	// Lerpで補間ベクトルの長さを求める
	float length = Lerp(length1, length2, t);

	// 長さを反映
	return Multiply(length, normalizeVector);
}

/// CatmullRom補間
Vector3 MyTools::CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) 
{ 
	const float s = 0.5f;	// 数式に出てくる 1/2 のこと。

	float t2 = t * t;	// t の2乗
	float t3 = t2 * t;	// t の3乗
	

	Vector3 e3 = Multiply(-1.f, p0);
	e3 = Add(e3, Multiply(3.0f, p1));
	e3 = Subtract(e3, Multiply(3.0f, p2));
	e3 = Add(e3, p3);
	/*Subtract(Add(Multiply(-1.f, p0), Multiply(3.f, p1)), Add(Multiply(3.f, p2), p3));*/
	
	Vector3 e2 = Multiply(2.0f, p0);
	e2 = Subtract(e2, Multiply(5.0f, p1));
	e2 = Add(e2, Multiply(4.0f, p2));
	e2 = Subtract(e2, p3);
	/*Add(Subtract(Multiply(2.f, p0), Multiply(5.f, p1)), Subtract(Multiply(4.f, p2), p3));*/
	
	Vector3 e1 = Multiply(-1.0f, p0);
	e1 = Add(e1, p2);
	/*Add(Multiply(-1.f, p0), p2);*/

	Vector3 e0 = Multiply(2.0f, p1);

	Vector3 ans = Multiply(t3, e3);
	ans = Add(ans, Multiply(t2, e2));
	ans = Add(ans, Multiply(t, e1));
	ans = Add(ans, e0);

	return /*Multiply(s, Add(Add(Add(Multiply(t3, e3), Multiply(t2, e2)), Multiply(t, e1)), e0))*/ Multiply(s, ans);
}

/// CatmullRomスプライン曲線上の座標を得る
Vector3 MyTools::CatmullRomPosition(const std::vector<Vector3>& points, float t) 
{ 
	assert(points.size() >= 4 && "制御点は4点以上必要です");

	// 区間数は制御点の数-1
	size_t division = points.size() - 1;
	// 1区間の長さ(全体を1.0とした割合)
	float areaWidth = 1.0f / division;

	// 区間内の始点を0.0f、終点を1.0fとしたときの現在位置
	float t_2 = std::fmod(t, areaWidth) * division;
	// 下限(0.0f)と上限(1.0f)の範囲に収める
	t_2 = Clamp(t_2, 0.0f, 1.0f);

	// 区間番号
	size_t index = static_cast<size_t>(t / areaWidth);
	// 区間番号が上限を超えないように収める
	index = index + 1 >= points.size() ? points.size() - 2 : index;

	// 4点分のインデックス
	size_t index0 = index - 1;
	size_t index1 = index;
	size_t index2 = index + 1;
	size_t index3 = index + 2;

	// 最初の区間のp0はp1を重複使用する
	if (index == 0) {
		index0 = index1;
	}
	
	// 最後の区間のp3はp2を重複使用する
	if (index3 >= points.size()) {
		index3 = index2;
	}

	// 4点の座標
	const Vector3& p0 = points.at(index0);
	const Vector3& p1 = points.at(index1);
	const Vector3& p2 = points.at(index2);
	const Vector3& p3 = points.at(index3);

	// 4点を指定してCatmul-Rom補間
	return CatmullRomInterpolation(p0, p1, p2, p3, t_2);
}

/// 正射影ベクトル(ベクトル射影)を返す関数
Vector3 MyTools::Project(const Vector3& v1, const Vector3& v2)
{
	return Multiply(Dot(v1, Normalize(v2)), Normalize(v2));
}

/// 最近接点を返す関数
Vector3 MyTools::ClosestPoint(const Vector3& point, const Segment& segment)
{
	Vector3 ans;

	ans = Add(segment.origin, Project(Subtract(point, segment.origin), segment.diff));

	return ans;
}

/// 垂直なベクトルを求める関数
Vector3 MyTools::Perpendicular(const Vector3& vector)
{
	if (vector.x != 0.0f || vector.y != 0.0f)
	{
		return { -vector.y, vector.x, 0.0f };
	}

	return { 0.0f, -vector.z, vector.y };
}

/// 
/// 3次元ベクトル ここまで
/// 

///
/// 描画関数 ここから
/// 
/// 
/// 描画関数 ここまで
/// 

