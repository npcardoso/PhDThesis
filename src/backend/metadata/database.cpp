#include "database.h"

#include <cassert>
#include <iostream>


t_artifact_id t_database::register_artifact (t_artifact_type type) {
    database.push_back(t_properties());
    database[database.size() - 1]["type"] = std::to_string(type);

    std::cerr << "Registering Artifact (" << (char) type << ", " << database.size() << ")" << std::endl;

    return database.size();
}

void t_database::register_metadata (t_artifact_id id,
                                    std::string key,
                                    std::string value) {
    assert(id > 0 && id <= database.size());
    database[id - 1][key] = value;

    std::cerr << "Registering Metadata (" << id << ", " << key << ", " << value << ")" << std::endl;
}