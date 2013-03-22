#include "io/graphviz.h"

#include "types.h"
#include "utils/debug.h"
#include "utils/preprocessor.h"
#include "utils/stl.h"

#include <iomanip>

#define _GvizObj(item) << "_" << (item)
#define GvizObj(prefix, ...) (prefix) FOREACH(_GvizObj, (__VA_ARGS__))


static const char GvizThreadPre[] = "Thread";
static const char GvizTransactionPre[] = "Transaction";
static const char GvizTransactionGatePre[] = "TransactionGate";
static const char GvizObservationPre[] = "Observation";
static const char GvizOracleResultPre[] = "OracleResult";

ostream & GvizRGBA(ostream & out, float r, float g, float b, float a) {
  ios::fmtflags flags = out.flags();
  char fill = out.fill('0');

  out << hex;
  out << setw(2) << (unsigned) (255 * r);
  out << setw(2) << (unsigned) (255 * g);
  out << setw(2) << (unsigned) (255 * b);
  out << setw(2) << (unsigned) (255 * a);

  out.fill(fill);
  out.flags(flags);
  return out;

}

ostream & GvizStateLabel(ostream & out, const t_state & st) {
  ios::fmtflags flags = out.flags();
  char fill = out.fill('0');

  out << "Data: ";
  out << hex;
  size_t i = 0;
  for(size_t j = 0; j < st.n_vars; j++) {
    out << " | ";
    for(size_t k = st.offset_end[j]; k-- > i;)
      out << setw(2) << (unsigned)st.data[k];
    i=st.offset_end[j];
  }
  out << " | ";
  out.fill(fill);
  out.flags(flags);
  return out;
}

ostream & GvizObservations(ostream & out, t_thread_id t_id, const t_thread_info & thr) {
  size_t observation_id = 0;
  foreach(obs, thr.probes) {
    out << GvizObj(GvizObservationPre, t_id, observation_id) << " [";
    out << "label=\"" << GvizObj(GvizObservationPre, t_id, observation_id) << "\\n";
    if((*obs)->state) {
      out << "State Observation: " << (*obs)->state->data_size() << " bytes\\n";
      GvizStateLabel(out, *(*obs)->state) << "\\n";
    }
    else
      out << "Hit Observation\\n";
    out << (*obs)->p_id << "@" << (*obs)->time;
    out << "\"";
    out << ", fillcolor=turquoise3, ";
    if((*obs)->state)
      out << "shape=box3d";
    else
      out << "shape=rectangle";
    out << "];\n";
    observation_id++;
  }
  return out;
}

ostream & GvizObservationLinks(ostream & out, t_thread_id t_id, const t_thread_info & thr) {
  size_t observation_id = 0;
  foreach(obs, thr.probes) {
    out  << GvizObj(GvizThreadPre, t_id) << " ->";
    out  << GvizObj(GvizObservationPre, t_id, observation_id) << ";\n";
    observation_id++;
  }
  return out;
}


ostream & GvizOracleResults(ostream & out, t_thread_id t_id, const t_thread_info & thr) {
  size_t oracle_result_id = 0;
  foreach(o_res, thr.oracles) {
    out << GvizObj(GvizOracleResultPre, t_id, oracle_result_id) << " [";
    out << "label=\"" << GvizObj(GvizOracleResultPre, t_id, oracle_result_id) << "\\n";
    out << "Health: " << (*o_res)->health << " Confidence: " << (*o_res)->confidence << "\\n";
    out << (*o_res)->o_id << "@" << (*o_res)->time;
    out << "\"";
    out << ", fillcolor=\"#";
    GvizRGBA(out, 1 - (*o_res)->health, (*o_res)->health, 0, (*o_res)->confidence);
    out << "\", ";
    out << "shape=triangle";
    out << "];\n";
    oracle_result_id++;
  }
  return out;
}

ostream & GvizOracleResultLinks(ostream & out, t_thread_id t_id, const t_thread_info & thr) {
  size_t oracle_result_id = 0;
  foreach(obs, thr.oracles) {
    out  << GvizObj(GvizThreadPre, t_id) << " ->";
    out  << GvizObj(GvizOracleResultPre, t_id, oracle_result_id) << ";\n";
    oracle_result_id++;
  }
  return out;
}

