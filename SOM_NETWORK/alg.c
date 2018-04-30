#include "alg.h"


/*Made by Gaëtan GOUSSEAUD , last edited 10/05/17, my email: gousseaud.gaetan.pro@gmail.com*/

//Professor : jjmariage@free.fr

//This define holds the number of different nametags in the database (temporary depending on time)
#define NT_INDEX_SIZE 3



/* max2 tool */
pair max2_rank(int a, int rank_a , int b, int rank_b){
	pair maxed;
	if(a > b){
		maxed.rank = rank_a;
		maxed.val = a;
		return maxed;
	}
	maxed.rank = rank_b;
	maxed.val = b;
	return maxed;
}

/* Euclidian distance (Heuristique) */
double euclidian_distance(value_type * vector_1 , value_type * vector_2, int vector_size){
	int value_iterator;
	double res=0;
	for(value_iterator = 0 ; value_iterator < vector_size ; value_iterator++){
		res += pow((vector_1[value_iterator] - vector_2[value_iterator]),2);
	}
	res = sqrt(res);
	return res;
}

void normalize(data my_vector, int sizeof_vector){
	int vector_ite;
	double norm = 0;
	for(vector_ite = 0 ; vector_ite < sizeof_vector ; vector_ite++)
		norm += pow(my_vector.v[vector_ite],2);
	norm = sqrt(norm);
	for(vector_ite = 0 ; vector_ite < sizeof_vector ; vector_ite++)
		my_vector.v[vector_ite] = my_vector.v[vector_ite]/norm;
}


/* index_shuffling function randomizes an integer vector for the purpose of index shuffling aka iteration database */
database index_shuffling(database my_database){
	int database_ite;
	int index_swap;
	int rand_index;
	for(database_ite= 0; database_ite < my_database.db_capacity; database_ite++){
		// printf("-------------------------------\n");
		rand_index = rand()%my_database.db_capacity;
		// printf("rand_index is: %d\n",rand_index);
		index_swap = my_database.index_vec[database_ite];
		// printf("index_swap is :%d\n",index_swap);
		my_database.index_vec[database_ite] = my_database.index_vec[rand_index];
		// printf("my_database.index_vec[%d] is :%d\n",database_ite, my_database.index_vec[rand_index]);		
		my_database.index_vec[rand_index] = index_swap;
		// printf("my_database.index_vec[rand_index(%d)] is :index_swap(%d)\n",rand_index, index_swap);
	}
	return my_database;
}

database initialise_database(FILE * fichier){
	database my_database;
	int database_ite,index_it;
	fscanf(fichier,"%d %d\n",&my_database.db_capacity,&my_database.v_capacity);//Charge la taille de la base de données et celle de ses vecteurs.
	//printf("db_capacity= %d v_capacity= %d \n ", my_database.db_capacity, my_database.v_capacity);
	my_database.data =(struct data *) malloc(my_database.db_capacity * sizeof(*my_database.data));//Alloue le tableau de structure data
	
	if(my_database.data == NULL){ //Verifies if malloc failed
		printf("ERROR: DATABASE DATA IS NULL\n");
		my_database.db_capacity = 0;
		exit(0);
	}

	for(database_ite = 0; database_ite < my_database.db_capacity ; database_ite++){
			my_database.data[database_ite].v = (value_type *) malloc(my_database.v_capacity * sizeof(*my_database.data[database_ite].v)); //Allocating memory for each database's line
		}
	
	my_database.index_vec = (int *) malloc(my_database.db_capacity * sizeof(*my_database.index_vec)); //Index's vector of database's size
	
	for(index_it = 0 ; index_it < my_database.db_capacity ; index_it++)
		my_database.index_vec[index_it] = index_it; //Initializes index's vector with indexes
	
	my_database.middle_v = (value_type *) malloc(my_database.v_capacity * sizeof(*my_database.middle_v));
	memset(my_database.middle_v,0, my_database.v_capacity);
	return my_database;
}

