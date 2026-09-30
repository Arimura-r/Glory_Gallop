#pragma once
#include "../Library/myDxLib.h"
#include <list>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <initializer_list>
#include <assert.h>

/// <summary>
/// 便利な機能をまとめた名前空間　綾部
/// </summary>
namespace Util
{
	// -------------------------------------------------
	// 角度関係
	// -------------------------------------------------

	// 角度を-180度～180度に正規化(ラジアン)
	inline void NormalizeAngle(float& _angle)
	{
		while (_angle > 180 * DegToRad)
			_angle -= 360 * DegToRad;
		while (_angle < -180 * DegToRad)
			_angle += 360 * DegToRad;
	}
	inline void NormalizeAngleV(VECTOR& _v)
	{
		NormalizeAngle(_v.x);
		NormalizeAngle(_v.y);
		NormalizeAngle(_v.z);
	}

	// 距離ベクトルから角度の差を得る
	inline VECTOR GetDiffAngle(const VECTOR& _dist)
	{
		return VGet(-atan2f(_dist.y, sqrtf(_dist.x * _dist.x + _dist.z * _dist.z)),
			atan2f(_dist.x, _dist.z), 0);
	}

	// -------------------------------------------------
	// 上下制限、乱数など範囲を指定する処理
	// -------------------------------------------------

	// 最小と最大の値を持つ
	template<typename T>
	struct Range
	{
		// minとmaxだとマクロ定義と被る
		T mini;
		T maxi;
		constexpr Range() : mini(T()), maxi(T()) {}
		constexpr Range(const T& _mini, const T& _maxi)
			: mini(_mini), maxi(_maxi) {}
	};
	inline Range<float>& operator +=(Range<float>& v1, const float& v2)
	{
		v1.mini += v2;
		v1.maxi += v2;
		return v1;
	}
	// VECTOR版 XYZそれぞれが最小と最大を持つ
	struct VRange
	{
		Range<float> x;
		Range<float> y;
		Range<float> z;
		VRange() {}
		constexpr VRange(const Range<float>& _x, const Range<float>& _y, const Range<float>& _z) : x(_x), y(_y), z(_z) {}
	};
	inline VRange& operator +=(VRange& v1, const VECTOR& v2)
	{
		v1.x += v2.x;
		v1.y += v2.y;
		v1.z += v2.z;
		return v1;
	}

	// ある値が指定の範囲内か
	template <typename T>
	bool JudgeInRange(const T& _base, const Range<T>& _range)
	{
		return (_base >= _range.mini && _base <= _range.maxi);
	}

	// クランプ(範囲制限)関数
	template <typename T>
	inline void Clamp(T& _clampNum, const Range<T>& _range)
	{
		if (_clampNum < _range.mini)
			_clampNum = _range.mini;
		if (_clampNum > _range.maxi)
			_clampNum = _range.maxi;
	}
	// ベクトルの各軸の値を－１～１に収める
	inline void ClampNorm(VECTOR& _norm)
	{
		Range<float> range = Range<float>(-1, 1);
		Clamp<float>(_norm.x, range);
		Clamp<float>(_norm.y, range);
		Clamp<float>(_norm.z, range);
	}

	// 符号取得
	template <typename T>
	inline int GetSign(const T& _num)
	{
		if (_num < 0)
			return -1;
		else
			return 1;
	}

	inline VECTOR GetSignV(const VECTOR& _v)
	{
		VECTOR v = VZero;
		v.x = (float)GetSign(_v.x);
		v.y = (float)GetSign(_v.y);
		v.z = (float)GetSign(_v.z);
		return v;
	}

	// 正負符号の乱数取得
	inline int GetRandSign()
	{
		return GetRand(1) == 1 ? 1 : -1;
	}
	// 正負符号の乱数取得(VECTOR)
	inline VECTOR GetRandSignV()
	{
		return VGet((float)GetRandSign(), (float)GetRandSign(), (float)GetRandSign());
	}
	// int型の乱数取得
	inline int GetRandI(const Range<int>& _r)
	{
		return _r.mini + GetRand((_r.maxi - _r.mini) + 1) - 1;
	}

	// GetRandFで使うために先に宣言
	struct RandF;
	// 先に宣言してRandFの定義後に中身を書く
	inline float GetRandF(const RandF& _r);

