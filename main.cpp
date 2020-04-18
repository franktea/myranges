#include <iostream>
#include <vector>
#include <string>
#include "more_ranges.h"

using namespace std;

namespace vs = std::ranges::views;
namespace rs = std::ranges;

int main() {
    std::vector<string> in {"甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"};

    auto a = in | vs::cycle | vs::take(20);

    for(auto&& i: a)
    	cout<<i<<" ";
    cout<<"\n";

    return 0;
}