/* Charge les données dans la structure */
void load_database(FILE * fichier, int car,database my_database){
	char  line[100];//Vector used to gather a line
	int i, linenum , vector_it = 0;
	const char s[2] = ","; //Delimiter Token
	char * token;//Token for tokenizing
	for(linenum= 0 ; car != EOF && linenum < my_database.db_capacity; linenum++)
	{
		for(i=0; ((car=fgetc(fichier)) != '\n') && (car != EOF); i++)
			line[i]=car;//Line gathering

		line[i]='\0';//Finalise string
	   token = strtok(line, s);//Getting first field of line for token
	   //printf("initial token is : %s\n",token);
	   // assert(token != NULL);

	   for(vector_it=0 , my_database.data[linenum].norm = 0; token != NULL  && vector_it < my_database.v_capacity; vector_it++) //Parcours chaque champs du string entre les délimiteurs
	   {
	      	//printf( "Supposed to be double: %s\n", token );
	    	my_database.data[linenum].v[vector_it] = atof(token);//Gathering values in the database
	   	   	token = strtok(NULL, s);
	   }
	    normalize(my_database.data[linenum],my_database.v_capacity);//Normalizing current vector in the database   
	   	for(vector_it=0 ; vector_it < my_database.v_capacity ; vector_it++)
	   		my_database.middle_v[vector_it] += my_database.data[linenum].v[vector_it];//Calculating middle vector
	    //printf( "Supposed to be string: %s\n", token );
		my_database.data[linenum].name = strdup(token);//Gathering current vector's nametag
	   	token = strtok(NULL, s);
	}
	for(vector_it=0 ; vector_it < my_database.v_capacity ; vector_it++)
		my_database.middle_v[vector_it] /= my_database.db_capacity;
}

void show_database(database my_database){
	int ite_line, ite_col;
	//Parcours de la base de données
	for(ite_line=0 ; ite_line < my_database.db_capacity ; ite_line++){
		for(ite_col = 0 ; ite_col < my_database.v_capacity ; ite_col++){


		printf("Line: %d Content(double): %lf \n", ite_line,my_database.data[ite_line].v[ite_col]);
		}
			// strlength = strlen(my_database.data->name);
			// for(i=0; i < strlength ; i++)
		printf("Line: %d Content(string): %s\n " , ite_line , my_database.data[ite_line].name);
	}
}

void read_vector(database my_database, int vec_type){
	int i;
	if(vec_type == 1){
		for(i=0; i<my_database.db_capacity ; i++){
			printf("Index_vector[%d] is: %d\n",i , my_database.index_vec[i]);
		}
	}else if(vec_type== 2){
		for(i=0; i<my_database.v_capacity ; i++){
			printf("middle_vector[%d] is: %lf\n",i , my_database.middle_v[i]);
		}
	}
}

t_ratio get_dividers(int n){	//This function was inspired by a post on StackOverflow
   int i=2;
  	t_ratio ratio;
   while(i <= sqrt(n))
    {
        if(n%i==0) {
            // printf("%d,",i);
            ratio.height = i;
            if (i != (n / i)) {
                // printf("%d,",n/i);
                ratio.width = n/i;
            }
        } 
        i++;
    }
    return ratio;
}


