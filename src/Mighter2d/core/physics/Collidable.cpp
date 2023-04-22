// ///////////////////////////////////////////////////////////////////////////
//  Mighter2d
//
//  Copyright (c) 2023 Kwena Mashamaite
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
// ///////////////////////////////////////////////////////////////////////////

#include "Mighter2d/core/physics/Collidable.h"
#include "Mighter2d/core/physics/CollisionDetector.h"
#include "Mighter2d/core/scene/Scene.h"

namespace mighter2d {
    namespace {
        bool canCollide(Collidable& colA, Collidable& colB) {
            // Prevent Self collision
            if (&colA == &colB)
                return false;

            // Collidables in excluded collision group do not collide (Collision filtering by group)
            if ((colA.getCollisionExcludeList().contains(colB.getCollisionGroup())) ||
                (colB.getCollisionExcludeList().contains(colA.getCollisionGroup())))
            {
                return false;
            }

            // Collidables with different collision id's do not collide (collision filtering by id)
            if (colA.getCollisionId() != colB.getCollisionId())
                return false;

            // Satisfies collision requirement
            return true;
        }
    }

    Collidable::Collidable(Scene &scene) :
        scene_(&scene),
        sceneDestrucListenerId_(-1),
        collisionId_{0}
    {
        scene_->addCollidable(this);

        // If the scene destructs before the collidable
        sceneDestrucListenerId_ = scene_->onDestruction([this] {
            scene_ = nullptr;
        });
    }

    void Collidable::setCollisionGroup(const std::string &colGroup) {
        collisionGroup_ = colGroup;
    }

    const std::string &Collidable::getCollisionGroup() const {
        return collisionGroup_;
    }

    void Collidable::setCollisionId(int id) {
        collisionId_ = id;
    }

    int Collidable::getCollisionId() const {
        return collisionId_;
    }

    CollisionExcludeList &Collidable::getCollisionExcludeList() {
        return excludeList_;
    }

    const CollisionExcludeList &Collidable::getCollisionExcludeList() const {
        return excludeList_;
    }

    void Collidable::handleCollidable(Collidable& other) {
        if (canCollide(*this, other)) {
            bool wasColliding = hasCollidable(&other);

            auto bb1 = getBoundingBox();
            auto bb2 = other.getBoundingBox();
            bool isColliding = priv::CollisionDetector::isColliding(bb1, bb2);
            float IoU = priv::CollisionDetector::getIoU(bb1, bb2);

            if (wasColliding) {
                if (isColliding) {
                    this->onOverlapStay(other, IoU);
                    other.onOverlapStay(*this, IoU);
                } else {
                    this->removeCollidable(&other);
                    other.removeCollidable(this);

                    // Invoke callbacks
                    this->onOverlapEnd(other);
                    other.onOverlapEnd(*this);
                }
            } else if (isColliding) {
                this->addCollidable(&other);
                other.addCollidable(this);

                //Invoke callbacks
                this->onOverlapStart(other, IoU);
                other.onOverlapStart(*this, IoU);
            }
        }
    }

    void Collidable::addCollidable(Collidable* collidable) {
        collidables_.push_back(collidable);
    }

    bool Collidable::hasCollidable(Collidable* collidable) {
        return std::find(collidables_.begin(), collidables_.end(), collidable) != collidables_.end();
    }

    void Collidable::removeCollidable(Collidable* collidable) {
        auto found = std::find(collidables_.begin(), collidables_.end(), collidable);

        if (found != collidables_.end())
            collidables_.erase(found);
    }

    Collidable::~Collidable() {
        if (scene_ != nullptr) {
            scene_->removeDestructionListener(sceneDestrucListenerId_);
            scene_->removeCollidable(this);
        }
    }
}
