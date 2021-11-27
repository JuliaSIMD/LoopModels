template <typename G>
void visit(std::vector<Int> sorted, G &graph, size_t idx) {
    auto outs = outNeighbors(graph, idx);
    visited(graph, idx) = true;
    for (size_t j = 0; j < length(outs); ++j) {
        if (!visited(graph, j)) {
            visit(sorted, graph, outs(j));
        }
    }
    sorted.push_back(idx);
}

template <typename G> std::vector<Int> topologicalSort(G &graph) {
    std::vector<Int> sorted;
    clearVisited(graph);
    for (size_t j = 0; j < nv(graph); j++) {
        if (!visited(graph, j))
            visit(sorted, graph, j);
    }
    std::reverse(sorted.begin(), sorted.end());
    return sorted;
}

template <typename G>
std::vector<std::vector<Int>> weaklyConnectedComponents(G &graph) {
    std::vector<std::vector<Int>> components;
    clearVisited(graph);
    for (size_t j = 0; j < nv(graph); ++j) {
        if (visited(graph, j))
            continue;
        std::vector<Int> sorted;
        visit(sorted, graph, j);
        std::reverse(sorted.begin(), sorted.end());
        components.emplace_back(sorted);
    }
    return components;
}

// ref:
// https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm#The_algorithm_in_pseudocode
template <typename G>
void strongConnect(
    std::vector<std::vector<Int>> &components, std::vector<size_t> &stack,
    std::vector<std::tuple<size_t, size_t, bool>> &indexLowLinkOnStack,
    size_t &index, G &graph, size_t v) {
    indexLowLinkOnStack[v] = std::make_tuple(index, index, true);
    index += 1;
    stack.push_back(v);

    auto outN = outNeighbors(graph, v);
    for (size_t w = 0; w < length(outN); ++w) {
        if (visited(graph, w)) {
            auto [wIndex, wLowLink, wOnStack] = indexLowLinkOnStack[w];
            if (wOnStack) {
                auto [vIndex, vLowLink, vOnStack] = indexLowLinkOnStack[v];
                indexLowLinkOnStack[v] = std::make_tuple(
                    vIndex, std::min(vLowLink, wIndex), vOnStack);
            }
        } else { // not visited
            strongConnect(components, stack, indexLowLinkOnStack, index, graph,
                          w);
            auto [vIndex, vLowLink, vOnStack] = indexLowLinkOnStack[v];
            indexLowLinkOnStack[v] = std::make_tuple(
                vIndex, std::min(vLowLink, std::get<1>(indexLowLinkOnStack[w])),
                vOnStack);
        }
    }
    auto [vIndex, vLowLink, vOnStack] = indexLowLinkOnStack[v];
    if (vIndex == vLowLink) {
        size_t w;
        std::vector<Int> component;
        do {
            w = stack[stack.size() - 1];
            stack.pop_back();
            auto [wIndex, wLowLink, wOnStack] = indexLowLinkOnStack[w];
            indexLowLinkOnStack[w] = std::make_tuple(wIndex, wLowLink, false);
            component.push_back(w);
        } while (w != v);
        components.emplace_back(component);
    }
}

template <typename G>
std::vector<std::vector<Int>> stronglyConnectedComponents(G &graph) {
    std::vector<std::vector<Int>> components;
    size_t nVertex = nv(graph);
    std::vector<std::tuple<size_t, size_t, bool>> indexLowLinkOnStack(nVertex);
    std::vector<size_t> stack;
    size_t index = 0;
    clearVisited(graph);
    for (size_t v = 0; v < nVertex; ++v) {
        if (!visited(graph, v))
            strongConnect(components, stack, indexLowLinkOnStack, index, graph,
                          v);
    }
    return components;
}

