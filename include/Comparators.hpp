#pragma once

#include "./POSet.hpp"
#include "Math.hpp"
#include "Symbolics.hpp"
#include <cassert>
#include <cstdint>
#include <llvm/ADT/SmallVector.h>
#include <ostream>

// For `== 0` constraints
struct EmptyComparator {
    static constexpr size_t numConstantTerms() { return 0; }
    static constexpr bool greaterEqual(llvm::ArrayRef<int64_t>,
                                       llvm::ArrayRef<int64_t>) {
        return true;
    }
    static constexpr bool greater(llvm::ArrayRef<int64_t>,
                                  llvm::ArrayRef<int64_t>) {
        return false;
    }
    static constexpr bool lessEqual(llvm::ArrayRef<int64_t>,
                                    llvm::ArrayRef<int64_t>) {
        return true;
    }
    static constexpr bool less(llvm::ArrayRef<int64_t>,
                               llvm::ArrayRef<int64_t>) {
        return false;
    }
    static constexpr bool equal(llvm::ArrayRef<int64_t>,
                                llvm::ArrayRef<int64_t>) {
        return true;
    }
    static constexpr bool greaterEqual(llvm::ArrayRef<int64_t>) { return true; }
    static constexpr bool greater(llvm::ArrayRef<int64_t>) { return false; }
    static constexpr bool lessEqual(llvm::ArrayRef<int64_t>) { return true; }
    static constexpr bool less(llvm::ArrayRef<int64_t>) { return false; }
    static constexpr bool equal(llvm::ArrayRef<int64_t>) { return true; }
    static constexpr bool equalNegative(llvm::ArrayRef<int64_t>,
                                        llvm::ArrayRef<int64_t>) {
        return true;
    }
    static constexpr bool lessEqual(llvm::ArrayRef<int64_t>, int64_t x) {
        return 0 <= x;
    }
};

