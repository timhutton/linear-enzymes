// STL:
#include <array>
#include <string>

struct Reaction {
    static constexpr int num_types = 10;
    static constexpr int num_states = 20;
    static constexpr int base = 4;
    static constexpr int num_entries = 8;
    static const std::array<int, num_entries> limits;
    static constexpr int num_digits = 15;
    using DigitsType = std::array<int, num_digits>;

    Reaction(char a_type, int a_pre, bool bonded_pre, char b_type, int b_pre, int a_post, bool bonded_post, int b_post);
    Reaction(const DigitsType& digits);

    std::array<int, num_entries> getEntries() const;
    DigitsType getBases() const;
    std::string getAsHumanReadableString() const;

    int a_pre, b_pre, a_post, b_post;       // 0, 1, 2, ... num_states-1
    char a_type, b_type;                    // 'a', 'b', 'c', ... (num_types-1)+'a'
    bool bonded_pre, bonded_post;
};
