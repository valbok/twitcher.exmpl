/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @package Twitcher
 */

#include <kernel/private/User.h>
#include <gtest/gtest.h>

using namespace std;
using namespace twitcher;

TEST(UserTest, testName) {
    User user("name");

    EXPECT_EQ("name", user.getName());
}

TEST(UserTest, testAddPost) {
    User user("name");
    user.addPost("post 1", 1);
    user.addPost("post 0", 2);
    user.addPost("post 3", 4);
    user.addPost("post 2", 5);

    auto posts = user.getPosts();
    EXPECT_EQ(4, posts.size());
    auto it = posts.begin();
    EXPECT_EQ(5, it->first);
    EXPECT_EQ("post 2", it++->second);
    EXPECT_EQ(4, it++->first);
    EXPECT_EQ(2, it++->first);
    EXPECT_EQ(1, it->first);    
}

TEST(UserTest, testGetPost) {
    User user("name");
    user.addPost("post 1", 1);
    user.addPost("post 0", 2);
    EXPECT_TRUE(user.hasPost(1));
    EXPECT_FALSE(user.hasPost(3));
    EXPECT_EQ("post 1", user.getPost(1));
}