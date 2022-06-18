// stdlib:
#include <vector>

using LongType = unsigned long long;

// converts a number (given as a list of digits in one base) to a number in another base
std::vector<int> ConvertBase(const std::vector<int>& digits, const int input_base, const int output_base);

// Converts a number (given as a list of digits in one base) to a single value
LongType BaseToValue(const std::vector<int>& digits, const int base);

// Converts a decimal value to a list of digits in the specified base
std::vector<int> ValueToBase(LongType value, const int base);

// Converts a number (given as a list of digits where each digits is in a separate base) to a single value
// (input bases must be sorted in descending order of size, else the result isn't unique)
template<int N>
LongType MultiBaseToValue(const std::array<int, N>& digits, const std::array<int, N>& input_bases)
{
    LongType value = digits[0];
    for(int i = 1; i < N; i++) {
        value = ( value * input_bases[i-1] ) + digits[i]; // (assumes doesn't overflow)
    }
    return value;
}

// Converts a value to a number (given as a list of digits where each digits is in a separate base)
// (output bases must be sorted in descending order of size, else the result isn't unique)
template<int N>
std::array<int, N> ValueToMultiBase(LongType value, const std::array<int, N>& output_bases)
{
    std::array<int, N> digits;
    for(int i = N-1; i >= 1; i--) {
        const int remainder = value % output_bases[i-1];
        digits[i] = remainder;
        value = ( value - remainder ) / output_bases[i-1];
    }
    digits[0] = value;
    return digits;
}

// Converts a number (given as a list of digits where each digits is in a separate base) to a number in a different base
// (input bases must be sorted in descending order of size, else the result isn't unique)
template<int N>
std::vector<int> ConvertFromMultiBase(const std::array<int, N>& digits, const std::array<int, N>& input_bases, const int output_base)
{
    return ValueToBase( MultiBaseToValue<N>(digits, input_bases), output_base);
}

// Converts a number (given as a list of digits in one base) to a number (given as a list of digits where each digits is in a separate base)
// (output bases must be sorted in descending order of size, else the result isn't unique)
template<int N>
std::array<int, N> ConvertToMultiBase(const std::vector<int>& digits, const int input_base, const std::array<int, N>& output_bases)
{
    return ValueToMultiBase<N>( BaseToValue(digits, input_base), output_bases);
}

// Converts a number (given as a list of digits where each digits is in a separate base) to a number in a different base
// (input bases must be sorted in descending order of size, else the result isn't unique)
template<int N, int M>
std::array<int, M> ConvertFromMultiBase(const std::array<int, N>& digits, const std::array<int, N>& input_bases, const int output_base)
{
    const std::vector<int> digits2 = ValueToBase( MultiBaseToValue<N>(digits, input_bases), output_base);
    std::array<int, M> zero_padded_digits{0};
    std::copy(digits2.begin(), digits2.end(), zero_padded_digits.begin() + zero_padded_digits.size() - digits2.size());
    return zero_padded_digits;
}

// Converts a number (given as a list of digits in one base) to a number (given as a list of digits where each digits is in a separate base)
// (output bases must be sorted in descending order of size, else the result isn't unique)
template<int N, int M>
std::array<int, N> ConvertToMultiBase(const std::array<int, M>& digits, const int input_base, const std::array<int, N>& output_bases)
{
    const std::vector<int> digits_vec(digits.begin(), digits.end());
    return ValueToMultiBase<N>( BaseToValue(digits_vec, input_base), output_bases);
}
