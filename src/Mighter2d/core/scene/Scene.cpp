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

#include "Mighter2d/core/scene/Scene.h"
#include "Mighter2d/core/engine/Engine.h"
#include "Mighter2d/core/exceptions/Exceptions.h"
#include "Mighter2d/utility/Helpers.h"
#include "Mighter2d/graphics/RenderTarget.h"
#include "Mighter2d/core/scene/BackgroundScene.h"
#include <utility>

namespace mighter2d {
    Scene::Scene() :
        engine_(nullptr),
        sceneStateObserver_(*this),
        timescale_{1.0f},
        isEntered_{false},
        isInitialized_{false},
        isActive_{false},
        isPaused_{false},
        isVisibleWhenPaused_{false},
        cacheState_{false, ""}
    {
        renderLayers_.create("default");
    }

    Scene::Scene(Scene&& other) noexcept :
        sceneStateObserver_(std::move(other.sceneStateObserver_))
    {
        *this = std::move(other);
    }

    Scene &Scene::operator=(Scene&& other) noexcept {
        // We can't use a default move assignment operator because of reference members
        if (this != &other) {
            Object::operator=(std::move(other));
            engine_ = other.engine_;
            camera_ = std::move(other.camera_);
            renderLayers_ = std::move(other.renderLayers_);
            timescale_ = other.timescale_;
            isVisibleWhenPaused_ = other.isVisibleWhenPaused_;
            cacheState_ = other.cacheState_;
            isEntered_ = other.isEntered_;
            isInitialized_ = other.isInitialized_;
            isActive_ = other.isActive_;
            isPaused_ = other.isPaused_;
            backgroundScene_ = std::move(other.backgroundScene_);
            sceneStateObserver_ = std::move(sceneStateObserver_);
        }

        return *this;
    }

    Scene::Ptr Scene::create() {
        return std::make_unique<Scene>();
    }

    void Scene::init(Engine &engine) {
        if (!isInitialized_) {
            isInitialized_ = true;
            engine_ = &engine;
            camera_ = std::make_unique<Camera>(*this, engine.getRenderTarget());
            emit("mighter2d_Scene_ready");
        }
    }

    void Scene::addUpdatable(IUpdatable *updatable) {
        if (!utility::findIn(updateList_, updatable).first) {
            updateList_.push_back(updatable);
        }
    }

    bool Scene::removeUpdatable(IUpdatable *updatable) {
        return utility::eraseIn(updateList_, updatable);
    }

    void Scene::addSystemEventHandler(ISystemEventHandler *sysEventHandler) {
        if (!utility::findIn(
                systemEventHandlerList_, sysEventHandler).first) {
            systemEventHandlerList_.push_back(sysEventHandler);
        }
    }

    bool Scene::removeSystemEventHandler(ISystemEventHandler *sysEventHandler) {
        return utility::eraseIn(systemEventHandlerList_, sysEventHandler);
    }

    std::string Scene::getClassName() const {
        return "Scene";
    }

    void Scene::setCacheOnExit(bool cache, const std::string& alias) {
        cacheState_.first = cache;
        cacheState_.second = alias;
    }

    bool Scene::isCached() const {
        return cacheState_.first;
    }

    void Scene::setVisibleOnPause(bool visible) {
        if (isVisibleWhenPaused_ != visible) {
            isVisibleWhenPaused_ = visible;

            emitChange(Property("visibleOnPause", isVisibleWhenPaused_));
        }
    }

    bool Scene::isVisibleOnPause() const {
        return isVisibleWhenPaused_;
    }

    void Scene::setBackgroundScene(std::unique_ptr<BackgroundScene> scene) {
        if (!isInitialized_)
            throw AccessViolationException("mighter2d::Scene::setBackgroundScene() must not be called before the parent scene is initialized");

        if (backgroundScene_ && backgroundScene_->isEntered_)
            backgroundScene_->exit();

        backgroundScene_ = std::move(scene);

        if (backgroundScene_) {
            backgroundScene_->init(*engine_);

            if (isEntered_)
                backgroundScene_->enter();
        }
    }

    BackgroundScene *Scene::getBackgroundScene() {
        return backgroundScene_.get();
    }

    const BackgroundScene *Scene::getBackgroundScene() const {
        return backgroundScene_.get();
    }

    bool Scene::hasBackgroundScene() const {
        return backgroundScene_ != nullptr;
    }

    bool Scene::isEntered() const {
        return isEntered_;
    }

    bool Scene::isActive() const {
        return isActive_;
    }

    bool Scene::isPaused() const {
        return isPaused_;
    }

    void Scene::setTimescale(float timescale) {
        if (timescale_ == timescale)
            return;

        if (timescale < 0)
            timescale_ = 0.0f;
        else
            timescale_ = timescale;

        emitChange(Property{"timescale", timescale});
    }

    float Scene::getTimescale() const {
        return timescale_;
    }

    Engine &Scene::getEngine() {
        return const_cast<Engine&>(std::as_const(*this).getEngine());
    }

    const Engine &Scene::getEngine() const {
        if (!engine_)
            throw AccessViolationException("mighter2d::Scene::getEngine() must not be called before the scene is initialized");
        else
            return *engine_;
    }

