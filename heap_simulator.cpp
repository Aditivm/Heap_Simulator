#include <stdio.h>
#include <stdlib.h>


struct node 
{
	int id;
	int origin;
	int end;
	struct node *link;
} *block_list = NULL;  

typedef struct node block_type;

//global variables for the simulator
int a; //for the size
int b; //for hole algo
int c; //for remaining
int d; //for best till now

void End(block_type * node); //destructor
void defragment();
void dellocate();
void allocate();
void printAllocationTable(); //print table
void Origin(); //constructor


int main ()
{
  	int choice;
  	do
    {
      	printf ("1] Enter parameters\n");
      	printf ("2] Allocate memory for block\n");
      	printf ("3] Defragment memory\n");
      	printf ("4] Deallocate memory for block\n");
      	printf ("5] End the Program\n");

      	printf ("\nEnter Selection: ");
      	scanf ("%d", &choice);
      	switch (choice)
		{
		case 1:
	  		Origin();
	  		break;
		case 2:
	  		allocate();
		  	break;
		case 3:
	  		defragment();
			break;
		case 4:
			dellocate();
	  		break;
		}
	}while (choice != 5);
  	End(block_list);
  	printf ("End of program");
  	return 0;
}

void Origin() 
{
	//Initialiser for memory size and free memory,also choose hole fitting.
	printf ("Size of physical memory: ");
	scanf ("%d", &a);
	c = a;
	d = a;

	printf ("Hole-fitting algorithm (0 or 1) "); //0 for first fir, 1 for best fit
	scanf ("%d", &b);
	return;
}

void printAllocationTable()
{
	block_type *next_block = block_list;
	// print table containing block id, origining address, ending address and actual memory address

	printf ("ID\tOrigin\tEnd\n");
	printf ("***********************\n");

	while (next_block != NULL)
    {
    	printf ("%d\t%d\t%d\t\n", next_block->id, next_block->origin,next_block->end);
    	next_block = next_block->link;
    }
	return;
}


void allocate()
{
    // serves as the variable name for pointer to this location, used for free()
	int block_id, block_size;
	block_type *new_block, *current_block, *best_block;
	int hole_origin, hole_end;
	int best_origin, best_end;
	int large_enough_hole = 0;

	printf ("Enter block id: "); 
	scanf ("%d", &block_id);
	printf ("Enter block size: ");
	scanf ("%d", &block_size);

	if (block_size > c)
    {
    	printf ("Cannot fit the block.");
    	return;
    }

	new_block = (block_type *) malloc (sizeof (block_type));
	new_block->id = block_id;

	if (block_list == NULL) //empty allocation list
	{
    	new_block->origin = 0;
    	new_block->end = block_size;
    	new_block->link = NULL;
    	block_list = new_block;

    	d -= block_size;	
    	c -= block_size;	
    	printAllocationTable ();
    	return;
    }
  //else traverse list until the appropriate hole is found or else the end of the list is reached
	current_block = block_list;

	while (current_block != NULL)
    {
		if (block_id == current_block->id)
		{
			printf ("Duplicate id\n");
			return;
		}

      	hole_origin = current_block->end;

      	if (current_block->link != NULL)
		{
			hole_end = current_block->link->origin;
		}
		else
		{
	  		hole_end = a;
		}

    	if (block_size <= (hole_end - hole_origin))
		{
	  		large_enough_hole = 1;
	  		if (b == 0)
	    	{
	      		new_block->origin = hole_origin;
	      		new_block->end = hole_origin + block_size;
	      		c -= block_size;
				new_block->link = current_block->link;
				current_block->link = new_block;
				printAllocationTable ();
				return;
	    	}
	  		
	  		else
	    	{
	      		if ((hole_end - hole_origin) <= d)
				{	
		  			d = hole_end - hole_origin;
		  			best_origin = hole_origin;
		  			best_end = hole_end;
		  			best_block = current_block;
				}
	    	}
		}
    	current_block = current_block->link;
    }       

	if (large_enough_hole == 0)
    {
    	printf ("No enough large hole available.");
    	return;
    }

	new_block->origin = best_origin;
	new_block->end = best_origin + block_size;
	new_block->link = best_block->link;
	best_block->link = new_block;
	c -= block_size;
	printAllocationTable ();
	return;
}

void dellocate()
{
	block_type *current_block = block_list;
	block_type *prev_block;
	int block_id;
	current_block = block_list;

	if (current_block == NULL)
    {
    	printf ("no blocks available");
    	return;
    }

	printf ("Enter block id: ");
	scanf ("%d", &block_id);

	while ((current_block != NULL) && (block_id != current_block->id))//linear search till we find block id in allocation list
    {
		prev_block = current_block;
    	current_block = current_block->link;
    }
  	
  	if (current_block == NULL)
    {
      	printf ("\nCould not find block ID\n");
    }
  	else
    {
      	prev_block->link = current_block->link;
      	c += current_block->end - current_block->origin;
      	d += c;
      	free (current_block);
    }
    //if block_id found, free it and add memory to free(this is where deframgentation would be done)
  	printAllocationTable ();
  	return;
}

void defragment()
{
  	block_type *current_block;
  	int current_size;
  	int prev_end = 0;

  	current_block = block_list;
  	while (current_block != NULL)
    {
      	current_size = current_block->end - current_block->origin;	
      	current_block->origin = prev_end;
      	current_block->end = current_block->origin + current_size;
      	prev_end = current_block->end;
      	current_block = current_block->link;
    }
  
  	printAllocationTable ();
  	return;
}

void End(block_type * node) //destructor
{

  	if (node == NULL)
    {
      	return;
    }
  	else
    {
      	End(node->link);
      	free(node);
    }
  	return;
}


