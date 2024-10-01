#include <algorithm>
#include <cassert>
#include <chrono>
#include <format>
#include <iostream>
#include <string>

#include "HashTable.hpp"

void test_emptyInsertGetCount()
{
    std::cout << "test_emptyInsertGetCount: Start" << std::endl;

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

    std::cout << "test_emptyInsertGetCount: End" << std::endl;
}

void test_exists()
{
    std::cout << "test_exists: Start" << std::endl;

    HashTable<std::string, int> phoneBook{};

    phoneBook.insert("Mom", 1);
    assert(phoneBook.exists("Mom"));

    phoneBook.erase("Mom");
    assert(!phoneBook.exists("Mom"));

    std::cout << "test_exists: End" << std::endl;
}

void test_getKeys()
{
    std::cout << "test_getKeys: Start" << std::endl;

    HashTable<int, std::string> phoneBook{};
    phoneBook.insert(0, "A");
    phoneBook.insert(1, "A");
    phoneBook.insert(3, "A");

    std::vector<int> keys = phoneBook.keys();

    assert(keys[0] == 0);
    assert(keys[1] == 1);
    assert(keys[2] == 3);

    std::cout << "test_getKeys: End" << std::endl;
}

void test_getValues()
{
    std::cout << "test_getValues: Start" << std::endl;

    HashTable<int, std::string> phoneBook{};
    phoneBook.insert(0, "A");
    phoneBook.insert(1, "B");
    phoneBook.insert(3, "C");

    std::vector<std::string> values = phoneBook.values();

    assert(values[0] == "A");
    assert(values[1] == "B");
    assert(values[2] == "C");

    std::cout << "test_getValues: End" << std::endl;
}

void test_resize()
{
    std::cout << "test_resize: Start" << std::endl;

    HashTable<int, int> phoneBook{};

    for (size_t i = 0; i < 1000; i++)
    {
        phoneBook.insert(i, i);
    }

    assert(phoneBook.count() == 1000);

    std::cout << "test_resize: End" << std::endl;
}

void test_iterators()
{
    std::cout << "test_iterators: Start" << std::endl;

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

    std::cout << "test_iterators: End" << std::endl;
}

void test_insertWithSubscriptOperator()
{
    std::cout << "test_insertWithSubscriptOperator: Start" << std::endl;

    HashTable<std::string, int> products{};
    products["Banana"] = 3;
    products["Watermelon"] = 1;

    assert(!products.isEmpty());
    assert(products["Banana"] == 3);
    assert(products["Watermelon"] == 1);

    std::cout << "test_insertWithSubscriptOperator: End" << std::endl;
}

void test_initializationList()
{
    std::cout << "test_initializationList: Start" << std::endl;

    HashTable<std::string, int> products{{"Banana", 3}, {"Watermelon", 1}};

    assert(!products.isEmpty());
    assert(products["Banana"] == 3);
    assert(products["Watermelon"] == 1);

    std::cout << "test_initializationList: End" << std::endl;
}

void test_copyConstructor()
{
    std::cout << "test_copyConstructor: Start" << std::endl;

    HashTable<std::string, int> products{{"Banana", 3}, {"Watermelon", 1}};
    HashTable<std::string, int> p{products};

    assert(p["Banana"] == 3);

    std::cout << "test_copyConstructor: End" << std::endl;
}

void test_copyAssigment()
{
    std::cout << "test_copyAssigment: Start" << std::endl;

    HashTable<std::string, int> products{{"Banana", 3}, {"Watermelon", 1}};
    HashTable<std::string, int> p{{"Apple", 2}, {"Apricot", 10}};
    p = products;

    assert(p["Banana"] == 3);
    assert(p["Watermelon"] == 1);

    std::cout << "test_copyAssigment: End" << std::endl;
}

void test_moveConstructor()
{
    std::cout << "test_moveConstructor: Start" << std::endl;

    HashTable<std::string, int> products{{"Banana", 3}, {"Watermelon", 1}};
    HashTable<std::string, int> p{std::move(products)};

    assert(p["Banana"] == 3);

    std::cout << "test_moveConstructor: End" << std::endl;
}

void test_moveAssigment()
{
    std::cout << "test_moveAssigment: Start" << std::endl;

    HashTable<std::string, int> products{{"Banana", 3}, {"Watermelon", 1}};
    HashTable<std::string, int> p{{"Apple", 2}, {"Apricot", 10}};
    p = std::move(products);

    assert(p["Banana"] == 3);
    assert(p["Watermelon"] == 1);

    std::cout << "test_moveAssigment: End" << std::endl;
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    test_emptyInsertGetCount();
    test_exists();
    test_getKeys();
    test_getValues();
    test_resize();
    test_iterators();
    test_insertWithSubscriptOperator();
    test_initializationList();

    test_copyConstructor();
    test_copyAssigment();
    test_moveConstructor();
    test_moveAssigment();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> delta = (end - start) * 1000;

    std::cout << "All tests have passed in " << delta.count() << "ms" << std::endl;
    return 0;
}
