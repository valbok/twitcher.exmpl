/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @package Twitcher
 */

#include "User.h"

namespace twitcher {

User::User()
    : mName("UNKNOWN")
{
}

User::User(const std::string& name)
    : mName(name)
{
}

const std::string& User::getName() const {
    return mName;
}

void User::addPost(const std::string& text, uint64_t timestamp) {
    mPosts[timestamp] = text;
}

const User::TPosts& User::getPosts() const {
    return mPosts;
}

User& User::operator=(const IUser& other) {
    mPosts = other.getPosts();
    mName = other.getName();

    return *this;
}

bool User::hasPost(uint64_t timestamp) const {
    return mPosts.find(timestamp) != mPosts.end();
}

const std::string& User::getPost(uint64_t timestamp) {
    return mPosts[timestamp];
}

} // namespace twitcher
