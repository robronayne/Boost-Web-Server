#ifndef ENTITY_MGR_HEADER_H
#define ENTITY_MGR_HEADER_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <set>
#include "entity.h"

/*
Entity manager class
Creating, removing, updating, deleting entities must be handled by the server
Implementing this functionality in a separate class allows us to simply
create an object of this class to manage all the entities.
*/

class entity_manager {

  public:
    entity_manager(std::string data_path);
    bool insert(entity& entity_);
    bool insert_with_id(entity& entity_);
    bool update(ent_id id, std::string data_);
    const entity* get(ent_id id);
    bool remove(ent_id id);
    bool exists(ent_id id);
    bool type_exists(std::string type_);
    std::string get_ids_by_type(std::string type_);
    void eraseFileLine(std::string eraseLine);

  private:

    /*
    Map for maintaining id's of different types
    for example, we may get requests like /api/Shoes and /api/Cars
    This will create two entries: [Shoes->1, Cars->1]
    If we get a subsequent request for POST /api/Shoes
    It will update to: [Shoes->2, Cars->1]
    */
    std::unordered_map<std::string, unsigned int> type_to_id;

    /*
    Maintains entities by id
    ent_id_hasher is specified as the class to be used for hashing
    useful for retrieving, inserting and updating entities
    */
    std::unordered_map<ent_id, entity, ent_id_hasher> id_map;

    /* Map for ID's by type
    Helpful in retrieving the IDs associated with a type
    For example, suppose we get 3x GET /api/Shoes
    The map will have [Shoes->{1, 2, 3}]
    But then we get DELETE /api/Shoes/2
    The map is [Shoes->{1,3}]
    So when GET /api/Shoes is received, we can return [1,3]
    */
    std::unordered_map<std::string, std::set<unsigned int>> type_map;

    // Private function to generate the next ID for a given type
    unsigned int generate_next_id(std::string type_);

    std::string data_path_;
    std::string temp_path_;

};

#endif