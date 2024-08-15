#include "entity_manager.h"
#include <sstream>
#include <boost/filesystem.hpp>

/*
Source: https://stackoverflow.com/questions/26576714/deleting-specific-line-from-file
*/
void entity_manager::eraseFileLine(std::string eraseLine) {
    std::string line;
    std::ifstream fin;
    
    fin.open(data_path_);
    // contents of path must be copied to a temp file then
    // renamed back to the path file
    std::ofstream temp;
    temp.open(temp_path_);

    while (getline(fin, line)) {
        // write all lines to temp other than the line marked for erasing
        if (line != eraseLine)
            temp << line << std::endl;
    }

    temp.close();
    fin.close();

    // required conversion for remove and rename functions
    const char *p = data_path_.c_str();
    std::remove(p);
    std::rename(temp_path_.c_str(), p);
}

entity_manager::entity_manager(std::string data_path)
{

	std::string input1;
	ent_id input3;
	temp_path_ = data_path + "/" + "temp.txt";
	data_path_ = data_path + "/" + "entity_info.txt";
	std::ifstream fin(data_path_, std::ios::in);

	boost::filesystem::path ent_file_path(data_path_);

	if (boost::filesystem::exists(ent_file_path)) {
		std::ifstream fin(data_path_, std::ios::in);
		while (fin >> input3.type) {
			fin >> input3.id_ >> input1;
			id_map[input3].path = input1;
			id_map[input3].id = input3.id_;
			id_map[input3].type = input3.type;
			type_map[input3.type].insert(input3.id_);
			std::ifstream ent_data(input1.c_str(), std::ios::in);
			if (ent_data) {
				std::stringstream temp;
				temp << ent_data.rdbuf();
				id_map[input3].data = temp.str();
			}
		}
		for (auto& p : type_map) {
			type_to_id[p.first] = *(p.second.rbegin());
		}
		fin.close();
	}


}

bool entity_manager::insert(entity& e)
{
	e.id = generate_next_id(e.type);

	ent_id insert_id;
	insert_id.id_ = e.id;
	insert_id.type = e.type;

	// append the newly generated id to the path
	e.path += std::to_string(e.id);

	// insert the new entity
	id_map[insert_id] = e;
	type_map[e.type].insert(e.id);

	// create a path object to check if directories needed exist
	boost::filesystem::path dir(e.path.substr(0, e.path.find_last_of("/")));

	// if directory(s) do not exist, create them
	if (!boost::filesystem::exists(dir)) {
		if (!boost::filesystem::create_directories(dir)) {
			return false;
		}
	}

	std::ofstream e_file(e.path.c_str(), std::ios::out);
	if (!e_file)
		return false;
	
	std::ofstream write_info(data_path_, std::ios_base::app);
	write_info << e.type << " " << e.id << " " << e.path << std::endl;
	e_file << e.data;
	e_file.close();
	return true;
}

bool entity_manager::insert_with_id(entity& e)
{
	e.path = e.path.substr(0, e.path.find_last_of("/")); // ../database/fish/1/ -> ../database/fish/1

	unsigned int last_slash = e.path.find_last_of("/");  
	unsigned int size = e.path.length();

	try {
		e.id = stoi(e.path.substr(last_slash + 1, size));
	} catch (std::exception& e) {
		return false;
	}

	ent_id insert_id;
	e.type = e.type.substr(0, e.type.find_last_of("/")); // /api/fish/2 -> /api/fish
	insert_id.id_ = e.id;
	insert_id.type = e.type;

	// insert the new entity
	id_map[insert_id] = e;
	type_map[e.type].insert(e.id);

	// create a path object to check if directories needed exist
	boost::filesystem::path dir(e.path.substr(0, e.path.find_last_of("/"))); // ../database/fish/1 -> ../database/fish

	// if directory(s) do not exist, create them
	if (!boost::filesystem::exists(dir)) {
		if (!boost::filesystem::create_directories(dir)) {
			return false;
		}
	}
	
	std::ofstream e_file(e.path.c_str(), std::ios::out);
	if (!e_file)
		return false;
	
	std::ofstream write_info(data_path_, std::ios_base::app);
	write_info << e.type << " " << e.id << " " << e.path << std::endl;
	e_file << e.data;
	e_file.close();
	return true;
}

bool entity_manager::update(ent_id id, std::string data_)
{
	if (!exists(id))
		return false;
	
	// write new data to map entry
	id_map[id].data = data_;

	// open a stream to write updated data to file
	std::ofstream e_file(id_map[id].path.c_str(), std::ios::out);
	if (!e_file)
		return false;
	
	// write new data to file
	e_file << data_;
	e_file.close();

	return true;
}

bool entity_manager::remove(ent_id id)
{
	if (!exists(id))
		return false;
	
	std::stringstream file_info;

	file_info << id_map[id].type << " " << id_map[id].id << " " << id_map[id].path;

	eraseFileLine(file_info.str());

	// remove file from file system
	std::remove(id_map[id].path.c_str());

	// check if file has been removed
	std::ifstream can_open(id_map[id].path.c_str());
	if (can_open)
		return false;

	// remove entity from both maps
	type_map[id_map[id].type].erase(id.id_);
	id_map.erase(id);

	return true;
}

const entity* entity_manager::get(ent_id id_)
{
	if (!exists(id_))
		return nullptr;
	
	return &(id_map[id_]);
}

std::string entity_manager::get_ids_by_type(std::string type_)
{
	if (!type_exists(type_) || type_map[type_].empty())
		return "[]";
	
	/*
	We want to create a string of the list of IDs
	Go from {x, y, z, ...} -> "[x, y, z, ...]"
	*/
	std::stringstream id_list;
	id_list << "[";

	// append a comma and space after each ID
	for (unsigned int id_ : type_map[type_])
	{
		id_list << id_ << ", ";
	}

	// get return string from the stringstream
	std::string r_string = id_list.str();

	// remove trailing comma and space after the last ID
	r_string.erase(r_string.size() - 2, 2); 
	r_string.append("]");
	return r_string;
}

bool entity_manager::exists(ent_id id)
{
	return (id_map.find(id) != id_map.end());
}

bool entity_manager::type_exists(std::string type_)
{
	return (type_map.find(type_) != type_map.end());
}

/* Find the next available id by searching for an available one */
unsigned int entity_manager::generate_next_id(std::string type_)
{
	type_to_id[type_]++;
    ent_id e_id;
    e_id.id_ = type_to_id[type_];
    e_id.type = type_;
    while (exists(e_id))
    {
        type_to_id[type_]++;
        e_id.id_ = type_to_id[type_]++;
    }
	return type_to_id[type_];
}