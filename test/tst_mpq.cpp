#include "mutable_priority_queue.h"
#include <gtest/gtest.h>
#include <random>
#include <ctime>
#include <vector>
#include <map>

using namespace std;

class MPQTest : public ::testing::Test
{
public:
    MPQTest()
        : mpq([](int &a, int &b) { return a > b; })
        , e(static_cast<unsigned int>(time(nullptr)))
    {

    }
    MutablePriorityQueue<int> mpq;
    default_random_engine e;
};

TEST_F(MPQTest, sizeAndEmpty)
{
    EXPECT_EQ(0, mpq.size());
    EXPECT_EQ(true, mpq.empty());

    mpq.push(1);
    EXPECT_EQ(1, mpq.size());
    EXPECT_EQ(false, mpq.empty());

    mpq.push(2);
    EXPECT_EQ(2, mpq.size());
    EXPECT_EQ(false, mpq.empty());

    mpq.pop();
    EXPECT_EQ(1, mpq.size());
    EXPECT_EQ(false, mpq.empty());

    mpq.pop();
    EXPECT_EQ(0, mpq.size());
    EXPECT_EQ(true, mpq.empty());
}

TEST_F(MPQTest, push)
{
    EXPECT_EQ(0, mpq.push(1));
    EXPECT_EQ(1, mpq.push(2));
    EXPECT_EQ(2, mpq.push(3));
    EXPECT_EQ(3, mpq.push(4));
    EXPECT_EQ(4, mpq.push(5));
    mpq.pop();
    EXPECT_EQ(4, mpq.push(6));
    mpq.pop();
    EXPECT_EQ(4, mpq.push(7));
    mpq.pop();
    mpq.pop();
    EXPECT_EQ(3, mpq.push(8));
    mpq.pop();
    mpq.pop();
    EXPECT_EQ(2, mpq.push(9));
    mpq.pop();
    mpq.pop();
    EXPECT_EQ(1, mpq.push(10));
    mpq.pop();
    mpq.pop();
    EXPECT_EQ(0, mpq.push(11));
}

TEST_F(MPQTest, popAndTop)
{
    vector<MutablePriorityQueue<int>::value_type> topResult;
    vector<MutablePriorityQueue<int>::value_type> popResult;

    int push1Num = 1024;
    int pop1Num = 512;
    int push2Num = 10;
    int pop2Num = push1Num + push2Num - pop1Num;

    for (int i = 0; i < push1Num; i++)
    {
        mpq.push(e());
    }
    for (int i = 0; i < pop1Num; i++)
    {
        topResult.push_back(mpq.top());
        popResult.push_back(mpq.pop());
    }

    size_t size = popResult.size();
    for (int i = 0; i < size - 1; i++)
    {
        EXPECT_GE(popResult[i], popResult[i + 1]);
    }
    for (int i = 0; i < size; i++)
    {
        EXPECT_EQ(popResult[i], topResult[i]);
    }
    popResult.clear();
    topResult.clear();

    for (int i = 0; i < push2Num; i++)
    {
        mpq.push(e());
    }
    for (int i = 0; i < pop2Num; i++)
    {
        topResult.push_back(mpq.top());
        popResult.push_back(mpq.pop());
    }
    size = popResult.size();
    for (int i = 0; i < size - 1; i++)
    {
        EXPECT_GE(popResult[i], popResult[i + 1]);
    }
    for (int i = 0; i < size; i++)
    {
        EXPECT_EQ(popResult[i], topResult[i]);
    }
}

TEST_F(MPQTest, topHandle)
{
    mpq.push(4);
    EXPECT_EQ(0, mpq.topHandle());
    mpq.push(5);
    EXPECT_EQ(1, mpq.topHandle());
    mpq.pop();
    EXPECT_EQ(0, mpq.topHandle());
    mpq.push(3);
    EXPECT_EQ(0, mpq.topHandle());
    mpq.pop();
    EXPECT_EQ(1, mpq.topHandle());
}

TEST_F(MPQTest, value)
{
    map<MutablePriorityQueue<int>::handle_type,
            MutablePriorityQueue<int>::value_type> handleAndValue;

    auto checkValue = [&]()
    {
        for (auto p : handleAndValue)
        {
            EXPECT_EQ(p.second, mpq.value(p.first));
        }
    };

    int push1Num = 10;
    int pop1Num = 5;
    int push2Num = 10;
    int pop2Num = push1Num + push2Num - pop1Num;

    for (int i = 0; i < push1Num; i++)
    {
        int value = e();
        handleAndValue[mpq.push(value)] = value;
    }
    checkValue();

    for (int i = 0; i < pop1Num; i++)
    {
        MutablePriorityQueue<int>::handle_type handle = mpq.topHandle();
        for (auto iter = handleAndValue.begin(); iter != handleAndValue.end(); iter++)
        {
            if (iter->first == handle)
            {
                handleAndValue.erase(iter);
                break;
            }
        }
        mpq.pop();
    }
    checkValue();

    for (int i = 0; i < push2Num; i++)
    {
        int value = e();
        handleAndValue[mpq.push(value)] = value;
    }
    checkValue();

    for (int i = 0; i < pop2Num; i++)
    {
        MutablePriorityQueue<int>::handle_type handle = mpq.topHandle();
        for (auto iter = handleAndValue.begin(); iter != handleAndValue.end(); iter++)
        {
            if (iter->first == handle)
            {
                handleAndValue.erase(iter);
                break;
            }
        }
        mpq.pop();
    }
    checkValue();
}

TEST_F(MPQTest, update)
{
    map<MutablePriorityQueue<int>::handle_type, MutablePriorityQueue<int>::value_type> handleAndValue;

    auto checkValue = [&]()
    {
        for (auto p : handleAndValue)
        {
            EXPECT_EQ(p.second, mpq.value(p.first));
        }
    };

    auto updateEvery = [&]()
    {
        for (auto p : handleAndValue)
        {
            int newValue = e();
            mpq.update(p.first, newValue);
            handleAndValue[p.first] = newValue;
        }
    };

    auto checkPopSequence = [&](int popNum)
    {
        vector<MutablePriorityQueue<int>::value_type> values;
        for (int i = 0; i < popNum; i++)
        {
            MutablePriorityQueue<int>::handle_type handle = mpq.topHandle();
            for (auto iter = handleAndValue.begin(); iter != handleAndValue.end(); iter++)
            {
                if (iter->first == handle)
                {
                    handleAndValue.erase(iter);
                    break;
                }
            }
            values.push_back(mpq.pop());
        }
        for (int i = 0; i < popNum - 1; i++)
        {
            EXPECT_GE(values[i], values[i + 1]);
        }
    };

    int push1Num = 10;
    int pop1Num = 5;
    int push2Num = 10;
    int pop2Num = push1Num + push2Num - pop1Num;

    for (int i = 0; i < push1Num; i++)
    {
        int value = e();
        handleAndValue[mpq.push(value)] = value;
    }
    updateEvery();
    checkValue();

    checkPopSequence(pop1Num);
    updateEvery();
    checkValue();

    for (int i = 0; i < push2Num; i++)
    {
        int value = e();
        handleAndValue[mpq.push(value)] = value;
    }
    updateEvery();
    checkValue();

    checkPopSequence(pop2Num);
}
