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

#ifndef MIGHTER2D_SCENE_H
#define MIGHTER2D_SCENE_H

#include "Mighter2d/Config.h"
#include "Mighter2d/core/time/Time.h"
#include "Mighter2d/core/event/SystemEvent.h"
#include "Mighter2d/core/event/GlobalEventEmitter.h"
#include "Mighter2d/core/input/InputManager.h"
#include "Mighter2d/core/audio/AudioManager.h"
#include "Mighter2d/core/time/TimerManager.h"
#include "Mighter2d/common/PropertyContainer.h"
#include "Mighter2d/common/PrefContainer.h"
#include "Mighter2d/core/scene/RenderLayerContainer.h"
#include "Mighter2d/ui/GuiContainer.h"
#include "Mighter2d/graphics/Camera.h"
#include "Mighter2d/core/grid/Grid.h"
#include "Mighter2d/common/ISystemEventHandler.h"
#include "Mighter2d/core/physics/Collidable.h"
#include "Mighter2d/core/scene/SceneStateObserver.h"
#include <string>
#include <memory>
#include <vector>

namespace mighter2d {
    class Engine;
    class Window;
    class BackgroundScene;

    /// @internal
    namespace priv {
       class SceneManager;
    }

    /**
     * @brief A base class for game scenes
     */
    class MIGHTER2D_API Scene : public Object {
    public:
        using Ptr = std::unique_ptr<Scene>; //!< Unique Scene pointer

        /**
         * @brief Default Constructor
         */
        Scene();

        /**
         * @brief Copy constructor
         */
        Scene(const Scene&) = delete;

        /**
         * @brief Copy assignment operator
         */
        Scene& operator=(const Scene&) = delete;

        /**
         * @brief Move constructor
         */
        Scene(Scene&&) noexcept ;

        /**
         * @brief Move assignment operator
         */
        Scene& operator=(Scene&&) noexcept ;

        /**
         * @brief Create an empty scene
         * @return The created scene
         */
        static Scene::Ptr create();

        /**
         * @brief Handle a scene initialization
         *
         * This function is called once by Mighter2d when the base scene is ready
         * to be used. It is called after the constructor but before
         * onStart().
         *
         * @note Some functions of mighter2d::Scene cannot be called in the
         * constructor because the scene is not yet initialized. Invoking
         * such functions will result in a mighter2d::AccessViolationException
         * being thrown. Therefore, this function is intended for situation where
         * you'd want to access such a function before the scene is entered
         *
         * Note that implementing this function is optional. Mighter2d will never
         * pu anything inside this function, therefore you don't have to call the
         * base class method in your implementation
         *
         * @see onStart
         */
         virtual void onReady() {}

        /**
         * @brief Handle a scene enter
         *
         * This function is called once by Mighter2d when the scene is started for
         * the first time
         *
         * Note that implementing this function is optional. Mighter2d will never put
         * anything inside this function, therefore you don't have to call the
         * base class method in your implementation
         *
         * @see onReady
         */
        virtual void onStart() {};

        /**
         * @brief Handle a scene system event
         * @param event A system event to be handled
         *
         * This function is called by Mighter2d when a system event is triggered.
         * It is called before the scene is updated and rendered.
         *
         * Note that implementing this function is optional. Mighter2d will never put
         * anything inside this function, therefore you don't have to call the
         * base class method in your implementation
         */
        virtual void onHandleEvent(SystemEvent event) {MIGHTER2D_UNUSED(event);};

        /**
         * @brief Handle a scene frame-rate dependent update
         * @param deltaTime The time passed since the last update
         *
         * This function is called by Mighter2d once per frame. The delta passed to it
         * is frame rate dependent. This means that it depends on how long the
         * previous frame took to complete. Note that this function is called
         * after onFixedUpdate()
         *
         * All updates that should be synced with the render fps must be
         * done in this function.
         *
         * Note that implementing this function is optional. Mighter2d will never put
         * anything inside this function, therefore you don't have to call the
         * base class method in your implementation
         *
         * @see onFixedUpdate
         */
        virtual void onUpdate(Time deltaTime) {MIGHTER2D_UNUSED(deltaTime);}

