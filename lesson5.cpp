// detecting wild expressions

#include <map>
#include <boost/proto/proto.hpp>
#include <boost/assert.hpp>

namespace proto = boost::proto;


// a proto callable functor that inserts into a map
struct map_insert : proto::callable
{
  typedef void result_type;

  template<class M, class K, class V>
  void operator()(M & m, K k, V v) const
  {
    m[k] = v;
  }
};

// static initailization
struct map_list_of_ {};

template< typename Expr >
struct map_list_of_expr;

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
};

map_list_of_expr< proto::terminal< map_list_of_ >::type > const map_list_of;

using namespace std;
using proto::_;

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

// a struct that accepts a maplistof with only int
struct MapListOfInt :
  proto::or_<
      proto::terminal<map_list_of_>
    , proto::function<
          MapListOf
        , proto::terminal<int>
        , proto::terminal<int>
      >
   >
{};



int main()
{
  BOOST_PROTO_ASSERT_MATCHES( map_list_of(1,2)(3,4), MapListOf );
  BOOST_PROTO_ASSERT_MATCHES( map_list_of(1,2.0)(3.3,4), MapListOf );
  BOOST_PROTO_ASSERT_MATCHES_NOT( map_list_of(1,2) * 32 << map_list_of,
                                 MapListOf );
  BOOST_PROTO_ASSERT_MATCHES_NOT( map_list_of(1,2.0)(3.3,4), MapListOfInt);

  // use MapListOf as a function
  std::map< int, int > next;
  MapListOf()( map_list_of(1,2)(2,3)(3,4)(4,5)(5,6), next);
  assert(next[1] == 2);
  assert(next[4] == 5);
  return 0;
}


