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
#include "Mighter2d/core/event/Event.h"
#include "Mighter2d/core/event/GlobalEventEmitter.h"
#include "Mighter2d/core/input/InputManager.h"
#include "Mighter2d/core/audio/AudioManager.h"
#include "Mighter2d/core/time/TimerManager.h"
#include "Mighter2d/common/PropertyContainer.h"
#include "Mighter2d/common/PrefContainer.h"
#include "Mighter2d/core/scene/GameObjectContainer.h"
#include "Mighter2d/core/scene/RenderLayerContainer.h"
#include "Mighter2d/core/scene/DrawableContainer.h"
#include "Mighter2d/core/scene/GridMoverContainer.h"
#include "Mighter2d/core/scene/CameraContainer.h"
#include "Mighter2d/ui/GuiContainer.h"
#include "Mighter2d/graphics/Camera.h"
#include "Mighter2d/core/grid/Grid.h"
#include <string>
#include <memory>
#include <vector>

namespace mighter2d {
    class Engine;
    class Window;

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
         * This function is called by Mighter2d when the base scene is ready
         * to be used. It is called once after the constructor but before
         * onEnter(). Note that mighter2d::Scene functions cannot be called in the
         * constructor, doing so is undefined behavior. Thus, this function is
         * intended for situation where Mighter2d scene functions need to be accessed
         * before the scene is entered.
         *
         * Note that implementing this function is optional. Mighter2d will never put
         * anything inside this function, therefore you don't have to call the
         * base class method in your implementation
         */
        virtual void onInit() {};

        /**
         * @brief Handle a scene enter
         *
         * This function is called once by Mighter2d when the scene is entered for
         * the first time
         *
         * Note that implementing this function is optional. Mighter2d will never put
         * anything inside this function, therefore you don't have to call the
         * base class method in your implementation
         */
        virtual void onEnter() {};

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
        virtual void onHandleEvent(Event event) {MIGHTER2D_UNUSED(event);};

        /**
         * @brief @brief Handle a scene pre-update
         * @param deltaTime The time passed since the last update
         *
         * This function is called by Mighter2d before the scene is updated. This
         * include physics, animations, timer updates etc. It is called before
         * onUpdate() and onFixedUpdate()
         *
         * Note that implementing this function is optional. Mighter2d will never put
         * anything inside this function, therefore you don't have to call the
         * base class method in your implementation
         */
        virtual void onPreUpdate(Time deltaTime) {MIGHTER2D_UNUSED(deltaTime);}

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
         * @brief Handle a scene post-update update
         * @param deltaTime The time passed since the last update
         *
         * This function is called by Mighter2d once per frame. It is called after
         * onUpdate() and onFixedUpdate(). It may be useful if you want to do
         * something after all normal updates have completed such as tracking
         * the position of an object whose position may have changed in
         * onFixedUpdate() or onUpdate().
         *
         * Note that implementing this function is optional. Mighter2d will never put
         * anything inside this function, therefore you don't have to call the
         * base class method in your implementation
         */
        virtual void onPostUpdate(Time deltaTime) {MIGHTER2D_UNUSED(deltaTime);}

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
        virtual void onResume() {};

        /**
         * @brief Handle a scene cache event
         *
         * This function is called by Mighter2d when this scene is cached.
         * (see setCached() and mighter2d::Engine::cacheScene)
         *
         * Note that implementing this function is optional. Mighter2d will never put
         * anything inside this function, therefore you don't have to call the
         * base class method in your implementation
         *
         * @see onResumeFromCache
         */
        virtual void onCache() {}

        /**
         * @brief Handle a scene 'resume from cache' event
         *
         * This function is called by Mighter2d when this scene is pushed to the
         * Engine from the engines scene cache list (see mighter2d::Engine::PushCachedScene)
         *
         * Note that implementing this function is optional. Mighter2d will never put
         * anything inside this function, therefore you don't have to call the
         * base class method in your implementation
         *
         * @see onCache, mighter2d::Engine::PushCachedScene, onPause, onResume
         */
        virtual void onResumeFromCache() {};

        /**
         * @brief Handle a scene exit event
         *
         * This function is called by Mighter2d when the scene is removed from the
         * engine (either by destruction or by caching). See mighter2d::Engine::popScene
         *
         * Note that implementing this function is optional. Mighter2d will never put
         * anything inside this function, therefore you don't have to call the
         * base class method in your implementation
         *
         * @see setCached, mighter2d::Engine::cacheScene
         */
        virtual void onExit() {};

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
         * @see getClassType
         */
        std::string getClassName() const override;

