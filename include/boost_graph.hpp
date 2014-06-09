#ifndef DAMOGRAN_BOOST_GRAPH_H_
#define DAMOGRAN_BOOST_GRAPH_H_
#ifdef USE_BOOST

#include <memory>
#include <tuple>
#include <deque>
#include <functional>
#include <type_traits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/breadth_first_search.hpp>

#include <boost/serialization/access.hpp>

#include "range.hpp"

namespace damogran {

// use these as type param below
struct dir_tag   { typedef ::boost::directedS      type; };
struct undir_tag { typedef ::boost::undirectedS    type; };
struct bidir_tag { typedef ::boost::bidirectionalS type; };

typedef ::boost::no_property null_prop;

// shortener t'defs
template <bool multi_edge>
using out_edge_list = typename std::conditional<multi_edge, boost::vecS, boost::setS>::type;
template <class type, bool multi_edge, class node_prop, class edge_prop, class graph_prop>
using graph_base = ::boost::adjacency_list<out_edge_list<multi_edge>, boost::vecS, typename type::type, node_prop, edge_prop, graph_prop>;
template <class base>
using graph_traits = ::boost::graph_traits<base>;



template <class type, bool multi_edge, class node_prop = null_prop, class edge_prop = null_prop, class graph_prop = null_prop>
class boost_graph : public graph_base<type, multi_edge, node_prop, edge_prop, graph_prop> {
	public:
		typedef std::shared_ptr<boost_graph> ptr;
		typedef std::weak_ptr<boost_graph>   wptr;
		typedef std::shared_ptr<const boost_graph> const_ptr;
		typedef std::weak_ptr<const boost_graph>   const_wptr;

		typedef graph_base<type, multi_edge, node_prop, edge_prop, graph_prop>   base;
		typedef graph_traits<base>                                         traits;

		typedef typename traits::vertex_descriptor                          node_handle;
		typedef typename traits::edge_descriptor                            edge_handle;

		typedef std::vector<node_handle>                                     node_handles;
		typedef std::vector<edge_handle>                                     edge_handles;

		typedef typename traits::vertex_iterator                            node_iter;
		typedef typename traits::edge_iterator                              edge_iter;
		typedef typename traits::out_edge_iterator                          out_edge_iter;
		typedef range<node_iter>                                            node_range;
		typedef range<edge_iter>                                    edge_range;
		typedef range<out_edge_iter>                                 out_edge_range;

		typedef std::pair<edge_handle, bool>                                 edge_add_result;
		typedef std::pair<node_handle, node_handle>                           edge_nodes;

		typedef std::function<void (boost_graph&, node_handle)>                    node_visitor;
		typedef std::function<void (boost_graph&, edge_handle)>                    edge_visitor;
		typedef std::function<bool (boost_graph&, edge_handle)>                    edge_predicate;
		typedef std::function<bool (boost_graph&, node_handle, node_handle)>        node_pair_predicate;

		typedef std::vector<unsigned int>                                   coloring;
		typedef std::vector<node_handle>                                     component;
		typedef std::vector<component>                                      components;

	public:
		boost_graph();
		virtual ~boost_graph();

		node_handle     add_node();
		edge_add_result  add_edge(node_handle src, node_handle tgt);

		node_range      nodes() const;
		edge_range      edges() const;

		out_edge_range   out_edges(node_handle node) const;
		edge_handles    out_edges(node_handles nodes) const;
		node_handle     source(edge_handle edge) const;
		node_handle     target(edge_handle edge) const;
		edge_nodes      nodes(edge_handle edge) const;

		unsigned int   num_nodes() const;
		unsigned int   num_edges() const;

		void           forall_nodes(node_visitor visitor);
		void           forall_edges(edge_visitor visitor);
		void           forall_out_edges(node_handle node, edge_visitor visitor);

		void           prune_edges(edge_predicate pred);

		// connected components
		unsigned int   connected_components(coloring& components) const;
		components     connected_components();
		edge_handles    edges(component& component);

		/**
		 *  Performs a breadth-first-search on this graph.
		 *
		 *  @param start Node handle used as traversal root.
		 *  @param node_visitor Visitor that gets called everytime a node is discovered.
		 *  @param edge_visitor Predicate that gets called everytime an edge is discovered. Returning false cancels the corresponding path.
		 */
		void breadth_first_search(node_handle start, node_visitor node_visitor = nullptr, node_pair_predicate edge_visitor = nullptr);

		/**
		 *  Performs a depth-first-search on this graph.
		 *
		 *  @param start Node handle used as traversal root.
		 *  @param node_visitor Visitor that gets called everytime a node is discovered.
		 *  @param edge_visitor Predicate that gets called everytime an edge is discovered. Returning false cancels the corresponding path.
		 */
		//void depth_first_search(node_handle start, node_visitor node_visitor = nullptr, node_pair_predicate edge_visitor = nullptr);

		/**
		 *  Traverses all nodes on the given tree depth. Makes only sense for trees.
		 *
		 *  @param start Node handle used as traversal root.
		 *  @param depth Tree depth to traverse.
		 *  @param node_visitor Visitor that gets called everytime a node is discovered.
		 */
		void traverse_level(node_handle start, uint32_t depth, node_visitor node_visitor = nullptr);

	protected:
		// serialization
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive& ar, const unsigned int version);

		struct bfs_visitor : public ::boost::default_bfs_visitor {
			bfs_visitor(node_visitor node_visitor, node_pair_predicate edge_visitor, boost_graph* graph);
			void examine_vertex(node_handle n, boost_graph g);
			void examine_edge(edge_handle e, boost_graph g);

			node_visitor                 node_visitor_;
			node_pair_predicate           edge_visitor_;

			boost_graph*                      graph_;

			node_handle                  last_node_;
			std::map<node_handle, bool>  valid_;
			std::map<node_handle, bool>  visited_;

		};

		//struct dfs_visitor : public ::boost::default_dfs_visitor {
			//dfs_visitor(node_visitor node_visitor, node_pair_predicate edge_visitor, boost_graph* graph);
			//void examine_vertex(node_handle n, boost_graph g);
			//void examine_edge(edge_handle e, boost_graph g);

			//node_visitor                 node_visitor_;
			//node_pair_predicate          edge_visitor_;

			//boost_graph*                 graph_;

			//node_handle                  last_node_;
			//std::map<node_handle, bool>  valid_;
			//std::map<node_handle, bool>  visited_;

		//};
};

} // damogran

#include "boost_graph.ipp"


#endif // USE_BOOST
#endif /* DAMOGRAN_BOOST_GRAPH_H_ */
