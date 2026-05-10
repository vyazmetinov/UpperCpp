#include "AdaptiveSet.h"

#include <cassert>

int main() {
    Set a{3};
    a.Add(1);
    a.Add(2);
    a.Add(3);
    a.Add(4);
    assert(a.Contains(3));
    a.Remove(3);
    assert(!a.Contains(3));

    Set b{3};
    b.Add(2);
    b.Add(4);
    b.Add(5);

    Set uni = a.Union(b);
    assert(uni.Contains(1));
    assert(uni.Contains(2));
    assert(uni.Contains(4));
    assert(uni.Contains(5));

    Set inter = a.Intersection(b);
    assert(inter.Contains(2));
    assert(inter.Contains(4));
    assert(!inter.Contains(1));
    return 0;
}
