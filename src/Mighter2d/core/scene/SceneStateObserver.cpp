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

#include "Mighter2d/core/scene/SceneStateObserver.h"
#include "Mighter2d/core/scene/Scene.h"
#include "Mighter2d/utility/Helpers.h"

namespace mighter2d {
    SceneStateObserver::SceneStateObserver(Scene &scene) :
        scene_(&scene)
    {}

    int SceneStateObserver::onReady(const Callback<> &callback) {
        return scene_->addEventListener("mighter2d_Scene_ready", callback);
    }

    int SceneStateObserver::onEnter(const Callback<> &callback) {
        return scene_->addEventListener("mighter2d_Scene_enter", callback);
    }

    int SceneStateObserver::onFrameBegin(const Callback<> &callback, bool oneTime) {
        return utility::addEventListener(*scene_, "mighter2d_Scene_frameBegin", callback, oneTime);
    }

    int SceneStateObserver::onFrameEnd(const Callback<> &callback, bool oneTime) {
        return utility::addEventListener(*scene_, "mighter2d_Scene_frameEnd", callback, oneTime);
    }

    int SceneStateObserver::onPause(const Callback<> &callback, bool oneTime) {
        return utility::addEventListener(*scene_, "mighter2d_Scene_pause", callback, oneTime);
    }

    int SceneStateObserver::onResume(const Callback<> &callback, bool oneTime) {
        return utility::addEventListener(*scene_, "mighter2d_Scene_resume", callback, oneTime);
    }

    int SceneStateObserver::onExit(const Callback<> &callback, bool oneTime) {
        return utility::addEventListener(*scene_, "mighter2d_Scene_exit", callback, oneTime);
    }

    int SceneStateObserver::onDestroy(const Callback<> &callback) {
        return scene_->addEventListener("mighter2d_Scene_destroy", callback);
    }

    bool SceneStateObserver::removeEventListener(int id) {
        return scene_->removeEventListener(id);
    }
}