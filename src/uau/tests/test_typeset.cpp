#include "lest/lest.hpp"
#include "typeset.h"

class First;
class Second;
class Third;
class Fifth;

const lest::test specification[] = {
    "separate validation", []{
        const uau::TypeSet<> &ts = uau::TypeSet<First, Second, Third>();

        EXPECT(ts.contains<First>());
        EXPECT(ts.contains<Second>());
        EXPECT(ts.contains<Third>());

        EXPECT(!ts.contains<long>());
    },

    "combinations validation", []{
        const uau::TypeSet<> &ts = uau::TypeSet<First, Second, Third, Fifth>();

        EXPECT( (ts.contains<Fifth, Third, First, Second>()) );
        EXPECT( (ts.contains<First, Second>()) );
        EXPECT( (ts.contains<First, Second>()) );
        EXPECT( (ts.contains<Second, Third>()) );
        EXPECT( (ts.contains<Third, First>()) );
        EXPECT( (ts.contains<Second, Third, First>()) );

        EXPECT( !(ts.contains<Second, int, First>()) );
    },

    "builtin types", []{
        const uau::TypeSet<> &ts = uau::TypeSet<char, int, double>();

        EXPECT( (ts.contains<char, int, double>()) );
        EXPECT( (ts.contains<int, double>()) );
        EXPECT( (ts.contains<char>()) );

        EXPECT( !(ts.contains<long>()) );
        EXPECT( !(ts.contains<short>()) );
        EXPECT( !(ts.contains<char, int, double, long>()) );
    },

    "cv-qualifiers, pointers and references", []{
        const uau::TypeSet<> &ts = uau::TypeSet<const First, volatile Second, Third *, Fifth &>();

        EXPECT( (ts.contains<const First, volatile Second, Third *>()) );
        EXPECT((ts.contains<Third *, Fifth &>()));

        EXPECT( !(ts.contains<const First, Second>()) );
        EXPECT( !(ts.contains<const First *, Second>()) );
        EXPECT( !(ts.contains<volatile First *, Third>()) );
    }
};

int main() {
    return lest::run(specification);
}
