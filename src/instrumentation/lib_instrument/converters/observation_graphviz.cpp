#include "observation_graphviz.h"

#include "types.h"
#include "utils/preprocessor.h"
#include "utils/base64.h"

#include <boost/foreach.hpp>
#include <iomanip>

using namespace instrumentation;


namespace converters {
#define _GvizObj(item) << "_" << (item)
        #define GvizObj(prefix, ...) (prefix) FOREACH(_GvizObj, (__VA_ARGS__))


static const char GvizTransactionPre[] = "Transaction";
static const char GvizTransactionGatePre[] = "Transaction Gate";
static const char GvizProbePre[] = "Probe";
static const char GvizOraclePre[] = "Oracle";


void t_observations_to_graphviz::rgba (float r, float g, float b, float a,
                                       std::ostream & out) {
    std::ios::fmtflags flags = out.flags();
    char fill = out.fill('0');


    out << std::hex;
    out << std::setw(2) << (unsigned) (255 * r);
    out << std::setw(2) << (unsigned) (255 * g);
    out << std::setw(2) << (unsigned) (255 * b);
    out << std::setw(2) << (unsigned) (255 * a);

    out.fill(fill);
    out.flags(flags);
}

void t_observations_to_graphviz::operator () (const instrumentation::t_probe_observation & p,
                                              std::ostream & out) {
    out << GvizObj(GvizProbePre, probe_id) << " [";
    out << "label=\"" << GvizObj(GvizProbePre, probe_id) << "\\n";

    if (p.state) {
        t_state & st = *p.state;
        out << "State Observation: " << p.state->size() << " variables\\n";
        std::ios::fmtflags flags = out.flags();
        char fill = out.fill('0');
        size_t i = 0;

        out << "Data: ";

        for (size_t j = 0; j < st.size(); j++) {
            out << " | " << base64_encode((unsigned char *) st.ptr(j), st.width(j));
        }

        out << " | ";
        out.fill(fill);
        out.flags(flags);
    }
    else
        out << "Hit Observation\\n";

    out << p.c_id << "@" << p.time;
    out << "\"";
    out << ", fillcolor=turquoise3, ";

    if (p.state)
        out << "shape=box3d";
    else
        out << "shape=rectangle";

    out << "];\n";
}

void t_observations_to_graphviz::operator () (const instrumentation::t_oracle_observation & o,
                                              std::ostream & out) {
    out << GvizObj(GvizOraclePre, oracle_id) << " [";
    out << "label=\"" << GvizObj(GvizOraclePre, oracle_id) << "\\n";
    out << "Health: " << o.health << " Confidence: " << o.confidence << "\\n";
    out << o.c_id << "@" << o.time;
    out << "\"";
    out << ", fillcolor=\"#";
    rgba(1 - o.health, o.health, 0, o.confidence, out);
    out << "\", ";
    out << "shape=triangle";
    out << "];\n";
}

void t_observations_to_graphviz::operator () (const instrumentation::t_transaction_observation & tr,
                                              std::ostream & out) {
    t_id tmp_id = ++transaction_id;


    out << "subgraph transaction_" << tmp_id << " {\n";
    out << "mindist=0;\n";
    out << "penwidth=3;\n";
    out << "node[ style = \"filled\"];\n";

    out << GvizObj(GvizTransactionPre, tmp_id) << " [";
    out << "label=\"" << GvizObj(GvizTransactionPre, tmp_id) << "\\n";
    out << GvizObj(GvizTransactionGatePre, tr.c_id_start) << "@";
    out << tr.time_start << "\\n";

    if (tr.ended()) {
        out << GvizObj(GvizTransactionGatePre, tr.c_id_end) << "@";
        out << tr.time_end;
    }

    out << "\"";
    out << ", fillcolor=orchid";

    if (tr.ended())
        out << ", shape=octagon";
    else
        out << ", shape=doubleoctagon";

    out << "];\n";

    BOOST_FOREACH(t_ptr<t_probe_observation> p,
                  tr.probes) {
        probe_id++;
        (* this)(* p, out);
        out << GvizObj(GvizTransactionPre, tmp_id) << " ->";
        out << GvizObj(GvizProbePre, probe_id) << ";\n";
    }

    BOOST_FOREACH(t_ptr<t_oracle_observation> o,
                  tr.oracles) {
        oracle_id++;
        (* this)(* o, out);
        out << GvizObj(GvizTransactionPre, tmp_id) << " ->";
        out << GvizObj(GvizOraclePre, oracle_id) << ";\n";
    }

    BOOST_FOREACH(t_ptr<t_transaction_observation> t,
                  tr.transactions) {
        (* this)(* t, out);
        out << GvizObj(GvizTransactionPre, tmp_id) << " ->";
        out << GvizObj(GvizTransactionPre, transaction_id) << ";\n";
    }
    out << "}\n";
}

void t_observations_to_graphviz::start (std::ostream & out) {
    probe_id = 0;
    oracle_id = 0;
    transaction_id = 0;
    out << "digraph g {\n";
}

void t_observations_to_graphviz::end (std::ostream & out) {
    out << "}\n";
}
}