/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @package Twitcher
 */

#ifndef TWITCHER_INDEXER
#define TWITCHER_INDEXER

#include <kernel/api/IIndexer.h>
#include <utility>

namespace twitcher {

/**
 * Implementation of Indexer interface.
 */
class Indexer: public IIndexer {
public:

    /**
     * @copydoc IIndexer::extractTopics()
     */
    virtual bool extractTopics(const std::string& text, TTopics& topics) const;

    /**
     * @copydoc IIndexer::index()
     */
    virtual bool index(const TTopics& topics, const std::string& userName, uint64_t postId);

    /**
     * @copydoc IIndexer::hasTopicItem()
     */
    virtual bool hasTopicItem(const std::string& topic) const;

    /**
     * @copydoc IIndexer::getTopicItem()
     */
    virtual const STopicItem& getTopicItem(const std::string& topic);

    /**
     * Returns size of stored topic items.
     * @return count
     */
    virtual uint64_t getTopicItemsSize() const;

    /**
     * @copydoc IIndexer::hasPostTopics()
     */
    virtual bool hasPostTopics(uint64_t postId) const;

    /**
     * @copydoc IIndexer::getPostTopics()
     */
    virtual const TTopicSet& getPostTopics(uint64_t postId);

    /**
     * Returns size of stored topics per post ids.
     * @return count
     */
    virtual uint64_t getPostTopicsSize() const;

    /**
     * @copydoc IIndexer::purgeByUser()
     */
    virtual bool purgeByUser(const std::string& userName);

    /**
     * Returns size of stored topics per user.
     * @return count
     */
    virtual uint64_t getUserTopicsSize() const;

protected:

    /**
     * Topics by user: {username:[topic1, topic2]}
     */
    typedef std::unordered_map<std::string, TTopicSet> TUserTopics;
    TUserTopics mUserTopics;

    /**
     * Topics by post id: {post_id:[topic1, topic2]}
     */
    typedef std::unordered_map<uint64_t, TTopicSet> TPostTopics;
    TPostTopics mPostTopics;

    /**
     * Topic item to keep references to user's posts: {topic:{count:0, users:{username:[post_id]}
     */
    typedef std::unordered_map<std::string, STopicItem> TTopicItems;
    TTopicItems mTopicItems;

};

} // namespace twitcher

#endif // TWITCHER_INDEXER