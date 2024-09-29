#include <limits>
#include <utility>
#include <vector>

template <typename KeyType, typename ValueType>
class HashTable
{
  public:
    struct Node
    {
        KeyType Key{};
        ValueType Value{};
        bool IsOccupied{false};
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

        Iterator(NodeIterator ptr, NodeIterator end) : m_Current(ptr), m_End(end)
        {
            FindNextValidNode();
        }

        reference operator*() const
        {
            return *m_Current;
        }

        pointer operator->() const
        {
            return &(*m_Current);
        }

        Iterator& operator++()
        {
            ++m_Current;
            FindNextValidNode();
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
            return m_Current == other.m_Current;
        }

        bool operator!=(const Iterator& other)
        {
            return !(*this == other);
        }

      private:
        NodeIterator m_Current;
        NodeIterator m_End;

        void FindNextValidNode()
        {
            while (m_Current != m_End && !m_Current->IsOccupied)
            {
                ++m_Current;
            }
        }
    };

    HashTable(size_t initialSize = HashTable::DEFAULT_CAPACITY,
              float loadFactor = HashTable::DEFAULT_LOAD_FACTOR)
        : m_InitialSize{initialSize}, m_Table{initialSize}, m_ElementsCount{0},
          m_LoadFactor{loadFactor}
    {
    }

    [[nodiscard]] constexpr bool IsEmpty() const noexcept
    {
        return m_ElementsCount == 0;
    }

    [[nodiscard]] constexpr size_t Count() const noexcept
    {
        return m_ElementsCount;
    }

    [[nodiscard]] constexpr size_t Size() const noexcept
    {
        return m_Table.size();
    }

    void Insert(const KeyType& key, const ValueType& value)
    {
        if (m_ElementsCount >= m_Table.size() * m_LoadFactor)
        {
            Resize();
        }

        size_t hashValue = Hash(key);
        // TODO: Resolve collisions
        m_Table[hashValue] = Node{.Key{key}, .Value{value}, .IsOccupied{true}};

        m_ElementsCount++;
    }

    void Erase(const KeyType& key)
    {
        size_t hashValue = Hash(key);

        Node& node = m_Table[hashValue];
        if (!node.IsOccupied)
        {
            return;
        }

        node.IsOccupied = false;
        node.Key = KeyType{};
        node.Value = ValueType{};

        m_ElementsCount--;
    }

    [[nodiscard]] constexpr bool Exists(const KeyType& key) const noexcept
    {
        size_t hashValue = Hash(key);
        Node node = m_Table[hashValue];

        return node.IsOccupied;
    }

    [[nodiscard]] constexpr ValueType At(const KeyType& key) const noexcept
    {
        size_t hashValue = Hash(key);

        Node node = m_Table[hashValue];
        if (!node.IsOccupied)
        {
            return ValueType{};
        }

        return node.Value;
    }

    ValueType& operator[](const KeyType& key)
    {
        size_t hashValue = Hash(key);

        if (!m_Table[hashValue].IsOccupied)
        {
            Insert(key, ValueType{});
        }

        return m_Table[hashValue].Value;
    }

    void Clear()
    {
        m_Table.clear();
    }

    std::vector<KeyType> Keys() const noexcept
    {
        std::vector<KeyType> keys{};
        keys.reserve(m_ElementsCount);

        for (Node k : m_Table)
        {
            if (!k.IsOccupied)
            {
                continue;
            }

            keys.push_back(k.Key);
        }

        return keys;
    }

    std::vector<ValueType> Values() const noexcept
    {
        std::vector<ValueType> values{};
        values.reserve(m_ElementsCount);

        for (Node k : m_Table)
        {
            if (!k.IsOccupied)
            {
                continue;
            }

            values.push_back(k.Value);
        }

        return values;
    }

    Iterator begin()
    {
        return Iterator(m_Table.begin(), m_Table.end());
    }

    Iterator end()
    {
        return Iterator(m_Table.end(), m_Table.end());
    }

  private:
    static constexpr size_t DEFAULT_CAPACITY = 16;
    static constexpr float DEFAULT_LOAD_FACTOR = 0.5f;

    std::vector<Node> m_Table;

    size_t m_InitialSize;
    size_t m_ElementsCount;
    float m_LoadFactor;

    size_t Hash(const KeyType& key) const noexcept
    {
        return std::hash<KeyType>{}(key) % m_Table.size();
    }

    void Resize()
    {
        const size_t maxSize = std::numeric_limits<size_t>::max() - 1;
        const size_t currentSize = m_Table.size();
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

        for (const Node& node : m_Table)
        {
            if (node.IsOccupied)
            {
                size_t newHash = std::hash<KeyType>{}(node.Key) % newSize;
                // TODO: Resolve collisions
                newTable[newHash] = node;
            }
        }

        m_Table = std::move(newTable);
    }
};