        /**
         * @brief Handle a scene frame-rate independent update
         * @param deltaTime The time passed since last update
         *
         * This function is called by Mighter2d every fixed frame-rate frame. It is
         * called before onUpdate() and may be called once per frame, multiple
         * times per frame or not called at all. The delta passed to it is always
         * the same and is independent of the render fps.  see mighter2d::Engine::setPhysicsUpdateFrameRate
         * to determine the delta
         *
         * Note that implementing this function is optional and must be
         * overridden if needed. Mighter2d will never put anything inside this
         * function, therefore you don't have to call the base class method
         * in your implementation
         *
         * @see onUpdate
         */
        virtual void onFixedUpdate(Time deltaTime) {MIGHTER2D_UNUSED(deltaTime);}

        /**
         * @brief Handle a scene pre-render event
         *
         * This function is called by Mighter2d once per frame before the scene is
         * rendered
         *
         * Note that implementing this function is optional. Mighter2d will never put
         * anything inside this function, therefore you don't have to call the
         * base class method in your implementation
         *
         * @see onPostRender
         */
        virtual void onPreRender() {}

        /**
         * @brief Handle a scene post-render event
         *
         * This function is called by Mighter2d once per frame after the scene is
         * rendered.
         *
         * Note that implementing this function is optional. Mighter2d will never put
         * anything inside this function, therefore you don't have to call the
         * base class method in your implementation
         *
         * @see onPreRender
         */
        virtual void onPostRender() {}

        /**
         * @brief Handle a scene pause event
         *
         * This function is called by Mighter2d if another scene is pushed over this
         * scene while it was active (see mighter2d::Engine::pushScene)
         *
         * Note that implementing this function is optional. Mighter2d will never put
         * anything inside this function, therefore you don't have to call the
         * base class method in your implementation
         *
         * @see onResume
         */
        virtual void onPause() {};

        /**
         * @brief Handle a scene resume event
         * @param fromCache True if the scene was resumed from the cache or false
         *                  if it was a normal resumed
         *
         * This function is called by Mighter2d when this scene is resumed from a
         * paused state. This occurs when a scene that was pushed over this
         * scene is removed from the Engine (see mighter2d::Engine::popScene)
         *
         * Note that implementing this function is optional. Mighter2d will never put
         * anything inside this function, therefore you don't have to call the
         * base class method in your implementation
         *
         * @see onPause
         */
        virtual void onResume(bool fromCache) {MIGHTER2D_UNUSED(fromCache); };

        /**
         * @brief Handle a scene stop event
         *
         * This function is called by Mighter2d when the scene is removed from the
         * engine (either by popping or by caching). See mighter2d::Engine::popScene
         *
         * Note that implementing this function is optional. Mighter2d will never put
         * anything inside this function, therefore you don't have to call the
         * base class method in your implementation
         *
         * @see setCacheOnExit, mighter2d::Engine::cacheScene
         */
        virtual void onStop() {};

        /**
         * @brief Handle a scene destroy event
         *
         * This function is called by Mighter2d when the scene is about to be
         * completely removed from the engine. After this function executes, the
         * scenes destructor will be invoked.
         *
         * Note that implementing this function is optional. Mighter2d will never put
         * anything inside this function, therefore you don't have to call the
         * base class method in your implementation
         */
        virtual void onDestroy() {}

        /**
         * @brief Handle a scene frame begin event
         *
         * This function is called by Mighter2d when the current frame begins while
         * this scene is active
         *
         * Note that implementing this function is optional. Mighter2d will never put
         * anything inside this function, therefore you don't have to call the
         * base class method in your implementation
         *
         * @see onFrameEnd
         */
        virtual void onFrameBegin() {};

        /**
         * @brief Handle a scene frame end event
         *
         * This function is called by Mighter2d when the current frame ends and this
         * scene is active
         *
         * Note that implementing this function is optional. Mighter2d will never put
         * anything inside this function, therefore you don't have to call the
         * base class method in your implementation
         *
         * @see onFrameBegin
         */
        virtual void onFrameEnd() {}

