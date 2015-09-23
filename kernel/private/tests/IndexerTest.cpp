/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @package Twitcher
 */

#include <kernel/private/Indexer.h>
#include <gtest/gtest.h>
#include <vector>

using namespace std;
using namespace twitcher;

TEST(IndexerTest, testExtractTopics) {
    Indexer i;
    {
        vector<string> v;
        bool found = i.extractTopics("", v);
        EXPECT_FALSE(found);
        EXPECT_EQ(0, v.size());
    }
    {
        vector<string> v;
        bool found = i.extractTopics("#", v);
        EXPECT_FALSE(found);
        EXPECT_EQ(0, v.size());
    }
    {
        vector<string> v;
        bool found = i.extractTopics("#!", v);
        EXPECT_FALSE(found);
        EXPECT_EQ(0, v.size());
    }
    {
        vector<string> v;
        bool found = i.extractTopics("no #bc!de", v);
        EXPECT_FALSE(found);
        EXPECT_EQ(0, v.size());
    }
    {
        vector<string> v;
        bool found = i.extractTopics("eating steak for dinner", v);
        EXPECT_FALSE(found);
        EXPECT_EQ(0, v.size());
    }
    {
        vector<string> v;
        bool found = i.extractTopics("eating #steak for dinner", v);
        EXPECT_TRUE(found);
        EXPECT_EQ(1, v.size());
        EXPECT_EQ("steak", v[0]);
    }
    {
        vector<string> v;
        bool found = i.extractTopics("#eating #steak for #dinner", v);
        EXPECT_TRUE(found);
        EXPECT_EQ(3, v.size());
        EXPECT_EQ("eating", v[0]);
        EXPECT_EQ("steak", v[1]);
        EXPECT_EQ("dinner", v[2]);
    }

    {
        vector<string> v;
        bool found = i.extractTopics("#eating, #steak. for #dinner. #exactly! #yes?", v);
        EXPECT_TRUE(found);
        EXPECT_EQ(5, v.size());
        EXPECT_EQ("eating", v[0]);
        EXPECT_EQ("steak", v[1]);
        EXPECT_EQ("dinner", v[2]);
        EXPECT_EQ("exactly", v[3]);
        EXPECT_EQ("yes", v[4]);
    }
    {
        vector<string> v;
        bool found = i.extractTopics("#eating!!!, #steak???. for #dinner...... #exactly!,,.: #yes?:", v);
        EXPECT_TRUE(found);
        EXPECT_EQ(5, v.size());
        EXPECT_EQ("eating", v[0]);
        EXPECT_EQ("steak", v[1]);
        EXPECT_EQ("dinner", v[2]);
        EXPECT_EQ("exactly", v[3]);
        EXPECT_EQ("yes", v[4]);
    }
}

TEST(IndexerTest, testGetUserPostIds) {
    Indexer i;
    const string username = "val";
    const uint64_t postId1 = 666;
    const uint64_t postId2 = 777;
    {
        const TTopics topics = {"please", "give", "me", "job"};
        EXPECT_TRUE(i.index(topics, username, postId1));

        EXPECT_FALSE(i.hasTopicItem("unknown"));
        EXPECT_TRUE(i.hasTopicItem("job"));
        auto item = i.getTopicItem("job");
        EXPECT_EQ(1, item.postsCount);
        auto postIds = item.userPostIds;
        EXPECT_EQ(1, postIds.size());
        auto it = postIds.begin();
        EXPECT_EQ(username, it->first);
        EXPECT_EQ(1, it->second.size());
        auto sit = it->second.begin();
        EXPECT_EQ(postId1, *sit);
    }
    {
        const TTopics topics = {"waiting", "for", "job"};
        EXPECT_TRUE(i.index(topics, username, postId2));

        EXPECT_FALSE(i.hasTopicItem("unknown"));
        EXPECT_TRUE(i.hasTopicItem("job"));
        auto item = i.getTopicItem("job");
        EXPECT_EQ(2, item.postsCount);
        auto postIds = item.userPostIds;
        EXPECT_EQ(1, postIds.size());
        auto it = postIds.begin();
        EXPECT_EQ(username, it->first);
        EXPECT_EQ(2, it->second.size());
        auto sit = it->second.begin();
        EXPECT_EQ(postId2, *sit);
        EXPECT_EQ(postId1, *(++sit));
    }
}