grid init_network(database my_database, double random_fork){
	// printf("random_fork is : %lf\n",random_fork);
	grid network; //Neural network getting initialized
	t_ratio ratio; //Ratio used for grid dimensions
	float nb_neurons = 0; //Is float for future regulation
	int line_iterator,column_iterator,value_iterator;
	double random_fork_value=0.0;
	nb_neurons = 5 * sqrt(my_database.db_capacity);
	// srand(0);
	if(nb_neurons == 0){
		printf("Network will be empty: ERROR init_network 1\n");
		exit(0);
	}
	//Regulation
	nb_neurons/=10;//Get's unit behind coma
	nb_neurons= ceil(nb_neurons);//get's one unit up
	nb_neurons *=10;//Getting back to real number state

	//Getting ratio for network grid
	ratio = get_dividers((int) nb_neurons);
	network.height = ratio.height;
	network.width = ratio.width;
	printf("grid's properties \n ratio width:%d  ratio height:%d  ratio percentage:%lf \n",ratio.width, ratio.height , (double)ratio.width/ratio.height);

	//Grid will be neurons[Line][Column]
	network.neurons = (struct node **) malloc(network.height * sizeof(*(network.neurons)));
	for(line_iterator = 0 ; line_iterator < network.height ; line_iterator++)
		network.neurons[line_iterator] = (struct node *) malloc(network.width * sizeof(*network.neurons[line_iterator]));

	//Malloc of neuron's fields
	for(line_iterator = 0 ; line_iterator < network.height ; line_iterator++){
		for(column_iterator = 0 ; column_iterator < network.width; column_iterator++){
			network.neurons[line_iterator][column_iterator].value = (value_type *) malloc(my_database.v_capacity * sizeof(*network.neurons[line_iterator][column_iterator].value));
			network.neurons[line_iterator][column_iterator].nametag_probability = (int *) malloc(NT_INDEX_SIZE * sizeof(*network.neurons[line_iterator][column_iterator].nametag_probability));
		}
	}
	
	//Network value initialization
	for(line_iterator = 0 ; line_iterator < network.height ; line_iterator++){
		for(column_iterator = 0 ; column_iterator < network.width ; column_iterator++){
			memset(network.neurons[line_iterator][column_iterator].nametag_probability, 0 , 3);
			//printf("--------------------------------------------------\n");
			for(value_iterator = 0 ; value_iterator < my_database.v_capacity ; value_iterator++){
				random_fork_value = (double) rand()/RAND_MAX*(2*random_fork)-random_fork; // Random between -random_fork and random_fork
				// printf("random_fork_value: %lf\n",random_fork_value);
				network.neurons[line_iterator][column_iterator].value[value_iterator] = (double)((my_database.middle_v[value_iterator]) + random_fork_value); // Initializing around middle vector with random_fork_value
				//printf("network.neurons[%d][%d].value = %lf\n", line_iterator, column_iterator , network.neurons[line_iterator][column_iterator].value[value_iterator]);
			}
		}
	}
	return network;
}

void show_network(grid network, database my_database,int type){
	int line_iterator,column_iterator,value_iterator;
	if (type == 0){
		printf("--------------------------------------------------\n");
		printf("--------------------------------------------------\n");
		printf("Width: %d  Height: %d  Alpha:     Phase:      Iterations:       \n\n", network.width, network.height);
		for(line_iterator = 0 ; line_iterator < network.height ; line_iterator++){
			for(column_iterator = 0 ; column_iterator < network.width ; column_iterator++){
				printf("--------------------------------------------------\n");

				for(value_iterator = 0 ; value_iterator < my_database.v_capacity ; value_iterator++){
					printf("neuron n= %d | %d value = %lf \n", (network.width * line_iterator + column_iterator), value_iterator , network.neurons[line_iterator][column_iterator].value[value_iterator]);
				}
				// printf("%lf ",network.neurons[line_iterator][column_iterator].act);
			printf("\n");
			}
		}
	}else if(type == 1){
		printf("SE : Iris-setosa , VI : Iris-virginica , VE : Iris-versicolor , .. : Not stimulated\n");
		for(line_iterator = 0 ; line_iterator < network.height ; line_iterator++){
			for(column_iterator = 0 ; column_iterator < network.width ; column_iterator++){
				printf("%s ",network.neurons[line_iterator][column_iterator].name);
			}
			printf("\n");
		}
	}
}

