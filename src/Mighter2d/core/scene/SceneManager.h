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

#ifndef MIGHTER2D_SCENEMANAGER_H
#define MIGHTER2D_SCENEMANAGER_H

#include "Mighter2d/core/time/Time.h"
#include "Mighter2d/core/event/SystemEvent.h"
#include "Mighter2d/core/scene/Scene.h"
#include <stack>
#include <memory>
#include <string>
#include <unordered_map>

namespace mighter2d {
    /// @internal
    namespace priv {
        class RenderTarget;
        class EngineScene;

        /**
         * @brief Manages game scenes
         *
         * The transition between scenes is managed using the Last In First
         * Out (LIFO) technique (The same way std::stack works). Therefore
         * you cannot transition between scenes at random. The order in which
         * scenes are added to the Engine is important.
         *
         * For example, if while in the "gameplay" scene, you push a "pause" scene
         * to the game engine, the "gameplay" scene will be paused (onPause called)
         * and the "pause" scene will be entered (onStart called on the pause scene
         * instance) and the "pause" scene will become the active scene (gets system
         * events, updates and rendered). If you pop the "pause" scene, the engine
         * will destroy it (onStop called on the pause scene instance) and return
         * to the "gameplay" scene (onResume called on the gameplay scene instance).
         * However, if you push (transition to) another scene while in the "pause"
         * scene, the process repeats; the "pause" scene gets paused and the the new
         * scene becomes active)
         */
        class SceneManager final {
        public:
            /**
             * @brief Constructor
             * @param engine Game Engine
             */
            explicit SceneManager(Engine* engine);

            /**
             * @brief Copy constructor
             */
            SceneManager(const SceneManager&) = delete;

            /**
             * @brief Copy assignment operator
             */
            SceneManager& operator=(const SceneManager&) = delete;

            /**
             * @brief Add a Scene
             * @param scene Scene to be added
             * @param enterScene True to immediately enter the scene or false to
             *                   delay entry
             *
             * The scene maybe be added and entered immediately or delayed. By
             * default the scene is delayed. This option allows multiple scenes
             * to be pushed at once to the game engine
             *
             * for example you may want to add multiple scenes in one frame but
             * only enter the last one first as follows:
             *
             * @code
             * sceneManager.pushScene(MainMenu);
             * sceneManager.pushScene(Loading);
             * sceneManager.pushScene(Splash, true);
             *
             * // Because of LIFO, the scenes will be arranged in reverse order:
             * //   Splash -> Loading -> MainMenu.
             * //
             * // Because the enter flag is set to true when pushing the "Splash"
             * // scene, the scene will be added and entered immediately. Let's
             * // assume that the "Splash" and the "Loading" scenes pop themselves
             * // from the game engine when ready. The game flow will be as follows:
             * //
             * // The "Splash" scene shows for some time then pops/removes itself
             * // from the game engine, this triggers the engine to check for the
             * // next available scene. It finds the "Loading" scene and enters it.
             * // After the "Loading" scene has finished (say, loading some resources),
             * // it pops/removes itself. This again triggers the engine to check
             * // for the next available scene, it finds the "MainMenu" scene and
             * // enters it. From here the user may transition to the "Gameplay"
             * // or "Quit" scene
             * @endcode
             *
             * @note A scene can only belong to one scene manager and it cannot
             * be added more than once to the scene manager that owns it
             *
             * @warning The pointer must not be null
             *
             * @see enterTopScene
             */
            void pushScene(Scene::Ptr scene, bool enterScene = false);

            /**
             * @brief Remove the current active scene
             * @param resumePrev True to resume the previous scene, otherwise false
             */
            void popScene(bool resumePrev = true);

            /**
             * @brief Get the top scene
             * @return A pointer to the top scene if active otherwise a nullptr
             */
            Scene* getActiveScene();
            const Scene* getActiveScene() const;

            /**
             * @brief Get the scene that was previously active
             * @return A pointer to a previously active scene it it exists,
             *         otherwise a nullptr
             */
            Scene* getPreviousScene();
            const Scene* getPreviousScene() const;

            /**
             * @brief Get the engine scene
             * @return A pointer to a engine scene it it exists,
             *         otherwise a nullptr
             */
            EngineScene* getEngineScene();
            const EngineScene* getEngineScene() const;

            /**
             * @brief Cache a scene
             * @param name The unique scene identifier
             * @param scene The scene to be cached
             */
            void cache(const std::string& name, Scene::Ptr scene);

            /**
             * @brief Check if a scene with a given name exists in the cache list
             * @param name The name of the scene to be checked
             * @return True if it exists, otherwise false
             */
            bool isCached(const std::string& name) const;

            /**
             * @brief Move a scene out of the cache
             * @param name The name of the scene to push
             * @return The requested scene if it exists, otherwise a nullptr
             */
            Scene::Ptr popCached(const std::string& name);

            /**
             * @brief Get access to a scene in the cache
             * @param name The name of the scene to get access to
             * @return A pointer to the scene if it exists, otherwise a nullptr
             */
            Scene* getCached(const std::string& name);
            const Scene* getCached(const std::string& name) const;

            /**
             * @brief Get the current number of scenes
             * @return The current number of scenes
             */
            std::size_t getSceneCount() const;

            /**
             * @brief Enter the scene at the top of the stack
             *
             * This function will enter the last scene to be added and set it
             * as the active scene. This function has no effect if the scene
             * is already entered or the manager has no scenes to manage
             */
            void enterTopScene() const;

            /**
             * @brief Destroy all scenes
             *
             * This function destroys all scenes regardless of their current
             * state
             *
             * @see clearAllExceptActive
             */
            void clear();

            /**
             * @brief Clear cached scenes
             */
            void clearCachedScenes();

            /**
             * @brief Remove all scenes except the current active scene
             *
             * Note that if there is no active scene (Scene at the top of
             * the stack is not entered), then all the scenes will be removed
             *
             * @see clear
             */
            void clearAllExceptActive();

            /**
             * @brief Check if the scene manager is empty or not
             * @return True if the scene manager is empty, otherwise false
             */
            bool isEmpty() const;

            /**
             * @brief Render the current scene
             */
            void render();

            /**
             * @brief Update the active scene
             * @param deltaTime Time passed since the last update
             * @param fixedUpdate True if its a fixed update, otherwise false
             */
            void update(const Time& deltaTime, bool fixedUpdate);

            /**
             * @brief Post update active scene
             */
            void postUpdate();

            /**
             * @brief Handle a system event
             * @param event Event to be handled
             */
            void handleEvent(SystemEvent event);

            /**
             * @brief Destructor
             */
            ~SceneManager();

        private:
            /**
             * @brief Update previous scene pointer
             */
            void updatePreviousScene();

        private:
            Engine* engine_;                //!< Pointer to the game engine
            std::stack<Scene::Ptr> scenes_; //!< Scenes container
            Scene* prevScene_;              //!< Pointer to the active scene before a push operation
            std::unordered_map<std::string, Scene::Ptr> cachedScenes_;
            std::unique_ptr<priv::EngineScene> engineScene_;
        };
    }
}

#endif // MIGHTER2D_SCENEMANAGER_H
