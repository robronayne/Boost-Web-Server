#ifndef ENTITY_HEADER_H
#define ENTITY_HEADER_H

#include <string>

/*
An entity is identified by a numeric id and a type
For example, in /api/Shoes/1, the type is /api/Shoes and the numeric id is 1
The numeric id is generated and maintained by the server
*/
struct ent_id
{

  unsigned int id_;
  std::string type;

  // Need to define comparison operator so that struct can be hashed
  bool operator==(const ent_id &right) const
  {
    return (id_ == right.id_ && type == right.type);
  }

};

// Hashing class for ent_ids
// Needed so that ent_id can be used as a key type in unordered_map<ent_id, ...>
struct ent_id_hasher
{

  std::size_t operator()(const ent_id& k) const
  {
    using std::size_t;
    using std::hash;
    using std::string;

	// hash function borrowed from stack overflow
    return ((hash<int>()(k.id_) ^
             hash<string>()(k.type) << 1) >> 1);
  }
};

struct entity
{
	unsigned int id;
	std::string data;
	std::string path;
	std::string type;

	std::string get_data() const {
		return data;
	}
};

#endif