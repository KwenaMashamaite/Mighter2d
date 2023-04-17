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

#include "Mighter2d/core/scene/SceneManager.h"
#include "Mighter2d/core/engine/Engine.h"
#include "Mighter2d/graphics/RenderTarget.h"
#include "Mighter2d/graphics/shapes/RectangleShape.h"
#include "Mighter2d/utility/Helpers.h"

namespace mighter2d::priv {
    SceneManager::SceneManager(Engine* engine) :
        engine_{engine},
        prevScene_{nullptr}
    {
        MIGHTER2D_ASSERT(engine, "Engine pointer cannot be a nullptr")

        engine->onFrameStart([this] {
            if (!scenes_.empty())
                scenes_.top()->frameBegin();
        });

        engine->onFrameEnd([this] {
            if (!scenes_.empty())
                scenes_.top()->frameEnd();
        });
    }

    void SceneManager::pushScene(Scene::Ptr scene, bool enterScene) {
        MIGHTER2D_ASSERT(scene, "Scene must not be a nullptr")

        if (!scenes_.empty()) {
            prevScene_ = scenes_.top().get();

            if (prevScene_->isEntered())
                prevScene_->pause();
        }

        scenes_.push(std::move(scene));
        Scene* newScene = scenes_.top().get();

        newScene->init(*engine_);

        if (enterScene) {
            if (newScene->isEntered())
                newScene->resume(newScene->isCached());
            else
                newScene->enter();
        }
    }

    Scene::Ptr SceneManager::popCached(const std::string &name) {
        auto found = cachedScenes_.find(name);

        if (found != cachedScenes_.end()) {
            Scene::Ptr scene = std::move(found->second);
            cachedScenes_.erase(found);
            return scene;
        }

        return nullptr;
    }

    Scene *SceneManager::getCached(const std::string &name) {
        return const_cast<Scene*>(std::as_const(*this).getCached(name));
    }

    const Scene *SceneManager::getCached(const std::string &name) const {
        if (isCached(name))
            return cachedScenes_.at(name).get();
        else
            return nullptr;
    }

    void SceneManager::cache(const std::string &name, Scene::Ptr scene) {
        MIGHTER2D_ASSERT(scene, "Cached scene must not be a nullptr")
        auto [iter, inserted] = cachedScenes_.insert(std::pair{name, std::move(scene)});

        if (inserted) {
            iter->second->setCacheOnExit(true, name);

            iter->second->onCache();
        }
    }

    bool SceneManager::isCached(const std::string &name) const {
        return cachedScenes_.find(name) != cachedScenes_.end();
    }

    void SceneManager::popScene(bool resumePrev) {
        if (scenes_.empty())
            return;

        prevScene_ = nullptr;

        // Call onExit() after removing scene from container because onExit may
        // push a scene and the new scene will be removed instead of this one
        Scene::Ptr poppedScene = std::move(scenes_.top());
        scenes_.pop();
        poppedScene->exit();

        // Attempt to cache the removed scene
        if (const auto& [isCached, cacheAlias] = poppedScene->cacheState_; isCached) {
            cache(cacheAlias, std::move(poppedScene));
        }

        // Activate a new scene
        if (!scenes_.empty()) {
            updatePreviousScene();

            Scene* newTopScene = scenes_.top().get();

            if (resumePrev) {
                if (newTopScene->isEntered())
                    newTopScene->resume();
                else
                    newTopScene->enter();
            }
        }
    }

    Scene* SceneManager::getActiveScene() {
        return const_cast<Scene*>(std::as_const(*this).getActiveScene());
    }

    const Scene *SceneManager::getActiveScene() const {
        if (scenes_.empty() || !scenes_.top()->isEntered())
            return nullptr;
        else
            return scenes_.top().get();
    }

    Scene *SceneManager::getPreviousScene() {
        return const_cast<Scene*>(std::as_const(*this).getPreviousScene());
    }

    const Scene *SceneManager::getPreviousScene() const {
        if (prevScene_ && prevScene_->isEntered())
            return prevScene_;

        return nullptr;
    }

    std::size_t SceneManager::getSceneCount() const {
        return scenes_.size();
    }

    void SceneManager::clear() {
        prevScene_ = nullptr;
        while (!scenes_.empty())
            scenes_.pop();
    }

    void SceneManager::clearCachedScenes() {
        cachedScenes_.clear();
    }

