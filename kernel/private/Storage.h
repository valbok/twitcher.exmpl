/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @package Twitcher
 */

#ifndef TWITCHER_STORAGE
#define TWITCHER_STORAGE

#include <kernel/api/IStorage.h>
#include "User.h"
#include <unordered_map>

namespace twitcher {

/**
 * Implementation of storage interface.
 */
class Storage: public IStorage {
public:

    /**
     * @copydoc IStorage::setUser()
     */
    virtual void setUser(const IUser& user);

    /**
     * @copydoc IStorage::userExists()
     */
    virtual bool userExists(const std::string& name) const;

    /**
     * @copydoc IStorage::getUser()
     */
    virtual IUser& getUser(const std::string& name);

    /**
     * @copydoc IStorage::deleteUser()
     */
    virtual void deleteUser(const std::string& name);

    virtual ~Storage() {}

private:

    /**
     * Stored users: {username: IUser}
     */
    typedef std::unordered_map<std::string, User> TUsers;
    TUsers mUsers;
};

} // namespace twitcher

#endif // TWITCHER_STORAGE