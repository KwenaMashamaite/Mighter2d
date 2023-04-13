////////////////////////////////////////////////////////////////////////////////
// Mighter2d
//
// Copyright (c) 2023 Kwena Mashamaite
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#include "Mighter2d/core/time/Clock.h"
#include <chrono>

namespace mighter2d {
    namespace {
        /**
         * @brief Get the amount of time that has passed since the process
         *        started executing
         * @return Time that has passed since the process started executing
         */
        Time getProcessTime() {
            using namespace std::chrono;
            auto now_in_ns = time_point_cast<std::chrono::nanoseconds>(steady_clock::now());
            return mighter2d::nanoseconds(now_in_ns.time_since_epoch().count());
        }
    }

    Clock::Clock() {
        startTime_ = getProcessTime();
    }

    Time Clock::getElapsedTime() const {
        return getProcessTime() - startTime_;
    }

    Time Clock::restart() {
        auto now = getProcessTime();
        auto elapsed = now - startTime_;
        startTime_ = now;
        return elapsed;
    }
}
