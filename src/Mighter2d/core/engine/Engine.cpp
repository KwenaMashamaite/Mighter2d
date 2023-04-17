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

#include "Mighter2d/core/engine/Engine.h"
#include "Mighter2d/core/time/Clock.h"
#include "Mighter2d/core/scene/SceneManager.h"
#include "Mighter2d/core/resources/ResourceManager.h"
#include "Mighter2d/graphics/RenderTarget.h"
#include "Mighter2d/utility/Helpers.h"
#include "Mighter2d/core/exceptions/Exceptions.h"

namespace mighter2d {
    namespace {
        bool isEngineInstantiated = false; // Only one engine instance can exist at a time

        template <class T>
        void setDefaultValueIfNotSet(PrefContainer& settings, const std::string& preference,
             PrefType  prefType , T&& defaultValue, const std::string& description)
        {
            if (settings.hasPref(preference) && settings.getPref(preference).hasValue())
                return;
            else if (!settings.hasPref(preference)) {
                settings.addPref({preference, prefType, std::forward<T>(defaultValue), description});
                MIGHTER2D_PRINT_WARNING(R"(Missing config entry ")" + preference + R"(", using default value)")
            } else {
                settings.getPref(preference).setValue<T>(std::forward<T>(defaultValue));
                MIGHTER2D_PRINT_WARNING(R"(Config entry ")" + preference + R"(" defined but it is not assigned any value, using default value)")
            }
        }
    }

    Engine::Engine(const std::string &gameName, const PrefContainer &settings) :
        Engine(gameName, "")
    {
        configs_ = settings;
    }

    Engine::Engine(const std::string &gameTitle, const std::string &settingsFile) :
        privWindow_{std::make_unique<priv::RenderTarget>()},
        window_{new Window(*privWindow_)},
        gameTitle_{gameTitle},
        settingFile_{settingsFile},
        isSettingsLoadedFromFile_(!settingsFile.empty() && settingsFile != "default"),
        isInitialized_{false},
        isRunning_{false},
        isPaused_{false},
        fixedUpdateFPS_{60},
        sceneManager_{std::make_unique<priv::SceneManager>(this)},
        popCounter_{0}
    {
        if (isEngineInstantiated)
            throw MultipleEngineInstanceException("Only one mighter2d::Engine instance can be created at a time");
        else
            isEngineInstantiated = true;
    }

    void Engine::initialize() {
        if (isSettingsLoadedFromFile_)
            loadSettings();

        processSettings();
        initResourceManager();
        initRenderTarget();

        eventDispatcher_ = GlobalEventEmitter::instance();
        isInitialized_ = true;

        eventEmitter_.emit("initialize");
    }

    void Engine::loadSettings() {
        configs_.load(settingFile_);
    }

    void Engine::processSettings() {
        setDefaultValueIfNotSet(configs_, "WINDOW_TITLE", PrefType::String, std::string("Untitled"), "The title of the render window");
        setDefaultValueIfNotSet(configs_, "WINDOW_ICON", PrefType::String, std::string("OS"), "The icon of the render window");
        setDefaultValueIfNotSet(configs_, "WINDOW_WIDTH", PrefType::Int, 600, "The width of the render window");
        setDefaultValueIfNotSet(configs_, "WINDOW_HEIGHT", PrefType::Int, 600, "The height of the render window");
        setDefaultValueIfNotSet(configs_, "FPS_LIMIT", PrefType::Int, 60, "The frames per second limit of the render window");
        setDefaultValueIfNotSet(configs_, "FULLSCREEN", PrefType::Bool, false, "Indicates whether or not the render window should be created in full screen mode");
        setDefaultValueIfNotSet(configs_, "V_SYNC", PrefType::Bool, false, "Indicates whether or not vertical synchronization should be enabled");
        setDefaultValueIfNotSet(configs_, "FONTS_DIR", PrefType::String, std::string(""), "The directory in which fonts can be found");
        setDefaultValueIfNotSet(configs_, "TEXTURES_DIR", PrefType::String, std::string(""), "The directory in which textures/images can be found");
        setDefaultValueIfNotSet(configs_, "SOUND_EFFECTS_DIR", PrefType::String, std::string(""), "The directory in which sound effects can be found");
        setDefaultValueIfNotSet(configs_, "MUSIC_DIR", PrefType::String, std::string(""), "The directory in which music can be found");
    }

