#include <algorithm>
#include <cassert>
#include <format>
#include <iostream>
#include <string>

#include "HashTable.hpp"

void test_emptyInsertGetCount()
{
    HashTable<std::string, int> phoneBook{};

    assert(phoneBook.isEmpty());

    phoneBook.insert("Mom", 1);
    phoneBook.insert("Dad", 2);
    phoneBook.insert("Son", 3);
    phoneBook.insert("Doughter", 4);

    assert(phoneBook["Mom"] == 1);
    assert(phoneBook.at("Doughter") == 4);
    assert(!phoneBook.isEmpty());
    assert(phoneBook.count() == 4);
}

void test_exists()
{
    HashTable<std::string, int> phoneBook{};

    phoneBook.insert("Mom", 1);
    assert(phoneBook.exists("Mom"));

    phoneBook.erase("Mom");
    assert(!phoneBook.exists("Mom"));
}

void test_getKeys()
{
    HashTable<int, std::string> phoneBook{};
    phoneBook.insert(0, "A");
    phoneBook.insert(1, "A");
    phoneBook.insert(3, "A");

    std::vector<int> keys = phoneBook.keys();

    assert(keys[0] == 0);
    assert(keys[1] == 1);
    assert(keys[2] == 3);
}

void test_getValues()
{
    HashTable<int, std::string> phoneBook{};
    phoneBook.insert(0, "A");
    phoneBook.insert(1, "B");
    phoneBook.insert(3, "C");

    std::vector<std::string> values = phoneBook.values();

    assert(values[0] == "A");
    assert(values[1] == "B");
    assert(values[2] == "C");
}

void test_resize()
{
    HashTable<int, int> phoneBook{};

    for (size_t i = 0; i < 100; i++)
    {
        phoneBook.insert(i, i);
    }

    assert(phoneBook.count() == 100);
}

void test_iterators()
{
    HashTable<std::string, int> phoneBook{};
    phoneBook.insert("Mom", 1);
    phoneBook.insert("Dad", 2);
    phoneBook.insert("Son", 3);
    phoneBook.insert("Doughter", 4);

    int count = 0;
    for (auto node : phoneBook)
    {
        count++;
    }
    assert(count == 4);

    auto it = std::find_if(phoneBook.begin(),
                           phoneBook.end(),
                           [](auto&& node) { return node.key.at(0) == 'S'; });

    assert(it != phoneBook.end());

    auto it2 = std::find_if(phoneBook.begin(),
                            phoneBook.end(),
                            [](auto&& node) { return node.key.at(0) == 'Z'; });

    assert(it2 == phoneBook.end());
}

void test_insertWithSubscriptOperator()
{
    HashTable<std::string, int> products{};
    products["Banana"] = 3;
    products["Watermelon"] = 1;

    assert(!products.isEmpty());
    assert(products["Banana"] == 3);
    assert(products["Watermelon"] == 1);
}

void test_initializationList()
{
    HashTable<std::string, int> products{{"Banana", 3}, {"Watermelon", 1}};

    assert(!products.isEmpty());
    assert(products["Banana"] == 3);
    assert(products["Watermelon"] == 1);
}

int main()
{
    test_emptyInsertGetCount();
    test_exists();
    test_getKeys();
    test_getValues();
    test_resize();
    test_iterators();
    test_insertWithSubscriptOperator();
    test_initializationList();

    std::cout << "If we are here, all test have passed!!!" << std::endl;
    return 0;
}
