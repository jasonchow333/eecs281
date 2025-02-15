#ifndef HASHTABLE_H
#define HASHTABLE_H

// Project Identifier: 2C4A3C53CD5AD45A7BEA3AE5356A3B1622C9D04B

// INSTRUCTIONS:
// fill out the methods in the class below.

// You may assume that the key and value types can be copied and have default
// constructors.

// You can also assume that the key type has (==) and (!=) methods.

// You may assume that Hasher is a functor type with a
// size_t operator()(const Key&) overload.

// The key/value types aren't guaranteed to support any other operations.

// Do not add, remove, or change any of the class's member variables.
// The num_deleted counter is *optional*. You might find it helpful, but it
// is not required to pass the lab assignment.

// Do not change the Bucket type.

// SUBMISSION INSTRUCTIONS:
// Submit this file, by itself, in a .tar.gz.
// Other files will be ignored.

#include <cstdint>
#include <functional> // where std::hash lives
#include <vector>
#include <cassert> // useful for debugging!

// A bucket's status tells you whether it's empty, occupied, 
// or contains a deleted element.
enum class Status : uint8_t {
    Empty,
    Occupied,
    Deleted
};

template<typename K, typename V, typename Hasher = std::hash<K>>
class HashTable {
    // used by the autograder; do not change/remove.
    friend class Verifier;
public:
    // A bucket has a status, a key, and a value.
    struct Bucket {
        // Do not modify Bucket.
        Status status = Status::Empty;
        K key;
        V val;
    };

    HashTable() {
        // TODO: a default constructor (possibly empty).
        buckets.resize(20);

        // You can use the following to avoid implementing rehash_and_grow().
        // However, you will only pass the AG test cases ending in _C.
        // To pass the rest, start with at most 20 buckets and implement rehash_and_grow().
        //    buckets.resize(10000);
    }

    size_t size() const {
        return num_elements;
    }

    // returns a reference to the value in the bucket with the key, if it
    // already exists. Otherwise, insert it with a default value, and return
    // a reference to the resulting bucket.
    V& operator[](const K& key) {
        // TODO
        Hasher hasher;
        size_t target = hasher(key) % buckets.size();
        size_t collision = solve_collision(target, key); //find
        if (buckets[collision].status == Status::Occupied && buckets[collision].key == key){
            return buckets[collision].val;
        }
        else{
            size_t sz = buckets.size();
            insert(key, V());
            if (sz != buckets.size()){
                collision = solve_collision(hasher(key) % buckets.size(), key);
            }
            return buckets[collision].val;
        }
    }

    // insert returns whether inserted successfully
    // (if the key already exists in the table, do nothing and return false).
    bool insert(const K& key, const V& val) {
        // TODO
        Hasher hasher;
        size_t target = hasher(key) % buckets.size();
        size_t collision = solve_collision(target, key); // find
        if (buckets[collision].status == Status::Occupied && buckets[collision].key == key){
            return false;
        }
        else{
            buckets[collision].status = Status::Occupied;
            buckets[collision].key = key;
            buckets[collision].val = val;
            num_elements++;
            if (double(num_elements) / double(buckets.size()) > 0.5){
                rehash_and_grow();
            }
            return true;
        }
    }
    // erase returns the number of items remove (0 or 1)
    size_t erase(const K& key) {
        // TODO
        Hasher hasher;
        size_t target = hasher(key) % buckets.size();
        size_t collision = solve_collision(target, key); // find
        if (!(buckets[collision].status == Status::Occupied && buckets[collision].key == key)){
            return 0;
        }
        else{
            buckets[collision].status = Status::Deleted;
            num_elements--;
            return 1;
        }
        
    }

private:
    size_t num_elements = 0;
    size_t num_deleted = 0; // OPTIONAL: you don't need to use num_deleted to pass
    std::vector<Bucket> buckets;

    void rehash_and_grow() {
        // You can avoid implementing rehash_and_grow() by calling
        //    buckets.resize(10000);
        // in the constructor. However, you will only pass the AG test cases ending in _C.
        // To pass the rest, start with at most 20 buckets and implement rehash_and_grow().
        std::vector<Bucket> cp_bucket = buckets;
        size_t sz = buckets.size();
        buckets.clear();
        buckets.resize(sz * 2);
        num_elements = 0;
        for (auto i : cp_bucket){
            if (i.status == Status::Occupied){
                insert(i.key, i.val);
            }
        }
    }

    // You can add methods here if you like.
    // TODO
    // find key first, if not found, return the insert position
    size_t solve_collision(size_t col_pos, const K& key){
        size_t ins_pos = col_pos;
        bool found_ins = false;
        if (buckets[col_pos].status == Status::Empty){
            return col_pos;
        }
        else if (buckets[col_pos].status == Status::Occupied){
            if (buckets[col_pos].key == key){
                return col_pos;
            }
        }
        else{
            found_ins = true;
        }
        size_t check_pos = (col_pos + 1) % buckets.size();
        while (check_pos != col_pos){
            if (buckets[check_pos].status == Status::Empty){
                if (!found_ins){
                    return check_pos;
                }
                else{
                    return ins_pos;
                }
            }
            else if (buckets[check_pos].status == Status::Occupied){
                if (buckets[check_pos].key == key){
                    return check_pos;
                }
            }
            else if (buckets[check_pos].status == Status::Deleted){
                if (!found_ins){
                    ins_pos = check_pos;
                    found_ins = true;
                }
            }
            check_pos = (check_pos + 1) % buckets.size();
        }
        return ins_pos;
    }
};

#endif // HASHTABLE_H