        /**
         * @brief Get the name of this class
         * @return The name of this class
         *
         * @see getBaseClassName
         */
        std::string getClassName() const override;

        /**
         * @brief Check if the scene has been started or not
         * @return True if the scene is started, otherwise false
         *
         * @see onStart
         */
        bool isStarted() const;

        /**
         * @brief Check if the scene is active or not
         * @return True if active, otherwise false
         *
         * The scene is active if the engine is currently running it.
         * Note that only one scene can be active at a time
         */
        bool isActive() const;

        /**
         * @brief Check if the scene is paused or not
         * @return True if paused, otherwise false
         *
         * @see onPause
         */
        bool isPaused() const;

        /**
         * @brief Set whether or not the scene is visible when it is paused
         * @param visible True to set visible, otherwise false
         *
         * By default, the scene is hidden when it is paused
         *
         * @see onPause
         */
        void setVisibleOnPause(bool visible);

        /**
         * @brief Check if the scene is visible when paused
         * @return True if visible, otherwise false
         *
         * @see setVisibleOnPause
         */
        bool isVisibleOnPause() const;

        /**
         * @brief Add a background scene to this scene
         * @param scene The scene to be the background of this scene
         *
         * Note that setting a new background scene destroys the previous
         * one. In addition, pass @a nullptr to remove the background scene
         * without replacing it
         *
         * By default, the scene does not have a background scene
         *
         * @see getBackgroundScene
         */
        void setBackgroundScene(std::unique_ptr<BackgroundScene> scene);

        /**
         * @brief Get the background scene of this scene
         * @return A pointer to the background scene if it exists, otherwise a
         *         nullptr
         *
         * @see setBackgroundScene
         */
        BackgroundScene* getBackgroundScene();
        const BackgroundScene* getBackgroundScene() const;

        /**
         * @brief Check if the scene has a background scene or not
         * @return True if it has a background scene, otherwise false
         *
         * @see setBackgroundScene
         */
        bool hasBackgroundScene() const;

        /**
         * @brief Set whether or not the scene is cached when popped from the engine
         * @param cache True to cache or false to uncache
         * @param alias A unique name for identification during retrieval
         *
         * A cached scene is not destroyed when popped from the Engine but
         * rather saved for reuse. For example, instead of instantiating a
         * new pause menu scene every time the game is paused, you can cache
         * the pause menu scene instance and return to it whenever the game
         * is paused. This may improve performance.
         *
         * Unlike mighter2d::Engine::cacheScene, this function will cache the scene
         * after it was active. To remove the scene from the cache simply call
         * @e setCacheOnExit(false)
         *
         * By default the scene is not cached when popped
         *
         * @see isCached, mighter2d::Engine::cacheScene, mighter2d::Engine::uncacheScene
         */
        void setCacheOnExit(bool cache, const std::string& alias = "");

        /**
         * @brief Check if the scene is cached or not
         * @return True if cached, otherwise false
         *
         * @see setCacheOnExit
         */
        bool isCached() const;

        /**
         * @brief Set the scene timescale factor
         * @param timescale The new scene timescale factor
         *
         * A timescale factor affects everything that requires a time
         * update. This includes timers, animations, physics etc...
         *
         * For example, if the timescale is set to 2.0f, then scene timers
         * will count twice as fast, animations will play twice as fast,
         * physics objects will move twice as fast etc..
         *
         * By default the timescale is 1.0f (real-time)
         *
         * @see getTimescale
         */
        void setTimescale(float timescale);

        /**
         * @brief Get the scenes timescale factor
         * @return The scenes timescale factor
         *
         * @see setTimescale
         */
        float getTimescale() const;

        /**
         * @brief Get a reference to the game engine
         * @return A reference to the game engine
         * @throws AccessViolationException If this function is called before
         *         the scene is initialized
         *
         * Note that all scenes refer to the same Engine instance
         */
        Engine& getEngine();
        const Engine& getEngine() const;

