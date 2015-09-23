/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @package Twitcher
 */

#include "Twitcher.h"
#include <api/exceptions.h>
#include <map>
#include <set>

namespace twitcher {

Twitcher::Twitcher(IStorage& storage, IIndexer& indexer)
    : mStorage(storage)
    , mIndexer(indexer) {
}

void Twitcher::addUser(const std::string& name) {
    if (mStorage.userExists(name)) {
        throw UserExistsException("User already exists: " + name);
    }

    mStorage.setUser(User(name));
}

void Twitcher::addPost(const std::string& name,
                        const std::string& text,
                        uint64_t timestamp) {
    if (!mStorage.userExists(name)) {
        throw UserNotFoundException("User does not exist: " + name);
    }
    const unsigned maxChars = 140;
    std::string postText = text.size() > maxChars ? text.substr(0, maxChars) : text;
    IUser& user = mStorage.getUser(name);
    user.addPost(postText, timestamp);
    TTopics topics;
    if (mIndexer.extractTopics(postText, topics)) {
        mIndexer.index(topics, name, timestamp);
    }
}

void Twitcher::deleteUser(const std::string& name) {
    mStorage.deleteUser(name);
    mIndexer.purgeByUser(name);
}

PostTexts Twitcher::getPostsForUser(const std::string& name) const {
    if (!mStorage.userExists(name)) {
        throw UserNotFoundException("User does not exist: " + name);
    }

    PostTexts result;
    IUser& user = mStorage.getUser(name);
    auto posts = user.getPosts();
    for (auto it = posts.begin(); it != posts.end(); ++it) {
        result.push_back(it->second);
    }

    return result;
}

PostTexts Twitcher::getPostsForTopic(const std::string& topic) const {
    PostTexts result;
    if (mIndexer.hasTopicItem(topic)) {
        auto topicItem = mIndexer.getTopicItem(topic);
        std::map<uint64_t, std::vector<std::string> > topicMap;
        for (auto& item: topicItem.userPostIds) {
            if (mStorage.userExists(item.first)) {
                auto& user = mStorage.getUser(item.first);

                for (auto& postId: item.second) {
                    if (user.hasPost(postId)) {
                        topicMap[postId].push_back(user.getPost(postId));
                    }
                }
            }
        }

        // Expand texts sorted on descending timestamp.
        for (auto it = topicMap.rbegin(); it != topicMap.rend(); ++it) {
            for (auto& text: it->second) {
                result.push_back(text);
            }
        }
    }

    return result;
}

Topics Twitcher::getTrendingTopics(uint64_t fromTimestamp,
                                    uint64_t toTimestamp) const {
    Topics result;
    std::map<std::string, uint64_t > topicMap;
    for (uint64_t i = fromTimestamp; i <= toTimestamp; ++i) {
        if (mIndexer.hasPostTopics(i)) {
            auto set = mIndexer.getPostTopics(i);
            for (auto& topic: set) {
                if (topicMap.find(topic) == topicMap.end()) {
                    topicMap[topic] = 0;
                }
                ++topicMap[topic];
            }
        }
    }

    // Sort by descending mention count primarily, alphabetically on topic secondarily.
    std::map<uint64_t, std::set<std::string> > topicCountMap;
    for (auto it = topicMap.rbegin(); it != topicMap.rend(); ++it) {
        topicCountMap[it->second].insert(it->first);
    }

    for (auto it = topicCountMap.rbegin(); it != topicCountMap.rend(); ++it) {
        for (auto& topic: it->second) {
            result.push_back(topic);
        }
    }

    return result;
}

} // namespace twitcher
