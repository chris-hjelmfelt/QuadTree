/**
 *  @file
 *	
 *	@brief The quadtree class contains a quad tree built from a bitmap image.
 *  The tree class recursively divides the image into 4 pieces until all of 
 *  the pixels inside of a quadrant are within a certain tolerance of the mean
 *  of that area. This class also provides a function to draw an overlay of the
 *  sub-quadrants over the compressed image to show the lines of division.
 * 
 *  @class quadTree
 *
 *	@author Chris Hjelmfelt
 *	@author Cheldon Coughlen
 */

//Guard against multiple inclusions
#ifndef _quad_Tree_
#define _quad_Tree_

//quadTree class interface
class quadTree
{
	private:
		///Structure to hold a region of a quadtree
		struct Node
		{
			///Value of a region, defaulted to -1
			short int value = -1;
		
			///Holds the level of the current leaf, useful for printing overlay
			int level = 0;
		
			///Integer to hold x location of the upper left hand corner
			int x;
		
			///Integer to hold the y location of the upper left hand corner
			int y;
		
			///Upper right quad
			Node *ur;
			
			///Upper left quad
			Node *ul;
		
			///Lower left quad
			Node *ll;
		
			///Lower right quad
			Node *lr;
		};
	
	
		///Counter for the leaves in the tree
		unsigned int numLeaves = 0;
		
		///Counter for the nodes in the tree
		unsigned int numNodes = 0;
	public:
		///Pointer to the root of the tree
		Node *root;
	
		///Constructor
		quadTree();
	
		///Destructor
		~quadTree();
	
		///Fills the tree recursively, calling valueMatch to check values
		void fillTree(Node*& current, int level, int x, int y);
	
		///Draw tree lines overlay 
		void printTree( Node* current, int level);
	
		///Return the number of leaves
		unsigned int leaves();	
		
		///Returns the number of nodes
		unsigned int nodes();
	
		///Checks for the values within the fudge factor, returns mean
		int valueMatch(Node *current);
	
		///Frees memory by traversing recursively
		void deleteAll(Node *&node);
};

#endif
