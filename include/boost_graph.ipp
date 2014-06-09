namespace damogran {

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::boost_graph() : base() {
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::~boost_graph() {
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline typename boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::node_handle boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::add_node() {
	return ::boost::add_vertex(*this);
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline typename boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::edge_add_result  boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::add_edge(node_handle src, node_handle tgt) {
	return ::boost::add_edge(src, tgt, *this);
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline typename boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::node_range  boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::nodes() const {
	return node_range(::boost::vertices(*this));
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline typename boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::edge_range  boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::edges() const {
	return edge_range(::boost::edges(*this));
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline typename boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::out_edge_range  boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::out_edges(node_handle node) const {
	return out_edge_range(::boost::out_edges(node, *this));
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline typename boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::edge_handles  boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::out_edges(node_handles nodes) const {
	edge_handles result;
	for (auto n : nodes) {
		auto range = out_edges(n);
		result.insert(result.end(), range.begin(), range.end());
	}
	std::sort(result.begin(), result.end());
	auto newEnd = std::unique(result.begin(), result.end());
	result.resize(std::distance(result.begin(), newEnd));
	return result;
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline typename boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::node_handle boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::source(edge_handle edge) const {
	return ::boost::source(edge, *this);
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline typename boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::node_handle boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::target(edge_handle edge) const {
	return ::boost::target(edge, *this);
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline typename boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::edge_nodes  boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::nodes(edge_handle edge) const {
	return {source(edge), target(edge)};
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline unsigned int boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::num_nodes() const {
	return ::boost::num_vertices(*this);
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline unsigned int boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::num_edges() const {
	return ::boost::num_edges(*this);
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline void boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::forall_nodes(node_visitor visitor) {
	for (auto n : nodes()) visitor(*this, n);
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline void boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::forall_edges(edge_visitor visitor) {
	for (auto e : edges()) visitor(*this, e);
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline void boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::forall_out_edges(node_handle node, edge_visitor visitor) {
	for (auto e : out_edges(node)) visitor(*this, e);
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline void boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::prune_edges(edge_predicate pred) {
	::boost::remove_edge_if(std::bind(pred, std::ref(*this), std::placeholders::_1), *this);
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
unsigned int boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::connected_components(coloring& components) const {
	components.resize(num_nodes());
	return ::boost::connected_components(*this, &components[0]);
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
typename boost_graph<type,multi_edge, node_prop, edge_prop, graph_prop>::components boost_graph<type,multi_edge, node_prop, edge_prop, graph_prop>::connected_components() {
	coloring coloring;
	auto count = connected_components(coloring);
	components components(count);
	unsigned int v = 0;
	for (auto vertex : nodes()) {
		components[coloring[v++]].push_back(vertex);
	}
	return components;
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
typename boost_graph<type,multi_edge, node_prop, edge_prop, graph_prop>::edge_handles boost_graph<type,multi_edge, node_prop, edge_prop, graph_prop>::edges(component& component) {
	edge_handles result;
	forall_edges([&] (boost_graph& g, edge_handle e) {
		node_handle src, tgt;
		std::tie(src, tgt) = nodes(e);
		if (std::find(component.begin(), component.end(), src) != component.end()
		&&  std::find(component.begin(), component.end(), tgt) != component.end()) result.push_back(e);
	});
	return result;
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
void boost_graph<type,multi_edge, node_prop, edge_prop, graph_prop>::breadth_first_search(node_handle start, node_visitor node_visitor, node_pair_predicate edge_visitor) {
	bfs_visitor vis(node_visitor, edge_visitor, this);
	::boost::breadth_first_search(*this, start, visitor(vis));
}

//template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
//void boost_graph<type,multi_edge, node_prop, edge_prop, graph_prop>::depth_first_search(node_handle start, node_visitor node_visitor, node_pair_predicate edge_visitor) {
	//dfs_visitor vis(node_visitor, edge_visitor, this);
	//::boost::depth_first_search(*this, start, visitor(vis));
//}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
void boost_graph<type,multi_edge, node_prop, edge_prop, graph_prop>::traverse_level(node_handle start, uint32_t depth, node_visitor node_visitor) {
	std::deque<node_handle> queue(1, start), levels(1, 0);

	while (queue.size()) {
		node_handle node = queue.front();
		uint32_t    level = levels.front();
		queue.pop_front();
		levels.pop_front();
		if (level == depth) node_visitor(*this, node);
		if (level < depth) {
			forall_out_edges(node,
					           [&] (auto, auto edge) {
										queue.push_back(this->target(edge));
										levels.push_back(level + 1);
			                 });
		}
	}
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
template <class Archive>
inline void boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::serialize(Archive& ar, const unsigned int version) {
	base* b = dynamic_cast<base*>(this);
	ar & (*b);
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::bfs_visitor::bfs_visitor(node_visitor node_visitor, node_pair_predicate edge_visitor, boost_graph* graph) : ::boost::default_bfs_visitor(), node_visitor_(node_visitor), edge_visitor_(edge_visitor), graph_(graph) {
	for (auto n : graph->nodes()) {
		valid_[n] = true;
		visited_[n] = false;
	}
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline void boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::bfs_visitor::examine_vertex(node_handle n, boost_graph) {
	last_node_ = n;
	visited_[n] = true;
	if (valid_[n] && node_visitor_) node_visitor_(*graph_, n);
}

template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
inline void boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::bfs_visitor::examine_edge(edge_handle e, boost_graph g) {
	node_handle src = g.source(e), tgt;
	if (src != last_node_) {
		src = g.target(e);
		tgt = g.source(e);
	} else {
		tgt = g.target(e);
	}
	if (!valid_[src]) {
		valid_[tgt] = false;
		return;
	}
	if (edge_visitor_) {
		bool cont = !visited_[tgt] && edge_visitor_(*graph_, src, tgt);
		if (!cont) valid_[tgt] = false;
	}
}

//template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
//inline boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::dfs_visitor::dfs_visitor(node_visitor node_visitor, node_pair_predicate edge_visitor, boost_graph* graph) : ::boost::default_dfs_visitor(), node_visitor_(node_visitor), edge_visitor_(edge_visitor), graph_(graph) {
	//for (auto n : graph->nodes()) {
		//valid_[n] = true;
		//visited_[n] = false;
	//}
//}

//template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
//inline void boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::dfs_visitor::examine_vertex(node_handle n, boost_graph) {
	//last_node_ = n;
	//visited_[n] = true;
	//if (valid_[n] && node_visitor_) node_visitor_(*graph_, n);
//}

//template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
//inline void boost_graph<type, multi_edge, node_prop, edge_prop, graph_prop>::dfs_visitor::examine_edge(edge_handle e, boost_graph g) {
	//node_handle src = g.source(e), tgt;
	//if (src != last_node_) {
		//src = g.target(e);
		//tgt = g.source(e);
	//} else {
		//tgt = g.target(e);
	//}
	//if (!valid_[src]) {
		//valid_[tgt] = false;
		//return;
	//}
	//if (edge_visitor_) {
		//bool cont = !visited_[tgt] && edge_visitor_(*graph_, src, tgt);
		//if (!cont) valid_[tgt] = false;
	//}
//}

} // damogran
