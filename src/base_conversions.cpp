// Local:
#include "base_conversions.hpp"

// converts a list of digits in the specified base to a single value
LongType BaseToValue(const std::vector<int>& entries, const int base)
{
    LongType value = 0;
    for(const int entry : entries) {
        value = ( value * base ) + entry; // (assumes doesn't overflow)
    }
    return value;
}

// converts a value to a list of digits in the specified base
std::vector<int> ValueToBase(LongType value, const int base)
{
    std::vector<int> reverse_entries;
    while( value > 0 ) {
        const int remainder = value % base;
        reverse_entries.push_back( remainder );
        value = ( value - remainder ) / base;
    }
    return { reverse_entries.rbegin(), reverse_entries.rend() };
}

// converts a number (given as a list of digits) in one base to a number in another base
std::vector<int> ConvertBase(const std::vector<int>& digits, const int input_base, const int output_base)
{
    return ValueToBase( BaseToValue( digits, input_base ), output_base );
}
