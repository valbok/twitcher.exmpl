/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @package Twitcher
 */

#include "Storage.h"

namespace twitcher {

void Storage::setUser(const IUser& user) {
    // Makes a copy of a user
    mUsers[user.getName()] = user;
}

bool Storage::userExists(const std::string& name) const {
    return mUsers.find(name) != mUsers.end();
}

IUser& Storage::getUser(const std::string& name) {
    return mUsers[name];
}

void Storage::deleteUser(const std::string& name) {
    mUsers.erase(name);
}

} // namespace twitcher

