// STL:
#include <array>
#include <vector>

struct Reaction {
    Reaction() {}
    Reaction( char a_type, int a_pre, bool bonded_pre, char b_type, int b_pre, int a_post, bool bonded_post, int b_post )
        : a_type( a_type)
        , a_pre( a_pre )
        , bonded_pre( bonded_pre )
        , b_type( b_type )
        , b_pre( b_pre )
        , a_post( a_post )
        , bonded_post( bonded_post )
        , b_post( b_post )
        {}
    //Reaction( const std::string& s );

    static constexpr int num_types = 6;
    static constexpr int num_states = 20;
    static constexpr int base = 5;
    static constexpr int num_entries = 8;
    static constexpr std::array<int, num_entries> limits = {
        num_states, num_states, num_states, num_states,
        num_types, num_types, 2, 2 }; // must be in descending order of size

    std::vector<int> getBases() const;

    int a_pre, b_pre, a_post, b_post;       // 0, 1, 2, ... num_states-1
    char a_type, b_type;                    // 'a', 'b', 'c', ... (num_types-1)+'a'
    bool bonded_pre, bonded_post;

private:
    std::array<int, num_entries> getEntries() const;

    //std::string getString() const;

    //static bool validSoFar( const std::string& s );
    //static bool isValid( const std::string& s );
};