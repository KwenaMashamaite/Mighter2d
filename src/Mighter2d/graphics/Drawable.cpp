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

#include "Mighter2d/graphics/Drawable.h"
#include "Mighter2d/core/scene/Scene.h"
#include "Mighter2d/core/scene/RenderLayer.h"

namespace mighter2d {
    namespace {
        priv::RenderLayer* getLayer(Scene& scene, const std::string& name) {
            RenderLayerContainer& renderLayers = scene.getRenderLayers();
            priv::RenderLayer::Ptr renderLayer = renderLayers.findByName(name);

            if (renderLayer)
                return renderLayer.get();
            else
                return renderLayers.create(name).get();
        }
    }

    Drawable::Drawable(Scene &scene) :
        scene_(&scene),
        isVisible_(true)
    {
        setRenderLayer("default", 0);
    }

    void Drawable::setRenderLayer(const std::string &renderLayer, int renderOrder) {
        if (renderLayer_ != renderLayer) {
            renderLayer_ = renderLayer;
            renderOrder_ = renderOrder;

            getLayer(*scene_, renderLayer)->add(*this, renderOrder);
            emitChange(Property{"renderLayer", renderLayer});
        }
    }

    std::string Drawable::getRenderLayer() const {
        return renderLayer_;
    }

    unsigned int Drawable::getRenderOrder() const {
        return renderOrder_;
    }

    void Drawable::setVisible(bool visible) {
        if (isVisible_ != visible) {
            isVisible_ = visible;

            emitChange(Property{"visible", visible});
        }
    }

    bool Drawable::isVisible() const {
        return isVisible_;
    }

    void Drawable::toggleVisibility() {
        setVisible(!isVisible());
    }

    std::string Drawable::getClassType() const {
        return "Drawable";
    }

    Drawable::~Drawable() {
        emitDestruction();
    }
}