ostream & GvizTransactions(ostream & out, t_thread_id t_id, const t_thread_info & thr) {
  size_t transaction_id = 0;
  //Finished Transactions
  foreach(tr, thr.transactions) {
    out << GvizObj(GvizTransactionPre, t_id, transaction_id) << " [";
    out << "label=\"" << GvizObj(GvizTransactionPre, t_id, transaction_id) << "\\n";
    out << GvizObj(GvizTransactionGatePre, (*tr)->gate_start) << "@";
    out << (*tr)->start << "\\n";
    if ((*tr)->ended()){
      out << GvizObj(GvizTransactionGatePre, (*tr)->gate_end) << "@";
      out << (*tr)->end;
    }
    out << "\"";
    out << ", fillcolor=orchid";
    if ((*tr)->ended())
      out << ", shape=octagon";
    else
      out << ", shape=doubleoctagon";
    out << "];\n";
    transaction_id++;
  }
  return out;
}

ostream & GvizTransactionLinks(ostream & out, t_thread_id t_id, const t_thread_info & thr) {
  size_t transaction_id = 0;
  foreach(tr, thr.transactions) {
    /* Observation Links */
    {
    t_thread_info::t_probe_storage::const_iterator begin, end;
    begin = thr.probes_after((*tr)->start, true);
    if((*tr)->ended())
      end = thr.probes_after((*tr)->end);
    else
      end = thr.probes.end();

    while(begin < end){
      out  << GvizObj(GvizTransactionPre, t_id, transaction_id) << " ->";
      out  << GvizObj(GvizObservationPre, t_id, begin - thr.probes.begin()) << ";\n";
      begin ++;
    }
    }
    
    /* OracleResult Links */
    {
    t_thread_info::t_oracle_storage::const_iterator begin, end;
    begin = thr.oracles_after((*tr)->start, true);
    if((*tr)->ended())
      end = thr.oracles_after((*tr)->end);
    else
      end = thr.oracles.end();

    while(begin < end){
      out  << GvizObj(GvizTransactionPre, t_id, transaction_id) << " ->";
      out  << GvizObj(GvizOracleResultPre, t_id, begin - thr.oracles.begin()) << ";\n";
      begin ++;
    }
    }
    out  << GvizObj(GvizThreadPre, t_id) << " ->";
    out  << GvizObj(GvizTransactionPre, t_id, transaction_id) << ";\n";
    transaction_id++;
  }
  return out;
}

ostream & GvizThreads(ostream & out, t_thread_id t_id, const t_thread_info & thr) {
  out  << GvizObj(GvizThreadPre, t_id) << " [";
  out  << "label=\"" << GvizObj(GvizThreadPre, t_id) << "\\n@";
  out << thr.start;
  if (thr.ended())
    out << " -> " << thr.end;
  out  << "\", ";
  if(thr.parent_id == t_id)
    out << "fillcolor=yellow";
  else
    out << "fillcolor=lightblue";
  if (thr.ended())
    out << ", shape=circle";
  else
    out << ", shape=doublecircle";
  out  << "];\n";
  return out;
}

ostream & GvizThreadLinks(ostream & out, const t_datastore & ds) {
  foreach(it, ds.thread_info){
    if(it->second->parent_id != it->first){
      out << GvizObj(GvizThreadPre, it->second->parent_id);
      out << "->" << GvizObj(GvizThreadPre, it->first) << ";\n";
    }
  }
  return out;
}

ostream & Graphviz(ostream & out, const t_datastore & ds) {
  out << "digraph g {\n";
  out << "mindist=0;\n";
  out << "clusterrank=\"local\";\n";
  out << "ranksep=\"10\";\n";
  out << "rankdir=LR;\n";
  out << "penwidth=3;\n";
  out << "node[ style = \"filled\"];\n";
  out << "rank=\"same\";\n";

  foreach(it, ds.thread_info){
    out << "subgraph cluster_" << it->first << "{\n";
    out << "style=filled;\n";
    out << "color=lightgrey;\n";
    GvizThreads(out, it->first, *it->second);

    GvizObservations(out, it->first, *it->second);
    
    GvizOracleResults(out, it->first, *it->second);
    //  GvizObservationLinks(out, it->first, *it->second);


    GvizTransactions(out, it->first, *it->second);
    GvizTransactionLinks(out, it->first, *it->second);
    out << "}\n";
  }
  GvizThreadLinks(out, ds);
  out << "}\n";
  return out;
}
