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
            _data[i] = nullptr; // Инициализируем все указатели как nullptr
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
                    index = (index + 1) % _size; // Продвигаемся к следующему слоту
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
            // Очищаем текущее состояние таблицы
            for (size_t i = 0; i < _size; i++) {
                delete _data[i];
            }
            _size = other._size;
            _data.resize(_size);
            for (size_t i = 0; i < _size; ++i) {
                if (other._data[i] && other._data[i]->_occupied) {
                    // Копируем элементы из другой таблицы
                    _data[i] = new Pair(other._data[i]->_key, other._data[i]->_value);
                    _data[i]->_occupied = true;
                }
                else {
                    _data[i] = nullptr; // Если слот в другой таблице пустой, оставляем его пустым и здесь
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
            index = (index + 1) % _size; // Продвигаемся к следующему слоту
        }
        _data[index] = new Pair(key, value);
        _data[index]->_occupied = true;
    }
    void insert_or_assign(const K& key, const V& value) {
        size_t index = hash(key);
        Pair* current = _data[index];
        while (current) {
            if (current->_key == key) {
                current->_value = value; // Обновляем значение, если ключ уже существует
                return;
            }
            index = (index + 1) % _size; // Продвигаемся к следующему слоту
            current = _data[index];
        }
        // Если мы дошли сюда, значит элемент с ключом key отсутствует
        // Вставляем новую пару ключ-значение
        insert(key, value);
    }
    bool erase(const K& key) {
        size_t index = hash(key);
        while (_data[index]) {
            if (_data[index]->_key == key) {
                _data[index]->_occupied = false; // Помечаем слот как удаленный
                return true;
            }
            index = (index + 1) % _size; // Продвигаемся к следующему слоту
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
            index = (index + 1) % _size; // Продвигаемся к следующему слоту
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