// for non-symbolic constraints
struct LiteralComparator {
    static constexpr size_t numConstantTerms() { return 1; }
    static inline bool greaterEqual(llvm::ArrayRef<int64_t> x,
                                    llvm::ArrayRef<int64_t> y) {
        return x[0] >= y[0];
    }
    static inline bool greater(llvm::ArrayRef<int64_t> x,
                               llvm::ArrayRef<int64_t> y) {
        return x[0] > y[0];
    }
    static inline bool lessEqual(llvm::ArrayRef<int64_t> x,
                                 llvm::ArrayRef<int64_t> y) {
        return x[0] <= y[0];
    }
    static inline bool less(llvm::ArrayRef<int64_t> x,
                            llvm::ArrayRef<int64_t> y) {
        return x[0] < y[0];
    }
    static inline bool equal(llvm::ArrayRef<int64_t> x,
                             llvm::ArrayRef<int64_t> y) {
        return x[0] == y[0];
    }
    static inline bool greaterEqual(llvm::ArrayRef<int64_t> x) {
        return x[0] >= 0;
    }
    static inline bool greater(llvm::ArrayRef<int64_t> x) { return x[0] > 0; }
    static inline bool lessEqual(llvm::ArrayRef<int64_t> x) {
        return x[0] <= 0;
    }
    static inline bool less(llvm::ArrayRef<int64_t> x) { return x[0] < 0; }
    static inline bool equal(llvm::ArrayRef<int64_t> x) { return x[0] == 0; }
    static inline bool equalNegative(llvm::ArrayRef<int64_t> x,
                                     llvm::ArrayRef<int64_t> y) {
        // this version should return correct results for
        // `std::numeric_limits<int64_t>::min()`
        return (x[0] + y[0]) == 0;
    }
    static inline bool lessEqual(llvm::ArrayRef<int64_t> y, int64_t x) {
        return y[0] <= x;
    }
};
// BaseComparator defines all other comparator methods as a function of
// `greaterEqual`, so that `greaterEqual` is the only one that needs to be
// implemented.
// An assumption is that index `0` is a literal constant, and only indices >0
// are symbolic. Thus, we can shift index-0 to swap between `(>/<)=` and ``>/<`
// comparisons.
//
// Note: only allowed to return `true` if known
// therefore, `a > b -> false` does not imply `a <= b`
template <typename T> struct BaseComparator {
    inline bool greaterEqual(llvm::MutableArrayRef<int64_t> delta,
                             llvm::ArrayRef<int64_t> x,
                             llvm::ArrayRef<int64_t> y) const {
        const size_t N = x.size();
        assert(N == y.size());
        assert(N == delta.size());
        for (size_t n = 0; n < N; ++n)
            delta[n] = x[n] - y[n];
        return static_cast<const T *>(this)->greaterEqual(delta);
    }
    inline bool greaterEqual(llvm::ArrayRef<int64_t> x,
                             llvm::ArrayRef<int64_t> y) const {
        llvm::SmallVector<int64_t> delta(x.size());
        return greaterEqual(delta, x, y);
    }
    inline bool less(llvm::ArrayRef<int64_t> x,
                     llvm::ArrayRef<int64_t> y) const {
        return greater(y, x);
    }
    inline bool greater(llvm::ArrayRef<int64_t> x,
                        llvm::ArrayRef<int64_t> y) const {
        const size_t N = x.size();
        assert(N == y.size());
        llvm::SmallVector<int64_t> delta(N);
        for (size_t n = 0; n < N; ++n)
            delta[n] = x[n] - y[n];
        --delta[0];
        return static_cast<const T *>(this)->greaterEqual(delta);
    }
    inline bool lessEqual(llvm::ArrayRef<int64_t> x,
                          llvm::ArrayRef<int64_t> y) const {
        return static_cast<const T *>(this)->greaterEqual(y, x);
    }
    inline bool equal(llvm::ArrayRef<int64_t> x,
                      llvm::ArrayRef<int64_t> y) const {
        // check cheap trivial first
        if (x == y)
            return true;
        llvm::SmallVector<int64_t> delta(x.size());
        return (greaterEqual(delta, x, y) && greaterEqual(delta, y, x));
    }
    inline bool greaterEqual(llvm::ArrayRef<int64_t> x) const {
        return static_cast<const T *>(this)->greaterEqual(x);
    }
    inline bool lessEqual(llvm::SmallVectorImpl<int64_t> &x) const {
        return lessEqual(llvm::MutableArrayRef<int64_t>(x));
    }
    inline bool lessEqual(llvm::MutableArrayRef<int64_t> x) const {
        for (auto &&a : x)
            a *= -1;
        bool ret = static_cast<const T *>(this)->greaterEqual(x);
        for (auto &&a : x)
            a *= -1;
        return ret;
    }
    inline bool lessEqual(llvm::MutableArrayRef<int64_t> x, int64_t y) const {
        int64_t x0 = x[0];
        x[0] = x0 - y;
        bool ret = lessEqual(x);
        x[0] = x0;
        return ret;
    }
    inline bool less(llvm::MutableArrayRef<int64_t> x) const {
        int64_t x0 = x[0];
        x[0] = -x0 - 1;
        for (size_t i = 1; i < x.size(); ++i)
            x[i] *= -1;
        bool ret = static_cast<const T *>(this)->greaterEqual(x);
        x[0] = x0;
        for (size_t i = 1; i < x.size(); ++i)
            x[i] *= -1;
        return ret;
    }
    inline bool lessEqual(llvm::ArrayRef<int64_t> x) const {
        llvm::SmallVector<int64_t, 16> y{x.begin(), x.end()};
        return lessEqual(llvm::MutableArrayRef<int64_t>(y));
    }
    inline bool less(llvm::ArrayRef<int64_t> x) const {
        llvm::SmallVector<int64_t, 16> y{x.begin(), x.end()};
        return less(llvm::MutableArrayRef<int64_t>(y));
    }
    inline bool greater(llvm::MutableArrayRef<int64_t> x) const {
        int64_t x0 = x[0]--;
        bool ret = static_cast<const T *>(this)->greaterEqual(x);
        x[0] = x0;
        return ret;
    }
    inline bool greater(llvm::ArrayRef<int64_t> x) const {
        // TODO: avoid this needless memcopy and (possible) allocation?
        llvm::SmallVector<int64_t, 8> xm{x.begin(), x.end()};
        return greater(xm);
    }
    inline bool equal(llvm::ArrayRef<int64_t> x) const {
        // check cheap trivial first
        return allZero(x) ||
               (static_cast<const T *>(this)->greaterEqual(x) && lessEqual(x));
    }
    inline bool equalNegative(llvm::ArrayRef<int64_t> x,
                              llvm::ArrayRef<int64_t> y) const {
        const size_t N = x.size();
        assert(N == y.size());
        bool allEqual = true;
        for (size_t i = 0; i < N; ++i)
            allEqual &= (x[i] + y[i]) == 0;
        if (allEqual)
            return true;
        llvm::SmallVector<int64_t, 8> delta(N);
        for (size_t i = 0; i < N; ++i)
            delta[i] = x[i] + y[i];
        return equal(delta);
    }
};