    void Engine::initRenderTarget() {
        auto title = configs_.getPref("WINDOW_TITLE").getValue<std::string>();
        auto width = configs_.getPref("WINDOW_WIDTH").getValue<int>();
        auto height = configs_.getPref("WINDOW_HEIGHT").getValue<int>();

        MIGHTER2D_ASSERT(width > 0, "The width of the window cannot be negative")
        MIGHTER2D_ASSERT(height > 0, "The height of the window cannot be negative")

        // Create the window
        privWindow_->create(title, width, height, window_->getStyle());
        window_->setFullScreen(configs_.getPref("FULLSCREEN").getValue<bool>());
        window_->setFrameRateLimit(configs_.getPref("FPS_LIMIT").getValue<int>());
        window_->setVerticalSyncEnable(configs_.getPref("V_SYNC").getValue<bool>());

        // Set the window icon
        if (configs_.getPref("WINDOW_ICON").getValue<std::string>() != "OS")
            privWindow_->setIcon(configs_.getPref("WINDOW_ICON").getValue<std::string>());

        // Shutdown engine when window close event is triggered
        window_->defaultWinCloseHandlerId_ = window_->onClose([this] {
            quit();
        });

        // Because the window is destroyed and recreated when we enter full screen,
        // SFML does not generate a resized event. This cause some issues with
        // TGUI, so we associate a full screen toggle with a resize event
        window_->onFullScreenToggle([this] (bool) {
            SystemEvent event{};
            event.type = SystemEvent::Resized;
            event.size.width = window_->getSize().x;
            event.size.height = window_->getSize().y;
            sceneManager_->handleEvent(event);
        });
    }

    void Engine::initResourceManager() {
        resourceManager_ = ResourceManager::getInstance();
        resourceManager_->setPathFor(ResourceType::Font, configs_.getPref("FONTS_DIR").getValue<std::string>());
        resourceManager_->setPathFor(ResourceType::Texture, configs_.getPref("TEXTURES_DIR").getValue<std::string>());
        resourceManager_->setPathFor(ResourceType::Image, configs_.getPref("TEXTURES_DIR").getValue<std::string>());
        resourceManager_->setPathFor(ResourceType::SoundEffect, configs_.getPref("SOUND_EFFECTS_DIR").getValue<std::string>());
        resourceManager_->setPathFor(ResourceType::Music, configs_.getPref("MUSIC_DIR").getValue<std::string>());
    }

    void Engine::processEvents() {
        SystemEvent event{};
        while (privWindow_->pollEvent(event)) {
            if (event.type == SystemEvent::Closed)
                window_->emitCloseEvent();
            else if (event.type == SystemEvent::Resized) {
                // A mighter2d::Window may have a min and max size bounds
                window_->setSize(Vector2u{event.size.width, event.size.height});
                Vector2u newWinSize = window_->getSize();
                event.size.width = newWinSize.x;
                event.size.height = newWinSize.y;

                // Resize for bounds dimension is emitted by Window::setSize
                if (newWinSize != window_->getMinSize() && newWinSize != window_->getMaxSize())
                    window_->emitResize({event.size.width, event.size.height});

            } else if (event.type == SystemEvent::GainedFocus)
                window_->emitFocusChange(true);
            else if (event.type == SystemEvent::LostFocus)
                window_->emitFocusChange(false);
            else if (event.type == SystemEvent::MouseEntered)
                window_->emitMouseCursor(true);
            else if (event.type == SystemEvent::MouseLeft)
                window_->emitMouseCursor(false);

            sceneManager_->handleEvent(event);
        }
    }

    void Engine::run() {
        if (isRunning_) {
            MIGHTER2D_PRINT_WARNING("Engine cannot be rerun while it is running, stop it first then call 'run' again")
            return;
        }

        if (!isInitialized_)
            throw AccessViolationException("mighter2d::Engine must be initialized (see mighter2d::Engine::initialize()) first before calling mighter2d::Engine::run()");

        if (sceneManager_->isEmpty())
            throw AccessViolationException("mighter2d::Engine must have at least one mighter2d::Scene pushed to it before calling mighter2d::Engine::run(), see mighter2d::Engine::pushScene()");

        isRunning_ = true;
        Time deltaTime;
        Clock gameClock;
        eventEmitter_.emit("start");
        sceneManager_->enterTopScene();
        eventEmitter_.emit("sceneActivate", sceneManager_->getActiveScene());

        while (window_->isOpen() && isRunning_ && !sceneManager_->isEmpty()) {
            eventEmitter_.emit("frameStart");
            deltaTime = gameClock.restart();
            processEvents();
            update(deltaTime);
            clear();
            render();
            display();
            postFrameUpdate();
            elapsedTime_ += deltaTime;
            eventEmitter_.emit("frameEnd");
        }

        shutdown();
    }

