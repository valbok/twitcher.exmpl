Twitcher
======
Twitcher - a micromessage communications platform.

How to build
************
    
Build:

    $ cmake .; make

Tests:

    $ cd kernel/private/tests/; ./TwitcherTest; ./IndexerTest; ./StorageTest; ./UserTest


NOTES
=====

The main api/Twitcher.h api class has been implemented in kernel/private/Twitcher.h

Tried to split out the task to few logical independent layers, where the highest belongs to Twitcher and it keeps data/handles requests/throws exceptions/etc.

Model layer provides user's behaviour: kernel/api/IUser.h. 
User objects contains all its posts. So now it is very easily, efficiently and logically to get posts by users. Model does not know anything about how to store data, where can it be fetched from, how to extract search keys etc.

Introduced storage layer to isolate and provide an ability to store data of users and exact posts: kernel/api/IStorage.h. Users are kept in this layer. Storage layer does not know anything about topics, how to extract them. But must know how to fetch user data from storage, bind data to user objects and store data back.

And index layer to grab keys/topics, store it internally and provide an access to efficient lookup: kernel/api/IIndex.h. All topics are stored there. Index does not know anything about users, where posts located, post texts, where users are and how to reach them. To combine functionality together is responsibility of highest layer Twitcher.

Splitting to layers and using dependency injections provides an ability to change/extend behavior without modifications. Extending storage implementation allows to change logic how to storage and fetch data. Extending indexer will allow to change a placement of storage of keys and different ways to access to.

Please, check out tests: kernel/private/tests

Remarks
*******

* Based on spec about timestamps, which unique and increasing monotonically, decided to work with them as post ids. Implemented a way to keep one post per one timestamp. In case if there is a need to use multiple posts per one timestamp, this will require small changes. But decided to keep timestamp as ids for simplicity.

* Sorting made by std::map and std::set.

* Since std::regex does not work for versions < GCC 4.9.0, decided to implement extracting topics manually.

* Binded gtest for testing.
