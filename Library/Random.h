#pragma once
#include <random>
#include <vector>



/// <summary>
///std::randomを使用して乱数を生成するヘッダー
///シード値によって乱数列を生成しているため、同じシード値を使用すれば同じ乱数列が得られる。
/// </summary>
namespace Random {

    //任意でシードを指定、未指定ならランダムシード
	void Init(unsigned int seed = std::random_device{}());

    // ===== 基本分布 =====▼
    
	// 一様分布の乱数を生成...0.0から1.0の範囲で値をランダムに返す
    double GetReal();      

	// 一様整数...乱数を生成...minからmaxの範囲で値をランダムに返す
    int GetInt(int min, int max); 

	// 一様浮動小数点数...乱数を生成...ave平均値から標準偏差devでバラつかせた値を返す
    float GetFloat(float ave, float dev);

    // 重み付き選択...各要素の重みを指定し、確率に応じてインデックスを返す。
	//例えば、weightsが{0.1, 0.3, 0.6}の場合、0番目の要素が10%、1番目が30%、2番目が60%の確率で選ばれる。
    int GetWeightedIndex(const std::vector<double>& weights);

	// 正規分布...平均値と標準偏差を指定して乱数を生成
    double GetNormal(double mean, double stddev);

	// ベルヌーイ分布...確率pでtrueを返す...0.3なら30%の確率でtrueを返す
    bool GetBernoulli(double p);       

	// 指数分布...平均値lambdaを指定して乱数を生成
    double GetExponential(double lambda);           

    // ===== ユーティリティ =====
    // 現在のシード値を取得
	unsigned int GetSeed();

};