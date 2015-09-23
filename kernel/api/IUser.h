/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @package Twitcher
 */

#ifndef TWITCHER_IUSER
#define TWITCHER_IUSER

#include <string>
#include <stdint.h>
#include <unordered_map>

namespace twitcher {

/**
 * Interface of user objects.
 * Provides access to user's data, like user's posts.
 */
class IUser {
public:

    /**
     * Posts of users.
     * key - canbe a post_id, but used for timestamps. Due to timestamps are unique per each post.
     */
    typedef std::unordered_map<uint64_t, std::string> TPosts;

    /**
     * Returns user name.
     * @return name
     */
    virtual const std::string& getName() const = 0;

    /**
     * @param Text
     * @param Timestamps are unique and
     *        increasing monotonically between each invocation of addPost.
     */
    virtual void addPost(const std::string& text, uint64_t timestamp) = 0;

    /**
     * Returns all stored post.
     */
    virtual const TPosts& getPosts() const = 0;

    /**
     * Checks if post exists by id.
     */
    virtual bool hasPost(uint64_t id) const = 0;

    /**
     * Checks if post exists.
     */
    virtual const std::string& getPost(uint64_t id) = 0;

    virtual ~IUser() {}
};

} // namespace twitcher

#endif // TWITCHER_IUSER