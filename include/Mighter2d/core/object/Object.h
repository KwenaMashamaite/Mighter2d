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

#ifndef MIGHTER2D_OBJECT_H
#define MIGHTER2D_OBJECT_H

#include "Mighter2d/Config.h"
#include "Mighter2d/core/event/EventEmitter.h"
#include "Mighter2d/common/Property.h"
#include <unordered_map>
#include <functional>
#include <string>
#include <memory>

namespace mighter2d {
    /**
     * @brief An abstract top-level base class for Mighter2d objects
     */
    class MIGHTER2D_API Object : public EventEmitter {
    public:
        using Ptr = std::unique_ptr<Object>; //!< Unique object pointer

        /**
         * @brief Default constructor
         */
        Object();

        /**
         * @brief Copy constructor
         * @param other The object to copy into this object
         *
         * @note The destruction listeners of @a other are not copied to
         * this object
         */
        Object(const Object& other);

        /**
         * @brief Copy assignment operator
         * @param other The object to copy assign to this one
         *
         * @note The destruction listeners of @a other are not assigned to
         * this object
         */
        Object& operator=(const Object& other);

        /**
         * @brief Move constructor
         */
        Object(Object&&) noexcept = default;

        /**
         * @brief Move assignment operator
         */
        Object& operator=(Object&&) noexcept = default;

        /**
         * @brief Assign the object an alias
         * @param tag The alias of the object
         *
         * This function is useful if you want to refer to the object by
         * a tag instead of its object id. Unlike an object id, multiple
         * objects may have the same tag
         *
         * By default, the tag is an empty string
         *
         * @see getObjectId
         */
        void setTag(const std::string& tag);

        /**
         * @brief Get the tag assigned to the object
         * @return The tag of the object
         *
         * @see setTag
         */
        const std::string& getTag() const;

        /**
         * @brief Get the unique id of the object
         * @return The unique id of the object
         *
         * Note that each instance of mighter2d::Object has a unique id
         *
         * @see setTag
         */
        unsigned int getObjectId() const;

        /**
         * @brief Get the name of the objects concrete class
         * @return The name of the objects concrete class
         *
         * This function is implemented by all internal classes that inherit
         * from this class (either directly or indirectly). Example:
         *
         * @code
         * class mighter2d::GameObject : public mighter2d::Object {...}
         * GameObject gObject;
         * std::cout << gObject.getClassName(); // Prints "GameObject"
         * @endcode
         *
         * @see getBaseClassName
         */
        virtual std::string getClassName() const = 0;

        /**
         * @brief Get the name of the direct parent of an object instance
         * @return The name of the direct parent of an object instance
         *
         * In contrast to getClassName() which returns the name of the concrete
         * class, this function returns the name of the concrete class's base
         * class. This function is implemented by all derived classes of mighter2d::Object
         * which also serve as base classes. For classes whose direct parent is
         * this class, this function will return the name of this class
         *
         * @code
         * auto rectangle = mighter2d::RectangleShape(); // RectangleShape is derived from Shape
         * std::cout << rectangle->getClassName(); // Prints "RectangleShape"
         * std::cout << rectangle->getBaseClassName(); // Prints "Shape"
         * @endcode
         *
         * @see getClassName
         */
        virtual std::string getBaseClassName() const;

        /**
         * @brief Add an event listener to a specific property change event
         * @param property The name of the property to add an event listener to
         * @param oneTime True to execute the callback one-time or false to
         *                execute it every time the event is triggered
         * @param callback The function to be executed when the property changes
         * @return The unique id of the event listener
         *
         * A property change event is triggered by any function that begins
         * with @a set, where the the text after @a set is the name of the
         * property. For example, the @a setTag() function, modifies the @a tag
         * property of the object, thus will generate a "tag" change event each
         * time it is called
         *
         * Note that multiple event listeners may be registered to the same
         * property change event. In addition, when adding a property change
         * event listener, the name of the property must be in lowercase.
         *
         * @code
         * // Prints the tag of the player object to the console everytime it changes
         * player.onPropertyChange("tag", [](const Property& property) {
         *      cout << "New tag: " <<  property.getValue<std::string>() << endl;
         * });
         *
         * ...
         * //Sets tag = "player1" and invokes event listener(s)
         * player.setTag("player1");
         * @endcode
         *
         * @see unsubscribe and onPropertyChange(const mighter2d::Callback<mighter2d::Property>&)
         */
        int onPropertyChange(const std::string& property, const Callback<Property>& callback,
            bool oneTime = false);

        /**
         * @brief Add an event listener to any property change event
         * @param callback The function to be executed when any property changes
         * @param oneTime True to execute the callback one-time or false to
         *                execute it every time the event is triggered
         * @return The unique id of the event listener
         *
         * When unsubscribing the any property change event handler, you must
         * pass @a "propertyChange" as the name of the event
         *
         * @see onPropertyChange(const std::string&, const mighter2d::Callback<mighter2d::Property>&)
         */
        int onPropertyChange(const Callback<Property>& callback, bool oneTime = false);

        /**
         * @brief Add a destruction listener
         * @param callback Function to be executed when the object is destroyed
         * @return The unique id of the destruction listener
         *
         * The destruction listener is called when the object reaches the end
         * of its lifetime. Note that an object may have multiple destruction
         * listeners registered to it and they are executed once.
         *
         * @note Destruction listeners are not copied when copy constructing
         * or assigning the object
         *
         * @see removeEventListener
         */
        int onDestruction(const Callback<>& callback);

        /**
         * @brief Check if another object is the same instance as this object
         * @param other The object to compare against this object
         * @return True if @a other is the same instance as this object,
         *         otherwise false
         *
         * This function makes a comparison by looking at the object id's. If they
         * are the same it returns true, otherwise false.
         */
        bool isSameObjectAs(const Object& other) const;

        /**
         * @brief Destructor
         */
        virtual ~Object();

    protected:
        /**
         * @brief Dispatch a property change event
         * @param property The property that changed
         *
         * This function will invoke all the event listeners of the specified
         * property
         *
         * @see emit
         */
        void emitChange(const Property& property);

        /**
         * @brief Emit a destruction event
         *
         * @note This function must be the first statement in the definition
         * of a destructor to avoid undefined behavior. In addition, note that
         * destruction listeners are invoked once. Therefore, multiple classes
         * in a hierarchy may call this function but the class that makes the
         * call first will be the one that invokes the destruction listeners
         */
        void emitDestruction();

    private:
        unsigned int id_;   //!< The id of the object
        std::string tag_;   //!< The object's tag
    };
}

/**
 * @class mighter2d::Object
 * @ingroup core
 *
 * This class is intended to group all Mighter2d objects under a common ancestor.
 * However, not all classes in Mighter2d inherited from it. The extension of mighter2d::Object
 * is not restricted to internal classes only, classes outside of Mighter2d may also
 * extend if need be
 */

#endif //MIGHTER2D_OBJECT_H
