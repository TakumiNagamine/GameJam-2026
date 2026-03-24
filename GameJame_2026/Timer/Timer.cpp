#include "../Timer/Timer.h"

//コンストラクタ
Timer::Timer() {
    Start();
}
//スタート処理
void Timer::Start() {
    startTime = std::chrono::steady_clock::now();
}
//リセット処理
void Timer::Reset() {
    Start();
}
//経過秒数取得処理
double Timer::GetElapsedSeconds() const {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - startTime;
    return elapsed.count();
}
//タイムアップ処理
bool Timer::IsTimeUp(double durationSeconds) const {
    return GetElapsedSeconds() >= durationSeconds;
}