/* LINKED LIST OPERATIONS */
void add_elem_end_link(l_Hdr * header_list , int pos_l , int pos_c , double act){
	liste new_elem;

	new_elem =(bmu *) malloc(sizeof(bmu));
	// printf("new_elem malloc'ed\n");
	new_elem->bmu_c = pos_c;
	new_elem->bmu_l = pos_l;
	new_elem->act = act;
	new_elem->next = NULL;

	if(header_list->L == NULL){ //Adding element to an empty list
		// printf("list is NULL\n");
		header_list->L = new_elem;
		header_list->nb_c ++;
		header_list->last = new_elem;
		// return header_list;
	}else{
		// printf("Adding new element to existing...\n");
		header_list->last->next = new_elem;
		header_list->last = new_elem;
		header_list->nb_c ++;
		// return header_list;
	}
}

void show_link(l_Hdr * header_list){
	int i;
	liste curr_elem;
	curr_elem = header_list->L;
	if(curr_elem == NULL){
		printf("WARNING(show_link): curr_elem NULL exception\n");
		exit(0);
	}
	for(i = 0; i < header_list->nb_c ; i++){
		printf("Node:%d pos_c: %d pos_l: %d act: %lf",i , curr_elem->bmu_c, curr_elem->bmu_l, curr_elem->act);
		curr_elem = curr_elem->next;
	}
	printf("\n");
}

bmu * get_specified_bmu(l_Hdr * header_list , int rank){
	if(rank > header_list->nb_c){
		printf("Accessing bmu out of range\n");
		exit(0);
	}

	bmu * current_bmu;
	int iterator;
	current_bmu = header_list->L;


	for(iterator = 1 ; iterator != rank; iterator++){
		current_bmu = current_bmu->next;
	}
	return current_bmu;
}

void erase_link(l_Hdr * linked_list){ //This function was inspired by a post on StackOverflow
	bmu *current_bmu, *next_bmu;
	current_bmu = linked_list->L;
	while (current_bmu != NULL){
		// printf("Erasing link\n");
		next_bmu = current_bmu->next;
		free(current_bmu);
		current_bmu = next_bmu;
	}
	linked_list->L = NULL;
	linked_list->last = NULL;
	linked_list->nb_c = 0;
}

int find_neigh_arc(int nb_neurons){
	nb_neurons = ceil(nb_neurons/2);
	printf("Minimal Neural neighbouring :%d\n", nb_neurons);
	int neighbouring_arc;
	for(neighbouring_arc = 1; (((neighbouring_arc * 2)-1) * ((neighbouring_arc * 2)-1)) < nb_neurons  ; neighbouring_arc ++)
		printf("Finding arc, neighbouring_arc : %d  Test :   %d < %d   ? \n", neighbouring_arc , (((neighbouring_arc * 2)-1) * ((neighbouring_arc * 2)-1)) , nb_neurons);
		/* Will proceed this way:
		- Initialize at 1
		- Calculates the number of neurons within the neighbouring square
		- If that number is below the minimum , increment neighbouring arc
		- If that number is above or equal to the minimum , returns neighbouring arc.
		*/
	return neighbouring_arc -1;

}

