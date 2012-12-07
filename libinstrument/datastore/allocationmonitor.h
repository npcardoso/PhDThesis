#pragma once

#include "../types.h"

#include <map>
#include <boost/shared_ptr.hpp>

using namespace std;

template <class StoredType, class OwnerType>
class AllocationMonitor {
  class Item {
  public:
    boost::shared_ptr<StoredType> obs;
    size_t size;
    OwnerType owner;

    inline Item(boost::shared_ptr<StoredType> obs,
                size_t size,
                const OwnerType & owner){
      this->obs = obs;
      this->size = size;
      this->owner = owner;
    }
  };

  typedef map<alloc_id_t, Item> storage_t;

  size_t max_bytes;
  size_t occupancy;
  alloc_id_t uid;

  storage_t storage;

public:

  inline AllocationMonitor(size_t max_bytes) {
    this->max_bytes = max_bytes;
    this->occupancy = 0;
    this->uid = 0;
  }

  inline bool canStore(size_t size) const {
    return max_bytes >= size + occupancy;
  }

  inline size_t getOccupancy() const {
    return occupancy;
  }

  inline size_t getSize() const {
    return max_bytes;
  }

  inline void softResize(size_t size) {
    max_bytes = size;
  }

  inline void resize(size_t size) {
    assert(occupancy <= size);

    if(occupancy <= size)
      max_bytes = size;
  }

  inline alloc_id_t store(boost::shared_ptr<StoredType> obs, size_t size, const OwnerType & owner) {
    assert(canStore(size));

    observation_id_t id = ++uid;
    occupancy += size;
    storage.insert(typename storage_t::value_type(id, Item(obs, size, owner)));

    return id;
  }

  inline OwnerType * free(observation_id_t observation_id) {
    typename storage_t::iterator it = storage.find(observation_id);

    assert(it != storage.end());

    OwnerType * ret = it->second.owner;
    storage.erase(it);
    return ret;
  }

};
