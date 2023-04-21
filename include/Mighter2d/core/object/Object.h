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
#include "Mighter2d/common/IClassifiable.h"
#include "Mighter2d/common/Destructible.h"
#include <unordered_map>
#include <functional>
#include <string>
#include <memory>

namespace mighter2d {
    /**
     * @brief An abstract top-level base class for Mighter2d objects
     */
    class MIGHTER2D_API Object : public EventEmitter, public Destructible, public IClassifiable {
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
         * @brief Get the base class name
         * @return The base class name
         *
         * @see getClassName
         */
        std::string getBaseClassName() const override;

        /**
         * @brief Get the name of this class
         * @return The name of this class
         *
         * @see getBaseClassName
         */
        std::string getClassName() const override;

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
