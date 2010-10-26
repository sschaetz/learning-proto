// detecting wild expressions

#include <map>
#include <algorithm>
#include <boost/proto/proto.hpp>
#include <boost/assert.hpp>

namespace proto = boost::proto;

// static initailization
struct map_list_of_ {};

template< typename Expr >
struct map_list_of_expr;

using proto::_;

using namespace std;

struct map_insert : proto::callable
{
  typedef void result_type;

  template<class M, class K, class V>
  void operator()(M & m, K k, V v) const
  {
    m[k] = v;
  }
};

// a struct that specifies a maplistof with any T
// we removed the fill_map function and instead added a proto callable that
// and call it in one of the transformations of this grammar
struct MapListOf :
  proto::or_<
      proto::when< proto::terminal<map_list_of_>, proto::_void >
    , proto::when<
          proto::function<MapListOf, proto::terminal<_>, proto::terminal<_> >
        , proto::and_<
              MapListOf(proto::_child0)
            , map_insert(proto::_state, proto::_value(proto::_child1),
                         proto::_value(proto::_child2))
        >
     >
   >
{};

struct map_list_of_domain
  : proto::domain< proto::generator< map_list_of_expr > >
{};

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
    BOOST_PROTO_ASSERT_MATCHES(*this, MapListOf);
    std::map<K, V, C, A> m;
    MapListOf()(*this, m);
    return m;
  }

};

map_list_of_expr< proto::terminal< map_list_of_ >::type > const map_list_of;

void print_pair(pair<const int, int> const & p)
{
  std::cout << p.first << " " << p.second << std::endl;
}

int main()
{
  map<int, int> m = map_list_of(1,2)(3,4);
  for_each( m.begin(), m.end(), print_pair);
  map<int, int> m2 = map_list_of(1,2)(3,4,5);
  std::cout << std::endl;
  return 0;
}



