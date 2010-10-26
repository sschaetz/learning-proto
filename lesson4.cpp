// detecting wild expressions

#include <boost/proto/proto.hpp>

namespace proto = boost::proto;


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
struct MapListOf :
  proto::or_<
      proto::terminal<map_list_of_>
    , proto::function<
          MapListOf
        , proto::terminal<_>
        , proto::terminal<_>
      >
   >
{};

// a struct that accepts a maplistof with only int
struct MapListOfInt :
  proto::or_<
      proto::terminal<map_list_of_>
    , proto::function<
          MapListOfInt
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
  return 0;
}

