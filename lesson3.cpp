// transform the proto expression tree into a map

#include <map>
#include <algorithm>

#include <boost/proto/proto.hpp>
#include <boost/foreach.hpp>

namespace proto = boost::proto;

// static initailization
struct map_list_of_ {};

// forward declare map_list_of_expr because we need it to create the domain
template< typename Expr >
struct map_list_of_expr;

// the domain
struct map_list_of_domain
  : proto::domain< proto::generator< map_list_of_expr > >
{};

// fill_map, notice that the specialization to end the recursion is defined
// after the actual type is declared
template<class Fun, class Map>
void fill_map(Fun const & f, Map & m)
{
  fill_map(proto::child_c<0>(f), m); // recurse on 0th (first) child
  m[ proto::value( proto::child_c<1>(f) ) ] =
    proto::value( proto::child_c<2>(f) );
}

// here we create a type that uses proto::extends
// it specifies an assignment to std::map to allow a straight forward assignment
// this makes explicitly calling fill_map obsolete
template<typename Expr>
struct map_list_of_expr
  : proto::extends< Expr, map_list_of_expr< Expr >, map_list_of_domain >
{
  map_list_of_expr(Expr const & expr = Expr()) :
    proto::extends< Expr, map_list_of_expr< Expr >, map_list_of_domain >(expr)
  {}

  template<class K, class V, class C, class A>
  operator std::map<K, V, C, A>() const
  {
    std::map<K, V, C, A> m;
    fill_map(*this, m);
    return m;
  }

};

// the fill_map specialization if a terminal is passed
template<typename Map>
void fill_map(map_list_of_expr< proto::terminal< map_list_of_ >::type >, Map &)       // end recursion
{}

// declare the map_list_of variable
map_list_of_expr< proto::terminal< map_list_of_ >::type > const map_list_of;

using namespace std;

// helper function to print the map
void print_pair(pair<const int, int> const & p)
{
  std::cout << p.first << " " << p.second << std::endl;
}

int main()
{
  map<int, int> m = map_list_of(1,2)(3,4);
  for_each( m.begin(), m.end(), print_pair);
  std::cout << std::endl;
  return 0;
}
