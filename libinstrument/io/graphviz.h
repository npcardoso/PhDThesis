#pragma once

#include <ostream>
#include "../datastore.h"

using namespace std;

ostream & Graphviz(ostream & out, const DataStore & ds);
