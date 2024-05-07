#ifndef __LRU__
#define __LRU__
#include <cassert>
#include <iostream>
#include <list>
#include <unordered_map>
#include <utility>

using namespace std;

template <typename Key, typename T> class LRU {
  public:
    LRU(size_t capacity) : capacity_(capacity) {}
    T get(Key);
    void put(Key, T);

  private:
    void move_to_head(typename list<pair<Key, T>>::iterator it);
    void add_to_head(pair<Key, T> &);
    void remove_elem(typename list<pair<Key, T>>::iterator it);
    Key remove_tail();
    size_t size_{0};
    size_t capacity_{0};
    list<pair<Key, T>> list_;
    unordered_map<Key, typename list<pair<Key, T>>::iterator> map_;
};

template <typename Key, typename T> T LRU<Key, T>::get(Key key) {
    if (map_.find(key) == map_.end()) {
        cout << "can't find " << key << " in cache." << endl;
        static T ret{};
        return ret;
    }

    auto &elem = map_[key];
    move_to_head(elem);
    return (*elem).second;
}

template <typename Key, typename T> void LRU<Key, T>::put(Key key, T value) {
    if (map_.find(key) == map_.end()) {
        list_.push_front({key, value});
        map_[key] = list_.begin();
        ++size_;
        if (size_ > capacity_) {
            auto removed = remove_tail();
            map_.erase(removed);
            --size_;
        }
    } else {
        move_to_head(map_[key]);
        (*map_[key]).second = value;
    }
}

template <typename Key, typename T>
void LRU<Key, T>::move_to_head(typename list<pair<Key, T>>::iterator it) {
    list_.splice(list_.begin(), list_, it);
}

template <typename Key, typename T>
void LRU<Key, T>::remove_elem(typename list<pair<Key, T>>::iterator it) {
    list_.erase(it);
}

template <typename Key, typename T> Key LRU<Key, T>::remove_tail() {
    auto key = (*prev(list_.end())).first;
    list_.erase(prev(list_.end()));
    return key;
}

// void testPutAndGet() {
//     LRU<int, int> cache(2); // 创建一个容量为2的缓存
//     cache.put(1, 100);
//     assert(cache.get(1) == 100); // 确认可以获取到刚放入的元素
//     std::cout << "Test put and get passed." << std::endl;
// }

// void testLRULogic() {
//     LRU<int, int> cache(2);
//     cache.put(1, 100);
//     cache.put(2, 200);
//     cache.put(3, 300);           // 此时应该淘汰键为1的元素
//     assert(cache.get(1) != 100); // 1 应该不在缓存中
//     assert(cache.get(2) == 200 && cache.get(3) == 300); // 2 和 3
//     应该在缓存中 std::cout << "Test LRU logic passed." << std::endl;
// }

// void testGetNonExistentKey() {
//     LRU<int, int> cache(2);
//     cache.put(1, 100);
//     // 假设对于不存在的键，get返回-1（需要修改LRU的get方法以支持此行为）
//     assert(cache.get(999) == 0); // 测试一个不存在的键
//     std::cout << "Test get non-existent key passed." << std::endl;
// }

// void test() {
//     testPutAndGet();
//     testLRULogic();
//     testGetNonExistentKey();
// }

#endif
