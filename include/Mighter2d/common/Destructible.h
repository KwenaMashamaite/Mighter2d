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

#ifndef MIGHTER2D_DESTRUCTIBLE_H
#define MIGHTER2D_DESTRUCTIBLE_H

#include "Mighter2d/Config.h"
#include "Mighter2d/core/event/EventEmitter.h"

namespace mighter2d {
    /**
     * @brief A base class for classes that want to inform others of their destruction
     */
    class MIGHTER2D_API Destructible {
        public:
        /**
         * @brief Constructor
         */
        Destructible() = default;

        /**
         * @brief Copy constructor
         */
        Destructible(const Destructible&);

        /**
         * @brief Copy assignment operator
         */
        Destructible& operator=(const Destructible&);

        /**
         * @brief Move constructor
         */
        Destructible(Destructible&&) = default;

        /**
         * @brief Move assignment operator
         */
        Destructible& operator=(Destructible&&) = default;

        /**
         * @brief Add a destruction listener
         * @param callback The function to be executed when the object about to be destroyed
         * @return The unique id of the destruction listener
         *
         * The destruction listener is called when the object reaches the end
         * of its lifetime. Note that an object may have multiple destruction
         * listeners registered to it and they are executed once.
         *
         * @example
         * @code
         * class Player : public mighter2d::Destructible {
         *  //class declaration
         * };
         *
         * Player player;
         *
         * player.onDestruction([] {
         *    // Logic for player destruction
         * });
         *
         * @endcode
         *
         * @note Destruction listeners are tied to this specific object, therefore they are not
         * copied when copy constructing or assigning the object
         *
         * @see removeDestructionListener
         */
        int onDestruction(const Callback<>& callback);

        /**
         * @brief Remove a destruction listener
         * @param id The unique identifier of the destruction listener
         * @return True if the destruction listener was removed or false
         *         if it does not exist
         *
         * @see onDestruction
         */
        bool removeDestructionListener(int id);

        /**
         * @brief Destructor
         */
        virtual ~Destructible();

    protected:
        /**
         * @brief Emit a destruction event
         *
         * @note Note that destruction listeners are invoked once. Therefore, multiple
         * classes in a hierarchy may call this function. However, the class that makes
         * the call first will be the one that invokes the destruction listeners.
         *
         * @warning It is recommended that this function be the first statement
         * in the definition of a derived class destructor to avoid undefined behavior.
         * If this function is not called in a derived class destructor, then destruction
         * listeners will be called by the base class. This means that the derived class
         * will be destroyed by that time and if the destruction listeners references
         * a member or function of the derived class, then undefined behavior.
         */
        void emitDestruction();

    private:
        EventEmitter eventEmitter_;
    };
}

#endif