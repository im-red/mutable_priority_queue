#include <benchmark/benchmark.h>
#include "mutable_priority_queue.h"
#include <vector>
#include <random>
#include <queue>
#include <iostream>
#include <cstdio>
#include <functional>
#include <type_traits>

using namespace std;

static void CustomArguments(benchmark::internal::Benchmark* b) 
{
  int begin = 1 << 10;
  int end = 1 << 20;
  for (int i = begin; i < end; i *= 1.5)
      b->Args({i});
}

struct Node
{
    int value;
    long long ext[10];
};

template <typename T>
vector<typename std::enable_if<std::is_arithmetic<T>::value, T>::type> genSamples(const int len)
{
    vector<T> samples;
    default_random_engine e(0);
    for (int i = 0; i < len; i++)
    {
        samples.push_back(e());
    }
    return move(samples);
}

template <typename T>
vector<typename std::enable_if<!std::is_arithmetic<T>::value, T>::type> genSamples(const int len)
{
    vector<T> samples;
    default_random_engine e(0);

    for (int i = 0; i < len; i++)
    {
        T t;
        t.value = e();
        samples.push_back(t);
    }

    return move(samples);
}

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, bool>::type comp(const T &a, const T &b)
{
    return a > b;
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, bool>::type comp(const T &a, const T &b)
{
    return a.value > b.value;
}

template <typename T>
static void STL_PQ_Push(benchmark::State& state) 
{
    const int len = state.range(0);
    vector<T> samples = genSamples<T>(len);
    priority_queue<T, vector<T>, decltype(comp<T>) *> pq(comp<T>);
    for (auto _ : state)
    {
        for (int i = 0; i < len; i++)
        {
            pq.push(samples[i]);
        }
    }
}
BENCHMARK_TEMPLATE(STL_PQ_Push, char)->Apply(CustomArguments)->Unit(benchmark::kMicrosecond);

template <typename T>
static void MPQ_Push(benchmark::State& state) 
{
    const int len = state.range(0);
    vector<T> samples = genSamples<T>(len);
    MutablePriorityQueue<T> pq(comp<T>);
    for (auto _ : state)
    {
        for (int i = 0; i < len; i++)
        {
            pq.push(samples[i]);
        }
    }
}
BENCHMARK_TEMPLATE(MPQ_Push, char)->Apply(CustomArguments)->Unit(benchmark::kMicrosecond);

template <typename T>
void STL_PQ_Pop(benchmark::State &state)
{
    const int len = state.range(0);
    vector<T> samples = genSamples<T>(len);

    for (auto _ : state)
    {
        state.PauseTiming();
        priority_queue<T, vector<T>, decltype(comp<T>) *> pq(comp<T>);
        for (int i = 0; i < len; i++)
        {
            pq.push(samples[i]);
        }
        state.ResumeTiming();
        T dummy;
        for (int i = 0; i < len; i++)
        {
            dummy = pq.top();
            pq.pop();
        }
    }
}
BENCHMARK_TEMPLATE(STL_PQ_Pop, char)->Apply(CustomArguments)->Unit(benchmark::kMicrosecond);

template <typename T>
static void MPQ_Pop(benchmark::State& state) 
{
    const int len = state.range(0);
    vector<T> samples = genSamples<T>(len);

    for (auto _ : state)
    {
        state.PauseTiming();
        MutablePriorityQueue<T> pq(comp<T>);
        for (int i = 0; i < len; i++)
        {
            pq.push(samples[i]);
        }
        state.ResumeTiming();
        T dummy;
        for (int i = 0; i < len; i++)
        {
            dummy = pq.pop();
        }
    }
}
BENCHMARK_TEMPLATE(MPQ_Pop, char)->Apply(CustomArguments)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();