TEST(IndexerTest, testGetUserPostIdsMultipleUsers) {
    Indexer i;
    const string username1 = "val";
    const string username2 = "kim";
    const uint64_t postId1 = 666;
    const uint64_t postId2 = 777;
    {
        const TTopics topics = {"find", "secret", "on", "valbok.com"};
        EXPECT_TRUE(i.index(topics, username1, postId1));

        auto item = i.getTopicItem("secret");
        auto postIds = item.userPostIds;
        EXPECT_EQ(1, postIds.size());
        auto it = postIds.begin();
        EXPECT_EQ(username1, it->first);
        EXPECT_EQ(1, it->second.size());
        auto sit = it->second.begin();
        EXPECT_EQ(postId1, *sit);
    }
    {
        const TTopics topics = {"open", "secret", "page"};
        EXPECT_TRUE(i.index(topics, username2, postId2));

        auto item = i.getTopicItem("secret");
        auto postIds = item.userPostIds;
        EXPECT_EQ(2, postIds.size());
        auto it = postIds.begin();
        EXPECT_EQ(username2, it->first);
        EXPECT_EQ(1, it->second.size());
        auto sit = it->second.begin();
        EXPECT_EQ(postId2, *sit);
        ++it;
        EXPECT_EQ(username1, it->first);
        EXPECT_EQ(1, it->second.size());
        sit = it->second.begin();
        EXPECT_EQ(postId1, *sit);
    }
}

TEST(IndexerTest, testGetPostTopic) {
    Indexer i;
    const string username = "val";
    const uint64_t postId = 666;
    {
        const TTopics topics = {"secret", "still", "there"};
        EXPECT_TRUE(i.index(topics, username, postId));
        EXPECT_FALSE(i.hasPostTopics(0));
        EXPECT_TRUE(i.hasPostTopics(postId));
        auto set = i.getPostTopics(postId);
        EXPECT_EQ(3, set.size());
        auto it = set.begin();
        EXPECT_EQ("there", *it);
        EXPECT_EQ("still", *++it);
        EXPECT_EQ("secret", *++it);
    }
    {
        EXPECT_TRUE(i.index({"check_html"}, username, postId));
        auto set = i.getPostTopics(postId);
        EXPECT_EQ(4, set.size());
        auto it = set.begin();
        EXPECT_EQ("check_html", *it);
    }
}

TEST(IndexerTest, testPurgeByUserSingleItem) {
    Indexer i;
    const string username = "val";
    const uint64_t postId = 666;
    const TTopics topics = {"check", "source", "html"};
    EXPECT_TRUE(i.index(topics, username, postId));
    EXPECT_TRUE(i.hasPostTopics(postId));
    EXPECT_TRUE(i.hasTopicItem("check"));
    EXPECT_TRUE(i.hasTopicItem("source"));
    EXPECT_TRUE(i.hasTopicItem("html"));

    EXPECT_FALSE(i.purgeByUser("kim"));
    EXPECT_TRUE(i.purgeByUser(username));

    EXPECT_FALSE(i.hasPostTopics(postId));
    EXPECT_FALSE(i.hasTopicItem("check"));
    EXPECT_FALSE(i.hasTopicItem("source"));
    EXPECT_FALSE(i.hasTopicItem("html"));
}

