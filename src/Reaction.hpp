// STL:
#include <array>
#include <string>

struct Reaction {
    static constexpr int num_types = 6;
    static constexpr int num_states = 20;
    static constexpr int base = 5;
    static constexpr int num_entries = 8;
    static const std::array<int, num_entries> limits;
    static constexpr int num_digits = 12; // compute from highest values (TODO: automate)

    Reaction(char a_type, int a_pre, bool bonded_pre, char b_type, int b_pre, int a_post, bool bonded_post, int b_post);
    Reaction(const std::array<int, num_digits>& digits);

    std::array<int, num_digits> getBases() const;
    std::string getAsHumanReadableString() const;

    int a_pre, b_pre, a_post, b_post;       // 0, 1, 2, ... num_states-1
    char a_type, b_type;                    // 'a', 'b', 'c', ... (num_types-1)+'a'
    bool bonded_pre, bonded_post;

private:
    std::array<int, num_entries> getEntries() const;

    //std::string getString() const;

    //static bool validSoFar( const std::string& s );
    //static bool isValid( const std::string& s );
};
