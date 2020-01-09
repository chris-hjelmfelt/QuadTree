/**************************************************************************//** 
 * @file
 * @brief The implimentation of the quadTree class
 *****************************************************************************/

//Include statements
#include "quadTree.h"
#include "globals.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

//Constructor, root is null
quadTree::quadTree()
{
	root = NULL;
}

//Destructor, calls the recursive deleteAll function to deallocate memory
quadTree::~quadTree()
{
	//Deallocate the memory
	deleteAll(root);
}

 /**************************************************************************//** 
 * @author Cheldon Coughlen
 * @author Chris Hjelmfelt
 * 
 * @par Description: 
 * Creates the nodes of our quad tree from the image array, recursive
 * counts nodes and leaves, creates leaves based on regions of similar value
 * when spacebar is pressed prints our image with lines overlayed
 * 
 * @param[in,out]      current - a pointer to the current node
 * @param[in]          level - the level of the tree we are currently at
 * @param[in]          x - the x coordinate for our corner pixel
 * @param[in]          y - the y coordinate for our corner pixel
 * 
 *****************************************************************************/
void quadTree::fillTree(Node*& current, int level, int x, int y)
{
	//Variables
	int val = 0;
	int rows;
	int cols;
	int i, j;
	
	//Allocate new node for current, check for success
	current = new Node;
	if (level == 0)
		root = current;
	if (current == NULL)
		return;
		
	//Increment count for number of nodes
	numNodes++;
	
	//Set current's x, y, and level
	current->x = x;
	current->y = y;
	current->level = level;
	
	//Check the values within the current sub-quadrant
	val = valueMatch(current);
	
	//If the values aren't within the tolerance, divide the region
	if (val == -1)
	{
		//Set value to -1 to indicate parent, not leaf
		current->value = val;
		
		//Recursively traverse the 4 sub-regions of the current region
		fillTree(current->ul, level + 1, x, y);
		fillTree(current->ur, level + 1, x + ncols / pow(2, level + 1), y);
		fillTree(current->ll, level + 1, x, y - nrows / pow(2, level + 1)); 
		fillTree(current->lr, level + 1, x + ncols / pow(2, level + 1), 
			y - nrows / pow(2, level + 1));
	}
	else
	{
		//Set the value to the mean returned by valueMatch
		current->value = val;
		
		//Increment the number of leaves by 1
		numLeaves += 1;
		
		//Determine the number of rows and columns for the region
		rows = nrows / pow(2, current->level);
		cols = ncols / pow(2, current->level);
		
		//Iteratively fill the region's values in the image2 array with the 
		//mean value
		for (i = 0; i < rows; i++)
		{
			for (j = 0; j < cols; j++)
			{
				image2[(i + current->y - rows) * (nrows) + j + current->x] = val;
				image3[(i + current->y - rows) * (nrows) + j + current->x] = val;
				
				//Fills the bottom and left borders of the region with white
				//for the overlay
				if (i == 0 || j == 0)
				{
					image3[(i + current->y - rows) * (nrows) + j + current->x] = 255;
				}
			}
		}
		
		//Fill top and right borders of overlay image with white
		for (i = 0; i < ncols; i++)
			image3[ncols * nrows - i] = 255;
		for (i = 0; i < nrows; i++)
			image3[ncols * (i + 1) - 1] = 255;
	}
	return;
}

 /**************************************************************************//** 
 * @author Cheldon Coughlen
 * 
 * @par Description: 
 * Returns the number of leaves in the tree
 * 
 * @returns number of leaves
 * 
 *****************************************************************************/
unsigned int quadTree::leaves()
{
	return numLeaves;
}

 /**************************************************************************//** 
 * @author Cheldon Coughlen
 * 
 * @par Description: 
 * Returns the number of nodes
 * 
 * @returns number of nodes
 * 
 *****************************************************************************/
unsigned int quadTree::nodes()
{
	return numNodes;
}

 /**************************************************************************//** 
 * @author Cheldon Coughlen
 * @author Chris Hjelmfelt
 * @par Description: 
 * Checks for the values within the region against the fudge factor
 * 
 * @param[in]      current - a pointer to the current node
 * 
 * @returns mean of the values in the region or -1 for parent
 * 
 *****************************************************************************/
int quadTree::valueMatch(Node *current)
{
	//Variables
	long int sum = 0;
	int i, j;
	int mean = 0;
	int rows = nrows / pow(2, current->level);
	int cols = ncols / pow(2, current->level);
	
	//Iteratively find the sum of the values within the region
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
			sum += image[(i + current->y - rows) * (nrows) + j + current->x];
	}
	
	//Calculate the mean using the sum, rows, and columns
	mean = sum / (rows * cols);
	
	//Iteratively check if each pixel in the region is within the fudge factor
	//from the mean
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
			if (abs(image[(i + current->y - rows) * (nrows) + j + current->x]
				- mean) > fudge)
				//Return -1 when the value is not within the range
				return -1;
	}
	
	//Otherwise, return the mean of the region
	return mean;
}

 /**************************************************************************//** 
 * @author Cheldon Coughlen
 * 
 * @par Description: 
 * Frees memory by traversing recursively and then deleting nodes.
 * Called by the destructor.
 * 
 * @param[in]      node - a pointer to the current node
 * 
 *****************************************************************************/
void quadTree::deleteAll(Node *&node)
{
	//Only traverses down if the node's children are not null
	//Prevents seg fault and tells the function when to stop
	if (node->ul != nullptr && node->ur != nullptr && node->lr != nullptr  && node->ll != nullptr)
	{
		deleteAll(node->ur);
		deleteAll(node->ul);
		deleteAll(node->ll);
		deleteAll(node->lr);
		
	}
	
	//Delete a node after deleting all of its children
	delete node;
}
