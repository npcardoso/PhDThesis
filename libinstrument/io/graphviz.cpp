#include "graphviz.h"

#include "../types.h"
#include "../utils/preprocessor.h"
#include "../utils/stl.h"

#define _GvizObj(item) << "_" << (item)
#define GvizObj(prefix, ...) (prefix) FOREACH(_GvizObj, (__VA_ARGS__))

static const char GvizThreadPre[] = "Thread";
static const char GvizTransactionPre[] = "Transaction";
static const char GvizTransactionGatePre[] = "TransactionGate";
static const char GvizObservationPre[] = "Observation";
#include "../utils/debug.h"

ostream & GvizStateLabel(ostream & out, const State & st) {
  ios::fmtflags flags = out.flags();

  out << "Data: ";
  out << hex;
  size_t i = 0;
  for(size_t j = 0; j < st.n_vars; j++) {
    out << " | ";
    for(size_t k = st.offset_end[j]; k-- > i;){
      out << (unsigned)st.data[k];
      if((k - i) % 2 == 0)
        out << ".";
    }
    i=st.offset_end[j];
  }
  out << " | ";
  out.flags(flags);
  return out;
}

ostream & GvizObservations(ostream & out, thread_id_t t_id, const ThreadInfo & thr) {
  size_t observation_id = 0;
  foreach(obs, thr.observations) {
    out << GvizObj(GvizObservationPre, t_id, observation_id) << " [";
    out << "label=\"" << GvizObj(GvizTransactionPre, t_id, observation_id) << "\\n";
    if((*obs)->state) {
      out << "State Observation: " << (*obs)->state->rawSize() << " bytes\\n";
      GvizStateLabel(out, *(*obs)->state) << "\\n";
    }
    else
      out << "Hit Observation\\n";
    out << (*obs)->p_id << "@" << (*obs)->time;
    out << "\"";
    out << ", fillcolor=green, ";
    if((*obs)->state)
      out << "shape=box3d";
    else
      out << "shape=rectangle";
    out << "];\n";
    observation_id++;
  }
  return out;
}

ostream & GvizObservationLinks(ostream & out, thread_id_t t_id, const ThreadInfo & thr) {
  size_t observation_id = 0;
  foreach(obs, thr.observations) {
    out  << GvizObj(GvizThreadPre, t_id) << " ->";
    out  << GvizObj(GvizObservationPre, t_id, observation_id) << ";\n";
    observation_id++;
  }
  return out;
}

ostream & GvizTransactions(ostream & out, thread_id_t t_id, const ThreadInfo & thr) {
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
    out << ", fillcolor=red";
    if ((*tr)->ended())
      out << ", shape=octagon";
    else
      out << ", shape=doubleoctagon";
    out << "];\n";
    transaction_id++;
  }
  return out;
}

ostream & GvizTransactionLinks(ostream & out, thread_id_t t_id, const ThreadInfo & thr) {
  size_t transaction_id = 0;
  foreach(tr, thr.transactions) {
    ThreadInfo::observation_storage_t::const_iterator begin, end;
    begin = thr.getObservationsAfter((*tr)->start, true);
    if((*tr)->ended())
      end = thr.getObservationsAfter((*tr)->end);
    else
      end = thr.observations.end();

    while(begin < end){
      out  << GvizObj(GvizTransactionPre, t_id, transaction_id) << " ->";
      out  << GvizObj(GvizObservationPre, t_id, begin - thr.observations.begin()) << ";\n";
      begin ++;
    }
    out  << GvizObj(GvizThreadPre, t_id) << " ->";
    out  << GvizObj(GvizTransactionPre, t_id, transaction_id) << ";\n";
    transaction_id++;
  }
  return out;
}

ostream & GvizThreads(ostream & out, thread_id_t t_id, const ThreadInfo & thr) {
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

ostream & GvizThreadLinks(ostream & out, const DataStore & ds) {
  foreach(it, ds.thread_info){
    if(it->second->parent_id != it->first){
      out << GvizObj(GvizThreadPre, it->second->parent_id);
      out << "->" << GvizObj(GvizThreadPre, it->first) << ";\n";
    }
  }
  return out;
}

ostream & Graphviz(ostream & out, const DataStore & ds) {
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
    //  GvizObservationLinks(out, it->first, *it->second);


    GvizTransactions(out, it->first, *it->second);
    GvizTransactionLinks(out, it->first, *it->second);
    out << "}\n";
  }
  GvizThreadLinks(out, ds);
  out << "}\n";
  return out;
}