    void Engine::quit() {
        isRunning_ = false;
    }

    void Engine::update(Time deltaTime) {
        if (isPaused_)
            return;

        const Time frameTime = seconds( 1.0f / static_cast<float>(fixedUpdateFPS_));
        static Time accumulator = Time::Zero;

        // Fixed update
        if (deltaTime.asSeconds() > 0.25f)
            deltaTime = seconds(0.25f);

        accumulator += deltaTime;

        while (accumulator >= frameTime) {
            sceneManager_->update(frameTime, true);
            accumulator -= frameTime;
        }

        sceneManager_->update(deltaTime, false);
    }

    void Engine::clear() {
        privWindow_->clear(window_->getClearColour());
    }

    void Engine::render() {
        sceneManager_->render();
    }

    void Engine::display() {
        privWindow_->display();
    }

    void Engine::pushScene(Scene::Ptr scene) {
        MIGHTER2D_ASSERT(scene, "A scene pushed to the engine cannot be a nullptr")
        if (!isRunning_)
            sceneManager_->pushScene(std::move(scene));
        else
            scenesPendingPush_.push(std::move(scene));
    }

    bool Engine::pushCachedScene(const std::string &name) {
        Scene::Ptr scene = sceneManager_->popCached(name);

        if (scene) {
            pushScene(std::move(scene));
            return true;
        }

        return false;
    }

    void Engine::popScene(int numScenes) {
        while (numScenes-- > 0) {
            if (!isRunning_)
                sceneManager_->popScene();
            else
                popCounter_++;
        }
    }

    void Engine::cacheScene(const std::string &name, Scene::Ptr scene) {
        MIGHTER2D_ASSERT(scene, "A cached scene cannot be a nullptr")
        sceneManager_->cache(name, std::move(scene));
    }

    Scene::Ptr Engine::uncacheScene(const std::string &name) {
        return sceneManager_->popCached(name);
    }

    bool Engine::isSceneCached(const std::string &name) const {
        return sceneManager_->isCached(name);
    }

    void Engine::removeAllScenesExceptActive() {
        sceneManager_->clearAllExceptActive();
        popCounter_ = 0;
    }

    void Engine::removeAllScenes() {
        popScene(static_cast<int>(sceneManager_->getSceneCount()));
    }

    std::size_t Engine::getSceneCount() const {
        return sceneManager_->getSceneCount();
    }

    Scene* Engine::getActiveScene() {
        return sceneManager_->getActiveScene();
    }

    const Scene* Engine::getActiveScene() const {
        return sceneManager_->getActiveScene();
    }

    Scene *Engine::getPreviousScene() {
        return const_cast<Scene*>(std::as_const(*this).getPreviousScene());
    }

    const Scene *Engine::getPreviousScene() const {
        if (isRunning_)
            return sceneManager_->getPreviousScene();
        else
            return nullptr;
    }

    Scene *Engine::getCachedScene(const std::string &name) {
        return sceneManager_->getCached(name);
    }

    const Scene *Engine::getCachedScene(const std::string &name) const {
        return sceneManager_->getCached(name);
    }

    void Engine::postFrameUpdate() {
        // Note: Always check pending pop first before pending pushes
        while (popCounter_ > 0) {
            if (sceneManager_->isEmpty()) { // Engine::PopScene called more than the number of scenes
                popCounter_ = 0;
                break;
            }

            bool isPrevSceneResumed = popCounter_ == 1 && scenesPendingPush_.empty();
            sceneManager_->popScene(isPrevSceneResumed);
            popCounter_--;

            if (isPrevSceneResumed && !sceneManager_->isEmpty())
                eventEmitter_.emit("sceneActivate", sceneManager_->getActiveScene());
        }

        while (!scenesPendingPush_.empty()) {
            Scene::Ptr scene = std::move(scenesPendingPush_.front());
            scenesPendingPush_.pop();
            bool isLastScene = scenesPendingPush_.empty();
            sceneManager_->pushScene(std::move(scene), isLastScene);

            if (isLastScene)
                eventEmitter_.emit("sceneActivate", sceneManager_->getActiveScene());
        }
    }

