
#include <map>
#include <string.h>
#include <stdint.h>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <types.h>
#include "Halide.h"

namespace tiramisu_torch
{

class function{
    std::string name; 

    public : 
        function(std::string name); 

        std::string get_name(); 
}; 

class super_function {
	/**
	 * Operator of the Computation graph will represent 
	 * C Functions like (Relu, MaxPool, Conv ..) 
	 **/
	std::unordered_set<function> functions; 

	public: 
		super_function(function f); 

		void print_function(int * index, bool show_computations); 
};



class computation_graph
{
	/**
	  * Computation graph root nodes.
	  * Used only to keep track of the allocated nodes 
	  * Traversal is done with cg_node
	  */
	std::unordered_set<cg_node *> nodes;

	/**
	 * Set of blocks.
	 * Used to keep track of all the allocated blocks.
	 **/
	std::unordered_set<cg_block *> blocks;

	/**
	 * root node of the graph.
	 * Needed to begin the traversal for the show_graph function 
	 * and the graph execution.
	 * Need to be either an operation node or a block node (IF/Loop)
	 **/
	cg_node * root; 




	public :
		/**
		 * Creates a computation graph with a set of nodes 
		 * and a root node to begin the traversal.
		 **/
		computation_graph(std::unordered_set<cg_node *> nodes, cg_node* root); 

		computation_graph(cg_node* root); 

		/** Initiate a computation graph **/ 
		computation_graph(); 

		/**
		 * Show the computation graph 
		 * */ 
		void show_graph(bool show_computations); 

		// Add a node to the graph 
		void add_node(cg_node node);

		/**
		 * Execute the graph and apply the suitable operator fusion. 
		 **/ 
		void graph_executor(); 

		

};

/**
  * Computation graph node.
  */
class cg_node
{
	protected: 
		/** 
		 * Operator that corresponds to the node (functions)
		 **/
		super_function opt ;

		/**
		 * List of inputs nodes 
		 **/
		std::vector<cg_node *> inputs;

		/**
		 * List of outputs nodes 
		 **/
		std::vector<cg_node *> outputs;


		/**
		 * Name of the Node
		 **/
		std::string name;

		/** 
		 * ID
		 **/
		int id_node; 

		/**
		 * List of attributes : 
		 **/
		std::unordered_map<std::string, tiramisu_torch::primitive_t > attr; 


		/**
		 * The graph that this node is assigned to 
		 **/
		computation_graph* owning_graph; 

		/**
		 * Node Type : 
		 * 		t_input
		 * 		t_op
		 * 		t_output
		 * 		t_block_if
		 * 		t_block_loop
		 **/
		tiramisu_torch::node_t type; 

		/**
		 * This needs to be discussed. When traversing the graph 
		 * each node has its next node associated to it. 
		 * The concept of Blocks (IF/Loop) is not yet defined, can we define it as a special node type ? 
		 **/
		cg_node *  next_in_graph; 

		Halide::Buffer<float> inputs; 

	public: 
		/**
		 * Default Constructor. 
		 * No node can be created before its graph. 
		 **/
		cg_node(std::string name, computation_graph* owening_graph ); 

		/**
		 * Add an input to the node
		 **/ 
		void add_input(cg_node input); 

		/**
		 * Add an output to the node
		 **/ 
		void add_output(cg_node output); 


		std::string get_name(); 

		/**
		 * Get the computation graph associated with the node.
		 **/ 
		computation_graph* getOwningGraph(); 

		/**
		 * Set node type 
		 **/
		void setType(node_t type); 

		/**
		 * Set the next node in graph (operation node only).
		 **/
		void setNextInGraph(cg_node* next); 

		/**
		 * Get the next node in graph.
		 * Used to iterate over the graph
		 **/ 
		cg_node* getNext(); 

		node_t getType(); 
	
		/** 
		 * Print inputs nodes
		 **/
		void print_inputs(int * index); 

		super_function getOperator(); 

		void addArgument(std::string name, primitive_t value ); 

		cg_node * get_outputs(); 
};

/**
 * Block of multiple sequential operations. 
 * While it is possible to work with a set of unordered nodes, It is 
 * much easier to debug and understand Blocks when there is a specific 
 * canonical order for all of the nodes.
 * This doesn't mean that the optimization passes don't change the order when
 * it would improve the performance. 
 * 
 * When Nodes are inserted into a Graph, they are inserted at a special "insertion point" 
 * that is part of the state of the Graph. On construction, this will go to the end of the Graph.
 **/
class cg_block
{	


};

class cg_if_block : cg_node {

	cg_node* cond() const {
    	return this->inputs[0];
  	}

	cg_node* thenBlock() const {
		return this->next_in_graph[0];
	}
	
	cg_node* elseBlock() const {
		return this->next_in_graph[1];
	}

	cg_node* thenOutputs() const {
		return thenBlock()->get_outputs(); 
	}
	cg_node * elseOutputs() const {
		return elseBlock()->get_outputs();
	}

};


class cg_loop_block : cg_node {

	// TODO : Define specific functions for the loop block : 
	// 		InputCond()
	//		CurrentIter()
	//		nextCond()
	// 		Ajust indices 

};

}

#endif
