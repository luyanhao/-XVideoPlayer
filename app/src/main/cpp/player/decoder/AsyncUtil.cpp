//
// Created by lyhao on 2023/12/27.
//

#include <thread>

class AsyncUtil {
private:
    std::mutex mMutex;
    long audioPts = 0;
    long diff = 0;
    AsyncUtil() = default;

public:
    AsyncUtil(const AsyncUtil &) = delete;
    AsyncUtil(const AsyncUtil &&) = delete;
    AsyncUtil &operator=(const AsyncUtil &) = delete;
    static AsyncUtil &getInstance() {
        static AsyncUtil s;
        return s;
    }
    void SetAudioPts(long audioPts) {
//        std::unique_lock<std::mutex> lock(mMutex);
        this->audioPts = audioPts;
    }
    long CalculateDiff(long videoPts) {
//        std::unique_lock<std::mutex> lock(mMutex);
        this->diff = videoPts - this->audioPts;
        return this->diff;
    }

};