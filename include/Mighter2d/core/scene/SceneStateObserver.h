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

#ifndef MIGHTER2D_SCENESTATEOBSERVER_H
#define MIGHTER2D_SCENESTATEOBSERVER_H

#include "Mighter2d/Config.h"
#include "Mighter2d/core/event/EventEmitter.h"

namespace mighter2d {
    class Scene;

    /**
     * @brief Emits Scene state changes
     */
    class MIGHTER2D_API SceneStateObserver {
    public:
        /**
         * @brief Constructor
         * @param scene The scene to be observed
         */
        explicit SceneStateObserver(Scene& scene);

        /**
         * @brief Add an event listener to a scene ready event
         * @param callback The function to be executed when the scene is ready
         * @param oneTime True to execute the callback one-time or false to
         *                execute it every time the event is triggered
         * @return The event listener unique identification number
         *
         * You can add any number of event handlers to this event
         */
        int onReady(const Callback<>& callback);

        /**
         * @brief Add an event listener to a scene start event
         * @param callback The function to be executed when the scene is started
         * @param oneTime True to execute the callback one-time or false to
         *                execute it every time the event is triggered
         * @return The event listener unique identification number
         *
         * You can add any number of event handlers to this event
         */
        int onStart(const Callback<>& callback);

        /**
         * @brief Add an event listener to a scene frame begin event
         * @param callback The function to be executed when the scene frame begins
         * @param oneTime True to execute the callback one-time or false to
         *                execute it every time the event is triggered
         * @return The event listener unique identification number
         *
         * You can add any number of event handlers to this event
         */
        int onFrameBegin(const Callback<>& callback, bool oneTime = false);

        /**
         * @brief Add an event listener to a scene frame end event
         * @param callback The function to be executed when the frame ends
         * @param oneTime True to execute the callback one-time or false to
         *                execute it every time the event is triggered
         * @return The event listener unique identification number
         *
         * You can add any number of event handlers to this event
         */
        int onFrameEnd(const Callback<>& callback, bool oneTime = false);

        /**
         * @brief Add an event listener to a scene pause event
         * @param callback The function to be executed when the scene is paused
         * @param oneTime True to execute the callback one-time or false to
         *                execute it every time the event is triggered
         * @return The event listener unique identification number
         *
         * You can add any number of event handlers to this event
         */
        int onPause(const Callback<>& callback, bool oneTime = false);

        /**
         * @brief Add an event listener to a scene resume event
         * @param callback The function to be executed when the scene is resumed
         * @param oneTime True to execute the callback one-time or false to
         *                execute it every time the event is triggered
         * @return The event listener unique identification number
         *
         * You can add any number of event handlers to this event
         */
        int onResume(const Callback<>& callback, bool oneTime = false);

        /**
         * @brief Add an event listener to a scene stop event
         * @param callback The function to be executed when the scene is exited
         * @param oneTime True to execute the callback one-time or false to
         *                execute it every time the event is triggered
         * @return The event listener unique identification number
         *
         * You can add any number of event handlers to this event
         */
        int onStop(const Callback<>& callback, bool oneTime = false);

        /**
         * @brief Add an event listener to a scene destroy event
         * @param callback The function to be executed when the scene is about
         *                 to be destroyed
         * @return The event listener unique identification number
         *
         * You can add any number of event handlers to this event
         */
        int onDestroy(const Callback<>& callback);

        /**
          * @brief Remove an event listener
          * @param id The event listener unique identification number
          * @return True if the event listener was removed or false if no such
          *         event listener exists
          */
        bool removeEventListener(int id);

    private:
        Scene* scene_;
    };
}

#endif //MIGHTER2D_SCENESTATEOBSERVER_H
