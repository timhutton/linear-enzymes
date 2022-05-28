// STL:
#include <string>

struct Reaction {
    Reaction() {}
    Reaction( int a_pre, bool bonded_pre, int b_pre, int a_post, bool bonded_post, int b_post )
        : a_pre( a_pre )
        , bonded_pre( bonded_pre )
        , b_pre( b_pre )
        , a_post( a_post )
        , bonded_post( bonded_post )
        , b_post( b_post )
        {}
    Reaction( const std::string& s );

    static bool validSoFar( const std::string& s );
    static bool isValid( const std::string& s );

    std::string getString() const;
    int a_pre,b_pre,a_post,b_post;
    bool bonded_pre,bonded_post;

    static const std::string start_marker;
    static const std::string end_marker;
    static const int num_bits = 5;
};