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

#ifndef MIGHTER2D_COLLIDABLE_H
#define MIGHTER2D_COLLIDABLE_H

#include "Mighter2d/Config.h"
#include "Mighter2d/common/Vector2.h"
#include "Mighter2d/core/physics/BoundingBox.h"
#include "Mighter2d/core/object/CollisionExcludeList.h"
#include <vector>

namespace mighter2d {
    class Scene;

    /**
     * @brief Base class for collidable entities
     *
     * This class is made a template so that collision callbacks can be called with the class
     * type to avoid dynamic casts. Derived classes of this class must use the Curiously Recurring
     * Template Pattern (CRTP). For example, suppose we have a player that is collidable, then:
     *
     * @code
     * // The Player class would inherit the Collidable class as follows:
     * class Player : public mighter2d::Collidable<Player> {
     * public:
     *     OnCollisionStart(Player& other) {// collision start behavior}
     *     onCollisionStay(Player& other) {// collision stay behavior}
     *     onCollisionEnd(Player& other) {// collision end behavior}
     * }
     * @endcode
     */
    class MIGHTER2D_API Collidable {
    public:
        /**
         * @brief Constructor
         * @param scene The scene the collidable belongs to
         */
        explicit Collidable(Scene& scene);

        /**
         * @brief Set whether or not this collidable can overlap with other collidables
         * @param enable True to enable overlap detection, otherwise false
         *
         * Overlap detection allows this collidable to register a collision between itself
         * and any collidable it overlaps with. When overlap detection is enabled, collision
         * resolution callbacks are invoked (onOverlapBegin(), onOverlapStay(), onOverlapEnd() )
         * on both collidables when an overlap is detected (or no longer detected). When it is
         * disabled, the collidables just overlap without invoking their collision resolution
         * callbacks (that is, collision detection is disabled).
         *
         * Note that overlap detection takes higher precedence than all the collision
         * filters (collision group, collision id etc...).
         *
         * By default, overlap detection is enabled
         *
         * @see setCollisionGroup, setCollisionId, setStatic
         */
        void setOverlapDetectionEnable(bool enable);

        /**
         * @brief Check if overlap detection is enabled or not
         * @return True if enabled, otherwise false
         *
         * @see setOverlapDetectionEnable
         */
        bool isOverlapDetectionEnabled() const;

        /**
         * @brief Set the objects collision group
         * @param colGroup The collision group to be set
         *
         * A collision group allows certain collidables to always collide
         * or never collide with each other. When a collision group is added
         * to the collidables collision exclusion list (see getCollisionExcludeList()),
         * the collidable will never collide with collidables in that group
         * (they will pass through each other without generating a collision
         * event), whereas when not added, the collidable will always collide
         * with collidables whose collision group does not appear in its colliion
         * exclusion list.
         *
         * For example, the following code makes objects in the "Enemies"
         * collision group to never collide with each other:
         *
         * @code
         * // Assume 'enemies' is a collection of 'mighter2d::Collidable' objects
         * for (auto& enemy : enemies) {
         *      enemy.setCollisionGroup("Enemies");
         *      enemy.getCollisionExcludeList().add("Enemies");
         * }
         * @endcode
         *
         * By default, the collision group is am empty string. This means that
         * the collidable does not belong to any collision group. Therefore, it
         * will collide with any other collidables whose collision id is the same
         * as theirs.
         *
         * @see getCollisionGroup, setCollisionId and getCollisionExcludeList
         */
        void setCollisionGroup(const std::string& colGroup);

        /**
         * @brief Get the collidables collision group
         * @return The collidables collision group
         *
         * @see setCollisionGroup
         */
        const std::string& getCollisionGroup() const;

        /**
         * @brief Set the collidables collision id
         * @param id The collision id to be set
         *
         * A collision id allows collidables to selectively collide with
         * each other within colliding groups (see setCollisionGroup()).
         * Collidables with the same collision id will collide with each
         * other, whilst those with different collision id's do not
         * collide with each other.
         *
         * Note that "collision group" filtering takes higher precedence
         * than "collision id" filtering. This means that regardless of
         * how collision id's are configured, a collision will never take
         * place if the collision group of one collidable is added to the
         * other collidables collision group exclude list (see getCollisionExcludeList())
         *
         * By default, the collision id is 0. This means that this collidable
         * will collide with any collidable object that is not in its collision
         * group exclude list.
         *
         * @see setCollisionGroup and getCollisionExcludeList
         */
        void setCollisionId(int id);

        /**
         * @brief Get collidables collision id
         * @return The collidables collision id
         *
         * @see setCollisionId
         */
        int getCollisionId() const;

        /**
         * @brief Set whether or not the collidable is static
         * @param isStatic True to set static, otherwise false
         *
         * Static collidables do not collide with other static collidables
         * regardless of the collision group or collision id. For example,
         * two walls may overlap but should not generate a collision.
         *
         * Note that setting a collidable that does not move throughout the
         * game as static can significantly improve performance
         *
         * By default, the collidable is not static
         *
         * @see setCollisionGroup, setCollisionId, isStatic
         */
        void setStatic(bool isStatic);

