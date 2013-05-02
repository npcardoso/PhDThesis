#ifndef __STATS_MHS_H__
#define __STATS_MHS_H__

#include "../../types.h"
#include <iostream>

class t_stats {
  public:
    t_count items_recv, items_sent, items_generated;
    t_time_interval total_comm, total_merge, total_calc, total_transfer;
    t_time_interval runtime;
    inline t_stats () {
      items_recv = 0;
      items_sent = 0;
      items_generated = 0;
      total_comm = 0;
      total_merge = 0;
      total_calc = 0;
      total_transfer = 0;
      runtime = 0;
    }

    std::ostream & print(std::ostream & out, int rank, bool print_runtime=false) const {
      out << "P" << rank << " Calculation Time: " << total_calc << std::endl;
      out << "P" << rank << " Communication Time: " << total_comm << std::endl;
      out << "P" << rank << " Merge Time: " << total_merge << std::endl;
      out << "P" << rank << " Transfer Time: " << total_transfer << std::endl;

      out << "P" << rank << " Total Generated: " << items_generated << std::endl;
      out << "P" << rank << " Total Sent: " << items_sent << std::endl;
      out << "P" << rank << " Total Recv: " << items_recv << std::endl;

      if(print_runtime)
        out << "P" << rank << " Run Time: " << runtime << std::endl;
      return out;
    }
};
#endif
