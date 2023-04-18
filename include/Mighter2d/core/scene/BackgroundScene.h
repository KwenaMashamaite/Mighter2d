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

#ifndef MIGHTER2D_BACKGROUNDSCENE_H
#define MIGHTER2D_BACKGROUNDSCENE_H

#include "Mighter2d/Config.h"
#include "Mighter2d/core/scene/Scene.h"

namespace mighter2d {
    /**
     * @brief A scene that can run in conjunction with another scene as its background
     *
     * @see Scene::setBackgroundScene
     */
    class MIGHTER2D_API BackgroundScene : public Scene {
    public:
        /**
         * @brief Constructor
         */
        BackgroundScene();

        /**
         * @brief Copy constructor
         */
        BackgroundScene(const BackgroundScene*) =  delete;

        /**
         * @brief Copy assignment operator
         */
        BackgroundScene& operator=(const BackgroundScene&) = delete;

        /**
         * @brief Move constructor
         */
        BackgroundScene(BackgroundScene&&) noexcept = default;

        /**
         * @brief Move assignment operator
         */
        BackgroundScene& operator=(BackgroundScene&&) noexcept = default;

        /**
         * @brief Get the name of the class
         * @return The name of the class
         */
        std::string getClassName() const override;

        /**
         * @brief Set whether or not the scene is rendered when the parent scene is rendered
         * @param drawable True to enable render, otherwise false
         *
         * When the @a drawable argument is set to @a true, the background scene is rendered
         * behind the parent scene
         *
         * By default, the background scene is rendered when the parent scene
         * is rendered
         *
         * @see isBackgroundSceneDrawable
         */
        void setDrawable(bool drawable);

        /**
         * @brief Check if the background scene is rendered when the parent scene is rendered
         * @return True if the background scene is rendered, otherwise false
         *
         * @see setDrawable
         */
        bool isDrawable() const;

        /**
         * @brief Set whether or not the background scene receives time updates from parent scene
         * @param updatable True to enable background scene time updates, otherwise false
         *
         * When time updates for a background scene are disabled, its onUpdate() and
         * onFixedUpdate() functions are not invoked. In addition, any instances
         * that belong to the scene that implement the IUpdatable interface are not
         * invoked.
         *
         * By default, time updates of the background scene are enabled
         *
         * @see isUpdateEnabled
         */
        void setUpdateEnable(bool updatable);

        /**
         * @brief Check if the background scene receives time updates from parent scene
         * @return True if it receives time updates, otherwise false
         *
         * @see setUpdateEnable
         */
        bool isUpdateEnabled() const;

        /**
         * @brief Set whether or not the background scene receives system updates from the parent scene
         * @param enable True to enable system events, otherwise false
         *
         * When system events for a background scene are disabled, its onHandleEvent()
         * function is not invoked. In addition, any instances that belong to the scene
         * that implement the ISystemEventHandler interface are not invoked.
         *
         * By default, background scene system events are disabled
         *
         * @see isSystemEventHandleEnabled
         */
        void setSystemEventHandleEnable(bool enable);

        /**
         * @brief Check if the background scene receives system events from the parent scene
         * @return True if system events are received, otherwise false
         *
         * @see isSystemEventHandleEnabled
         */
        bool isSystemEventHandleEnabled() const;

        /**
         * @brief Destructor
         */
        ~BackgroundScene() override;

    private:
        bool isDrawable_;            //!< A flag indicating whether or not the scenes background scene is rendered
        bool isUpdated_;             //!< A flag indicating whether or not the scenes background scene receives time updates
        bool isSystemEventsEnabled_; //!< A flag indicating whether or not the scenes background scene receives system events
    };
}

#endif //MIGHTER2D_BACKGROUNDSCENE_H
