/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @package Twitcher
 */

#include "Indexer.h"
#include <sstream>
#include <algorithm>

namespace twitcher {

bool Indexer::extractTopics(const std::string& text, TTopics& topics) const {
    bool result = false;
    std::istringstream ss(text);
    std::string word;
    char pmarks[] = ".,!?:";
    char* end = pmarks + sizeof(pmarks) / sizeof(char);

    while (ss >> word) {
        // @todo Regex does not work on versions < GCC 4.9.0, add regex when possible.
        // Remove punctuation marks.
        auto last = word[word.size() - 1];
        while (word.size() > 0 && std::find(pmarks, end, last) != end) {
            word.pop_back();
            last = word[word.size() - 1];
        }

        if (word.size() > 1 && word[0] == '#') {
            std::string topic = word.substr(1);
            bool valid = true;
            for(char& s: topic) {
                bool validChar = false;
                validChar |= s >= '0' && s <= '9';
                validChar |= s >= 'a' && s <= 'z';
                validChar |= s >= 'A' && s <= 'Z';
                validChar |= s == '_';
                if (!validChar) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                topics.push_back(topic);
                result = true;
            }
        }
    }

    return result;
}

bool Indexer::index(const TTopics& topics, const std::string& userName, uint64_t postId) {
    bool result = false;
    for (auto& topic: topics) {
        mUserTopics[userName].insert(topic);
        mPostTopics[postId].insert(topic);

        if (mTopicItems.find(topic) == mTopicItems.end()) {
            mTopicItems[topic] = {0, TUserPostIds()};
        }

        auto& item = mTopicItems[topic];
        item.userPostIds[userName].insert(postId);
        ++item.postsCount;
        result = true;
    }

    return result;
}

bool Indexer::hasTopicItem(const std::string& topic) const {
    return mTopicItems.find(topic) != mTopicItems.end();
}

const STopicItem& Indexer::getTopicItem(const std::string& topic) {
    return mTopicItems[topic];
}

bool Indexer::hasPostTopics(uint64_t postId) const {
    return mPostTopics.find(postId) != mPostTopics.end();
}

const TTopicSet& Indexer::getPostTopics(uint64_t postId) {
    return mPostTopics[postId];
}

bool Indexer::purgeByUser(const std::string& userName) {
    // Fetch topics by username from {username: [topic1, topic2]}
    auto userTopics = mUserTopics.find(userName);
    if (userTopics == mUserTopics.end()) {
        return false;
    }

    // Go through found topics and clear items.
    auto topicSet = userTopics->second;
    for (auto it = topicSet.begin(); it != topicSet.end(); ++it) {
        if (hasTopicItem(*it)) {
            uint64_t purgedPostSize = 0;
            // Contains postsCount and userPostIds.
            auto& item = mTopicItems[*it];

            // Clear mPostTopics.
            auto user = item.userPostIds.find(userName);
            if (user != item.userPostIds.end()) {
                purgedPostSize = user->second.size();

                for (auto& postId: user->second) {
                    mPostTopics.erase(postId);
                }
            }

            // Purge post ids by user.
            item.userPostIds.erase(userName);

            if (purgedPostSize > item.postsCount) {
                purgedPostSize = item.postsCount;
            }
            item.postsCount -= purgedPostSize;

            // If all post ids have been removed, purge the topic.
            if (item.postsCount == 0) {
                mTopicItems.erase(*it);
            }
        }
    }

    mUserTopics.erase(userName);

    return true;
}

uint64_t Indexer::getTopicItemsSize() const {
    return mTopicItems.size();
}

uint64_t Indexer::getPostTopicsSize() const {
    return mPostTopics.size();
}

uint64_t Indexer::getUserTopicsSize() const {
    return mUserTopics.size();
}

} // namespace twitcher

