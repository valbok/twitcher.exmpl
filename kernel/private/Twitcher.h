/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @package Twitcher
 */

#ifndef TWITCHER_TWITCHER
#define TWITCHER_TWITCHER

#include <api/ITwitcher.h>
#include "Storage.h"
#include "Indexer.h"

namespace twitcher {

/**
 * Implementation of Twitcher API interface.
 */
class Twitcher: public ITwitcher {
public:

    /**
     * @param Storage injection to provide an access to actual data.
     * @param Indexer injection to grab topics/tags and create an index to search by.
     */
    Twitcher(IStorage& storage, IIndexer& indexer);

    /**
     * @copydoc Twitcher::addUser()
     */
    virtual void addUser(const std::string& name);

    /**
     * @copydoc Twitcher::addPost()
     */
    virtual void addPost(const std::string& name,
                         const std::string& text,
                         uint64_t timestamp);

    /**
     * @copydoc Twitcher::deleteUser()
     */
    virtual void deleteUser(const std::string& name);

    /**
     * @copydoc Twitcher::getPostsForUser()
     */
    virtual PostTexts getPostsForUser(const std::string &userName) const;

    /**
     * @copydoc Twitcher::getPostsForTopic()
     */
    virtual PostTexts getPostsForTopic(const std::string &topic) const;

    /**
     * @copydoc Twitcher::getTrendingTopics()
     */
    virtual Topics getTrendingTopics(uint64_t fromTimestamp,
                                     uint64_t toTimestamp) const;
    virtual ~Twitcher() {}

private:

    /**
     * Provides an access to a data. For example users, their posts etc.
     */
    IStorage& mStorage;

    /**
     * Provides an ability to index data, grab keys and search/lookup.
     */
    IIndexer& mIndexer;
};

} // namespace twitcher

#endif // TWITCHER_TWITCHER