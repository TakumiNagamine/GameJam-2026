#pragma once
#include <chrono>

class Timer {
public:
    Timer();  //コンストラクタ

    void Start();  //スタート処理
    void Reset();  //リセット処理
    double GetElapsedSeconds() const;  //経過秒数取得処理
    bool IsTimeUp(double durationSeconds) const;  //タイムアップ処理

private:
    std::chrono::steady_clock::time_point startTime;  //
};
