#include "utils/debug.h"
#include <iostream>
#include <string>
#include "instrumentation/sinks/collector.h"
#include "converters/observation_spectra.h"
#include "serialization/unserializers/json.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#define BUF_SIZE 102400
using namespace std;
using namespace converters;
using namespace diagnosis;
using namespace instrumentation;
using namespace instrumentation::sinks;
typedef std::pair<float, bool> t_feedback_token;

class t_knn : public  std::set<t_feedback_token> {
  public:
    bool operator()(float val, t_count k) {
      iterator left = lower_bound(t_feedback_token(val, true));
      iterator right;
      if(left == end()) {
        right = end();
        left--;
      }
      else {
        right = left;
        right++;
      }
      
      if(k > size())
        k = size();

      t_count fail_count;
      t_count i = k;
      while(i--) {
        iterator tmp;

        if(left == end())
          tmp = right++;
        else if(right == end())
          tmp = left--;
        else if(abs(val - left->first) < abs(val - right->first)) {
          tmp = left;
          if(left == begin())
            left = end();
          else
            left--;
        }
        else
          tmp = right++;
        
        if(tmp->second)
          fail_count++;
//        cout << i << " -- "<< tmp->first << ": " << tmp->second << " fc: " << fail_count << " ret:" << (fail_count *2 >k)<<std::endl;
      }
      return (fail_count * 2 > k);
    }
};


int main() {
  t_knn knn;

  knn.insert(t_feedback_token(1,true));
  knn.insert(t_feedback_token(2,true));
  knn.insert(t_feedback_token(3,false));
  knn.insert(t_feedback_token(4,false));
  while(cin){
    float tmp;
    cin >> tmp;
    cout << knn(tmp, 3)<<endl;;
  }
  return 0;
}