    void Engine::shutdown() {
        eventEmitter_.emit("shutdown");
        sceneManager_->clear();
        sceneManager_->clearCachedScenes();
        eventEmitter_.clear();
        isSettingsLoadedFromFile_ = false;
        elapsedTime_ = Time::Zero;
        gameTitle_.clear();
        settingFile_.clear();
        configs_.clear();
        dataSaver_.clear();
        diskDataSaver_.clear();
        resourceManager_.reset();
        eventDispatcher_.reset();

        while (!scenesPendingPush_.empty())
            scenesPendingPush_.pop();

        popCounter_ = 0;
        isInitialized_ = false;
        window_->close();
        isRunning_ = false;

        if (onShutdownComplete_) {
            onShutdownComplete_();
            onShutdownComplete_ = nullptr;
        }
    }

    bool Engine::isRunning() const {
        return isRunning_;
    }

    void Engine::setPause(bool pause) {
        isPaused_ = pause;
    }

    bool Engine::isPaused() const {
        return isPaused_;
    }

    void Engine::setPhysicsUpdateFrameRate(unsigned int frameRate) {
        if (frameRate == 0)
            throw InvalidArgumentException("Physics update frame rate cannot be zero");

        fixedUpdateFPS_ = frameRate;
    }

    unsigned int Engine::getPhysicsUpdateFrameRate() const {
        return fixedUpdateFPS_;
    }

    Time Engine::getElapsedTime() const {
        return elapsedTime_;
    }

    PrefContainer &Engine::getConfigs() {
        return const_cast<PrefContainer&>(std::as_const(*this).getConfigs());
    }

    const PrefContainer &Engine::getConfigs() const {
        if (!isInitialized_)
            throw AccessViolationException("mighter2d::Engine::getConfigs() must not be called before the engine is initialized, see mighter2d::Engine::initialize()");
        else
            return configs_;
    }

    const std::string &Engine::getGameName() const {
        return gameTitle_;
    }

    PropertyContainer &Engine::getCache() {
        return dataSaver_;
    }

    const PropertyContainer &Engine::getCache() const {
        return dataSaver_;
    }

    PrefContainer &Engine::getSavableCache() {
        return diskDataSaver_;
    }

    const PrefContainer &Engine::getSavableCache() const {
        return diskDataSaver_;
    }

    priv::RenderTarget &Engine::getRenderTarget() {
        return *privWindow_;
    }

    void Engine::suspendedEventListener(int id, bool suspend) {
        eventEmitter_.suspendEventListener(id, suspend);
    }

    bool Engine::isEventListenerSuspended(int id) const {
        return eventEmitter_.isEventListenerSuspended(id);
    }

    bool Engine::removeEventListener(int id) {
        return eventEmitter_.removeEventListener(id);
    }

    Window &Engine::getWindow() {
        return const_cast<Window&>(std::as_const(*this).getWindow());
    }

    const Window &Engine::getWindow() const {
        if (!isInitialized_)
            throw AccessViolationException("mighter2d::Engine::getWindow() must not be called before the engine is initialized, see mighter2d::Engine::initialize()");
        else
            return *window_;
    }

    int Engine::onInit(const Callback<>& callback) {
        return eventEmitter_.addEventListener( "initialize", callback);
    }

    int Engine::onSceneActivate(const Callback<Scene*> &callback, bool oneTime) {
        return utility::addEventListener(eventEmitter_, "sceneActivate", callback, oneTime);
    }

    int Engine::onFrameStart(const Callback<>& callback, bool oneTime) {
        return utility::addEventListener(eventEmitter_, "frameStart", callback, oneTime);
    }

    int Engine::onFrameEnd(const Callback<>& callback, bool oneTime) {
        return utility::addEventListener(eventEmitter_, "frameEnd", callback, oneTime);
    }

    int Engine::onStart(const Callback<>& callback) {
        return eventEmitter_.addEventListener("start", callback);
    }

    int Engine::onShutDown(const Callback<>& callback) {
        return eventEmitter_.addEventListener("shutdown", callback);
    }

    void Engine::onShutdownComplete(const Callback<> &callback) {
        onShutdownComplete_ = callback;
    }

    Engine::~Engine() {
        isEngineInstantiated = false;
    }
}
