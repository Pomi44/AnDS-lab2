#include <iostream>
#include <cstdlib>
#include <ctime> 
#include <vector>
#include<list>
#include <stdexcept>
#include <string>

using namespace std;

template <typename K, typename V>
class HashTable {
    struct Pair {
        K _key;
        V _value;
        bool _occupied;
        Pair() : _occupied(false) {}
        Pair(K key, V value) : _key(key), _value(value), _occupied(true) {}
    };
    std::vector<Pair*> _data;
    size_t _size;
    size_t hash(K key) {
        return std::hash<K>()(key) % _size;
    };
public:
    HashTable(size_t size) {
        if (size == 0)
            throw std::invalid_argument("Size=0");
        _size = size;
        _data.resize(size);
        for (size_t i = 0; i < _size; ++i) {
            _data[i] = nullptr; // �������������� ��� ��������� ��� nullptr
        }
    }
    HashTable(size_t size, const V& max, const V& min) {
        if (size == 0)
            throw std::invalid_argument("Size=0");
        _size = size;
        _data.resize(size);
        for (int i = 0; i < size; ++i) {
            V value = rand() % (max - min + 1) + min;
            _data[i] = new Pair(i, value);
        }
    };

    HashTable(size_t size, const V& max, const V& min, size_t el_quant) {
        if (size == 0)
            throw std::invalid_argument("Size=0");
        _size = size;
        _data.resize(size);
        for (int i = 0; i < el_quant; ++i) {
            K key = rand();
            V value = rand() % (max - min + 1) + min;
            insert(key, value);
        }
    };

    HashTable(const HashTable& other) {
        _size = other._size;
        _data.resize(_size);
        for (size_t i = 0; i < _size; ++i) {
            if (other._data[i] && other._data[i]->_occupied) {
                size_t index = hash(other._data[i]->_key);
                while (_data[index]) {
                    index = (index + 1) % _size; // ������������ � ���������� �����
                }
                _data[index] = new Pair(other._data[i]->_key, other._data[i]->_value);
                _data[index]->_occupied = true;
            }
        }
    }
    ~HashTable() {
        for (size_t i = 0; i < _size; i++) {
            if (_data[i]) {
                delete _data[i];
            }
        }
        _size = 0;
    }
    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            // ������� ������� ��������� �������
            for (size_t i = 0; i < _size; i++) {
                delete _data[i];
            }
            _size = other._size;
            _data.resize(_size);
            for (size_t i = 0; i < _size; ++i) {
                if (other._data[i] && other._data[i]->_occupied) {
                    // �������� �������� �� ������ �������
                    _data[i] = new Pair(other._data[i]->_key, other._data[i]->_value);
                    _data[i]->_occupied = true;
                }
                else {
                    _data[i] = nullptr; // ���� ���� � ������ ������� ������, ��������� ��� ������ � �����
                }
            }
        }
        return *this;
    }


    size_t get_size() const {
        return _size;
    }
    vector<Pair*> get_data() const {
        return _data;
    }


    void print() {
        for (size_t i = 0; i < _size; ++i) {
            if (_data[i] && _data[i]->_occupied) {
                std::cout << _data[i]->_key << ": " << _data[i]->_value << std::endl;
            }
        }
    }
    void insert(const K& key, const V& value) {
        size_t index = hash(key);
        while (_data[index] && _data[index]->_occupied) {
            index = (index + 1) % _size; // ������������ � ���������� �����
        }
        _data[index] = new Pair(key, value);
        _data[index]->_occupied = true;
    }
    void insert_or_assign(const K& key, const V& value) {
        size_t index = hash(key);
        Pair* current = _data[index];
        while (current) {
            if (current->_key == key) {
                current->_value = value; // ��������� ��������, ���� ���� ��� ����������
                return;
            }
            index = (index + 1) % _size; // ������������ � ���������� �����
            current = _data[index];
        }
        // ���� �� ����� ����, ������ ������� � ������ key �����������
        // ��������� ����� ���� ����-��������
        insert(key, value);
    }
    bool erase(const K& key) {
        size_t index = hash(key);
        while (_data[index]) {
            if (_data[index]->_key == key) {
                _data[index]->_occupied = false; // �������� ���� ��� ���������
                return true;
            }
            index = (index + 1) % _size; // ������������ � ���������� �����
        }
        return false;
    }

    bool contains(const V& value) const {
        for (size_t i = 0; i < _size; ++i) {
            if (_data[i] && _data[i]->_value == value && _data[i]->_occupied) {
                return true;
            }
        }
        return false;
    }

    V* search(const K& key) {
        size_t index = hash(key);
        while (_data[index]) {
            if (_data[index]->_key == key && _data[index]->_occupied) {
                return &(_data[index]->_value);
            }
            index = (index + 1) % _size; // ������������ � ���������� �����
        }
        return nullptr;
    }
    int count(const K& key) {
        int count = 0;
        for (size_t i = 0; i < _size; ++i) {
            if (_data[i] && _data[i]->_key == key && _data[i]->_occupied) {
                ++count;
            }
        }
        return count;
    }
};
void countDuplicates(const std::vector<int>& array) {
    // ������� ���-������� ��� �������� ����� � �� ������
    HashTable<int, int> frequencyTable(100); // ������ ������� �������� �����������, ����� 100

    // ��������� �������
    for (int num : array) {
        // ���� ����� ��� ������������ � �������, ����������� ��� ������� �� 1
        int* frequency = frequencyTable.search(num);
        if (frequency) {
            (*frequency)++;
        }
        // ���� ����� ������� �����������, ��������� ��� � ������� �� ��������� ������� 1
        else {
            frequencyTable.insert(num, 1);
        }
    }

    // ������� ����������
    std::cout << "Frequencies of numbers in the array:" << std::endl;
    for (int num : array) {
        int* frequency = frequencyTable.search(num);
        if (frequency) {
            std::cout << num << ": " << *frequency << std::endl;
            // ������� ����� �� �������, ����� �������� ��������� �������
            frequencyTable.erase(num);
        }
    }
}

