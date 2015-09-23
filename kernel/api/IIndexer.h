/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @package Twitcher
 */

#ifndef TWITCHER_IINDEXER
#define TWITCHER_IINDEXER

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace twitcher {

/**
 * Vector of topic names.
 */
typedef std::vector<std::string> TTopics;

/**
 * Post ids by users: {username: [post_id1, post_id2]}
 */
typedef std::unordered_map<std::string, std::unordered_set<uint64_t> > TUserPostIds;

/**
 * Set to keep unique topics.
 */
typedef std::unordered_set<std::string> TTopicSet;

/**
 * Item to store by topic.
 */
struct STopicItem {

    /**
     * Count of posts that contain current topic.
     */
    uint64_t postsCount;

    /**
     * References to users and their post ids that mentioned current topic.
     */
    TUserPostIds userPostIds;
};

/**
 * Indexer interface to provide an ability to grab search keys, store it and lookup.
 */
class IIndexer {
public:

    /**
     * Extracts topics/keys from \a text.
     * @param Text
     * @param[] Topics
     * @return true If something found.
     */
    virtual bool extractTopics(const std::string& text, TTopics& topics) const = 0;

    /**
     * Stores topics by username and its post id.
     * @param Topics
     * @param Username
     * @param Postid
     * @return true If successful.
     */
    virtual bool index(const TTopics& topics, const std::string& userName, uint64_t postId) = 0;

    /**
     * Checks if topic item exists.
     * @param Topic, just a tag/key.
     * @return true If found.
     */
    virtual bool hasTopicItem(const std::string& topic) const = 0;

    /**
     * Returns topic item by string name.
     * @return struct to find users and their posts.
     */
    virtual const STopicItem& getTopicItem(const std::string& topic) = 0;

    /**
     * Checks if post topic exists.
     * @param Post id
     * @return true If found.
     */
    virtual bool hasPostTopics(uint64_t postId) const = 0;

    /**
     * Returns post topic by post id.
     * @return Set with unique string topics.
     */
    virtual const TTopicSet& getPostTopics(uint64_t postId) = 0;

    /**
     * Deletes all data by user.
     * @param Username
     * @return true If successful.
     */
    virtual bool purgeByUser(const std::string& userName) = 0;

    virtual ~IIndexer() {}
};

} // namespace twitcher

#endif // TWITCHER_IINDEXER