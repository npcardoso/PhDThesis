#ifndef __DATABASE_H_29b68eff9ffe1542cea80b5724014fe3e1a14e72__
#define __DATABASE_H_29b68eff9ffe1542cea80b5724014fe3e1a14e72__

#include "../types.h"

#include <map>
#include <vector>

class t_database {
public:
    enum t_artifact_type {TRANSACTION = 'T', PROBE = 'P', ORACLE = 'O'};


    t_artifact_id register_artifact (t_artifact_type type);
    void register_metadata (t_artifact_id id,
                            std::string key,
                            std::string value);

private:

    typedef std::map<std::string, std::string> t_properties;
    std::vector<t_properties> database;
};

#endif