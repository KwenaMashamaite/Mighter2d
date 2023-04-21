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
         * @brief Handle an overlap between this object and another
         * @param other The collidable that's starting to overlap with
         *              this collidable
         *
         * This function is called automatically once per frame when this collidable
         * and another collidable start to overlap.
         *
         * @see onOverlapEnd
         */
        virtual void onOverlapStart(Collidable& other) { MIGHTER2D_UNUSED(other); };

        /**
         * @brief Handle an overlap stay
         * @param other The collidable that is remaining in contact with this collidable
         *
         * This function is called automatically once per frame when this collidable and
         * another collidable remain in contact after the initial overlap.
         *
         * @see onOverlapStart
         */
        virtual void onOverlapStay(Collidable& other) { MIGHTER2D_UNUSED(other);};

        /**
         * @brief Handle an overlap end event
         * @param other The collidable that stopped overlapping with this collidable
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
        std::vector<Collidable*> collidables_; //!< Collidables currently overlapping with this collidable
    };
}

#endif //MIGHTER2D_COLLIDABLE_H
