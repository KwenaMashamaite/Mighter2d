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

#ifndef MIGHTER2D_TMIGHTER2DRMANAGER_H
#define MIGHTER2D_TMIGHTER2DRMANAGER_H

#include "Mighter2d/Config.h"
#include "Mighter2d/core/time/Timer.h"
#include "Mighter2d/core/time/Time.h"
#include <functional>
#include <list>
#include <stack>

namespace mighter2d {
    class Scene;

    /**
     * @brief Manages multiple Timer instances
     *
     * This class is not meant to be instantiated directly, use mighter2d::Scene::getTimer
     * or mighter2d::Engine::getTimer
     */
    class MIGHTER2D_API TimerManager {
    public:
        template <typename... Args>
        using Callback = std::function<void(Args...)>; //!< Event listener

        /**
         * @brief Constructor
         * @param scene The scene the timer manager belongs to
         */
        explicit TimerManager(Scene& scene);

        /**
         * @brief Schedule a one time callback
         * @param delay The time to wait before executing the callback
         * @param callback The function to be executed
         * @return A reference to the callback execution timer
         * @throws InvalidArgumentException if the specified callback argument
         *          is a nullptr
         *
         * On invocation, the callback is passed the Timer that is responsible
         * for the callback execution, the timer can be used to extend the
         * duration, stop the schedule and so on...
         *
         * @note The returned reference will be invalidated when the timer
         * stops, either explicitly or when it expires
         *
         * @warning The callback must not be a nullptr
         */
        Timer& setTimeout(Time delay, Callback<Timer&> callback);

        /**
         * @brief Schedule a one time callback
         * @param delay The time to wait before executing the callback
         * @param callback The function to be executed
         * @return A reference to the callback execution timer
         * @throws InvalidArgumentException if the specified callback argument
         *          is a nullptr
         *
         * @note The returned reference will be invalidated when the timer
         * stops, either explicitly or when it expires
         *
         * @warning The callback must not be a nullptr
         */
        Timer& setTimeout(Time delay, Callback<> callback);

        /**
         * @brief Schedule a repeated callback
         * @param delay The time to wait before executing the callback
         * @param callback The callback to be executed
         * @param repeatCount The number of times to repeat the callback
         * @return A reference to the callback execution timer
         * @throws InvalidArgumentException if the specified callback argument
         *          is a nullptr
         *
         * On invocation, the callback is passed the Timer that is responsible
         * for the callback execution, the timer can be used to extend the
         * duration, stop the schedule and so on... However note that the
         * timer will be destroyed after the callback is invoked or if it is
         * stopped before the callback is invoked.
         *
         * By default, if @a repeatCounter is left unspecified then the
         * callback will be executed forever every @a delay
         *
         * @note The returned reference will be invalidated when the timer
         * stops, either explicitly or when it expires
         *
         * @warning The callback must not be a nullptr
         */
        Timer& setInterval(Time delay, Callback<Timer&> callback, int repeatCount = -1);

        /**
         * @brief Schedule a repeated callback
         * @param delay The time to wait before executing the callback
         * @param callback The callback to be executed
         * @param repeatCount The number of times to repeat the callback
         * @return A reference to the callback execution timer
         * @throws InvalidArgumentException if the specified callback argument
         *          is a nullptr
         *
         * By default, if @a repeatCounter is left unspecified then the
         * callback will be executed forever every @a delay. The callback
         * must not be a nullptr
         *
         * @note The returned reference will be invalidated when the timer
         * stops, either explicitly or when it expires
         */
        Timer& setInterval(Time delay, Callback<> callback, int repeatCount = -1);

        /**
         * @brief Get the total number of timers
         * @return The total number of timers
         */
        std::size_t getTimerCount() const;

        /**
         * @brief Remove all timers
         */
        void clear();

    private:
        /**
         * @brief Add a timer to active timers
         * @param timer The timer to be added
         * @return A reference to the timer after its added to the manager
         */
        Timer& addTimer(Timer::Ptr timer);

    private:
        Scene* scene_;
        std::list<Timer::Ptr> activeTimers_;   //!< Timers that are counting down
    };
}

#endif //MIGHTER2D_TMIGHTER2DRMANAGER_H