int main() {
    srand(time(nullptr)); // ������������� ���������� ��������� �����

    // ������� ���-������� � �������� 10
    HashTable<int, int> table(10);

    // ��������� ��������� ���������
    table.insert(1, 10);
    table.insert(2, 20);
    table.insert(3, 30);
    table.insert(11, 40); // ����� ��������

    // ������� ���������� �������
    std::cout << "HashTable after initial inserts:" << std::endl;
    table.print();
    std::cout << std::endl;

    // ��������� �������� ��������
    table.insert_or_assign(2, 25);

    // ������� ���������� ������� ����� ���������� ��������
    std::cout << "HashTable after update:" << std::endl;
    table.print();
    std::cout << std::endl;

    // ��������� ������� �������� � ������������ ���������
    int valueToCheck = 25;
    if (table.contains(valueToCheck)) {
        std::cout << "Value " << valueToCheck << " found in HashTable." << std::endl;
    }
    else {
        std::cout << "Value " << valueToCheck << " not found in HashTable." << std::endl;
    }
    std::cout << std::endl;

    // ������� ������� � ������ 2
    table.erase(2);

    // ������� ���������� ������� ����� �������� ��������
    std::cout << "HashTable after erase:" << std::endl;
    table.print();
    std::cout << std::endl;

    // ��������� ����� ��������
    int keyToSearch = 1;
    int* foundValue = table.search(keyToSearch);
    if (foundValue) {
        std::cout << "Value for key " << keyToSearch << " found: " << *foundValue << std::endl;
    }
    else {
        std::cout << "Value for key " << keyToSearch << " not found." << std::endl;
    }
    std::cout << std::endl;

    // ��������� ������� ���������� ��������� � ������������ ������
    int keyToCount = 11;
    int count = table.count(keyToCount);
    std::cout << "Number of elements with key " << keyToCount << ": " << count << std::endl << std::endl;

    // ���������� ��������� ������ �����
    std::vector<int> randomArray;
    for (int i = 0; i < 20; ++i) { // 20 - ������������ ������ �������
        randomArray.push_back(rand() % 10); // ���������� ����� �� 0 �� 9 ��� �������
    }

    // ������� ��������������� ������
    std::cout << "The generated array:" << std::endl;
    for (int num : randomArray) {
        std::cout << num << " ";
    }
    std::cout << std::endl << std::endl;

    // ������������ ���������� ���������� ����� � ������� � �������������� ���-�������
    countDuplicates(randomArray);

    return 0;
}