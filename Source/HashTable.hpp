#include <limits>
#include <ranges>
#include <utility>
#include <vector>

template <typename NodeType, typename NodeIteratorType>
class NodeIterator
{
  public:
    // Alias using names compatible with STL algorithms
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = NodeType;
    using pointer = value_type*;
    using reference = value_type&;

    NodeIterator(NodeIteratorType ptr, NodeIteratorType end) : m_current(ptr), m_end(end)
    {
        findNextValidNode();
    }

    reference operator*() const
    {
        return *m_current;
    }

    pointer operator->() const
    {
        return &(*m_current);
    }

    NodeIterator& operator++()
    {
        ++m_current;
        findNextValidNode();
        return *this;
    }

    NodeIterator operator++(int)
    {
        NodeIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const NodeIterator& other)
    {
        return m_current == other.m_current;
    }

    bool operator!=(const NodeIterator& other)
    {
        return !(*this == other);
    }

  private:
    NodeIteratorType m_current;
    NodeIteratorType m_end;

    void findNextValidNode()
    {
        while (m_current != m_end && !m_current->isUsed)
        {
            ++m_current;
        }
    }
};

template <typename KeyType, typename ValueType>
class HashTable
{
  public:
    struct Node
    {
        KeyType key{};
        ValueType value{};
        bool isUsed{false};
    };

    using Iterator = NodeIterator<Node, typename std::vector<Node>::iterator>;
    using ConstIterator = NodeIterator<const Node, typename std::vector<Node>::const_iterator>;

    HashTable(size_t initialSize = kDefaultCapacity, float loadFactor = kDefaultLoadFactor)
        : m_table{initialSize}, m_loadFactor{loadFactor}
    {
        // std::cout << "" << std::endl;
    }

    HashTable(std::initializer_list<std::pair<KeyType, ValueType>> nodes)
        : m_table{nodes.size()}, m_loadFactor{kDefaultLoadFactor}
    {
        // std::cout << "Constructor Initializer List!" << std::endl;
        for (const auto& n : nodes)
        {
            insert(n.first, n.second);
        }
    }

    /*
    HashTable(HashTable& other) : m_table{other.m_table}, m_loadFactor{other.m_loadFactor}
    {
        std::cout << "Copy constructor!" << std::endl;
    }

    HashTable& operator=(HashTable& other)
    {
        std::cout << "Copy Assigment!" << std::endl;

        m_table.clear();

        m_table = other.m_table;
        m_loadFactor = other.m_loadFactor;

        other.m_table.clear();
        other.m_loadFactor = 0;

        return *this;
    }

    HashTable(HashTable&& other)
        : m_table{std::move(other.m_table)}, m_loadFactor{other.m_loadFactor}
    {
        std::cout << "Move constructor!" << std::endl;
    }

    HashTable& operator=(HashTable&& other)
    {
        std::cout << "Move Assigment!" << std::endl;

        m_table.clear();

        m_table = std::move(other.m_table);
        m_loadFactor = other.m_loadFactor;

        other.m_table.clear();
        other.m_loadFactor = 0;

        return *this;
    }
    */

    [[nodiscard]] constexpr bool isEmpty() const noexcept
    {
        return m_elementsCount == 0;
    }

    [[nodiscard]] constexpr size_t count() const noexcept
    {
        return m_elementsCount;
    }

    [[nodiscard]] constexpr size_t size() const noexcept
    {
        return m_table.size();
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        if (m_elementsCount >= m_table.size() * m_loadFactor)
        {
            resize();
        }

        size_t hashValue = hash(key);

        // Re-hash until find a free slot
        while (m_table[hashValue].isUsed)
        {
            hashValue = (hashValue + 1) % m_table.size();
        }

        m_table[hashValue] = std::move(Node(key, value, true));

        m_elementsCount++;
    }

    void erase(const KeyType& key)
    {
        size_t hashValue = hash(key);

        if (!m_table[hashValue].isUsed)
        {
            return;
        }

        // We re-hash the value until we find the key
        while (m_table[hashValue].key != key)
        {
            hashValue = (hashValue + 1) % m_table.size();
        }

        Node& node = m_table[hashValue];
        node.isUsed = false;
        node.key = KeyType{};
        node.value = ValueType{};

        m_elementsCount--;
    }

    [[nodiscard]] constexpr bool exists(const KeyType& key) const noexcept
    {
        size_t hashValue = hash(key);

        if (!m_table[hashValue].isUsed)
        {
            return false;
        }

        // We re-hash the value until we find the correct key
        while (m_table[hashValue].key != key)
        {
            hashValue = (hashValue + 1) % m_table.size();
        }

        return m_table[hashValue].isUsed;
    }

