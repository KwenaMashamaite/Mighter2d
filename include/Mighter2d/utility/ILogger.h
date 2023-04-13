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

#ifndef MIGHTER2D_LOGGER_H
#define MIGHTER2D_LOGGER_H

#include <string>

namespace mighter2d {
    namespace utility {
        /**
         * @brief Type of message to be logged
         */
        enum class MessageType {
            General, //!< General message
            Status,  //!< Status reporting message
            Warning, //!< Warning message
            Error    //!< Error message
        };

        /**
         * @brief Interface for logging classes
         */
        class MIGHTER2D_API ILogger {
        public:
            /**
             * @brief Log message
             * @param type Type of message to be logged
             * @param msg Message to log
             */
            virtual void log(MessageType type, const std::string& msg) = 0;

            /**
             * @brief Destructor
             */
            virtual ~ILogger() = default;
        };
    }
}

#endif // MIGHTER2D_LOGGER_H
