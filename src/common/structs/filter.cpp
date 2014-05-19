#include "filter.h"

namespace diagnosis {
namespace structs {
t_filter::t_filter () {
    filtered_count = 0;
}

t_id t_filter::next (t_id c) const {
    if (c >= _filter.size())
        return c + 1;
    else
        return _filter[c];
}

t_id t_filter::next_filtered (t_id c) const {
    if (c >= _filter.size())
        return 0;
    else
        for (t_id i = c; i < _filter.size(); i++)
            if (_filter[i] != i + 1)
                return i + 1;

    return 0;
}

t_id t_filter::size () const {
    return _filter.size() ? (_filter.size() - 1) : 0UL;
}

t_id t_filter::get_filtered_count () const {
    return filtered_count;
}

bool t_filter::is_filtered (t_id c) const {
    assert(c > 0);

    if (c >= _filter.size())
        return false;

    return _filter[c - 1] != c;
}

void t_filter::filter_all (const structs::t_candidate & candidate) {
    structs::t_candidate::const_reverse_iterator it = candidate.rbegin();


    while (it != candidate.rend())
        filter(*(it++));
}

void t_filter::filter_all_but (const t_candidate & candidate) {
    if (candidate.size() > 0 &&
        *candidate.rbegin() >= _filter.size())
        resize(*candidate.rbegin());

    t_candidate::reverse_iterator it = candidate.rbegin();
    t_id next = _filter.size();
    t_id next_pos = _filter.size();

    for (; it != candidate.rend(); it++) {
        t_id current = *it;

        for (; next_pos > *it; next_pos--)
            _filter[next_pos - 1] = next;

        if (_filter[current - 1] == current)
            next = current;
    }

    for (; next_pos > 0; next_pos--)
        _filter[next_pos - 1] = next;
}

void t_filter::filter (t_id c) {
    assert(c > 0);

    if (is_filtered(c))
        return;

    filtered_count++;

    if (c >= _filter.size())
        resize(c);

    t_id next = _filter[c];
    t_id i = c;

    while (i-- && _filter[i] == c)
        _filter[i] = next;
}

void t_filter::filter (const t_filter & filter) {
    t_id id = 0;


    while ((id = filter.next_filtered(id)))
        this->filter(id);
}

void t_filter::unfilter (t_id id) {
    if (!is_filtered(id))
        return;

    filtered_count--;

    t_id next = _filter[id];
    t_id i = id;

    while (i-- && _filter[i] == next)
        _filter[i] = id;
}

void t_filter::unfilter (const t_filter & filter) {
    t_id id = 0;


    while ((id = filter.next_filtered(id)))
        unfilter(id);
}

void t_filter::resize (t_id size) {
    t_count old_size = _filter.size();


    assert(size >= old_size);


    _filter.resize(size + 1);

    while (old_size++ < _filter.size())
        _filter[old_size - 1] = old_size;
}

t_filter_iterator::t_filter_iterator (t_count max,
                                      const t_filter * filter) {
    this->max = max;
    this->filter = filter;
    id = 0;
}

t_id t_filter_iterator::get () const {
    return id;
}

void t_filter_iterator::set (t_id id) {
    assert(id <= max);
    assert((filter && id) ? (!filter->is_filtered(id)) : true);
    this->id = id;
}

t_id t_filter_iterator::next () {
    if (filter == NULL)
        id++;
    else
        id = filter->next(id);

    if (id > max)
        id = 0;

    return id;
}
}
}