/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @package Twitcher
 */

#ifndef TWITCHER_EXCEPTIONS
#define TWITCHER_EXCEPTIONS

#include <exception>
#include <string>

namespace twitcher {

/**
 * Base exception class to override
 */
class Exception: public std::exception {
public:

    /**
     * @param Message
     */
    Exception(const std::string& message): mMessage(message) {}

    /**
     * @copydoc std::exception::what()
     */
    const char* what() const noexcept
    {
        return mMessage.c_str();
    }

protected:

    /**
     * Message to display.
     */
    std::string mMessage;
};

/**
 * Custom exception when user was not found.
 */
class UserNotFoundException: public Exception {
public:
    UserNotFoundException(const std::string& message): Exception(message) {}
};

/**
 * When user already exists.
 */
class UserExistsException: public Exception {
public:
    UserExistsException(const std::string& message): Exception(message) {}
};

} // namespace twitcher

#endif // TWITCHER_EXCEPTIONS