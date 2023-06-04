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

#ifndef MIGHTER2D_ENGINESETTINGS_H
#define MIGHTER2D_ENGINESETTINGS_H

#include "Mighter2d/Config.h"
#include "Mighter2d/common/PrefContainer.h"

namespace mighter2d {
    /**
     * @brief Specify the initial engine settings
     *
     * Note that all engine settings have defults
     */
    class MIGHTER2D_API EngineSettings {
    public:
        /**
         * @brief Constructor
         */
        EngineSettings();

        /**
         * @brief Load the settings from a file
         * @param filename The filename of the settings file
         * @throws FileNotFoundException If @a filename cannot be opened for reading
         * @throws InvalidParseException If the contents of @a filename are not in the correct format
         *
         * The correct format of file entries is @b KEY:TYPE=VALUE where:
         * @b KEY is the name of a variable, @b TYPE is the type of the KEY (STRING, BOOL, INT, UINT, FLOAT and DOUBLE)
         * and VALUE is the value of the KEY. Note that the KEY must not contain any whitespaces or
         * special characters except underscores. For BOOL types use 1 for true and 0 for false
         *
         * @example
         * WINDOW_TITLE:STRING=My awesome game
         * WINDOW_FULLSCREEN:BOOL=0
         * WINDOW_WIDTH:INT=600
         *
         * @note If a setting is not found in the specified file, it will be replaced by its default value
         */
        void loadFromFile(const std::string& filename);

        /**
         * @brief Set the initial title of the game window
         * @param title The title to be set
         *
         * Filename key-value pair: WINDOW_TITLE:STRING=value
         *
         * @brief By default, the title is "Untitled"
         *
         * @see getWindowTitle
         */
        void setWindowTitle(const std::string& title);

        /**
         * @brief Get the title of the game window
         * @return The title of the game window
         *
         * @see setWindowTitle
         */
        std::string getWindowTitle() const;

        /**
         * @brief Set the initial window icon
         * @param iconFilename The filename of the window icon
         *
         * Note that the icon image must be placed in the images/textures
         * directory. This function does not load the icon, it saves the
         * filename and the icon will be loaded by the engine during the
         * initialization stage
         *
         * Filename key-value pair: WINDOW_ICON:STRING=value
         *
         * By default, the icon is the operating system icon
         *
         * @see getWindowIcon, setTexturesDirectory
         */
        void setWindowIcon(const std::string& iconFilename);

        /**
         * @brief Get the initial icon of the window
         * @return The icon of the window
         *
         * @see setWindowIcon
         */
        std::string getWindowIcon() const;

        /**
         * @brief Set the initial width of the game window
         * @param width The width to be set
         *
         * Filename key-value pair: WINDOW_WIDTH:INT=value
         *
         * By default, the width is 600
         *
         * @see getWindowWidth
         */
        void setWindowWidth(int width);

        /**
         * @brief Get the initial width of the game window
         * @return The initial width of the game window
         *
         * @see setWindowWidth
         */
        int getWindowWidth() const;

        /**
         * @brief Set the initial height of the game window
         * @param height The height to be set
         *
         * Filename key-value pair: WINDOW_HEIGHT:INT=value
         *
         * By default, the height is 600
         *
         * @see getWindowHeight
         */
        void setWindowHeight(int height);

        /**
         * @brief Get the initial height of the game window
         * @return The initial height of the game window
         *
         * @see setWindowHeight
         */
        int getWindowHeight() const;

        /**
         * @brief Set the initial frames per second (fps) limit of the game window
         * @param fpsLimit The fps limit to be set
         *
         * Filename key-value pair: WINDOW_FPS_LIMIT:INT=value
         *
         * By default, the fps is 60
         *
         * @see getWindowFPSLimit
         */
        void setWindowFPSLimit(int fpsLimit);

        /**
         * @brief Get the initial frames per seconds (fps) of the game window
         * @return The initial fps of the game window
         *
         * @see setWindowFPSLimit
         */
        int getWindowFPSLimit() const;

        /**
         * @brief Set whether the window is initially created in fullscreen mode or not
         * @param fullscreen True to create window in fullscreen, otherwise false
         *
         * Filename key-value pair: WINDOW_FULLSCREEN:BOOL=value
         *
         * By default, the window is not created in fullscreen mode
         *
         * @see getWindowFullScreenEnabled
         */
        void setWindowFullScreenEnable(bool fullscreen);

        /**
         * @brief Get window initial fullscreen enabled
         * @return True if initially th window is created in fullscreen mode, otherwise false
         *
         * @see setWindowFullScreenEnable
         */
        bool getWindowFullscreenEnabled() const;

        /**
         * @brief Set whether the window is created with vertical synchronization (v-sync) enabled
         * @param enable True to create window with v-sync enabled, otherwise false
         *
         * Filename key-value pair: WINDOW_V_SYNC:BOOL=value
         *
         * By default, v-sync is disabled
         *
         * @see getWindowVerticalSyncEnabled
         */
        void setWindowVerticalSyncEnable(bool enable);

        /**
         * @brief Get the windows initial vertical synchronization (v-sync)
         * @return True if enabled, otherwise false
         *
         * @see setWindowVerticalSyncEnable
         */
        bool getWindowVerticalSyncEnabled() const;

        /**
         * @brief Set the directory in which fonts can be found
         * @param dir The directory in fonts can be found
         *
         * Filename key-value pair: FONTS_DIR:STRING=value
         *
         * By default, the directory is the game executable directory
         *
         * @see getFontsDirectory
         */
        void setFontsDirectory(const std::string& dir);

        /**
         * @brief Get the directory in which fonts can be found
         * @return The directory in which fonts can be found
         *
         * By default, the directory is the game executable directory
         *
         * @see setFontsDirectory
         */
        std::string getFontsDirectory() const;

        /**
         * @brief Set the directory in which textures/images can be found
         * @param dir The directory in textures/image can be found
         *
         * Filename key-value pair: TEXTURES_DIR:STRING=value
         *
         * By default, the directory is the game executable directory
         *
         * @see getTexturesDirectory
         */
        void setTexturesDirectory(const std::string& dir);

        /**
         * @brief Get the directory in which textures/images can be found
         * @return The directory in which textures/images can be found
         *
         * By default, the directory is the game executable directory
         *
         * @see setTexturesDirectory
         */
        std::string getTexturesDirectory() const;

        /**
         * @brief Set the directory in which sound effects can be found
         * @param dir The directory in sound effects can be found
         *
         * Filename key-value pair: SOUND_EFFECTS_DIR:STRING=value
         *
         * By default, the directory is the game executable directory
         *
         * @see getSoundEffectsDirectory
         */
        void setSoundEffectsDirectory(const std::string& dir);

        /**
         * @brief Get the directory in which sound effects can be found
         * @return The directory in which sound effects can be found
         *
         * @see setSoundEffectsDirectory
         */
        std::string getSoundEffectsDirectory() const;

        /**
         * @brief Set the directory in which music can be found
         * @param dir The directory in music can be found
         *
         * Filename key-value pair: MUSIC_DIR:STRING=value
         *
         * By default, the directory is the game executable directory
         *
         * @see getMusicDirectory
         */
        void setMusicDirectory(const std::string& dir);

        /**
         * @brief Get the directory in which music can be found
         * @return The directory in which music can be found
         *
         * @see setMusicDirectory
         */
        std::string getMusicDirectory() const;

    private:
        /**
         * @brief Set the default engine settings
         */
        void setDefaultSettings();

    private:
        PrefContainer settings_; //!< Stores the engine settings
    };
}

#endif