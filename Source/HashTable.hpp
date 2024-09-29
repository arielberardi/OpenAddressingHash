#include <limits>
#include <utility>
#include <vector>

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

    struct Iterator
    {
        using NodeIterator = typename std::vector<Node>::iterator;

        // Alias with compatible name to the STL, won't follo my code conve
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Node;
        using pointer = value_type*;
        using reference = value_type&;

        Iterator(NodeIterator ptr, NodeIterator end) : m_current(ptr), m_end(end)
        {
            findNextValidIterator();
        }

        reference operator*() const
        {
            return *m_current;
        }

        pointer operator->() const
        {
            return &(*m_current);
        }

        Iterator& operator++()
        {
            ++m_current;
            findNextValidIterator();
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other)
        {
            return m_current == other.m_current;
        }

        bool operator!=(const Iterator& other)
        {
            return !(*this == other);
        }

      private:
        NodeIterator m_current;
        NodeIterator m_end;

        void findNextValidIterator()
        {
            while (m_current != m_end && !m_current->isUsed)
            {
                ++m_current;
            }
        }
    };

    HashTable(size_t initialSize = HashTable::kDefaultCapacity,
              float loadFactor = HashTable::kDefaultLoadFactor)
        : m_initialSize{initialSize}, m_table{initialSize}, m_elementsCount{0},
          m_loadFactor{loadFactor}
    {
    }

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
        // TODO: Resolve collisions
        m_table[hashValue] = Node{.key{key}, .value{value}, .isUsed{true}};

        m_elementsCount++;
    }

    void erase(const KeyType& key)
    {
        size_t hashValue = hash(key);

        Node& node = m_table[hashValue];
        if (!node.isUsed)
        {
            return;
        }

        node.isUsed = false;
        node.key = KeyType{};
        node.value = ValueType{};

        m_elementsCount--;
    }

    [[nodiscard]] constexpr bool exists(const KeyType& key) const noexcept
    {
        size_t hashValue = hash(key);
        Node node = m_table[hashValue];

        return node.isUsed;
    }

    [[nodiscard]] constexpr ValueType at(const KeyType& key) const noexcept
    {
        size_t hashValue = hash(key);

        Node node = m_table[hashValue];
        if (!node.isUsed)
        {
            return ValueType{};
        }

        return node.value;
    }

    ValueType& operator[](const KeyType& key)
    {
        size_t hashValue = hash(key);

        if (!m_table[hashValue].isUsed)
        {
            insert(key, ValueType{});
        }

        return m_table[hashValue].value;
    }

    void clear()
    {
        m_table.clear();
    }

    std::vector<KeyType> keys() const noexcept
    {
        std::vector<KeyType> keys{};
        keys.reserve(m_elementsCount);

        for (Node k : m_table)
        {
            if (!k.isUsed)
            {
                continue;
            }

            keys.push_back(k.key);
        }

        return keys;
    }

    std::vector<ValueType> values() const noexcept
    {
        std::vector<ValueType> values{};
        values.reserve(m_elementsCount);

        for (Node k : m_table)
        {
            if (!k.isUsed)
            {
                continue;
            }

            values.push_back(k.value);
        }

        return values;
    }

    Iterator begin()
    {
        return Iterator(m_table.begin(), m_table.end());
    }

    Iterator end()
    {
        return Iterator(m_table.end(), m_table.end());
    }

  private:
    static constexpr size_t kDefaultCapacity = 16;
    static constexpr float kDefaultLoadFactor = 0.5f;

    std::vector<Node> m_table;

    size_t m_initialSize;
    size_t m_elementsCount;
    float m_loadFactor;

    size_t hash(const KeyType& key) const noexcept
    {
        return std::hash<KeyType>{}(key) % m_table.size();
    }

    void resize()
    {
        const size_t maxSize = std::numeric_limits<size_t>::max() - 1;
        const size_t currentSize = m_table.size();
        if (currentSize == maxSize)
        {
            return;
        }

        size_t newSize = currentSize * 2;
        if (currentSize >= maxSize / 2)
        {
            newSize = maxSize;
        }

        std::vector<Node> newTable{newSize};

        for (const Node& node : m_table)
        {
            if (node.isUsed)
            {
                size_t newHash = std::hash<KeyType>{}(node.key) % newSize;
                // TODO: Resolve collisions
                newTable[newHash] = node;
            }
        }

        m_table = std::move(newTable);
    }
};