grid learning_phase_1(database my_database, grid network, double alpha_init, int nb_iterations){
	//Represents the size of the neighbouring_arc
	int neighbouring_arc = find_neigh_arc(network.width * network.height);
	//Represents the neighbouring transition during phase 1
	int neighbouring_update = (int) nb_iterations / neighbouring_arc;
	//Iterators
	int iteration_iterator, neuron_line_iterator, neuron_column_iterator, value_iterator, database_iterator, neigh_line, neigh_col;

	double current_alpha;

	l_Hdr * bmu_link;
	bmu_link = (l_Hdr *) malloc(sizeof(l_Hdr));
	bmu * bmu_item;
	bmu_link->L = NULL;
	bmu_link->last = NULL;


	printf("Phase 1 : neighbouring_arc(%d) nb_iterations(%d) neighbouring_update(%d)\n", neighbouring_arc,nb_iterations,neighbouring_update);
	for (iteration_iterator = 0 ; iteration_iterator < nb_iterations ; iteration_iterator++){
			// printf("Iteration : %d\n", iteration_iterator);
			my_database = index_shuffling(my_database);
			// read_vector(my_database,1);
			current_alpha =(double) alpha_init * (1 - (iteration_iterator/nb_iterations));
		for(database_iterator = 0 ; database_iterator < my_database.db_capacity ; database_iterator ++){
			for(neuron_line_iterator = 0 ; neuron_line_iterator < network.height; neuron_line_iterator++){
				for(neuron_column_iterator = 0 ; neuron_column_iterator < network.width ; neuron_column_iterator++){
					network.neurons[neuron_line_iterator][neuron_column_iterator].act = euclidian_distance(my_database.data[my_database.index_vec[database_iterator]].v , network.neurons[neuron_line_iterator][neuron_column_iterator].value , my_database.v_capacity);
					if(bmu_link->L != NULL && (network.neurons[neuron_line_iterator][neuron_column_iterator].act < bmu_link->L->act)){
						//resets the list with new bmu
						erase_link(bmu_link);
						add_elem_end_link(bmu_link, neuron_line_iterator , neuron_column_iterator , network.neurons[neuron_line_iterator][neuron_column_iterator].act);
					}else if(bmu_link->L != NULL && (network.neurons[neuron_line_iterator][neuron_column_iterator].act == bmu_link->L->act)){
						//Adds an element to the bmu list
						add_elem_end_link(bmu_link, neuron_line_iterator , neuron_column_iterator , network.neurons[neuron_line_iterator][neuron_column_iterator].act);
					}else if(bmu_link->L == NULL){
						add_elem_end_link(bmu_link, neuron_line_iterator , neuron_column_iterator , network.neurons[neuron_line_iterator][neuron_column_iterator].act);						
					}
				}
			}
			//Randomly taking a bmu among the bmu_link
			bmu_item = get_specified_bmu(bmu_link , rand()%bmu_link->nb_c + 1);
			//printf("bmu_item->bmu_c : %d , bmu_item->bmu_l : %d\n" , bmu_item->bmu_c , bmu_item->bmu_l);
			//General learning
				for(neigh_line = (bmu_item->bmu_l - neighbouring_arc); neigh_line <= (bmu_item->bmu_l + neighbouring_arc) ; neigh_line++){
					if(neigh_line < 0)
						continue;
					else if(neigh_line >= network.height)
						break;
					for(neigh_col = (bmu_item->bmu_c - neighbouring_arc) ; neigh_col <= (bmu_item->bmu_c + neighbouring_arc) ; neigh_col++){
						if(neigh_col < 0)
							continue;
						else if(neigh_col >= network.width)
							break;
							for(value_iterator = 0 ; value_iterator < my_database.v_capacity ; value_iterator++){
								// printf("network.neurons[%d][%d].value[%d] = %lf (before)\n",neigh_line,neigh_col,value_iterator,network.neurons[neigh_line][neigh_col].value[value_iterator] );
								//printf("neigh_line : %d neigh_col : %d \n",neigh_line,neigh_col);
								network.neurons[neigh_line][neigh_col].value[value_iterator] += (current_alpha * (my_database.data[my_database.index_vec[database_iterator]].v[value_iterator] - network.neurons[neigh_line][neigh_col].value[value_iterator]));
								// printf("network.neurons[%d][%d].value[%d] = %lf (after)\n",neigh_line,neigh_col,value_iterator,network.neurons[neigh_line][neigh_col].value[value_iterator] );
						
						}
					}
				}	
		// printf("Erase link\n");
		erase_link(bmu_link);
		}
		//Reduces neighbouring arc based on iteration modulo
		if(neighbouring_arc > 1 && ((iteration_iterator + 1) %neighbouring_update) == 0){
			printf("Arc : %d -> %d at iteration %d\n",neighbouring_arc , neighbouring_arc -1, iteration_iterator);
			neighbouring_arc--;
		}
	}
	//free(bmu_link);
	return network;
}

