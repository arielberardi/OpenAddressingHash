#include <algorithm>
#include <format>
#include <iostream>
#include <string>

#include "HashTable.hpp"

int main()
{
    HashTable<std::string, std::string> phoneBook{16};

    // Test Empty when it's empty
    if (!phoneBook.isEmpty())
    {
        std::cerr << "Hash Table should be empty" << std::endl;
    }

    // Test Insert
    phoneBook.insert("Mom", "07952998755");
    phoneBook.insert("Dad", "07952998234");
    phoneBook.insert("Son", "07953298755");
    phoneBook.insert("Doughter", "+44790932456");

    // Test Empty when it's not empty
    if (phoneBook.isEmpty())
    {
        std::cerr << "Hash Table should NOT be empty by now" << std::endl;
    }

    // Test Count
    if (phoneBook.count() != 4)
    {
        std::cerr << "Count method not working" << std::endl;
    }

    // Test Erase
    phoneBook.erase("Mom");

    if (phoneBook.count() != 3)
    {
        std::cerr << "Error, Erase method didn't work" << std::endl;
    }

    // Test Get Value
    std::cout << "Dad: " << phoneBook.at("Dad") << std::endl;
    std::cout << "Son: " << phoneBook["Son"] << std::endl;

    if (phoneBook.exists("Mom"))
    {
        std::cerr << "Mom element shouldn't exist" << std::endl;
    }

    // Test Range Iterator over Keys
    std::cout << "Keys: ";
    for (auto key : phoneBook.keys())
    {
        std::cout << key << ",";
    }
    std::cout << std::endl;

    // Test Range Iterator over Values
    std::cout << "Values: ";
    for (auto value : phoneBook.values())
    {
        std::cout << value << ",";
    }
    std::cout << std::endl;

    // Test Resizing of the Hash
    for (size_t i = 0; i < 1000; i++)
    {
        std::string key{std::format("SomeAlternative{}Text{}ToAvoid{}Collisions", i, i, i)};
        phoneBook.insert(key, std::to_string(i));
    }

    std::cout << "Dad: " << phoneBook.at("Dad") << std::endl;
    std::cout << "Son: " << phoneBook["Son"] << std::endl;
    std::cout << phoneBook.count() << std::endl;
    std::cout << phoneBook.size() << std::endl;

    // Test Iteation over the pairs
    std::cout << "Iterator begin:" << std::endl;
    auto begin = phoneBook.begin();
    std::cout << "(" << begin->key << "," << begin->value << ")" << std::endl;

    std::cout << "Range Loop:" << std::endl;
    for (auto node : phoneBook)
    {
        std::cout << "(" << node.key << "," << node.value << ")" << std::endl;
    }

    // Find Element that starts with an "S" using ranges
    auto it = std::find_if(phoneBook.begin(),
                           phoneBook.end(),
                           [](auto&& node) { return node.key.at(0) == 'S'; });

    if (it != std::end(phoneBook))
    {
        std::cout << "Element found: " << it->key << ", " << it->value << std::endl;
    }
    else
    {
        std::cerr << "Element not found." << std::endl;
    }

    auto it2 = std::find_if(phoneBook.begin(),
                            phoneBook.end(),
                            [](auto&& node) { return node.key.at(0) == 'Z'; });

    if (it2 != std::end(phoneBook))
    {
        std::cout << "Element found: " << it2->key << ", " << it2->value << std::endl;
    }
    else
    {
        std::cerr << "Element not found." << std::endl;
    }

    // Test Insert new elements using [] operator
    HashTable<std::string, int> products{};

    products["Banana"] = 3;
    products["Watermelon"] = 1;

    if (products.isEmpty())
    {
        std::cerr << "Hash Table should NOT be empty by now" << std::endl;
    }

    std::cout << products["Banana"] << std::endl;
    std::cout << products.at("Watermelon") << std::endl;

    // TODO: Initializer list for pairs
    /*
    // Insert new elements at the constructor
    HashTable<int, std::string> letters{{0, "A"}, {1, "B"}};

    if (letters.IsEmpty())
    {
        std::cerr << "Hash Table should NOT be empty by now" << std::endl;
    }

    std::cout << letters[0] << std::endl;
    std::cout << letters.at(1) << std::endl;
    */
}
