// a first simple example

#include <boost/proto/proto.hpp>

namespace proto = boost::proto;

// static initailization
struct map_list_of_ {};

// nested static initialization (proto::terminal contains a type map_list_of_)
proto::terminal<map_list_of_>::type const map_list_of = {{}};

int main()
{
  // this type has an overloaded function operator that
  // returns something that also has an overloaded function operator
  map_list_of(1,2)(2,3)(3,4)(4,5)(5,6);

  // uncomment to print the tree using display_expr
  // proto::display_expr(map_list_of(1,2)(2,3)(3,4)(4,5)(5,6));

  /**
   a tree structure that contains all terminals that were passed in
   all nodes and leaves contain a tag (func has tag function, terminals have tag
   terminal)

                    func
                   / | \
                   func 5  6
                   / | \
                func 4  5
                / | \
             func 3  4
             / | \
         func 3  4
         / | \
      func 2  3
      / | \
   seed 1  2

  */
  return 0;
}