        /**
         * @brief Check if the scene has been entered or not
         * @return True if the scene has been entered, otherwise false
         *
         * @see onEnter
         */
        bool isEntered() const;

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
         * @throws AccessViolationException If this function is called before
         *         this scene is initialized or entered. Furthermore, the exception
         *         is thrown if this scene is a background scene or the scene
         *         that is passed in is as an argument is already a background
         *         scene of another scene or it has a background scene
         *
         * Note that the scene manages the lifecycle of its background scene.
         * That is, the background scene is destroyed when its parent scene
         * is destroyed. Furthermore, a scene can only have one background
         * scene at a time. Setting a new background scene destroys the previous
         * scene. Pass nullptr to remove the background scene.
         *
         * @warning Mighter2d does not support nested background scene. That is, a
         * background scene of another scene cannot itself have a background
         * scene. Setting a scene that has a background scene as a background
         * scene for another scene throws a mighter2d::AccessViolationException
         * exception
         *
         * By default, the scene does not have a background scene
         *
         * @see onInit, onEnter
         */
        void setBackgroundScene(Scene::Ptr scene);

        /**
         * @brief Get the scene this scene is a background of
         * @return Pointer to the parent scene if this scene is a background
         *         scene, otherwise a nullptr
         *
         * The scene has a parent scene when it is set as the background of
         * another scene. The scene it is set as the background of becomes
         * its parent scene
         *
         * By default, the scene does not have a parent scene
         *
         * @see setBackgroundScene
         */
        Scene* getParentScene();
        const Scene* getParentScene() const;

        /**
         * @brief Get the background scene of this scene
         * @return A pointer to the background scene if it exists, otherwise a
         *         nullptr
         *
         * @see setBackgroundScene
         */
        Scene* getBackgroundScene();
        const Scene* getBackgroundScene() const;

        /**
         * @brief Check if the scene is in a background scene
         * @return True if it is a background scene, otherwise false
         *
         * The scene becomes a background scene when it is set as the
         * background of another scene
         *
         * By default, the scene is not a background scene
         *
         * @see setBackgroundScene
         */
        bool isBackgroundScene() const;

        /**
         * @brief Check if the scene has a background scene or not
         * @return True if it has a background scene, otherwise false
         *
         * @see setBackgroundScene
         */
        bool hasBackgroundScene() const;

        /**
         * @brief Set whether or not the scenes background scene is rendered
         * @param drawable True to render the background scene, otherwise false
         *
         * When the @a drawable argument is set to @a false, the background
         * scene is not rendered when this scene is rendered and when it is
         * set to @a true, the background scene is rendered behind this scene
         *
         * By default, the background scene is rendered when the parent scene
         * is rendered
         *
         * @see isBackgroundSceneDrawable
         */
        void setBackgroundSceneDrawable(bool drawable);

        /**
         * @brief Check if the scenes background scene is rendered
         * @return True if the background scene is rendered, otherwise false
         *
         * @see setBackgroundSceneDrawable
         */
        bool isBackgroundSceneDrawable() const;

        /**
         * @brief Set whether or not the scenes background scene receives time updates
         * @param enable True to enable background scene time updates, otherwise false
         *
         * When updates for a background scene is disabled, its onUpdate(),
         * onFixedUpdate(), onPreUpdate() and onPostUpdate() functions
         * are not invoked
         *
         * By default, the background scene is updated
         *
         * @see setBackgroundScene, isBackgroundSceneUpdateEnabled
         */
        void setBackgroundSceneUpdateEnable(bool enable);

        /**
         * @brief Check if the scenes background scene receives time updates or not
         * @return True if it receives time updates, otherwise false
         *
         * @see setBackgroundSceneUpdateEnable
         */
        bool isBackgroundSceneUpdateEnabled() const;

        /**
         * @brief Enable or disable events for the scenes background scene
         * @param enable True to enable background scene events, otherwise false
         *
         * When events for a background scene are disabled, the background
         * scene does not receive system updates such as input (keyboard,
         * mouse, joystick etc), window events etc. Furthermore its
         * onHandleEvent() function is not invoked
         *
         * By default, background scene events are disabled
         *
         * @see isBackgroundSceneEventsEnabled
         */
        void setBackgroundSceneEventsEnable(bool enable);