grid learning_phase_2(database my_database, grid network, double alpha_init, int starting_neigh, int nb_iterations){
	//Represents the size of the neighbouring_arc
	int neighbouring_arc = starting_neigh;
	//Represents the neighbouring transition during phase 1
	int neighbouring_update = (int) nb_iterations / neighbouring_arc;
	//Iterators
	int iteration_iterator, neuron_line_iterator, neuron_column_iterator, value_iterator, database_iterator, neigh_line, neigh_col;

	double current_alpha;

	l_Hdr * bmu_link;
	bmu_link = (l_Hdr *) malloc(sizeof(l_Hdr));
	bmu * bmu_item;
	bmu_link->L = NULL;
	bmu_link->last = NULL;

	printf("Phase 2 : neighbouring_arc(%d) nb_iterations(%d) neighbouring_update(%d)\n", neighbouring_arc,nb_iterations,neighbouring_update);
	for (iteration_iterator = 0 ; iteration_iterator < nb_iterations ; iteration_iterator++){
			// printf("Iteration : %d\n", iteration_iterator);
			my_database = index_shuffling(my_database);
			// printf("After shuffling\n");
			current_alpha = alpha_init * (1 - (iteration_iterator/nb_iterations));
		for(database_iterator = 0 ; database_iterator < my_database.db_capacity ; database_iterator ++){
			for(neuron_line_iterator = 0 ; neuron_line_iterator < network.height; neuron_line_iterator++){
				for(neuron_column_iterator = 0 ; neuron_column_iterator < network.width ; neuron_column_iterator++){
					
					network.neurons[neuron_line_iterator][neuron_column_iterator].act = euclidian_distance(my_database.data[my_database.index_vec[database_iterator]].v , network.neurons[neuron_line_iterator][neuron_column_iterator].value , my_database.v_capacity);
					//printf("After euclidian: %lf\n",network.neurons[neuron_line_iterator][neuron_column_iterator].act);
					if(bmu_link->L != NULL && (network.neurons[neuron_line_iterator][neuron_column_iterator].act < bmu_link->L->act)){
						//resets the list with new bmu
						//printf("Going to erase bruh \n");
						erase_link(bmu_link);
						add_elem_end_link(bmu_link, neuron_line_iterator , neuron_column_iterator , network.neurons[neuron_line_iterator][neuron_column_iterator].act);
					}else if(bmu_link->L != NULL && (network.neurons[neuron_line_iterator][neuron_column_iterator].act == bmu_link->L->act)){
						//Adds an element to the bmu list
						add_elem_end_link(bmu_link, neuron_line_iterator , neuron_column_iterator , network.neurons[neuron_line_iterator][neuron_column_iterator].act);
					}else if(bmu_link->L == NULL){
						add_elem_end_link(bmu_link, neuron_line_iterator , neuron_column_iterator , network.neurons[neuron_line_iterator][neuron_column_iterator].act);						
					}
				}
			}
			//Randomly taking a bmu among the bmu_link
			bmu_item = get_specified_bmu(bmu_link , rand()%bmu_link->nb_c + 1);
			//printf("bmu_item->bmu_c : %d , bmu_item->bmu_l : %d\n" , bmu_item->bmu_c , bmu_item->bmu_l);
			//General learning
				for(neigh_line = (bmu_item->bmu_l - neighbouring_arc); neigh_line <= (bmu_item->bmu_l + neighbouring_arc) ; neigh_line++){
					if(neigh_line < 0)
						continue;
					else if(neigh_line >= network.height)
						break;
					for(neigh_col = (bmu_item->bmu_l - neighbouring_arc) ; neigh_col <= (bmu_item->bmu_c + neighbouring_arc) ; neigh_col++){
						if(neigh_col < 0)
							continue;
						else if(neigh_col >= network.width)
							break;
						for(value_iterator = 0 ; value_iterator < my_database.v_capacity ; value_iterator++){
							// printf("network.neurons[%d][%d].value[%d] = %lf (before)\n",neigh_line,neigh_col,value_iterator,network.neurons[neigh_line][neigh_col].value[value_iterator] );
							network.neurons[neigh_line][neigh_col].value[value_iterator] += (current_alpha * (my_database.data[my_database.index_vec[database_iterator]].v[value_iterator] - network.neurons[neigh_line][neigh_col].value[value_iterator]));
							// printf("network.neurons[%d][%d].value[%d] = %lf (after)\n",neigh_line,neigh_col,value_iterator,network.neurons[neigh_line][neigh_col].value[value_iterator] );
						
						}
					}
				}
			
		erase_link(bmu_link);
		}
		//Reduces neighbouring arc based on iteration modulo
		if(neighbouring_arc > 1 && ((iteration_iterator+1) %neighbouring_update) == 0){
			printf("Arc : %d -> %d at iteration %d\n",neighbouring_arc , neighbouring_arc -1, iteration_iterator);
			neighbouring_arc--;
		}
	}
	//free(bmu_link);
	return network;
}

