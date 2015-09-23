/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @package Twitcher
 */

#include <kernel/private/Twitcher.h>
#include <gtest/gtest.h>
#include <api/exceptions.h>

using namespace std;
using namespace twitcher;

TEST(TwitcherTest, testAddUser) {
    Storage s;
    Indexer i;
    Twitcher c(s,i);
    EXPECT_NO_THROW(c.addUser("name"));
    ASSERT_THROW(c.addUser("name"), UserExistsException);
}

TEST(TwitcherTest, testAddPost) {
    Storage s;
    Indexer i;
    Twitcher c(s,i);
    ASSERT_THROW(c.addPost("john", "error", 1), UserNotFoundException);
    c.addUser("john");
    EXPECT_NO_THROW(c.addPost("john", "just #chilling today", 1));
    EXPECT_NO_THROW(c.addPost("john", "eating #steak for dinner", 2));
    EXPECT_NO_THROW(c.addPost("john", "ugh! this #steak tasted like dog food", 3));

    auto posts = c.getPostsForUser("john");
    EXPECT_EQ(3, posts.size());
    EXPECT_EQ("ugh! this #steak tasted like dog food", posts[0]);
    EXPECT_EQ("eating #steak for dinner", posts[1]);
    EXPECT_EQ("just #chilling today", posts[2]);
}

TEST(TwitcherTest, testDeleteUserExceptions) {
    Storage s;
    Indexer i;
    Twitcher c(s,i);
    EXPECT_NO_THROW(c.addUser("john"));
    c.deleteUser("john");
    EXPECT_NO_THROW(c.addUser("john"));
    ASSERT_THROW(c.addUser("john"), UserExistsException);
    c.deleteUser("john");
    c.deleteUser("john");
    ASSERT_THROW(c.addPost("john", "just #chilling today", 1), UserNotFoundException);
}

TEST(TwitcherTest, testGetPostsForTopic) {
    Storage s;
    Indexer i;
    Twitcher c(s,i);
    const vector<string> posts =
    {
        "just #chilling today",
        "eating #steak for dinner",
        "ugh! this #steak tasted like dog food",
        "no topics there",
        "Joking, #steak is good",
        "Solve the puzzle with date: %Y-%m-%d",
        "# do not think it is a # topic",
        "just open #admin page",

    };

    c.addUser("john");
    for (unsigned i = 0; i < posts.size(); ++i) {
        c.addPost("john", posts[i], i + 1);
    }

    {
        auto foundPosts = c.getPostsForTopic("steak");
        EXPECT_EQ(3, foundPosts.size());
        EXPECT_EQ(posts[4], foundPosts[0]);
        EXPECT_EQ(posts[2], foundPosts[1]);
        EXPECT_EQ(posts[1], foundPosts[2]);
    }

    {
        auto foundPosts = c.getPostsForTopic("chilling");
        EXPECT_EQ(1, foundPosts.size());
        EXPECT_EQ(posts[0], foundPosts[0]);
    }

    {
        auto foundPosts = c.getPostsForTopic("testing");
        EXPECT_EQ(0, foundPosts.size());
    }

    {
        auto foundPosts = c.getPostsForTopic("");
        EXPECT_EQ(0, foundPosts.size());
    }

    {
        auto foundPosts = c.getPostsForTopic("admin");
        EXPECT_EQ(1, foundPosts.size());
        EXPECT_EQ(posts[7], foundPosts[0]);
    }

}