        /**
         * @brief Get the game window
         * @return The game window
         * @throws AccessViolationException If this function is called before
         *         the scene is initialized
         *
         * Note that all scenes refer to the same game Window instance
         */
        Window& getWindow();
        const Window& getWindow() const;

        /**
         * @brief Get the scene state observer
         * @return The scene state observer
         *
         * This object allows you to add event listeners to the
         * state changes of the scene:
         *
         * @code
         * scene.getStateObserver().onPause([] {
         *      std::cout << "scene paused";
         * });
         * @endcode
         */
        SceneStateObserver& getStateObserver();
        const SceneStateObserver& getStateObserver() const;

        /**
         * @brief Get the scene level camera
         * @return The scene level camera
         * @throws AccessViolationException If this function is called before
         *         the scene is initialized
         *
         * Note that this camera is the default/main camera and is always
         * rendered last. You can add other cameras to the scene using
         * getCameras()
         *
         * @see getCameras
         */
        Camera& getCamera();
        const Camera& getCamera() const;

        /**
         * @brief Get the engine level cache
         * @return The engine level cache
         * @throws AccessViolationException If this function is called before
         *         the scene is initialized
         *
         * Data stored in the cache persists from scene to scene. This means
         * that another scene can access or modify data stored by another scene.
         * The data can also be accessed using mighter2d::Engine::getCache
         *
         * @note The cache only stores data, while the engine is running. When
         * the engine is shutdown, the data in the cache is destroyed
         *
         * @see getSCache
         */
        PropertyContainer& getCache();
        const PropertyContainer& getCache() const;

        /**
         * @brief Get the engine level savable cache
         * @return The engine level savable cache
         * @throws AccessViolationException If this function is called before
         *         the scene is initialized
         *
         * Data stored in the cache persists from scene to scene. This means
         * that another scene can access or modify data stored by another scene.
         * Unlike mighter2d::Scene::getCache, this cache can be initialized with data
         * read from a file and can also be saved to a file. The data can also
         * be accessed using mighter2d::Engine::getSavableCache
         *
         * @note The cache only stores data, while the engine is running. When
         * the engine is shutdown, the data in the cache is destroyed
         *
         * @see getCache
         */
        PrefContainer& getSCache();
        const PrefContainer& getSCache() const;

        /**
         * @brief Get the scene render layers
         * @return The scene render layers
         *
         * Render layers allow the scene to be rendered in separate layers
         * which are then composed back together. By default the scene has
         * a "default" layer at index 0. When a drawable object is added to
         * the scene without an explicit layer, it will be added to the default
         * layer. You can add objects to the "default" layer or even remove
         * the "default" layer from the render layer container, however you
         * mus not forget to reallocate the objects in the "default" layer to
         * another layer, otherwise they will not be drawn to the screen
         */
        RenderLayerContainer& getRenderLayers();
        const RenderLayerContainer& getRenderLayers() const;

        /**
         * @internal
         * @brief Initialize the scene
         * @param engine The engine that runs the scene
         *
         * This function initializes all scene components that depend on the
         * engine for their functionality. This function is called before the
         * scene is entered
         *
         * @warning This function is intended for internal use and should
         * never be called outside of Mighter2d
         */
        void init(Engine& engine);

        /**
         * @internal
         * @brief Add an updatable to the update list
         * @param updatable The updatable to be added
         *
         * @warning This function is intended for internal use only
         */
        void addUpdatable(IUpdatable* updatable);

        /**
         * @internal
         * @brief Remove an updatable from the update list
         * @param updatable The updatable to be removed
         * @return True if the updatable was removed or false if it does not exist
         *
         * @warning This function is intended for internal use only
         */
        bool removeUpdatable(IUpdatable* updatable);

        /**
         * @internal
         * @brief Add an collidable to the update list
         * @param collidable The collidable to be added
         *
         * @warning This function is intended for internal use only
         */
        void addCollidable(Collidable* collidable);

        /**
         * @internal
         * @brief Remove a collidable from the update list
         * @param collidable The collidable to be removed
         * @return True if the collidable was removed or false if it does not exist
         *
         * @warning This function is intended for internal use only
         */
        bool removeCollidable(Collidable* collidable);

