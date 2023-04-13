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

#ifndef MIGHTER2D_PREFCONTAINER_H
#define MIGHTER2D_PREFCONTAINER_H

#include "Mighter2d/Config.h"
#include "Mighter2d/common/Preference.h"
#include "Mighter2d/common/PropertyContainer.h"
#include <vector>
#include <optional>

namespace mighter2d {
    /**
     * @brief Container for Preference instances
     *
     * A PrefContainer is very similar to a PropertyContainer. What sets them
     * apart is that a PrefContainer can load/save its contents from/to a file
     * on the disk. In addition, a PrefContainer is convertible to a PropertyContainer
     */
    class MIGHTER2D_API PrefContainer {
    public:
        /**
         * @brief Load preferences from the disk
         * @param filename The name of the file to load the preferences from
         * @throws FileNotFoundException If @a filename cannot be opened for reading
         * @throws InvalidParseException If the contents of @a filename cannot
         *         be successfully parsed
         *
         * @note All current data is destroyed on subsequent function calls
         *
         * @see save
         */
        void load(const std::string& filename);

        /**
         * @brief Add a preference to the container
         * @param pref The preference to be added
         * @return True if the preference was added or false if a preference
         *         with the same key already exits in the container
         */
        bool addPref(const Preference& pref);
        bool addPref(Preference&&);

        /**
         * @brief Get a preference
         * @param key The key of the preference to retrieve
         * @return A preference with the specified key
         */
        Preference& getPref(const std::string& key);
        const Preference& getPref(const std::string& key) const;

        /**
         * @brief Check if the container has a preference or not
         * @param key The key of the preference to be checked
         * @return True if the preference exist, otherwise false
         */
        bool hasPref(const std::string& key);

        /**
         * @brief Remove a preference from the container
         * @param key The key of the preference to be removed
         */
        void removePref(const std::string& key);

        /**
         * @brief Remove all preferences from the container
         */
        void clear();

        /**
         * @brief Get the number of preferences in the container
         * @return The number of preferences in the container
         */
        std::size_t getCount() const;

        /**
         * @brief Convert preference to properties
         * @return Preferences as properties
         *
         * @attention This function was released in Mighter2d @a v2.1.0 and is intended
         * for compatibility with previous release (Mighter2d @a v2.0.0). In Mighter2d @a v2.1.0,
         * mighter2d::utility::ConfigFileParser is deprecated and replaced by mighter2d::PrefContainer,
         * thus you can load configs with mighter2d::PrefContainer and use this function to
         * convert mighter2d::PrefContainer into mighter2d::PropertyContainer which was returned by
         * mighter2d::utility::ConfigFileParser without the need to change your code to
         * work with mighter2d::PrefContainer instances.
         */
        PropertyContainer asPropertyContainer() const;

        /**
         * @brief Save preferences to the disk
         * @param filename The name of the file to save the preferences to
         * @throws FileNotFoundException If @a filename cannot be opened for writing
         *
         * If @a filename is left unspecified, the engine will attempt to
         * write the data to the same file as the load file.
         *
         * @warning This function will overwrite the file
         *
         * @see load
         */
        void save(const std::string& filename = "sameAsLoadFile");

    private:
        /**
         * @brief Get the index of a preference in the container
         * @param key The index of the preference with the given key
         * @return An optional with a value of the index of the preference
         *         with the given key or a std::nullopt if a preference with
         *         the given key does not exist
         */
        std::optional<int> getIndex(const std::string& key) const;

    private:
        std::string filename_;          //!< The name of the file prefs were loaded from
        std::vector<Preference> prefs_; //!< Preference container
    };
}

#endif //MIGHTER2D_PREFCONTAINER_H
