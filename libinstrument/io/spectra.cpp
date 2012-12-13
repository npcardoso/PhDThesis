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
                                         const Observation & obs){

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

inline ostream & StateSpectraTransaction(ostream & out,
                                         const ThreadInfo & thr,
                                         const Transaction & tr) {

  streamsize precision = out.precision(1024);

  ThreadInfo::observation_storage_t::const_iterator begin, end;
  begin = thr.getObservationsAfter(tr.start, true);

  if(tr.ended())
    end = thr.getObservationsAfter(tr.end);
  else
    end = thr.observations.end();

  while(begin < end)
    StateSpectraObservation(out, **(begin++)) << "\n";
  out.precision(precision);
  return out;
}

inline ostream & StateSpectraArtifacts(ostream & out,
                                       const DataStore::artifact_metadata_storage_t & artifacts) {
  artifact_id_t id = 0;
  foreach(artifact, artifacts){
    out << "[" << id++ << "]\n";
    foreach(metadata, *artifact)
      out << "\"" << metadata->first << "\"" << " = " << "\"" << metadata->second << "\"" << "\n"; 
  }
  return out;
}

ostream & StateSpectra(ostream & out, const DataStore & ds) {
  out << "-- Start Probes --\n";
  StateSpectraArtifacts(out, ds.probe_metadata);
  out << "-- End Probes --\n";

  out << "-- Start Transaction Gates --\n";
  StateSpectraArtifacts(out, ds.transaction_gate_metadata);
  out << "-- End Transaction Gates --\n";
  
  out << "-- Start Oracles --\n";
  StateSpectraArtifacts(out, ds.oracle_metadata);
  out << "-- End Oracles --\n";

  foreach(thr, ds.thread_info)
    foreach(tr, thr->second->transactions) {
      out << "-- Start Transaction --\n";
      StateSpectraTransaction(out, *(thr->second), **tr);
      out << "-- End Transaction --\n";
    }

  return out;
}