        /**
         * @brief Check if events are enabled for the background scene or not
         * @return True if events are enabled, otherwise false
         *
         * @see setBackgroundSceneEventsEnable
         */
        bool isBackgroundSceneEventsEnabled() const;

        /**
         * @brief Cache or uncahe the scene
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
         * @e setCached(false)
         *
         * By default the scene is not cached
         *
         * @see isCached, mighter2d::Engine::cacheScene, mighter2d::Engine::uncacheScene
         */
        void setCached(bool cache, const std::string& alias = "");

        /**
         * @brief Check if the scene is cached or not
         * @return True if cached, otherwise false
         *
         * @see setCached
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
         * @brief Get the scene level camera container
         * @return The scene level camera container
         * @throws AccessViolationException If this function is called before
         *         the scene is initialized
         *
         * @see getCamera
         */
        CameraContainer& getCameras();
        const CameraContainer& getCameras() const;

        /**
         * @brief Get the scene level GridMover container
         * @return The scene level grid mover container
         */
        GridMoverContainer& getGridMovers();
        const GridMoverContainer& getGridMovers() const;

        /**
         * @brief Get the scene level input manager
         * @return The scene level input manager
         *
         * The input manager is local to the scene instance. This means
         * that input listeners registered on it are only invoked when
         * the scene is active and de-registered when the scene is destroyed
         *
         * @see mighter2d::Engine::getInputManager
         */
        input::InputManager& getInput();
        const input::InputManager& getInput() const;

        /**
         * @brief Get the scene level audio manager
         * @return The scene level audio manager
         *
         * The audio manager is local to the scene instance. All audio played
         * by it is destroyed when the scene is destroyed
         *
         * @see mighter2d::Engine::getAudioManager
         */
        audio::AudioManager& getAudio();
        const audio::AudioManager& getAudio() const;

        /**
         * @brief Get the scene level timer manager
         * @return The scene level timer manager
         *
         * The timer manager is local to the scene instance. This means that
         * callbacks scheduled on it will only be dispatched when the scene
         * is active
         *
         * @see mighter2d::Engine::getTimer
         */
        TimerManager& getTimer();
        const TimerManager& getTimer() const;

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
         * @brief Get the scene level grid
         * @return The scene level grid
         * @throws AccessViolationException If this function is called without
         *         creating the grid first
         *
         * Note that only one grid can be created per scene
         *
         * @warning The grid must be created before it is used. Calling
         * this function before the grid is created is undefined behavior
         *
         * @see createGrid2D
         */
        Grid& getGrid();
        const Grid& getGrid() const;

        /**
         * @brief Get the scene level gui container
         * @return The scene level gui container
         * @throws AccessViolationException If this function is called before
         *         the scene is initialized
         *
         * The gui container is local to the scene. This means that all widgets
         * in it are destroyed when the scene is destroyed
         *
         * @see mighter2d::Engine::getGui
         */
        ui::GuiContainer& getGui();
        const ui::GuiContainer& getGui() const;

        /**
         * @brief Get the scene geometry shape container
         * @return The geometry shape container
         *
         * You may use this class to create geometry shape instead of using
         * their respective constructors. Consult the ShapeContainer class
         * definition for more info
         *
         * @warning Do not keep the returned reference
         */
        ShapeContainer& getShapes();
        const ShapeContainer& getShapes() const;

        /**
         * @brief Get the scene level game object container
         * @return The scene level game object container
         *
         * This class stores game objects that belong to this scene. All
         * game objects in this container are automatically updated
         *
         * @warning Do not keep the returned reference
         */
        GameObjectContainer& getGameObjects();
        const GameObjectContainer& getGameObjects() const;

        /**
         * @brief Get the scene level sprite container
         * @return The scene level sprite container
         *
         * This class stores the sprites in the scene. The sprite's animator
         * will automatically be updated
         */
        SpriteContainer& getSprites();
        const SpriteContainer& getSprites() const;

