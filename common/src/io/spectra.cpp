#include "io/spectra.h"

#include "types.h"
#include "utils/stl.h"

#include <iomanip>

inline ostream & StateSpectraVariable(ostream & out,
                                      const void * buf,
                                      size_t left,
                                      size_t right) {
  ios::fmtflags flags = out.flags();
  char fill = out.fill('0');
  out << hex << "0x";
  while(right-- > left)
    out << setw(2) << (unsigned)((const unsigned char*)buf)[right];
  out.fill(fill);
  out.flags(flags);
  return out;
}

inline ostream & StateSpectraState(ostream & out,
                                   const State & st){

  size_t i = 0;
  for(size_t j = 0; j < st.n_vars; j++) {
    out << "|";
    StateSpectraVariable(out, st.data, i,
                         st.offset_end[j]);
    i=st.offset_end[j];
  }

  return out;
}

inline ostream & StateSpectraObservation(ostream & out,
                                         size_t t_id,
                                         const Observation & obs){

  out << "obs,";
  out << obs.p_id << ",";
  out << t_id << ",";
  out << "\"";
  unsigned long time = obs.time;
  StateSpectraVariable(out, &time, 0, sizeof(time));
  if(obs.state)
    StateSpectraState(out, *obs.state);
  out <<  "\"";
  return out;
}

inline ostream & StateSpectraOracleResult(ostream & out,
                                          size_t t_id,
                                          const OracleResult & o_res){

  out << "or,";
  out << o_res.o_id << ",";
  out << t_id << ",\"";
  unsigned long time = o_res.time;
  StateSpectraVariable(out, &time, 0, sizeof(time)) << "|";
  out << o_res.health << "|";
  out << o_res.confidence <<"\"";
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
  out << "type, id, first, second\n";
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
