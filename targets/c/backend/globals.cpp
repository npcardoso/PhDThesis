#include "globals.h"

#include "observations/sinks/verbose.h"

t_sink * gen_verbose_sink () {
    return new t_verbose();
}

t_tracker * init_tracker () {
    t_tracker * tracker = new t_tracker_multithread(gen_verbose_sink, gen_verbose_sink);


    return tracker;
}

t_tracker * tracker = init_tracker();


t_database * init_database () {
    t_database * database = new t_database();


    return database;
}

t_database * database = init_database();