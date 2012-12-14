#include "spectra.h"

#include "../types.h"
#include "../utils/stl.h"

inline ostream & StateSpectraState(ostream & out,
                                   const State & st){
  ios::fmtflags flags = out.flags();
  size_t i = 0;
  for(size_t j = 0; j < st.n_vars; j++) {
    out << "," << j << ":";
    for(;i < st.offset_end[j]; i++)
      out << (unsigned)st.data[i];
  }
  out.flags(flags);
  return out;
}

inline ostream & StateSpectraObservation(ostream & out,
                                         size_t t_id,
                                         const Observation & obs){

  out << "obs,";
  out << t_id << ",";
  out << obs.time << ",";
  out << obs.p_id << ",";
  if(obs.state) {
    out << obs.state->n_vars << "";
    StateSpectraState(out, *obs.state);
  }
  else
    out << "0";
  return out;
}

inline ostream & StateSpectraOracleResult(ostream & out,
                                          size_t t_id,
                                          const OracleResult & o_res){

  out << "or,";
  out << t_id << ",";
  out << o_res.time << ",";
  out << o_res.o_id << ",";
  out << o_res.health << ",";
  out << o_res.confidence;
  return out;
}

inline ostream & StateSpectraTransaction(ostream & out,
                                         size_t t_id,
                                         const ThreadInfo & thr,
                                         const Transaction & tr) {

  streamsize precision = out.precision(1024);

  /* Observations */
  {
    ThreadInfo::observation_storage_t::const_iterator begin, end;
    begin = thr.getObservationsAfter(tr.start, true);

    if(tr.ended())
      end = thr.getObservationsAfter(tr.end);
    else
      end = thr.observations.end();

    while(begin < end)
      StateSpectraObservation(out, t_id, **(begin++)) << "\n";
  }
  /* Oracle Results */
  {
    ThreadInfo::oracle_result_storage_t::const_iterator begin, end;
    begin = thr.getOracleResultsAfter(tr.start, true);

    if(tr.ended())
      end = thr.getOracleResultsAfter(tr.end);
    else
      end = thr.oracle_results.end();

    while(begin < end)
      StateSpectraOracleResult(out, t_id, **(begin++)) << "\n";
  }
  out.precision(precision);
  return out;
}

inline ostream & StateSpectraArtifacts(ostream & out,
                                       const char * pre,
                                       const DataStore::artifact_metadata_storage_t & artifacts) {
  artifact_id_t id = 0;
  foreach(artifact, artifacts){
    foreach(metadata, *artifact)
      out << pre << "," << id <<",\"" << metadata->first << "\"" << "," << "\"" << metadata->second << "\"" << "\n";
    id++;
  }
  return out;
}

ostream & StateSpectra(ostream & out, const DataStore & ds) {
  StateSpectraArtifacts(out, "metapr", ds.probe_metadata);
  StateSpectraArtifacts(out, "metatg", ds.transaction_gate_metadata);
  StateSpectraArtifacts(out, "metaor", ds.oracle_metadata);

  size_t t_id = 0;
  foreach(thr, ds.thread_info)
    foreach(tr, thr->second->transactions) {
      StateSpectraTransaction(out, t_id++, *(thr->second), **tr);
    }

  return out;
}
