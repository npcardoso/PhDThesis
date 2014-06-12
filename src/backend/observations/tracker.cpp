#include "tracker.h"

#include <boost/foreach.hpp>
#include <cassert>

t_tracker::t_tracker(t_sink_gen sink_gen, t_sink_gen auto_sink_gen):sink_gen(sink_gen), auto_sink_gen(auto_sink_gen) {}


t_tracker_singlethread::t_tracker_singlethread (t_sink_gen sink_gen, t_sink_gen auto_sink_gen) : t_tracker(sink_gen, auto_sink_gen) {
    sink = NULL;
}

t_tracker_singlethread::~t_tracker_singlethread(){
    if(sink)
        delete sink;
}

void t_tracker_singlethread::start (t_thread_id thread_id) {
    assert(sink == NULL);
    sink = sink_gen();
}

void t_tracker_singlethread::end (t_thread_id thread_id) {
    assert(sink != NULL);
    delete sink;
}

t_sink & t_tracker_singlethread::get (t_thread_id thread_id) {
    if(!sink)
        sink = auto_sink_gen();

    return *sink;
}


t_tracker_multithread::t_tracker_multithread(t_sink_gen sink_gen, t_sink_gen auto_sink_gen) : t_tracker(sink_gen, auto_sink_gen) {}

t_tracker_multithread::~t_tracker_multithread() {
    BOOST_FOREACH(auto & sink, sinks) {
        delete sink.second;
    }
}


void t_tracker_multithread::start (t_thread_id thread_id) {
    std::unique_lock<std::mutex> lock(mutex);


    assert(sinks.find(thread_id) != sinks.end());

    t_tracker_singlethread * tmp = new t_tracker_singlethread(sink_gen, auto_sink_gen);
    tmp->start();
    sinks[thread_id] = tmp;
}

void t_tracker_multithread::end (t_thread_id thread_id) {
    std::unique_lock<std::mutex> lock(mutex);

    auto it = sinks.find(thread_id);
    assert(it != sinks.end());

    it->second->end();
    delete it->second;
    sinks.erase(it);
}

t_sink & t_tracker_multithread::get (t_thread_id thread_id) {
    std::unique_lock<std::mutex> lock(mutex);

    auto it = sinks.find(thread_id);
    if(it == sinks.end()) {
        t_tracker_singlethread * tmp = new t_tracker_singlethread(sink_gen, auto_sink_gen);
        sinks[thread_id] = tmp;
        return tmp->get();
    }

    return it->second->get();
}