    Window &Scene::getWindow() {
        return const_cast<Window&>(std::as_const(*this).getWindow());
    }

    const Window &Scene::getWindow() const {
        if (!isInitialized_)
            throw AccessViolationException("mighter2d::Scene::getWindow() must not be called before the scene is initialized");
        else
            return engine_->getWindow();
    }

    SceneStateObserver &Scene::getStateObserver() {
        return const_cast<SceneStateObserver&>(std::as_const(*this).getStateObserver());
    }

    const SceneStateObserver &Scene::getStateObserver() const {
        return sceneStateObserver_;
    }

    Camera &Scene::getCamera() {
        return const_cast<Camera&>(std::as_const(*this).getCamera());
    }

    const Camera &Scene::getCamera() const {
        if (!camera_)
            throw AccessViolationException("mighter2d::Scene::getCamera() must not be called before the scene is initialized");
        else
            return *camera_;
    }

    PropertyContainer &Scene::getCache() {
        return const_cast<PropertyContainer&>(std::as_const(*this).getCache());
    }

    const PropertyContainer &Scene::getCache() const {
        if (!isInitialized_)
            throw AccessViolationException("mighter2d::Scene::getCache() must not be called before the scene is initialized");
        else
            return engine_->getCache();;
    }

    PrefContainer &Scene::getSCache() {
        return const_cast<PrefContainer&>(std::as_const(*this).getSCache());
    }

    const PrefContainer &Scene::getSCache() const {
        if (!isInitialized_)
            throw AccessViolationException("mighter2d::Scene::getSCache() must not be called before the scene is initialized");
        else
            return engine_->getSavableCache();
    }

    RenderLayerContainer &Scene::getRenderLayers() {
        return renderLayers_;
    }

    const RenderLayerContainer &Scene::getRenderLayers() const {
        return renderLayers_;
    }

    void Scene::enter() {
        if (!isEntered_ && isInitialized_) {
            if (backgroundScene_)
                backgroundScene_->enter();

            isEntered_ = isActive_ = true;

            emit("mighter2d_Scene_enter");
            onEnter();
        }
    }

    void Scene::pause() {
        if (!isPaused_ && isEntered_) {
            if (backgroundScene_)
                backgroundScene_->pause();

            isActive_ = false;
            isPaused_ = true;

            emit("mighter2d_Scene_pause");
            onPause();
        }
    }

    void Scene::resume(bool fromCache) {
        if (isEntered_) {
            if (backgroundScene_)
                backgroundScene_->resume(fromCache);

            isPaused_ = false;
            isActive_ = true;

            emit("mighter2d_Scene_resume");

            onResume(fromCache);
        }
    }

    void Scene::exit() {
        if (isEntered_) {
            if (backgroundScene_)
                backgroundScene_->exit();

            isActive_ = false;

            emit("mighter2d_Scene_exit");
            onExit();
        }
    }

    void Scene::destroy() {
        if (backgroundScene_)
            backgroundScene_->destroy();

        isActive_ = false;

        emit("mighter2d_Scene_destroy");
        onDestroy();
    }

    void Scene::update(const Time &deltaTime, bool isFixedUpdate) {
        if (isActive_) {
            if (backgroundScene_ && backgroundScene_->isUpdateEnabled())
                backgroundScene_->update(deltaTime, isFixedUpdate);

            Time scaledDeltaTime = deltaTime * getTimescale();

            if (isFixedUpdate) {
                for (auto &updatable: updateList_)
                    updatable->fixedUpdate(scaledDeltaTime);

                onFixedUpdate(scaledDeltaTime);
            } else {
                for (auto &updatable: updateList_)
                    updatable->update(scaledDeltaTime);

                onUpdate(scaledDeltaTime);
            }
        }
    }

    void Scene::handleEvent(SystemEvent event) {
        if (isActive_) {
            if (backgroundScene_ && backgroundScene_->isSystemEventHandleEnabled())
                backgroundScene_->handleEvent(event);

            for (auto& sysEventHandler : systemEventHandlerList_) {
                sysEventHandler->handleEvent(event);
            }

            onHandleEvent(event);
        }
    }

    void Scene::render() {
        if (isActive_ && camera_->isDrawable()) {
            if (backgroundScene_ && backgroundScene_->isDrawable())
                backgroundScene_->render();

            onPreRender();

            priv::RenderTarget& renderTarget = getEngine().getRenderTarget();

            // Reset view so that the scene can be rendered on the current camera
            const sf::View& view = std::any_cast<std::reference_wrapper<const sf::View>>(camera_->getInternalView()).get();
            renderTarget.getThirdPartyWindow().setView(view);

            renderLayers_.render(renderTarget);

            onPostRender();
        }
    }

    void Scene::frameBegin() {
        if (isActive_) {
            if (backgroundScene_)
                backgroundScene_->frameBegin();

            emit("mighter2d_Scene_frameBegin");
            onFrameBegin();
        }
    }

    void Scene::frameEnd() {
        if (isActive_) {
            if (backgroundScene_)
                backgroundScene_->frameEnd();

            emit("mighter2d_Scene_frameEnd");
            onFrameEnd();
        }
    }

    Scene::~Scene() {
        emitDestruction();
    }
}