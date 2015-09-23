/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @package Twitcher
 */

#ifndef TWITCHER_USER
#define TWITCHER_USER

#include <kernel/api/IUser.h>

namespace twitcher {

/**
 * Implementation of user interface.
 */
class User: public IUser {
public:

    /**
     * Only for copying purpose.
     */
    User();

    /**
     * @param Username
     */
    User(const std::string& name);

    /**
     * @copydoc IUser::getName()
     */
    virtual const std::string& getName() const;

    /**
     * @copydoc IUser::addPost()
     */
    virtual void addPost(const std::string& text, uint64_t timestamp);

    /**
     * @copydoc IUser::getPosts()
     */
    virtual const TPosts& getPosts() const;

    /**
     * @copydoc IUser::hasPost()
     */
    virtual bool hasPost(uint64_t id) const;

    /**
     * @copydoc IUser::getPost()
     */
    virtual const std::string& getPost(uint64_t id);

    virtual ~User() {}

    /**
     * Only for copying purpose.
     */
    User& operator=(const IUser& other);

protected:

    /**
     * Prevents copy constructor.
     */
    User(const User& other);


    /**
     * Name.
     */
    std::string mName;

    /**
     * List of posts.
     */
    TPosts mPosts;
};

} // namespace twitcher

#endif // TWITCHER_USER