        /**
         * @brief Check if the collidable is static or not
         * @return True if static, otherwise false
         *
         * @see setStatic
         */
        bool isStatic() const;

        /**
         * @brief Get the collidables collision exclude list
         * @return The collidables collision exclude list
         *
         * This list allows you to specify which collidables this collidable
         * can collide with. This collidable will not collide with any
         * collidable whose collision group exists in its collision
         * exclude list. They will simply overlap without generation
         * a collision event.
         *
         * @code Example
         * // Assume 'enemies' is a collection of 'mighter2d::Collidable' objects
         * for (auto& enemy : enemies) {
         *      enemy.setCollisionGroup("Enemies");
         *      enemy.getCollisionExcludeList().add("Enemies");
         * }
         * @endcode
         *
         * By default, the collision exclude list is empty, this means that
         * the collidable can collides with all collidables
         *
         * @see setCollisionId, setCollisionGroup
         */
        CollisionExcludeList& getCollisionExcludeList();
        const CollisionExcludeList& getCollisionExcludeList() const;

        /**
         * @brief Handle an overlap between this object and another
         * @param other The collidable that's starting to overlap with
         *              this collidable
         * @param IoU The percentage of overlap between the two collidables in the range [0, 1]
         *
         * The IoU (Intersection over Union) specifies how much the bounding boxes of the two
         * collidables overlap. For example, 0 = very small overlap, 0.5 = 50% overlap and 1 = 100%
         * overlap.
         *
         * Note that IoU only works when the bounding boxes of the two collidables are the same size.
         *
         * @note This function is called automatically once per frame when this collidable
         * and another collidable start to overlap.
         *
         * @see onOverlapEnd
         */
        virtual void onOverlapStart(Collidable& other, float IoU) { MIGHTER2D_UNUSED(other); MIGHTER2D_UNUSED(IoU);}

        /**
         * @brief Handle an overlap stay
         * @param other The collidable that is remaining in contact with this collidable
         *
         * The IoU (Intersection over Union) specifies how much the bounding boxes of the two
         * collidables overlap. For example, 0 = very small overlap, 0.5 = 50% overlap and 1 = 100%
         * overlap.
         *
         * Note that IoU only works when the bounding boxes of the two collidables are the same size.
         *
         * @note This function is called automatically once per frame when this collidable and
         * another collidable remain in contact after the initial overlap.
         *
         * @see onOverlapStart
         */
        virtual void onOverlapStay(Collidable& other, float IoU) { MIGHTER2D_UNUSED(other); MIGHTER2D_UNUSED(IoU);}

        /**
         * @brief Handle an overlap end event
         * @param other The collidable that stopped overlapping with this collidable
         * @param IoU The percentage of overlap between the two collidables in the range [0, 1]
         *
         * This function is called automatically once per frame when this collidable
         * and another collidable stop overlapping.
         *
         * @see onOverlapStart, onOverlapStay
         */
        virtual void onOverlapEnd(Collidable& other) { MIGHTER2D_UNUSED(other);};

        /**
         * @brief Get the collidables bounding box
         */
        virtual const BoundingBox& getBoundingBox() const = 0;

        /**
         * @brief Destructor
         */
        virtual ~Collidable();

    public:
        ////////////////////////////////////////////////////////////
        //  Internal functions
        ////////////////////////////////////////////////////////////

        /**
         * @internal
         * @brief Handler a collidable overlap
         * @param other The collidable
         *
         * @warning This function is intended for internal use only
         */
        void handleCollidable(Collidable& other);

    private:
        /**
         * @brief Add a collidable with this game object
         * @param collidable The overlapper to be added
         */
        void addCollidable(Collidable* collidable);

        /**
         * @brief Check if this object has an overlapper
         * @param collidable The overlapper to be checked
         * @return True if it has it, otherwiae false
         */
        bool hasCollidable(Collidable* collidable);

        /**
         * @internal
         * @brief Remove an overlapper
         * @param collidable The overlapper to be removed
         */
        void removeCollidable(Collidable* collidable);

    private:
        Scene* scene_;                         //!< The scene the collidable belongs to
        int sceneDestrucListenerId_;           //!< The id of the scenes destruction listener
        std::string collisionGroup_;           //!< The collidables collision group (collision filtering)
        int collisionId_;                      //!< The collidables collision id (collision filtering)
        bool isStatic_;                        //!< A flag indicating whether or not the collidable is static (i.e immovable)
        bool isOverlapDetEnabled_;             //!< A flag indicating whether or not overlap detection is enabled
        CollisionExcludeList excludeList_;     //!< Stores the collision groups of collidables this collidable must not collide with
        std::vector<Collidable*> collidables_; //!< Collidables currently overlapping with this collidable
    };
}

#endif //MIGHTER2D_COLLIDABLE_H