struct SymbolicComparator : BaseComparator<SymbolicComparator> {
    PartiallyOrderedSet POSet;
    llvm::SmallVector<Polynomial::Monomial> monomials;
    static SymbolicComparator construct(PartiallyOrderedSet poset) {
        SymbolicComparator sc{.POSet = std::move(poset),
                              .monomials =
                                  llvm::SmallVector<Polynomial::Monomial>{}};

        return sc;
    }
    static SymbolicComparator construct(llvm::ArrayRef<MPoly> x,
                                        PartiallyOrderedSet poset) {
        SymbolicComparator sc{SymbolicComparator::construct(poset)};
        for (auto &p : x)
            for (auto &t : p)
                if (t.exponent.degree())
                    addTerm(sc.monomials, t.exponent);
        return sc;
    }
    size_t getNumConstTerms() const { return 1 + monomials.size(); }
    MPoly getPoly(llvm::ArrayRef<int64_t> x) const {
        MPoly delta;
        assert(x.size());
        assert(x.size() == 1 + monomials.size());
        for (size_t i = 0; i < monomials.size(); ++i)
            if (int64_t d = x[i + 1])
                delta.terms.emplace_back(d, monomials[i]);
        if (int64_t d = x[0])
            delta.terms.emplace_back(d);
        return delta;
    }
    bool greaterEqual(llvm::ArrayRef<int64_t> x,
                      llvm::ArrayRef<int64_t> y) const {
        MPoly delta;
        assert(x.size());
        assert(x.size() == y.size());
        assert(x.size() == 1 + monomials.size());
        for (size_t i = 0; i < monomials.size(); ++i)
            if (int64_t d = x[i + 1] - y[i + 1])
                delta.terms.emplace_back(d, monomials[i]);
        if (int64_t d = x[0] - y[0])
            delta.terms.emplace_back(d);
        return POSet.knownGreaterEqualZero(delta);
    }
    bool greaterEqual(llvm::ArrayRef<int64_t> x) const {
        return POSet.knownGreaterEqualZero(getPoly(x));
    }
    std::ostream &printSymbol(std::ostream &os, llvm::ArrayRef<int64_t> x,
                              int64_t mul = 1) const {
        os << mul * x[0];
        for (size_t i = 1; i < x.size(); ++i)
            os << "+" << Polynomial::Term{x[i] * mul, monomials[i - 1]};
        return os;
    }
};

template <typename T>
concept Comparator = requires(T t, llvm::ArrayRef<int64_t> x, int64_t y) {
    { t.getNumConstTerms() } -> std::convertible_to<size_t>;
    { t.greaterEqual(x) } -> std::convertible_to<bool>;
    { t.lessEqual(x) } -> std::convertible_to<bool>;
    { t.greater(x) } -> std::convertible_to<bool>;
    { t.less(x) } -> std::convertible_to<bool>;
    { t.equal(x) } -> std::convertible_to<bool>;
    { t.greaterEqual(x, x) } -> std::convertible_to<bool>;
    { t.lessEqual(x, x) } -> std::convertible_to<bool>;
    { t.greater(x, x) } -> std::convertible_to<bool>;
    { t.less(x, x) } -> std::convertible_to<bool>;
    { t.equal(x, x) } -> std::convertible_to<bool>;
    { t.equalNegative(x, x) } -> std::convertible_to<bool>;
    { t.lessEqual(x, y) } -> std::convertible_to<bool>;
};