TEST(TwitcherTest, testGetPostsForTopicMultiUsers) {
    Storage s;
    Indexer i;
    Twitcher c(s,i);
    const vector<string> posts1 =
    {
        "Still looking for #something",
        "No tags today",
        "Do not hesitate to want #something",
        "Yes tags #today, more #technology for cats.",
    };

    const vector<string> posts2 =
    {
        "Something is a good #job",
        "#something is waiting for you",
        "In #technology we trust.",
        "Deal with #something, start today.",
    };

    c.addUser("val");
    c.addPost("val", posts1[0], 10); // something
    c.addPost("val", posts1[1], 11);
    c.addUser("kim");
    c.addPost("kim", posts2[0], 15); // job
    c.addPost("val", posts1[2], 16); // something
    c.addPost("kim", posts2[1], 20); // something
    c.addPost("kim", posts2[2], 21); // technology
    c.addPost("val", posts1[3], 30); // today, technology
    c.addPost("kim", posts2[3], 40); // something

    {
        auto foundPosts = c.getPostsForTopic("something");
        EXPECT_EQ(4, foundPosts.size());
        EXPECT_EQ(posts2[3], foundPosts[0]);
        EXPECT_EQ(posts2[1], foundPosts[1]);
        EXPECT_EQ(posts1[2], foundPosts[2]);
        EXPECT_EQ(posts1[0], foundPosts[3]);
    }

    {
        auto foundPosts = c.getPostsForTopic("today");
        EXPECT_EQ(1, foundPosts.size());
        EXPECT_EQ(posts1[3], foundPosts[0]);
    }

    {
        auto foundPosts = c.getPostsForTopic("technology");
        EXPECT_EQ(2, foundPosts.size());
        EXPECT_EQ(posts1[3], foundPosts[0]);
        EXPECT_EQ(posts2[2], foundPosts[1]);

    }

    {
        auto foundPosts = c.getPostsForTopic("job");
        EXPECT_EQ(1, foundPosts.size());
        EXPECT_EQ(posts2[0], foundPosts[0]);
    }

}

TEST(TwitcherTest, testGetTrendingTopics) {
    Storage s;
    Indexer i;
    Twitcher c(s,i);
    c.addUser("john");
    c.addPost("john", "#a", 1);
    c.addPost("john", "#z #d", 2);
    c.addPost("john", "#d", 3);
    c.addPost("john", "#z #k", 5);
    c.addPost("john", "#k #z", 10);

    {
        auto topics = c.getTrendingTopics(1, 3);
        EXPECT_EQ(3, topics.size());
        EXPECT_EQ("d", topics[0]);
        EXPECT_EQ("a", topics[1]);
        EXPECT_EQ("z", topics[2]);
    }
    {
        auto topics = c.getTrendingTopics(2, 3);
        EXPECT_EQ(2, topics.size());
        EXPECT_EQ("d", topics[0]);
        EXPECT_EQ("z", topics[1]);
    }
    {
        auto topics = c.getTrendingTopics(2, 10);
        EXPECT_EQ(3, topics.size());
        EXPECT_EQ("z", topics[0]);
        EXPECT_EQ("d", topics[1]);
        EXPECT_EQ("k", topics[2]);
    }

    {
        auto topics = c.getTrendingTopics(1, 10);
        EXPECT_EQ(4, topics.size());
        EXPECT_EQ("z", topics[0]);
        EXPECT_EQ("d", topics[1]);
        EXPECT_EQ("k", topics[2]);
        EXPECT_EQ("a", topics[3]);
    }

}

TEST(TwitcherTest, testDeleteUser) {
    Storage s;
    Indexer i;
    Twitcher c(s,i);
    c.addUser("john");
    c.addPost("john", "#a", 1);
    c.addPost("john", "#b #a", 2);
    c.addPost("john", "#c", 3);
    c.addPost("john", "d", 5);
    c.addPost("john", "f", 10);

    c.addUser("kim");
    c.addPost("kim", "#c", 4);
    {
        EXPECT_EQ(3, c.getTrendingTopics(1, 10).size());
        EXPECT_EQ(5, c.getPostsForUser("john").size());
        EXPECT_EQ(1, c.getPostsForUser("kim").size());
        EXPECT_EQ(2, c.getPostsForTopic("a").size());
        EXPECT_EQ(2, c.getPostsForTopic("c").size());
    }
    c.deleteUser("john");
    {
        EXPECT_EQ(1, c.getTrendingTopics(1, 10).size());
        ASSERT_THROW(c.getPostsForUser("john"), UserNotFoundException);
        EXPECT_EQ(1, c.getPostsForUser("kim").size());
        EXPECT_EQ(0, c.getPostsForTopic("a").size());
        EXPECT_EQ(1, c.getPostsForTopic("c").size());
    }
}

TEST(TwitcherTest, testSize) {
    Storage s;
    Indexer i;
    Twitcher c(s,i);
    c.addUser("name");
    c.addPost("name", std::string(200, '.'), 1);
    EXPECT_EQ(1, c.getPostsForUser("name").size());
    EXPECT_EQ(140, c.getPostsForUser("name")[0].size());
}
