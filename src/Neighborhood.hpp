#ifndef NEIGHBORHOOD_HPP
#define NEIGHBORHOOD_HPP
                                          // as squared Euclidean distance r2:
enum class Neighborhood { vonNeumann      // r2 <= 1
                        , Moore           // r2 <= 2
                        , vonNeumann2     // r2 <= 4
                        , knight          // r2 <= 5
                        , Moore2          // r2 <= 8
                        };

#endif
