#pragma once

#include "./ir.hpp"
#include "./math.hpp"
#include <algorithm>
#include <vector>

Permutation getpermutation(Schedule x) {
    return Permutation(x.ptr, getNLoops(x));
};
Vector<Int, 0> getbeta(Schedule x) {
    return Vector<Int, 0>(x.ptr + 2 * getNLoops(x), 1 + 2 * getNLoops(x));
};

size_t schedule_size(size_t nloops) { return 4 * nloops + 1; };
size_t schedule_size(Schedule x) { return schedule_size(getNLoops(x)); };

struct BaselineModelCost {};

// template <typename A>
void visit(std::vector<Int> sorted, Function fun, size_t idx) {
    if (fun.visited(idx))
        return;
    auto outs = outneighbors(fun, idx);
    for (size_t j = 0; j < length(outs); ++j)
        visit(sorted, fun, outs(j));
    sorted.push_back(idx);
}

std::vector<Int> topologicalSort(Function fun) {
    std::vector<Int> sorted;
    clear(fun);
    for (size_t j = 0; j < nv(fun); j++) {
        visit(sorted, fun, j);
    }
    std::reverse(sorted.begin(), sorted.end());
    return sorted;
}

typedef std::vector<std::vector<Int>> TermBundle;

TermBundle weaklyConnectedComponents(Function fun) {
    TermBundle components;
    clear(fun);
    for (size_t j = 0; j < nv(fun); ++j) {
        if (fun.visited(j))
            continue;
        std::vector<Int> sorted;
        visit(sorted, fun, j);
        std::reverse(sorted.begin(), sorted.end());
        components.emplace_back(sorted);
    }
    return components;
}

void fillfastmemcostsum(Function fun) { return; }

//
// x = foo(...)
// y = bar(...)
// z = x + y
// Maybe we do want x and z together, and y separate
//
// T* mem = alloca(2048);
// d, r = divrem(length(I), 2048);
// for di in 1:d
// ...
// for i in I
//   mem[i] = y = bar(...)
// end
// for i in I
//   x = foo(...)
//   z = x + mem[i]
// end
// end
//

// fusion(level) -> order(level) -> cost(level) ->
// fusion(level+1) -> ...
//
// For each level, allocate a (triangular) matrix of schedules and costs; upper
// triangular, col represents fused len, row fused num This can store/cache all
// possible fused combinations. We can create views as appropriate so that we
// can still fill correctly through the recursion. On entry  (updateCost =
// true), this is when we use the temp at that level to update bestcost.

void scheduleBundleFusion(Function fun){

}

template <typename I>
void scheduleBundleFusion(Function fun, I tidx_begin, I tidx_end, size_t level,
                          bool updateCost) {
    for (auto it = tidx_begin; it != tidx_end; ++it) {
        
        auto [c0, isvalid] =
            scheduleBundleOrder(fun, tidx_begin, it + 1, level);
        if (!isvalid)
            break;
        double c1;
        if (it == tidx_begin) {
            c1 = scheduleBundleFusion(fun, it + 1, tidx_end, level, false);
        } else {
            c1 = getbestcost(fun, it + 1, tidx_end, level);
        }
        if (((c0 + c1) < best_cost)) {

            if (updateCost) {
                updateBestCost();
            }
        }
    }
}
// Evaluates [`tidx_begin`, `tidx_end`) as a bundle together, fused at level
// `level`
template <typename I>
void schedule_bundle_order(Function fun, I tidx_begin, I tidx_end,
                           size_t level) {}

void schedule_bundle_level(Function fun, TermBundle tb, size_t level) {}

// Greedily prefuse elements in tb.
TermBundle &prefuse(Function fun, std::vector<Int> tb) {
    TermBundle tb;
    std::vector<Int> currentBundle;
    currentBundle.push_back(tb(0));
    for (size_t j = 1; j < tb.size(); ++j) {
        Term termHead = getTerm(fun, j - 1);
        Term termTemp = getTerm(fun, j);
        if ((termTemp.lnid != termHead.tid) &
            (termTemp.loopdeps == termHead.loopdeps)) {
            // Passed cheap checks.
            // Now check memory accesses
            // Also, should check if they're reductions... We do not want to
            // pre-fuse register tiling candiates.
            if (contiguousMask(fun, termTemp) ==
                contiguousMask(fun, termHead)) {
                tb.emplace_back(currentBundle);
                currentBundle.clear();
            }
        }
        currentBundle.push_back(tb(j));
    }
    tb.emplace_back(currentBundle);
    return tb;
}


// at level `l`...
// 1. iterate over fusion combinations



void scheduleBundleLevel(Function fun, BundleGraph bg, size_t level){
    TermBundle headbundle = tbs[0];

}

// prefuse terms
void scheduleBundle(Function fun, std::vector<Int> wcc) {
    BundleGraph bg = prefuse(fun, wcc);
    scheduleBundleLevel(fun, bg, 0);
    return;
}

/*
void schedule(Function fun, TermBundle tb) {
    for (size_t j = 0; j < length(tb); ++j) {
        scheduleBundle(fun, tb[j]);
    }
    return;
}
*/