#include "Random.h"

namespace {
	// 乱数生成器とシード値を保持する匿名名前空間
	unsigned int seed_ = 0;//シード値
	std::mt19937 engine_;// メルセンヌ・ツイスタ乱数生成器
}

void Random::Init(unsigned int seed)
{
	seed_ = seed;
	// 乱数生成器をシード値で初期化
	engine_.seed(seed);
}

double Random::GetReal()
{
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(engine_);
}

int Random::GetInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine_);
}

float Random::GetFloat(float ave, float dev)
{
    std::normal_distribution<float> dist(ave,dev);
    return dist(engine_);
}

int Random::GetWeightedIndex(const std::vector<double>& weights)
{
    std::discrete_distribution<int> dist(weights.begin(), weights.end());
    return dist(engine_);
}

double Random::GetNormal(double mean, double stddev)
{
    std::normal_distribution<double> dist(mean, stddev);
    return dist(engine_);
}

bool Random::GetBernoulli(double p)
{
    std::bernoulli_distribution dist(p);
    return dist(engine_);
}

double Random::GetExponential(double lambda)
{
    std::exponential_distribution<double> dist(lambda);
    return dist(engine_);
}
unsigned int Random::GetSeed()
{
    return seed_;
}
