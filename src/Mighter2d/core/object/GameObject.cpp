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

#include "Mighter2d/core/object/GameObject.h"
#include "Mighter2d/core/scene/Scene.h"
#include "Mighter2d/utility/Helpers.h"

namespace mighter2d {
    GameObject::GameObject(Scene& scene) :
        scene_{scene},
        state_{-1},
        isActive_{true},
        sprite_(std::make_unique<Sprite>(scene))
    {
        initEvents();
    }

    GameObject::GameObject(const GameObject &other) :
        Object(other),
        scene_{other.scene_},
        state_{other.state_},
        isActive_{other.isActive_},
        transform_{other.transform_},
        sprite_{std::make_unique<Sprite>(*other.sprite_)}
    {
        initEvents();
    }

    GameObject &GameObject::operator=(const GameObject &other) {
        if (this != &other) {
            auto temp{other};
            Object::operator=(temp);
            swap(temp);
            initEvents();
        }

        return *this;
    }

    GameObject::GameObject(GameObject&& other) noexcept :
        scene_(other.scene_)
    {
        *this = std::move(other);
        initEvents();
    }

    GameObject &GameObject::operator=(GameObject&& rhs) noexcept {
        if (this != &rhs) {
            Object::operator=(std::move(rhs));
            swap(rhs);
        }

        return *this;
    }

    void GameObject::swap(GameObject &other) {
        std::swap(scene_, other.scene_);
        std::swap(state_, other.state_);
        std::swap(isActive_, other.isActive_);
        std::swap(transform_, other.transform_);
        std::swap(sprite_, other.sprite_);
        std::swap(userData_, other.userData_);
    }

    GameObject::Ptr GameObject::create(Scene &scene) {
        return std::make_unique<GameObject>(scene);
    }

    GameObject::Ptr GameObject::copy() const {
        return std::make_unique<GameObject>(*this);
    }

    void GameObject::setState(int state) {
        if (state_ == state)
            return;

        state_ = state;
        emitChange(Property{"state", state_});
    }

    int GameObject::getState() const {
        return state_;
    }

    void GameObject::setActive(bool isActive) {
        if (isActive_ == isActive)
            return;

        isActive_ = isActive;

        emitChange(Property{"active", isActive_});
    }

    bool GameObject::isActive() const {
        return isActive_;
    }

    PropertyContainer& GameObject::getUserData() {
        return userData_;
    }

    const PropertyContainer &GameObject::getUserData() const {
        return userData_;
    }

    std::string GameObject::getClassName() const {
        return "GameObject";
    }

    std::string GameObject::getBaseClassName() const {
        return "GameObject";
    }

    Transform &GameObject::getTransform() {
        return transform_;
    }

    const Transform &GameObject::getTransform() const {
        return transform_;
    }

    Scene &GameObject::getScene() {
        return scene_;
    }

    const Scene &GameObject::getScene() const {
        return scene_;
    }

    void GameObject::resetSpriteOrigin() {
        transform_.setOrigin(sprite_->getLocalBounds().width / 2.0f, sprite_->getLocalBounds().height / 2.0f);
    }

    Sprite &GameObject::getSprite() {
        return *sprite_;
    }

    const Sprite &GameObject::getSprite() const {
        return *sprite_;
    }

    void GameObject::initEvents() {
        // Always keep the game object origin at the centre of sprite
        sprite_->onPropertyChange([this](const Property& property) {
            if (property.getName() == "scale" || property.getName() == "texture" || property.getName() == "textureRect")
                resetSpriteOrigin();
        });

        // Keep the sprite in sync with the objects transfor changes
        transform_.onPropertyChange([this](const Property& property) {
            const auto& name = property.getName();
            if (name == "position") {
                sprite_->setPosition(transform_.getPosition());
                emitChange(Property{name, transform_.getPosition()});
            } else if (name == "origin") {
                sprite_->setOrigin(transform_.getOrigin());
                emitChange(Property{name, transform_.getOrigin()});
            } else if (name == "scale") {
                sprite_->setScale(transform_.getScale());
                emitChange(Property{name, transform_.getScale()});
            } else if (name == "rotation") {
                sprite_->setRotation(transform_.getRotation());
                emitChange(Property{name, transform_.getRotation()});
            }
        });
    }

    GameObject::~GameObject() {
        emitDestruction();
    }
}
