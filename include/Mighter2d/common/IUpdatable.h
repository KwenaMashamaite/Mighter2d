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

#ifndef MIGHTER2D_IUPDATABLE_H
#define MIGHTER2D_IUPDATABLE_H

#include "Mighter2d/Config.h"
#include "Mighter2d/core/time/Time.h"

namespace mighter2d {
    /**
     * @brief Interface for updatable entities
     */
    class MIGHTER2D_API IUpdatable {
    public:
        /**
         * @brief Perform update
         * @param deltaTime The time past since last update
         *
         * @note This function will be called automatically by Mighter2d.
         * @a deltaTime is synced with the render FPS. In other words, it
         * is frame-rate dependent.
         */
        virtual void update(Time deltaTime) = 0;

        /**
         * @brief Perform an update every fixed interval
         * @param deltaTime The time passed since the last update
         *
         * This function will be called by Mighter2d every fixed frame-rate
         * frame. It is called before update() and it may be called once per
         * frame, multiple times per frame or not called at all. The delta
         * passed to it is always the same and is independent of the render fps
         */
        virtual void fixedUpdate(Time deltaTime) {MIGHTER2D_UNUSED(deltaTime);};
    };
}

#endif //MIGHTER2D_IUPDATABLE_H
