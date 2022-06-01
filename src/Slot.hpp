// STL:
#include <vector>

class Slot {
    public:
        bool isEmpty() const;
        void addAtom( size_t iAtom );
        void removeAtom( size_t iAtom );
        size_t getAtom( size_t iiAtom );
        size_t numAtoms() { return this->atoms.size(); }
        void clear() { this->atoms.clear(); }
    private:
        std::vector<size_t> atoms;
};