        /**
         * @internal
         * @brief Add a system event handler to the event list
         * @param sysEventHandler The event handler to be added
         *
         * @warning This function is intended for internal use only
         */
        void addSystemEventHandler(ISystemEventHandler* sysEventHandler);

        /**
         * @internal
         * @brief Remove an updatable from the update list
         * @param sysEventHandler The system event handler to be removed
         * @return True if the system event handler was removed or false if it does not exist
         *
         * @warning This function is intended for internal use only
         */
        bool removeSystemEventHandler(ISystemEventHandler* sysEventHandler);

        /**
         * @brief Destructor
         */
        ~Scene() override;

    private:
        /**
         * @brief Helper function for starting a scene
         */
        void start();

        /**
         * @brief Helper function for setting scene as ready
         */
        void ready();

        /**
         * @brief Helper function for pausing a scene
         */
        void pause();

        /**
         * @brief Helper function for resuming a scene
         */
        void resume(bool fromCache = false);

        /**
         * @brief Helper function for exiting a scene
         */
        void stop();

        /**
         * @brief Helper function for destroying a scene
         */
        void destroy();

        /**
         * @brief Helper function for post updating a scene
         */
        void postUpdate();

        /**
         * @brief Helper function for updating a scene
         * @param isFixedUpdate True if update is fixed, false if variable
         */
        void update(const Time& deltaTime, bool isFixedUpdate);

        /**
         * @brief Helper function for handling a scene event
         * @param event The event to be handled
         */
        void handleEvent(SystemEvent event);

        /**
         * @brief Helper function for drawing the scene
         */
        void render();

        /**
         * @brief Helper function for executing scene frame begin
         */
        void frameBegin();

        /**
         * @brief Helper function for executing scene frame end
         */
        void frameEnd();

    private:
        std::vector<IUpdatable*> updateList_; //!< Update list
        std::vector<Collidable*> collidableList_; //!< Update list
        std::vector<ISystemEventHandler*> systemEventHandlerList_; //!< Update list
        Engine* engine_;                      //!< Game engine
        std::unique_ptr<Camera> camera_;      //!< Scene level camera
        SceneStateObserver sceneStateObserver_;//!< Scene level gui container
        RenderLayerContainer renderLayers_;   //!< Render layers for this scene
        float timescale_;                     //!< Controls the speed of the scene without affecting the render fps
        bool isStarted_;                      //!< A flag indicating whether or not the scene has been started
        bool isInitialized_;                  //!< A flag indicating whether or not the scene has been initialized
        bool isActive_;                       //!< A flag indicating whether or not the scene is active
        bool isPaused_;                       //!< A flag indicating whether or not the scene is paused
        bool isVisibleWhenPaused_;            //!< A flag indicating whether or not the scene is rendered behind the active scene when it is paused
        std::pair<bool, std::string> cacheState_;
        std::unique_ptr<BackgroundScene> backgroundScene_; //!< The background scene of this scene

        friend class priv::SceneManager;      //!< Pre updates the scene
    };
}

#endif // MIGHTER2D_SCENE_H

/**
 * @class mighter2d::Scene
 * @ingroup core
 *
 * A mighter2d::Scene represents a distinct state of your game, for example
 * loading, main menu, gameplay, paused and so on. All game scenes
 * exist in isolation and have no knowledge of each other. Only
 * one scene can be active at a time. For example, the game cannot
 * be in a main menu state and a gameplay state at the same time.
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
 *
 * Usage example:
 * @code
 * class StartUpScene : public mighter2d::Scene {
 *      void onStart() override {
 *          mighter2d::ui::Label::Ptr greeting = mighter2d::ui::Label::create("Thank you for using Infinite Motion Engine");
 *          greeting->setOrigin(0.5f, 0.5f);
 *          greeting->setPosition("50%", "50%");
 *          getGui().addWidget(std::move(greeting), "lblGreeting");
 *      }
 * }
 *
 * ...
 *
 * engine.pushScene(std::make_unique<StartUpScene>());
 * @endcode
 */
