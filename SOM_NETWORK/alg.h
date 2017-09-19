#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <time.h>

// #define INDEX_SIZE

typedef double value_type;

/* Main database structure */

typedef struct data{	//Structure sur a line in a database
	value_type * v; 	//Normalized vector's data
	char * name;		//Nametag
	double norm;		//Norm for potential reversing
} data;

typedef struct database{//Main database structure
	int db_capacity;	//Database's size
	int v_capacity;		//Database's vector size
	int * index_vec;	//Shuffled database index for learning
	value_type * middle_v;//Database's middle vector
	struct data * data;	//Database's data structure vector
} database;

/* Grid structure */

typedef struct node{	//One neuron
	value_type * value;	//Value of the node
	int * nametag_probability; //Vector that holds nametag probabilities
	double act;			//Euclidian distance between testing vector & neuron
	char * name;		//Nametag for nametagging phase
} node;

typedef struct grid{	//Grid itself
	int width; 			//Grid's width
	int height;			//Grid's height
	struct node ** neurons;//Corresponding neurons
} grid;

typedef struct t_ratio{	//Structure for grid's dimensions
	int width;			//Neuron grid's weight
	int height;			//Neuron grid's height
} t_ratio;

typedef struct pair{	//Structure for grid's dimensions
	int val;			//Neuron grid's weight
	int rank;			//Neuron grid's height
} pair;

/*Linked list that'll help to find BMU , it'll gather all
 *minimums to choose who'll win at the end*/

typedef struct bmu{		//Linked list element
	int bmu_l;			//Line coordinate of BMU(Winning neuron)
	int bmu_c;			//Column coordinate of BMU(Winning neuron)
	double act;			//Activation value (Euclidian distance between Test_vector and Neuron_vector)
	struct bmu * next;	//Pointer on next element
} bmu;
typedef struct bmu * liste;

typedef struct L_Hdr{	//Linked list header
	int nb_c; 			//Size of list
	struct bmu * L; 	//Pointer at the beginning of list
	struct bmu * last;	//Pointer at the end of the list
} l_Hdr;

pair max2_rank(int, int, int, int); // Returns highest value with specified rank
double euclidian_distance(value_type *, value_type *, int);// Euclidian distance between two vectors
void normalize(data, int);// Normalizes a data
database index_shuffling(database);// Shuffles a vector that'll be used to access database during learning phase
database initialise_database(FILE *);// Allocating memory of database structure
void load_database(FILE *, int, database);// Loading values in database structure from file.txt
void show_database(database);// Tool function used to show database's insides
void read_vector(database, int);// Tool function used to print vectors
t_ratio get_dividers(int);// Function gathering all dividers for a specified number (Used for network's proportions).
grid init_network(database, double);// Allocating memory and initializing network's values
void show_network(grid, database,int);// Tool function used with two types, type == 0 will show network's insides , type == 1 will show nametagged map
void add_elem_end_link(l_Hdr *, int, int, double);// Adds an element to a linked list
void show_link(l_Hdr *);// Tool function used to show the insides of a linked list
bmu * get_specified_bmu(l_Hdr *, int);// Function used to get a linkined list element at specified rank
void erase_link(l_Hdr *);// Function used for deleting a linked list (free's are included)
int find_neigh_arc(int);// Function used to find best starting neighbouring arc (Phase 1 : 50 % number of neurons)
grid learning_phase_1(database, grid, double, int);// Learning phase 1
grid learning_phase_2(database, grid, double, int, int);// Learning phase 2
grid tagging(database my_database, grid network);// Tags the nametag field of each neurons
/*Regular structures*/

/* Questions:
- Nametag for neuron?
- Good random for middle vector?
*/