    void SceneManager::clearAllExceptActive() {
        if (!scenes_.empty()) {
            if (scenes_.top()->isEntered()) {
                Scene::Ptr activeScene = std::move(scenes_.top());
                clear();
                scenes_.push(std::move(activeScene));
            } else
                clear();
        }
    }

    void SceneManager::enterTopScene() const {
        if (!scenes_.empty())
            scenes_.top()->enter();
    }

    bool SceneManager::isEmpty() const {
        return scenes_.empty();
    }

    void SceneManager::render(priv::RenderTarget &window) {
        auto static renderScene = [](Scene* scene, Camera* camera, priv::RenderTarget& renderWindow) {
            scene->onPreRender();

            if (!camera->isDrawable())
                return;

            // Reset view so that the scene can be rendered on the current camera
            const sf::View& view = std::any_cast<std::reference_wrapper<const sf::View>>(camera->getInternalView()).get();
            renderWindow.getThirdPartyWindow().setView(view);

            scene->renderLayers_.render(renderWindow);

            scene->onPostRender();
        };

        // Render the scene on each camera to update its view
        auto static renderEachCam = [](Scene* scene, priv::RenderTarget& renderTarget) {
            // Render main/default camera
            renderScene(scene, &scene->getCamera(), renderTarget);
        };

        if (!scenes_.empty() && scenes_.top()->isEntered()) {
            // Render previous scene
            if (prevScene_ && prevScene_->isEntered() && prevScene_->isVisibleOnPause()) {
                Scene* bgScene = prevScene_->getBackgroundScene();

                if (bgScene && prevScene_->isBackgroundSceneDrawable())
                    renderEachCam(bgScene, window);

                renderEachCam(prevScene_, window);
            }

            // Render the active scenes background scene
            Scene* activeScene = scenes_.top().get();
            Scene* bgScene = activeScene->getBackgroundScene();

            if(bgScene && activeScene->isBackgroundSceneDrawable())
                renderEachCam(bgScene, window);

            // Render the active scene
            renderEachCam(activeScene, window);
        }
    }

    void SceneManager::handleEvent(SystemEvent event) {
        if (scenes_.empty())
            return;

        if (!scenes_.top()->isEntered())
            return;

        // Update all system components of a scene
        static auto updateSystem = [](Scene* scene, SystemEvent e) {
            // Absorb key event if Keyboard is disabled
            if (!scene->inputManager_.isInputEnabled(input::InputType::Keyboard) &&
                (e.type == SystemEvent::KeyPressed || e.type == SystemEvent::KeyReleased))
            {
                return;
            }

            // Absorb mouse event if the Mouse is disabled
            if (!scene->inputManager_.isInputEnabled(input::InputType::Mouse) &&
                (e.type == SystemEvent::MouseButtonPressed || e.type == SystemEvent::MouseButtonReleased
                 || e.type == SystemEvent::MouseMoved || e.type == SystemEvent::MouseWheelScrolled))
            {
                return;
            }

            // Absorb joystick event if Joystick is disabled
            if (!scene->inputManager_.isInputEnabled(input::InputType::Joystick) &&
                (e.type == SystemEvent::JoystickButtonPressed || e.type == SystemEvent::JoystickButtonReleased ||
                 e.type == SystemEvent::JoystickMoved))
            {
                return;
            }

            for (auto& sysEventHandler : scene->systemEventHandlerList_) {
                sysEventHandler->handleEvent(e);
            }

            scene->onHandleEvent(e);
        };

        Scene* activeScene = scenes_.top().get();
        Scene* bgScene = activeScene->getBackgroundScene();

        if (bgScene && activeScene->isBackgroundSceneEventsEnabled())
            updateSystem(bgScene, event);

        updateSystem(activeScene, event);
    }

    void SceneManager::update(Time deltaTime) {
        update(deltaTime, false);
    }

    void SceneManager::fixedUpdate(Time deltaTime) {
        update(deltaTime, true);
    }

    void SceneManager::update(const Time &deltaTime, bool fixedUpdate) {
        if (!scenes_.empty())
            scenes_.top()->update(deltaTime,  fixedUpdate);
    }

    void SceneManager::updatePreviousScene() {
        if (scenes_.size() >= 2) {
            Scene::Ptr currentScene = std::move(scenes_.top());
            scenes_.pop();
            prevScene_ = scenes_.top().get();
            scenes_.push(std::move(currentScene));
        }
    }

    SceneManager::~SceneManager() {
        prevScene_ = nullptr;
    }
}