grid tagging(database my_database, grid network){
	int neuron_line_iterator, neuron_column_iterator, database_iterator;

	pair maxed;
	l_Hdr * bmu_link;
	bmu_link = (l_Hdr *) malloc(sizeof(l_Hdr));
	bmu * bmu_item;
	bmu_link->L = NULL;
	bmu_link->last = NULL;

	printf("Nametagging...\n");
	index_shuffling(my_database);
			// printf("After shuffling\n");
	for(database_iterator = 0 ; database_iterator < my_database.db_capacity ; database_iterator ++){
		for(neuron_line_iterator = 0 ; neuron_line_iterator < network.height; neuron_line_iterator++){
			for(neuron_column_iterator = 0 ; neuron_column_iterator < network.width ; neuron_column_iterator++){
				
				network.neurons[neuron_line_iterator][neuron_column_iterator].act = euclidian_distance(my_database.data[my_database.index_vec[database_iterator]].v , network.neurons[neuron_line_iterator][neuron_column_iterator].value , my_database.v_capacity);
				//printf("After euclidian: %lf\n",network.neurons[neuron_line_iterator][neuron_column_iterator].act);
				if(bmu_link->L != NULL && (network.neurons[neuron_line_iterator][neuron_column_iterator].act < bmu_link->L->act)){
					//resets the list with new bmu
					//printf("Going to erase bruh \n");
					erase_link(bmu_link);
					add_elem_end_link(bmu_link, neuron_line_iterator , neuron_column_iterator , network.neurons[neuron_line_iterator][neuron_column_iterator].act);
				}else if(bmu_link->L != NULL && (network.neurons[neuron_line_iterator][neuron_column_iterator].act == bmu_link->L->act)){
					//Adds an element to the bmu list
					add_elem_end_link(bmu_link, neuron_line_iterator , neuron_column_iterator , network.neurons[neuron_line_iterator][neuron_column_iterator].act);
				}else if(bmu_link->L == NULL){
					add_elem_end_link(bmu_link, neuron_line_iterator , neuron_column_iterator , network.neurons[neuron_line_iterator][neuron_column_iterator].act);						
				}
			}
		}
		//Randomly taking a bmu among the bmu_link
		bmu_item = get_specified_bmu(bmu_link , rand()%bmu_link->nb_c + 1);
		// printf("bmu_link->nb_c : %d \n", bmu_link->nb_c);
		
		if(strcmp(my_database.data[my_database.index_vec[database_iterator]].name, "Iris-setosa") == 0){
			network.neurons[bmu_item->bmu_l][bmu_item->bmu_c].nametag_probability[0] ++;
		}else if(strcmp(my_database.data[my_database.index_vec[database_iterator]].name, "Iris-versicolor") == 0){
			network.neurons[bmu_item->bmu_l][bmu_item->bmu_c].nametag_probability[1] ++;				
		}else if(strcmp(my_database.data[my_database.index_vec[database_iterator]].name, "Iris-virginica") == 0){
			network.neurons[bmu_item->bmu_l][bmu_item->bmu_c].nametag_probability[2] ++;				
		}	
	erase_link(bmu_link);
	}

	for(neuron_line_iterator = 0 ; neuron_line_iterator < network.height; neuron_line_iterator++){
		for(neuron_column_iterator = 0 ; neuron_column_iterator < network.width ; neuron_column_iterator++){
			//Storing highest nametag stimulated for corresponding neuron
			maxed =	max2_rank(
						network.neurons[neuron_line_iterator][neuron_column_iterator].nametag_probability[0], 0,
						network.neurons[neuron_line_iterator][neuron_column_iterator].nametag_probability[1],1);
			maxed =	max2_rank(
						maxed.val, maxed.rank,
						network.neurons[neuron_line_iterator][neuron_column_iterator].nametag_probability[2],2);

			//Tagging
			if(maxed.rank == 0 && maxed.val != 0)
				network.neurons[neuron_line_iterator][neuron_column_iterator].name = "SE";
			else if(maxed.rank == 1 && maxed.val != 0)
				network.neurons[neuron_line_iterator][neuron_column_iterator].name = "VE";
			else if(maxed.rank == 2 && maxed.val != 0)
				network.neurons[neuron_line_iterator][neuron_column_iterator].name = "VI";
			else if(maxed.val == 0)
				network.neurons[neuron_line_iterator][neuron_column_iterator].name = "..";

		}	
	}
	return network;
}


