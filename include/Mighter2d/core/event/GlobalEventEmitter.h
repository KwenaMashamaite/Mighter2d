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

#ifndef MIGHTER2D_GLOBALEVENTEMITTER_H
#define MIGHTER2D_GLOBALEVENTEMITTER_H

#include "Mighter2d/Config.h"
#include "EventEmitter.h"
#include <memory>
#include <string>

namespace mighter2d {
    /**
     * @brief A singleton for creating and dispatching events
     */
    class MIGHTER2D_API GlobalEventEmitter : public EventEmitter {
    public:
        using Ptr = std::shared_ptr<GlobalEventEmitter>; //!< Shared GlobalEventEmitter pointer

        /**
         * @brief Copy constructor
         */
        GlobalEventEmitter(const GlobalEventEmitter&) = delete;

        /**
         * @brief Copy assignment operator
         */
        GlobalEventEmitter& operator=(const GlobalEventEmitter&) = delete;

        /**
         * @brief Get class instance
         * @return Shared pointer to class instance
         */
        static GlobalEventEmitter::Ptr instance();

    private:
        /**
         * @brief Default constructor
         */
        GlobalEventEmitter() = default;

    private:
        GlobalEventEmitter::Ptr instance_;   //!< The only class instance
        inline static std::mutex mutex_;     //!< Synchronization primitive
    };
}

#endif

/**
 * @class mighter2d::GlobalEventEmitter
 * @ingroup core
 *
 * The global event emitter is available to anything class that needs
 * it (class, function etc..). Its responsibility is to decouple classes
 * from one another. You can emit a signal and anyone listening for that
 * signal will pick it up without knowing and caring where the signal came
 * from. Here is a simple example:
 *
 * @code
 * // main.cpp
 *
 * // We subscribe to a loading event that will be dispatched by some scene
 * // we don't know or care about, we are just interested in knowing that
 * // resource loading is complete
 * GlobalEventEmitter::instance()->on("loadingComplete", Callback<>([&engine] {
 *    engine.popScene();                // Remove the scene that emitted the event
 *    engine.pushScene(gameplayScene);  // Start the gameplay scene
 * }));
 *
 * engine.pushScene(loadingScene);
 * engine.run();
 *
 * // LoadingScene.cpp
 * // ...
 * // The caller does not need to keep asking the loading scene if it has
 * // finished loading the game assets, the scene lets everyone whose is
 * // interested know when it is done without knowing and caring who they are
 *
 * GlobalEventEmitter::instance()->emit("loadingComplete");
 * @endcode
 *
 * @note This classes instance is accessible from anywhere withing the
 * program, however the instance is destroyed when the last pointer to it
 * goes out of scope. This means that all event listeners that have been
 * registered will be destroyed and a call to dispatchEvent will not do
 * anything. Therefore there must be at least one pointer to the class
 * instance that keeps it alive for as long as its being used. The Engine
 * keeps an instance alive for as long as it is running, therefore you
 * should use the global dispatcher only when the engine is running or keep
 * an instance alive yourself
 */
