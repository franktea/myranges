#include <iostream>
#include <vector>
#include <boost/type_index.hpp>
#include "more_ranges.h"

using namespace std;

namespace vs = std::ranges::views;
namespace rs = std::ranges;

int main() {
    std::vector<uint64_t> in {1, 4, 6, 89, 56, 45, 7};

    auto b = in | vs::add_constant;
    std::cout<<boost::typeindex::type_id_with_cvr<decltype(b)>().pretty_name()<<"\n";
    for (auto && i : b)
        std::cout << i << ' ';
    std::cout << '\n'; // should print: 43 47 64 131 98 87 49

    // combine it with other views:
    for (auto && i : in | vs::add_constant | vs::take(3))
        std::cout << i << ' ';
    std::cout << '\n'; // should print: 43 47 64

	return 0;
}
