// Max guests task
/* 
The task:
Guests come to the hotel for some hours.
Need to calculate maximum number of guests that were in hotel simultaneously.
*/

#include <iostream>
#include <cassert>
#include <algorithm>
#include <array>
#include <vector>

size_t maxGuests(const std::vector<std::pair<int, int>>& segments);

int main(int, char**)
{
    assert(maxGuests({}) == 0);
    assert(maxGuests({{1, 2}}) == 1);
    assert(maxGuests({{1, 2}, {2, 3}}) == 1);
    assert(maxGuests({{1, 5}, {0, 1}, {4, 5}}) == 2);
    assert(maxGuests({{1, 5}, {1, 2}, {4, 5}}) == 2);
}

size_t maxGuests(const std::vector<std::pair<int, int>>& segments)
{
    std::array<int, 24> hours{};

    for(auto seg : segments)
    {
        std::vector<int> guestHours;
        
        int firstHour = seg.first;
        int lastHour = seg.second;

        std::generate_n(
                        std::back_inserter(guestHours), 
                        lastHour - firstHour, 
                        [&firstHour]() { return firstHour++; }
                       );
        
        for(auto hour : guestHours)
            hours[hour] += 1;
    }

    auto max = *std::max_element(std::begin(hours), std::end(hours));
    
    return max;
}
