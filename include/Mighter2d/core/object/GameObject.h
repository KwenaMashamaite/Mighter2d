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

#ifndef MIGHTER2D_GAMEOBJECT_H
#define MIGHTER2D_GAMEOBJECT_H

#include "Mighter2d/Config.h"
#include "Mighter2d/common/Vector2.h"
#include "Mighter2d/common/Transform.h"
#include "Mighter2d/common/PropertyContainer.h"
#include "Mighter2d/common/IUpdatable.h"
#include "Mighter2d/core/object/Object.h"
#include "Mighter2d/graphics/Sprite.h"
#include <memory>

namespace mighter2d {
    class Scene;

    /**
     * @brief Class for modelling game objects (players, enemies etc...)
     */
    class MIGHTER2D_API GameObject : public Object, public IUpdatable {
    public:
        using Ptr = std::unique_ptr<GameObject>;                      //!< Unique game object pointer
        using CollisionCallback = Callback<GameObject*, GameObject*>; //!< Collision callback

        /**
         * @brief Constructor
         * @param scene The scene this game object belongs to
         */
        explicit GameObject(Scene& scene);

        /**
         * @brief Copy constructor
         */
        GameObject(const GameObject&);

        /**
         * @brief Copy assignment operator
         */
        GameObject& operator=(const GameObject&);

        /**
         * @brief Move constructor
         */
        GameObject(GameObject&&) noexcept;

        /**
         * @brief Move assignment operator
         */
        GameObject& operator=(GameObject&&) noexcept;

        /**
         * @brief Swap the game object with another game object
         * @param other The game object to swap with this game object
         */
        void swap(GameObject& other);

        /**
         * @brief Create a new GameObject instance
         * @param scene The scene the object belongs to
         * @return The created instance
         */
        static GameObject::Ptr create(Scene& scene);

        /**
         * @brief Create a copy of the game object
         * @return A new game object
         */
        GameObject::Ptr copy() const;

        /**
         * @brief Set current state
         * @param state The current state
         *
         * The state is number that should be associated with something
         * (maybe an enum) in your game.
         *
         * Be default, the state is -1, which is supposed to indicate
         * that there is no state. The state property is optional and
         * may be used if needs be. It is not used by Mighter2d
         */
        void setState(int state);

        /**
         * @brief Get the current state of the game object
         * @return The current state of the game object
         */
        int getState() const;

        /**
         * @brief Set whether or not the game object is active
         * @param isActive True to set as active or false to set as inactive
         *
         * Note that an inactive game object does not participate in
         * collision detection. In addition the game object's update()
         * method will not be invoked.
         *
         * By default, the object is active
         *
         * @see isActive
         */
        void setActive(bool isActive);

        /**
         * @brief Check if the game object is active or not
         * @return True if the game object is active, otherwise false
         *
         * @see setActive
         */
        bool isActive() const;

        /**
         * @brief Get the user data added to game object
         * @return The user data
         *
         * The user data object can be used to store additional information
         * about the game object. For example, you may store a profile associated
         * with the game object or when the game object was instantiated etc...
         * You can store any type and any number of data in the user date object
         *
         * Note that Mighter2d does not store anything inside the user data object,
         * it is reserved for external use only
         */
        PropertyContainer& getUserData();
        const PropertyContainer& getUserData() const;

        /**
         * @brief Get the name of the class the game object is instantiated from
         * @return The name of the concrete class the game object is instantiated
         *         from
         *
         * Note that this function must be overridden further if this class
         * is extended, otherwise it will return the name of this class
         * instead of your class name
         *
         * @see getClassType
         */
        std::string getClassName() const override;

        /**
         * @brief Get the name of this class
         * @return The name of this class
         *
         * Note that this function is only implemented by child classes
         * of Object which also serve as a base class for other classes
         *
         * @see getClassName
         */
        std::string getClassType() const override;

        /**
         * @brief Get the game objects transform
         * @return The game objects transform
         *
         * The transform can be used to query or modify the game object
         * position, scale, rotation and origin
         */
        Transform& getTransform();
        const Transform& getTransform() const;

        /**
         * @brief Get the scene the game object belongs to
         * @return The scene the game object belongs to
         */
        Scene& getScene();
        const Scene& getScene() const;

        /**
         * @brief Reset the origin of the sprite
         *
         * The origin is reset to the local centre of the sprite
         *
         * @note This function must be called everytime the sprites texture,
         * texture rectangle size or scale is changed
         */
        void resetSpriteOrigin();

        /**
         * @brief Get the game objects graphical representation
         * @return The game objects graphical representation
         *
         * By default, the sprite is empty
         */
        Sprite& getSprite();
        const Sprite& getSprite() const;

        /**
         * @brief Update the game object
         * @param deltaTime Time past since last update
         *
         * @note This function will be called automatically by Mighter2d.
         * @a deltaTime is synced with the render FPS. In other words, it
         * is frame-rate dependent.
         *
         * The base class version must be called if this function
         * is overridden
         */
        void update(Time deltaTime) override;

        /**
         * @brief Destructor
         */
        ~GameObject() override;

    private:
        /**
         * @brief Subscribe game object to events
         */
        void initEvents();

    private:
        std::reference_wrapper<Scene> scene_; //!< The scene this game object belongs to
        int state_;                           //!< The current state of the game object
        bool isActive_;                       //!< A flag indicating whether or not the game object is active
        Transform transform_;                 //!< The objects transform
        std::unique_ptr<Sprite> sprite_;       //!< The objects visual representation
        PropertyContainer userData_;          //!< Used to store metadata about the object
    };
}

#endif
