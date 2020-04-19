#include <iostream>
#include <vector>
#include <string>
#include "more_ranges.h"

using namespace std;

namespace vs = std::ranges::views;
namespace rs = std::ranges;

int main() {
    std::vector<string> v1 {"甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"};
    std::vector<string> v2 {"子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"};

    auto a = v1 | vs::cycle;
    auto b = v2 | vs::cycle;
    auto c = vs::zip(a, b) | vs::take(60);

    for(auto&& [x, y]: c)
    	cout<<x<<y<<" ";
    cout<<"\n";

    return 0;
}
