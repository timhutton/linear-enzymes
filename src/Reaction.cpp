// local:
#include "Reaction.hpp"

// STL:
#include <sstream>
#include <bitset>
using namespace std;

Reaction::Reaction( const std::string& s ) {
    if( s.length() < 18 )
        throw std::runtime_error("String too short to extract Reaction");
    // convert bit string to reaction and return true
    bonded_pre = ( s[0] == '0' );
    bonded_post = ( s[1] == '0' );
    stringstream ss;
    string b,sr;
    char *end;
    ss << s[2] << s[6] << s[10] << s[14];
    b = ss.str();
    sr.assign( b.rbegin(), b.rend() );
    a_pre = strtol( sr.c_str(), &end, 2 );
    ss.str("");
    ss << s[3] << s[7] << s[11] << s[15];
    b = ss.str();
    sr.assign( b.rbegin(), b.rend() );
    b_pre = strtol( sr.c_str(), &end, 2 );
    ss.str("");
    ss << s[4] << s[8] << s[12] << s[16];
    b = ss.str();
    sr.assign( b.rbegin(), b.rend() );
    a_post = strtol( sr.c_str(), &end, 2 );
    ss.str("");
    ss << s[5] << s[9] << s[13] << s[17];
    b = ss.str();
    sr.assign( b.rbegin(), b.rend() );
    b_post = strtol( sr.c_str(), &end, 2 );
}

//----------------------------------------------------------------------------

string Reaction::getString() const {
    stringstream ss;
    stringstream a_pre_bits_ss;
    a_pre_bits_ss << bitset< num_bits >( a_pre );
    stringstream b_pre_bits_ss;
    b_pre_bits_ss << bitset< num_bits >( b_pre );
    stringstream a_post_bits_ss;
    a_post_bits_ss << bitset< num_bits >( a_post );
    stringstream b_post_bits_ss;
    b_post_bits_ss << bitset< num_bits >( b_post );
    string interlaced_bits;
    for( int i = 0; i < num_bits; ++i ) {
        interlaced_bits += a_pre_bits_ss.str()[ num_bits - i - 1 ];
        interlaced_bits += b_pre_bits_ss.str()[ num_bits - i - 1 ];
        interlaced_bits += a_post_bits_ss.str()[ num_bits - i - 1 ];
        interlaced_bits += b_post_bits_ss.str()[ num_bits - i - 1 ];
    }
    ss << (bonded_pre?"0":"1") << (bonded_post?"0":"1") << interlaced_bits;
    return ss.str();
}

//----------------------------------------------------------------------------

bool Reaction::validSoFar( const string& s ) {
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
}

//----------------------------------------------------------------------------

bool Reaction::isValid( const string& s ) {
    const int num_specified_states = 4;
    const int min_num_bits = 4;
    const size_t num_non_state_bits = 2;
    if( s.length() >= ( num_non_state_bits + num_specified_states * min_num_bits )
        && ( s.length() - num_non_state_bits ) % num_specified_states == 0 )
            return true;
    return false;
}

//----------------------------------------------------------------------------
