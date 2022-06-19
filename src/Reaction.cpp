// local:
#include "Reaction.hpp"
#include "base_conversions.hpp"

// stdlib:
#include <iostream>
#include <sstream>

const std::array<int, Reaction::num_entries> Reaction::limits = {
    Reaction::num_states, Reaction::num_states, Reaction::num_states, Reaction::num_states,
    Reaction::num_types, Reaction::num_types, 2, 2 }; // must be in descending order of size

Reaction::Reaction( char a_type, int a_pre, bool bonded_pre, char b_type, int b_pre, int a_post, bool bonded_post, int b_post )
    : a_type( a_type)
    , a_pre( a_pre )
    , bonded_pre( bonded_pre )
    , b_type( b_type )
    , b_pre( b_pre )
    , a_post( a_post )
    , bonded_post( bonded_post )
    , b_post( b_post )
{
}

Reaction::Reaction( const Reaction::DigitsType& digits ) {
    const std::array<int, num_entries> entries = ConvertToMultiBase<num_entries, num_digits>( digits, base, limits );
    a_pre = entries[0];
    b_pre = entries[1];
    a_post = entries[2];
    b_post = entries[3];
    a_type = entries[4] + 'a';
    b_type = entries[5] + 'a';
    bonded_pre = entries[6];
    bonded_post = entries[7];
}

std::array<int, Reaction::num_entries> Reaction::getEntries() const
{
    return { a_pre, b_pre, a_post, b_post, a_type-'a', b_type-'a', bonded_pre?1:0, bonded_post?1:0 };
}

Reaction::DigitsType Reaction::getBases() const
{
    return ConvertFromMultiBase<num_entries, num_digits>( getEntries(), limits, base );
}

std::string Reaction::getAsHumanReadableString() const
{
    std::ostringstream oss;
    oss << a_type << a_pre << (bonded_pre?"":" + ") << b_type << b_pre << " -> "
        << a_type << a_post << (bonded_post?"":" + ") << b_type << b_post;
    return oss.str();
}

//----------------------------------------------------------------------------

/*bool Reaction::validSoFar( const string& s ) {
    const int num_specified_states = 4;
    const int max_num_bits = 5;
    const size_t num_non_state_bits = 2;
    if( s.length() > num_non_state_bits + num_specified_states * max_num_bits )
        return false;
    for( char c : s ) {
        if( c != '0' && c != '1' && c != '6' && c != '7' )
            return false;
    }
    return true;
}*/

//----------------------------------------------------------------------------

/*bool Reaction::isValid( const string& s ) {
    const int num_specified_states = 4;
    const int min_num_bits = 4;
    const size_t num_non_state_bits = 2;
    if( s.length() >= ( num_non_state_bits + num_specified_states * min_num_bits )
        && ( s.length() - num_non_state_bits ) % num_specified_states == 0 )
            return true;
    return false;
}*/

//----------------------------------------------------------------------------
