#include<computation_graph.h>

namespace tiramisu_torch{
    /** Constructors **/ 
    tiramisu_torch::computation_graph::computation_graph(std::unordered_set<cg_node *> nodes, cg_node* root){
        this->nodes = nodes; 
        this->root = root; 
    }

    tiramisu_torch::computation_graph::computation_graph(cg_node* root){
        this->nodes.insert(root); 
        this->root = root; 
    }

    tiramisu_torch::super_function::super_function(function f){
        this->functions.insert(f); 
    }

    tiramisu_torch::cg_node::cg_node(std::string name, computation_graph* owning_graph ){
        this->name = name; 
        this->owning_graph = owning_graph;  
    }

    /** Utilities **/

    /**
     * Print the inputs of one node : 
     * index. (input Name) : (type) (size) 
     **/
    void tiramisu::cg_node::print_inputs(int * index){
        for (auto& input : this->inputs) {
                std::cout << index << '.' + input->input.get_name(); 
                index++; 
        }
    }

    /**
     * Print the operator of one node : 
     * index. (operator name) : (type) (name)(input number)
     * If show_computations is true : 
     *      print each computation expression of the function 
     **/
    void tiramisu::super_function::print_function(int * index, bool show_computations){
        for (auto& func : this->functions){
               std::cout << index << '.' + func.get_name();
               index++; 
               if (show_computations){
                   std::vector<computation *> computations = func.get_computations(); 
                   int index_comp = 1 ; 
                    for( auto& comp : computations){
                        std::cout << "\t" << index << '.' << index_comp << comp->get_name(); 
                        index_comp++; 
                    }
               }
        }
    }

    /** 
     * Show the graph structure: 
     * Traverse the graph, for each node : 
     *      print its inputs 
     *      print the operator 
     *      print its outputs 
     * go to the next node of the graph using nextNode()
     **/ 
    void tiramisu::computation_graph::show_graph(bool show_computations){
        root = this->root; 
        int index = 1; 
        while (root != NULL){
            // Print all its inputs 
            root->print_inputs(&index); 

            // prints the operator 
            super_function fct = root->getOperator();
            fct.print_function(&index, show_computations); 

            // Prints all outputs 

            root = root->getNext(); 
        }
    }

    /** 
     * Add node to the graph
     **/ 
    void tiramisu::computation_graph::add_node(cg_node node){
        this->nodes.insert(&node); 
    }

    /**
     * Add input node to an operation node 
     **/
    void tiramisu::cg_node::add_input(cg_node input){
        if (this->getType() == node_op) {
            this->inputs.insert(this->inputs.end(), &input); 
        }else{
            std::cout << "Error : Inserting an input node in a non operation node. "; 
        }  
    }


    /**
     * Add output node to an operation node 
     **/
    void tiramisu::cg_node::add_output(cg_node output){
        if (this->getType() == node_op) {
            this->outputs.insert(this->outputs.end(), &output); 
        }else{
            std::cout << "Error : Inserting an output node in a non operation node. "; 
        }  
    }

    std::string tiramisu::cg_node::get_name(){
        return this->name;
    }


    computation_graph* tiramisu::cg_node::getOwningGraph(){
        return this->owning_graph; 
    }

    void tiramisu::cg_node::setType(node_t type){
        this->type = type; 
    }

    /**
     * Set the next node in graph. 
     **/
    void tiramisu::cg_node::setNextInGraph(cg_node* next){
        if (this->getType() == node_op) {
            this->next_in_graph = next; 
        }else{
            std::cout << "Error : The next node in graph must be an operation or block node. "; 
        }
    }

    cg_node* tiramisu::cg_node::getNext(){
        return this->next_in_graph; 
    }

    node_t tiramisu::cg_node::getType(){
        return this->type; 
    }

    super_function tiramisu::cg_node::getOperator(){
        return this->opt; 
    }

    /**
     * The arguments is a hash table that contains the name and values of each argument. 
     **/
    void tiramisu::cg_node::addArgument(std::string name, primitive_t value ){
        this->attr.insert({name, value}); 
    }

    cg_node * tiramisu::cg_node::get_outputs(){
        return this->outputs; 
    }

}