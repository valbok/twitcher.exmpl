/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @package Twitcher
 */

#include <kernel/private/Storage.h>
#include <gtest/gtest.h>

using namespace std;
using namespace twitcher;

TEST(StorageTest, testStoreUser) {
    Storage storage;
    User user("name");
    EXPECT_FALSE(storage.userExists("name"));
    storage.setUser(user);
    EXPECT_TRUE(storage.userExists("name"));
    IUser& f = storage.getUser("name");
    EXPECT_EQ("name", f.getName());
}

TEST(StorageTest, testDeleteUser) {
    Storage storage;
    User user("name");
    storage.setUser(user);
    EXPECT_TRUE(storage.userExists("name"));
    storage.deleteUser("name");
    EXPECT_FALSE(storage.userExists("name"));
}

