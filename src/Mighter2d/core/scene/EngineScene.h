// ///////////////////////////////////////////////////////////////////////////
//  Mighter2d
//
//  Copyright (c) 2023 Kwena Mashamaite
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
// ///////////////////////////////////////////////////////////////////////////

#ifndef MIGHTER2D_ENGINESCENE_H
#define MIGHTER2D_ENGINESCENE_H

#include "Mighter2d/core/input/InputManager.h"
#include "Mighter2d/core/time/TimerManager.h"
#include "Mighter2d/core/audio/AudioManager.h"
#include "Mighter2d/ui/GuiContainer.h"
#include "Mighter2d/core/scene/Scene.h"

namespace mighter2d::priv {
    /**
     * @brief Instantiates and automatically updates engine level components
     *
     * This scene starts running when the engine starts running and stops
     * when the engine stops running. During scene pop and push operations
     * it is not paused, stopped or destroyed.
     */
    class EngineScene : public Scene {
    public:
        /**
         * @brief Constructor
         */
        EngineScene();

        input::InputManager inputManager_; //!< Engine level input manager
        TimerManager timerManager_;        //!< Engine level timer manager
        audio::AudioManager audioManager_; //!< Engine level audio manager
        ui::GuiContainer guiContainer_;    //!< Engine level gui
    };
}

#endif //MIGHTER2D_ENGINESCENE_H