        /**
         * @brief Create the scene level grid instance
         * @param tileWidth The width of the grid
         * @param tileHeight The height of the grid
         *
         * Note that this function only creates a grid instance so that
         * it can be used. You still need to construct the grid using
         * the appropriate member function
         *
         * @warning Only a single grid can be created, therefore calling
         * this function will destroy the previous grid
         *
         * @see getGrid
         */
        void createGrid2D(unsigned int tileWidth, unsigned int tileHeight);

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
         * @brief Add an event listener to a scene event
         * @param event Event to add event listener to
         * @param callback Function to be execute when the event is fired
         * @return The callback id
         *
         * @warning This function is intended for internal use and should
         * never be called outside of Mighter2d
         */
        template<typename ...Args>
        int on_(const std::string& event, Callback<Args...> callback) {
            return internalEmitter_.on(event, callback);
        }

        /**
         * @internal
         * @brief Unsubscribe from a scene event
         * @param event The event to unsubscribe from
         * @param id The event listeners id
         * @return True if the event listener was unsubscribed or false if
         *         @a event does not have a listener with the @a id
         *
         * @warning This function is intended for internal use and should
         * never be called outside of Mighter2d
         */
        bool unsubscribe_(const std::string& event, int id);

        /**
         * @brief Destructor
         */
        ~Scene() override;

    private:
        std::unique_ptr<Camera> camera_;      //!< Scene level camera
        input::InputManager inputManager_;    //!< Scene level input manager
        audio::AudioManager audioManager_;    //!< Scene level audio manager
        EventEmitter internalEmitter_;        //!< Emits internal scene events
        TimerManager timerManager_;           //!< Scene level timer manager
        ui::GuiContainer guiContainer_;       //!< Scene level gui container
        RenderLayerContainer renderLayers_;   //!< Render layers for this scene
        GridMoverContainer gridMovers_;       //!< Stores grid movers that belong to the scene
        std::unique_ptr<Grid> grid2D_;      //!< Scene level grid
        float timescale_;                     //!< Controls the speed of the scene without affecting the render fps
        bool isEntered_;                      //!< A flag indicating whether or not the scene has been entered
        bool isInitialized_;                  //!< A flag indicating whether or not the scene has been initialized
        bool isPaused_;                       //!< A flag indicating whether or not the scene is paused
        bool isVisibleWhenPaused_;            //!< A flag indicating whether or not the scene is rendered behind the active scene when it is paused
        bool isBackgroundSceneDrawable_;      //!< A flag indicating whether or not the scenes background scene is rendered
        bool isBackgroundSceneUpdated_;       //!< A flag indicating whether or not the scenes background scene receives time updates
        bool isBackgroundSceneEventsEnabled_; //!< A flag indicating whether or not the scenes background scene receives system events
        bool hasGrid2D_;                      //!< A flag indicating whether or not the scene has a grid
        std::pair<bool, std::string> cacheState_;
        Scene* parentScene_;                  //!< The parent scene of this scene when it is in the background of another scene
        Scene::Ptr backgroundScene_;          //!< The background scene of this scene
        friend class priv::SceneManager;      //!< Pre updates the scene

        std::unique_ptr<std::reference_wrapper<Engine>> engine_;           //!< A reference to the game engine
        std::unique_ptr<std::reference_wrapper<Window>> window_;           //!< A reference to the game window
        std::unique_ptr<CameraContainer> cameraContainer_;                 //!< Stores cameras that belong to the scene
        std::unique_ptr<SpriteContainer> spriteContainer_;                 //!< Stores sprites that belong to the scene
        std::unique_ptr<GameObjectContainer> entityContainer_;             //!< Stores game objects that belong to the scene
        std::unique_ptr<ShapeContainer> shapeContainer_;                   //!< Stores shapes that belong to the scene
        std::unique_ptr<std::reference_wrapper<PropertyContainer>> cache_; //!< The engine level cache
        std::unique_ptr<std::reference_wrapper<PrefContainer>> sCache_; //!< The engine level savable cache
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
 * and the "pause" scene will be entered (onEnter called on the pause scene
 * instance) and the "pause" scene will become the active scene (gets system
 * events, updates and rendered). If you pop the "pause" scene, the engine
 * will destroy it (onExit called on the pause scene instance) and return
 * to the "gameplay" scene (onResume called on the gameplay scene instance).
 * However, if you push (transition to) another scene while in the "pause"
 * scene, the process repeats; the "pause" scene gets paused and the the new
 * scene becomes active)
 *
 * Usage example:
 * @code
 * class StartUpScene : public mighter2d::Scene {
 *      void onEnter() override {
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
