// transform the proto expression tree into a map

#include <map>
#include <algorithm>

#include <boost/proto/proto.hpp>
#include <boost/foreach.hpp>

namespace proto = boost::proto;

// static initailization
struct map_list_of_ {};

// nested static initialization (proto::terminal contains a type map_list_of_)
proto::terminal<map_list_of_>::type const map_list_of = {{}};


template<typename Map>
void fill_map(proto::terminal<map_list_of_>::type, Map &)       // end recursion
{
  // nothing to do, we reached a terminal
}

// the fill_map algorithm recursively calls itself on the map and the next proto
// expression (the 0th child of each expression)
// it then adds the 1st and 2nd child into the map
template<class Fun, class Map>
void fill_map(Fun const & f, Map & m)
{
  // uncomment this to see where we start to traverse the tree

  //std::cout << "node (" << proto::value( proto::child_c<1>(f) ) << "," <<
  //  proto::value( proto::child_c<2>(f) ) << ")" << std::endl;

  fill_map(proto::child_c<0>(f), m); // recurse on 0th (first) child
  m[ proto::value( proto::child_c<1>(f) ) ] =
    proto::value( proto::child_c<2>(f) );
}

using namespace std;

void print_pair(pair<const int, int> const & p)
{
  std::cout << p.first << " " << p.second << std::endl;
}

int main()
{
  // create map
  map<int, int> m;
  fill_map(map_list_of(1,2)(2,3)(4,5)(5,6)(3,4), m);

  // see what we got
  for_each( m.begin(), m.end(), print_pair);
  std::cout << std::endl;

  // the order of the output does not give a good picture of how the tree nodes
  // are actually traversed, to see this remove the print comment from the
  // fill_map method

  return 0;
}