	// floatの乱数で使う型
	struct RandF
	{
		Range<float> range; // 乱数範囲
		int decimalPlace;	// 小数点第何位までを有効にするか
		constexpr RandF() : decimalPlace(1) {}
		// Range<float>と同じにならないように暗黙化は禁止
		explicit constexpr RandF(const float& _mini, const float& _maxi, const int& _decimalPlace = 1)
			: range(Range<float>(_mini, _maxi)), decimalPlace(_decimalPlace) {
		}
		// {}でも初期化可能に
		constexpr RandF(std::initializer_list<float> _il) : range(), decimalPlace(1)
		{
			// デフォルト引数があるので２個か３個の要素数が前提
			assert(_il.size() == 2 || _il.size() == 3);

			auto it = _il.begin();
			range.mini = *it++;	// min
			range.maxi = *it++;	// max
			// デフォルト引数に代入があれば
			if (_il.size() == 3)
				decimalPlace = (int)(*it);
		}
	};
	inline RandF& operator +=(RandF& _r, const float& _f)
	{
		_r.range.mini += _f;
		_r.range.maxi += _f;
		return _r;
	}
	inline RandF& operator *=(RandF& _r, const float& _f)
	{
		_r.range.mini *= _f;
		_r.range.maxi *= _f;
		return _r;
	}
	// VECTOR版 XYZそれぞれの乱数型を持つ
	struct VRandF
	{
		RandF x;
		RandF y;
		RandF z; 
		VRandF() {}
		VRandF(const RandF& _x, const RandF& _y, const RandF& _z) : x(_x), y(_y), z(_z) {}
		// {}でも初期化可能に
		VRandF(std::initializer_list<RandF> il) : x(), y(), z()
		{
			assert(il.size() == 3);
			auto it = il.begin();
			x = *it++;
			y = *it++;
			z = *it++;
		}
	};
	inline VRandF& operator +=(VRandF& v1, const VECTOR& v2)
	{
		v1.x += v2.x;
		v1.y += v2.y;
		v1.z += v2.z;
		return v1;
	}
	inline VRandF& operator *=(VRandF& v1, const float& r)
	{
		v1.x *= r;
		v1.y *= r;
		v1.z *= r;
		return v1;
	}

	/// <summary>
	/// float型の乱数取得
	/// </summary>
	/// <param name="_r">乱数の範囲</param>
	/// <returns>乱数結果</returns>
	inline float GetRandF(const RandF& _r)
	{
		int dig = 1;
		int count = _r.decimalPlace;
		while (count > 0)
		{
			dig *= 10;
			count--;
		}
		int min = (int)(_r.range.mini * dig);
		int max = (int)(_r.range.maxi * dig);
		float result = min + (float)GetRand((max - min) + 1) - 1;
		result /= dig;
		return result;
	}
	// VECTOR型の乱数取得
	inline VECTOR GetRandV(const VRandF& _vRange)
	{
		VECTOR result = VZero;
		result.x = GetRandF(_vRange.x);
		result.y = GetRandF(_vRange.y);
		result.z = GetRandF(_vRange.z);
		return result;
	}

	// floatかRandFどちらかの値を入れる型
	struct FParam
	{
		bool isRand; // 乱数の値か
		float value; // 単体の値
		RandF rand;	 // 乱数範囲
		FParam() : isRand(false), value(0), rand() {}
		// floatの時
		constexpr FParam(const float& _v) : isRand(false), value(_v), rand(RandF(_v, _v)) {}
		// 乱数範囲(RandF)の時
		constexpr FParam(const RandF& _r) : isRand(true), value(0), rand(_r) {}

		// 乱数結果をvalueに入れて得る
		float DoRand() { return value = isRand ? GetRandF(rand) : value; }
		// 定数または乱数結果がセットされたvalueを得る
		float Get() const { return value; }
	};


	// １か－１を乱数で返す
	template <typename T>
	inline void SetSignRand(T& _num)
	{
		int sign = (bool)GetRand(1) ? 1 : -1;
		_num *= sign;
	}
	// １か－１を乱数で返す(VECTOR)
	inline void SetSignRandV(VECTOR& _num)
	{
		SetSignRand<float>(_num.x);
		SetSignRand<float>(_num.y);
		SetSignRand<float>(_num.z);
	}

	// 角度(ラジアン)の乱数を得る(-180～180度)
	inline float GetRandAngle()
	{
		return GetRandF(Util::RandF(-180 * DegToRad, 180 * DegToRad));
	}

	// -------------------------------------------------
	// その他
	// -------------------------------------------------

	// 最新と過去の値を持つ
	template <typename T>
	struct Change
	{
		T current;
		T last;
	};
	// bool型の初期化
	template <>
	struct Change<bool>
	{
		bool current;
		bool last;
		Change() : current(false), last(false) {}
	};

	// 開始と終了の値を持つ
	template <typename T>
	struct Length
	{
		T start;
		T end;
	};

	// リストに一致する要素があるか
	template <typename T>
	bool DoExist(const std::list<T>& _list, const T& _target)
	{
		return (std::find(_list.begin(), _list.end(), _target) != _list.end());
	}

	// マップから最大値の要素を得る
	template <typename Key, typename Value>
	std::pair<Key, Value> GetMaxElementFromMap(const std::unordered_map<Key, Value>& _map)
	{
		// 空ならデフォルト値を返す
		if (_map.empty())
			return std::pair<Key, Value>{};

		auto it = std::max_element(_map.begin(), _map.end(),
			[](const auto& a, const auto& b)
			{
				return a.second < b.second;
			});
		return { it->first, it->second };
	}

	// 動的キャストの省略用
	template<class Derived, class Base>
	Derived* DynamicCast(Base* _base)
	{
		Derived* derived = dynamic_cast<Derived*>(_base);
		return derived;
	}

	// 末尾に乱数結果の数字を付けた名前を返す
	std::string GetRandName(const std::string& _baseName, const int& _randNum);
}