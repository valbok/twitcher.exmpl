/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @package Twitcher
 */

#ifndef TWITCHER_ISTORAGE
#define TWITCHER_ISTORAGE

#include "IUser.h"

namespace twitcher {

/**
 * Interface to provide an ability to access the data.
 */
class IStorage {
public:

    /**
     * Stores user within current storage context.
     * @param IUser
     */
    virtual void setUser(const IUser& user) = 0;

    /**
     * Checks if user exists by name.
     * @param name
     * @return true If found.
     */
    virtual bool userExists(const std::string& name) const = 0;

    /**
     * Returns user object by name.
     * @param name
     * @return IUser
     */
    virtual IUser& getUser(const std::string& name) = 0;

    /**
     * Deletes user and all its data.
     * @param Username.
     */
    virtual void deleteUser(const std::string& name) = 0;

    virtual ~IStorage() {}
};

} // namespace twitcher

#endif // TWITCHER_ISTORAGE