    [[nodiscard]] constexpr ValueType at(const KeyType& key) const noexcept
    {
        size_t hashValue = hash(key);

        if (!m_table[hashValue].isUsed)
        {
            return ValueType{};
        }

        // We re-hash the value until we find the correct key
        while (m_table[hashValue].key != key)
        {
            hashValue = (hashValue + 1) % m_table.size();
        }

        return m_table[hashValue].value;
    }

    ValueType& operator[](const KeyType& key)
    {
        size_t hashValue = hash(key);

        if (!m_table[hashValue].isUsed)
        {
            insert(key, ValueType{});
        }

        // We re-hash the value until we find the correct key
        while (m_table[hashValue].key != key)
        {
            hashValue = (hashValue + 1) % m_table.size();
        }

        return m_table[hashValue].value;
    }

    void clear()
    {
        m_table.clear();
    }

    [[nodiscard]] std::vector<KeyType> keys() const noexcept
    {
        std::vector<KeyType> keys{};
        keys.reserve(m_elementsCount);

        // for (const Node& node : m_table)
        // {
        //     if (!node.isUsed)
        //     {
        //         continue;
        //     }

        //     keys.emplace_back(node.key);
        // }

        auto usedNode = [](const Node& n) { return n.isUsed; };
        auto getKey = [](const Node& n) { return n.key; };

        std::ranges::copy(m_table | std::views::filter(usedNode) | std::views::transform(getKey),
                          std::back_inserter(keys));

        return std::move(keys);
    }

    [[nodiscard]] std::vector<ValueType> values() const noexcept
    {
        std::vector<ValueType> values{};
        values.reserve(m_elementsCount);

        // for (const Node& node : m_table)
        // {
        //     if (!node.isUsed)
        //     {
        //         continue;
        //     }

        //     values.emplace_back(node.value);
        // }

        auto usedNode = [](const Node& n) { return n.isUsed; };
        auto getValue = [](const Node& n) { return n.value; };

        std::ranges::copy(m_table | std::views::filter(usedNode) | std::views::transform(getValue),
                          std::back_inserter(values));

        return std::move(values);
    }

    [[nodiscard]] Iterator begin()
    {
        return Iterator(m_table.begin(), m_table.end());
    }

    [[nodiscard]] Iterator end()
    {
        return Iterator(m_table.end(), m_table.end());
    }

    [[nodiscard]] ConstIterator cbegin() const
    {
        return ConstIterator(m_table.begin(), m_table.end());
    }

    [[nodiscard]] ConstIterator cend() const
    {
        return ConstIterator(m_table.end(), m_table.end());
    }

  private:
    static constexpr size_t kDefaultCapacity = 16;
    static constexpr float kDefaultLoadFactor = 0.5f;

    std::vector<Node> m_table;
    size_t m_elementsCount{0};
    float m_loadFactor;

    size_t hash(const KeyType& key) const noexcept
    {
        return std::hash<KeyType>{}(key) % m_table.size();
    }

    void resize()
    {
        const size_t maxSize = std::numeric_limits<size_t>::max() - 1;
        const size_t currentSize = m_table.size();

        if (currentSize >= maxSize)
        {
            return;
        }

        size_t newSize = currentSize * 2;
        if (currentSize >= maxSize / 2)
        {
            newSize = maxSize;
        }

        std::vector<Node> newTable;
        newTable.resize(newSize);

        auto insertInNewTable = [&](const Node& node)
        {
            size_t hashValue = std::hash<KeyType>{}(node.key) % newSize;

            // We re-hash until we find a free slot
            while (newTable[hashValue].isUsed)
            {
                hashValue = (hashValue + 1) % newSize;
            }

            newTable[hashValue] = node;
        };

        std::ranges::for_each(m_table | std::views::filter([](const Node& n) { return n.isUsed; }),
                              insertInNewTable);

        // for (const Node& node : m_table)
        // {
        //     if (node.isUsed)
        //     {
        //         size_t hashValue = std::hash<KeyType>{}(node.key) % newSize;

        //         // We re-hash until we find a free slot
        //         while (newTable[hashValue].isUsed)
        //         {
        //             hashValue = (hashValue + 1) % newSize;
        //         }

        //         newTable[hashValue] = node;
        //     }
        // }

        m_table = std::move(newTable);
    }
};
