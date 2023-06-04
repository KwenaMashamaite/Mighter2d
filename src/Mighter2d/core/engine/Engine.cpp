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
#include "Mighter2d/core/scene/EngineScene.h"

namespace mighter2d {
    namespace {
        bool isEngineInstantiated = false; // Only one engine instance can exist at a time
    }

    Engine::Engine() :
        privWindow_{std::make_unique<priv::RenderTarget>()},
        window_{new Window(*privWindow_)},
        configs_{nullptr},
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

    void Engine::initialize(const EngineSettings& settings) {
        if (!isInitialized_) {
            configs_ = &settings;

            initResourceManager();
            initRenderTarget();

            eventDispatcher_ = GlobalEventEmitter::instance();
            isInitialized_ = true;

            eventEmitter_.emit("initialize");
        }
    }

    bool Engine::isInitialized() const {
        return isInitialized_;
    }

    void Engine::initRenderTarget() {
        auto title = configs_->getWindowTitle();
        auto width = configs_->getWindowWidth();
        auto height = configs_->getWindowHeight();

        MIGHTER2D_ASSERT(width > 0, "The width of the window cannot be negative")
        MIGHTER2D_ASSERT(height > 0, "The height of the window cannot be negative")

        // Create the window
        privWindow_->create(title, width, height, window_->getStyle());
        window_->setFullScreen(configs_->getWindowFullscreenEnabled());
        window_->setFrameRateLimit(configs_->getWindowFPSLimit());
        window_->setVerticalSyncEnable(configs_->getWindowVerticalSyncEnabled());

        // Set the window icon
        if (auto icon = configs_->getWindowIcon(); icon != "OS")
            privWindow_->setIcon(icon);

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
        resourceManager_->setPathFor(ResourceType::Font, configs_->getFontsDirectory());
        resourceManager_->setPathFor(ResourceType::Texture, configs_->getTexturesDirectory());
        resourceManager_->setPathFor(ResourceType::Image, configs_->getTexturesDirectory());
        resourceManager_->setPathFor(ResourceType::SoundEffect, configs_->getSoundEffectsDirectory());
        resourceManager_->setPathFor(ResourceType::Music, configs_->getMusicDirectory());
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
        sceneManager_->postUpdate();
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
        configs_ = nullptr;
        elapsedTime_ = Time::Zero;
        gameTitle_.clear();
        settingFile_.clear();
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

    const EngineSettings &Engine::getSettings() const {
        if (!isInitialized_)
            throw AccessViolationException("mighter2d::Engine::getSettings() must not be called before the engine is initialized, see mighter2d::Engine::initialize()");
        else
            return *configs_;
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

    ui::GuiContainer &Engine::getGui() {
        return const_cast<ui::GuiContainer&>(std::as_const(*this).getGui());;
    }

    const ui::GuiContainer &Engine::getGui() const {
        if (!isInitialized_)
            throw AccessViolationException("mighter2d::Engine::getGui() must not be called before the engine is initialized, see mighter2d::Engine::initialize()");
        else
            return sceneManager_->getEngineScene()->guiContainer_;
    }

    input::InputManager &Engine::getInputManager() {
        return const_cast<input::InputManager&>(std::as_const(*this).getInputManager());;
    }

    const input::InputManager &Engine::getInputManager() const {
        if (!isInitialized_)
            throw AccessViolationException("mighter2d::Engine::getInputManager() must not be called before the engine is initialized, see mighter2d::Engine::initialize()");
        else
            return sceneManager_->getEngineScene()->inputManager_;
    }

    TimerManager &Engine::getTimerManager() {
        return const_cast<TimerManager&>(std::as_const(*this).getTimerManager());;
    }

    const TimerManager &Engine::getTimerManager() const {
        if (!isInitialized_)
            throw AccessViolationException("mighter2d::Engine::getTimerManager() must not be called before the engine is initialized, see mighter2d::Engine::initialize()");
        else
            return sceneManager_->getEngineScene()->timerManager_;
    }

    audio::AudioManager &Engine::getAudioManager() {
        return const_cast<audio::AudioManager&>(std::as_const(*this).getAudioManager());
    }

    const audio::AudioManager &Engine::getAudioManager() const {
        if (!isInitialized_)
            throw AccessViolationException("mighter2d::Engine::getAudioManager() must not be called before the engine is initialized, see mighter2d::Engine::initialize()");
        else
            return sceneManager_->getEngineScene()->audioManager_;
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