TEST(IndexerTest, testPurgeByUser) {
    Indexer i;
    const string username1 = "val";
    const uint64_t postId1 = 666;
    const string username2 = "kim";
    const uint64_t postId2 = 777;

    EXPECT_TRUE(i.index(TTopics({"href", "cv", "date"}), username1, postId1));
    EXPECT_TRUE(i.index(TTopics({"cv"}), username1, postId1 + 1));

    EXPECT_TRUE(i.hasPostTopics(postId1));
    EXPECT_TRUE(i.hasPostTopics(postId1 + 1));
    EXPECT_FALSE(i.hasPostTopics(postId2));
    EXPECT_TRUE(i.hasTopicItem("href"));
    EXPECT_TRUE(i.hasTopicItem("cv"));
    EXPECT_TRUE(i.hasTopicItem("date"));

    EXPECT_EQ(2, i.getTopicItem("cv").postsCount);

    EXPECT_TRUE(i.index(TTopics({"href", "cv"}), username2, postId2));
    EXPECT_TRUE(i.index(TTopics({"cv"}), username2, postId2 + 1));

    EXPECT_TRUE(i.hasPostTopics(postId2));
    EXPECT_TRUE(i.hasPostTopics(postId2 + 1));
    EXPECT_EQ(4, i.getTopicItem("cv").postsCount);
    EXPECT_EQ(2, i.getTopicItem("href").postsCount);

    EXPECT_TRUE(i.purgeByUser(username1));

    EXPECT_FALSE(i.hasPostTopics(postId1));
    EXPECT_FALSE(i.hasPostTopics(postId1 + 1));

    EXPECT_TRUE(i.hasPostTopics(postId2));
    EXPECT_TRUE(i.hasPostTopics(postId2 + 1));
    EXPECT_EQ(2, i.getTopicItem("cv").postsCount);
    EXPECT_EQ(1, i.getTopicItem("href").postsCount);
    EXPECT_FALSE(i.hasTopicItem("date"));

    EXPECT_TRUE(i.purgeByUser(username2));

    EXPECT_FALSE(i.hasPostTopics(postId2));
    EXPECT_FALSE(i.hasPostTopics(postId2 + 1));
    EXPECT_FALSE(i.hasTopicItem("cv"));
    EXPECT_FALSE(i.hasTopicItem("href"));
}

TEST(IndexerTest, testSizes) {
    Indexer i;

    EXPECT_TRUE(i.index({"Y",}, "val", 1));
    EXPECT_EQ(1, i.getTopicItemsSize());
    EXPECT_EQ(1, i.getPostTopicsSize());
    EXPECT_EQ(1, i.getUserTopicsSize());

    EXPECT_TRUE(i.index({"m"}, "val", 1));
    EXPECT_EQ(2, i.getTopicItemsSize());
    EXPECT_EQ(1, i.getPostTopicsSize());
    EXPECT_EQ(1, i.getUserTopicsSize());

    EXPECT_TRUE(i.index({"d"}, "val", 2));
    EXPECT_EQ(3, i.getTopicItemsSize());
    EXPECT_EQ(2, i.getPostTopicsSize());
    EXPECT_EQ(1, i.getUserTopicsSize());

    EXPECT_TRUE(i.index({"find"}, "kim", 3));
    EXPECT_EQ(4, i.getTopicItemsSize());
    EXPECT_EQ(3, i.getPostTopicsSize());
    EXPECT_EQ(2, i.getUserTopicsSize());

    EXPECT_TRUE(i.index({"Y"}, "kim", 3));
    EXPECT_EQ(4, i.getTopicItemsSize());
    EXPECT_EQ(3, i.getPostTopicsSize());
    EXPECT_EQ(2, i.getUserTopicsSize());

    EXPECT_TRUE(i.purgeByUser("val"));

    EXPECT_EQ(2, i.getTopicItemsSize());
    EXPECT_EQ(1, i.getPostTopicsSize());
    EXPECT_EQ(1, i.getUserTopicsSize());

    EXPECT_TRUE(i.purgeByUser("kim"));

    EXPECT_EQ(0, i.getTopicItemsSize());
    EXPECT_EQ(0, i.getPostTopicsSize());
    EXPECT_EQ(0, i.getUserTopicsSize());

    EXPECT_TRUE(i.index({"find", "admin", "page"}, "kim", 4));
    EXPECT_EQ(3, i.getTopicItemsSize());
    EXPECT_EQ(1, i.getPostTopicsSize());
    EXPECT_EQ(1, i.getUserTopicsSize());

}