int main(int argc, char * argv[]){
	if(argc != 7){
		printf("Usage: ./a.out database.txt random_fork(float()) alpha_init(phase1) starting_neigh(phase2) random_seed iteration_ratio_divider\n\n");
		printf("-database.txt : your file with your database inside , this version only supports Iris-data\n-random_fork(float()): random range for -itself +itself used for neuron's values initialization\n-alpha_init(phase1): alpha initialization for phase 1 , phase 2 will have 10 percent of it\n-starting_neigh(phase2): value representing the neighbouring_arc at the beginning of phase 2\n-random_seed: seed used for srand\n-iteration_ratio_divider: divider used for phase 1 / phase 2 proportions such as phase 1 : 1/divider & phase 2 : (1/divider)*(divider-1)\n\n");
		exit(0);
	}
	if(sizeof(value_type) == sizeof(double)){
		double alpha_init = atof(argv[3]);
		int starting_neigh = atoi(argv[4]);
		int ite_ratio = atoi(argv[6]);
		long int random_seed = (long int) atoi(argv[5]);
		srand(random_seed);
		FILE* fichier;
		database main_database;
		grid network;
		static int car = 0;
		fichier = fopen(argv[1],"r");
		main_database = initialise_database(fichier);
		load_database(fichier , car,main_database);
		//show_database(main_database);
		//index_shuffling(main_database);
		//read_vector(main_database,1);
		//read_vector(main_database,2);
		network = init_network(main_database, atof(argv[2]));
		//show_network(network, main_database, 0);
		printf("Phase 1 : %d , Phase 2 : %d \n",((500 * main_database.db_capacity)/ite_ratio) ,((500 * main_database.db_capacity)/ite_ratio) * (ite_ratio - 1));
		network = learning_phase_1(main_database,network , alpha_init,((500 * main_database.db_capacity)/ite_ratio));
		network = learning_phase_2(main_database,network , alpha_init/10, starting_neigh, ((500 * main_database.db_capacity)/ite_ratio) * (ite_ratio -1) );
		network = tagging(main_database,network);
		show_network(network, main_database, 1);
		show_network(network, main_database, 0);
		fclose(fichier);
	}else{
		printf("ERROR : Please define proper type. Check .h\n");
		exit(0);
	}
	return 0;
	//free(main_database.data);
}
