// std::any_of - returns true if f is true for any element from [first, last)
template <typename It, typename Pred>
bool AnyOf(It first, It last, Pred f) {
     return std::find_if(first, last, f) != last;
}

/* std::remove_copy_if - copies the elements of [first, last) to a sequence starting
with out, but only those, for which f is false */
template <typename InIter, typename OutIter, typename Predicate>
OutIter RemoveCopyIf(InIter first, InIter last, OutIter out, Predicate f) {
    while (first != last) {
        if (!f(*first)) {
            *out = *first;
            ++out;
        }
        ++first;
    }
    return out;
}

/* std::unique - removes all but first elements from
ranges of consecutive equal elements */
template <typename Iter>
Iter Unique(Iter first, Iter last) {
    if (first == last) {
        return first;
    }
    Iter out = first;
    ++first;
    while (first != last) {
        if (!(*out == *first)) {
            ++out;
            *out = *first;
        }
        ++first;
    }
    return ++out;
}

/* std::set_difference - writes all elements from difference between [first1, last1)
and [first2, last2) into range starting with out */
template <typename InIter1, typename InIter2, typename OutIter>
OutIter SetDifference(InIter1 first1, InIter1 last1,
                      InIter2 first2, InIter2 last2, OutIter out) {
    while (first1 != last1) {
        if (first2 == last2) {
            return std::copy(first1, last1, out);
        }
        if (*first1 < *first2) {
            *out = *first1;
            ++out;
            ++first1;
        } else if (*first2 < *first1) {
            ++first2;
        } else {
            ++first1;
            ++first2;
        }
    }
    return out;
}

/* std::reverse_copy - copies elements of [first, last) in reverse order
to a range starting with out */
template <typename InIter, typename OutIter>
OutIter ReverseCopy(InIter first, InIter last, OutIter out) {
    while (first != last) {
        --last;
        *out = *last;
        ++out;
    }
    return out;
}

/* transform_if - applies f to those elements of [first, last) for which
f is true, stores result in the range starting from out */
template <typename InIter, typename OutIter, typename Predicate, typename Action>
OutIter TransformIf(InIter first, InIter last, OutIter out, Predicate condition, Action f) {
    while (first != last) {
        if (condition(*first)) {
            *out = f(*first);
            ++out;
        }
        ++first;
    }
    return out;
}

/* sideways_merge - merges ranges [first1, last1) and [first2, last2) into one sorted
range. [first1, last1) is sorted and [first2, last2) is in reverse order */
#include <algorithm>
#include <iostream>

template <typename It, typename OutputIt>
OutputIt SidewaysMerge(It first1, It last1, It first2, It last2, OutputIt out) {
    if (first2 == last2) {
        return std::copy(first1, last1, out);
    }
    --last2;
    while (first1 != last1) {
        if (first2 == last2 + 1) {
            return std::copy(first1, last1, out);
        }
        if (*last2 < *first1) {
            *out = *last2;
            --last2;
        } else {
            *out = *first1;
            ++first1;
        }
        ++out;
    }
    return std::reverse_copy(first2, last2 + 1, out);
}