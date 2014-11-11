#ifndef BINTREE_H_
#define BINTREE_H_

#include <stdexcept>
#include <math.h>

#include "binnode.h"

/********************************************************\
   template class for a binary tree
\********************************************************/
      

template <typename dataType> class bintree
{  
   private:
      binNode<dataType> *root;
      int numItems;
	  
	  int numNodes() const
      {
	     // return a count of the number of nodes in the tree
		 // used by tree verify function
         if (root == NULL) 
         {
            return 0;
         } 
         else 
         {
            return root->numNodes();
         }
      }
      
   public:
      /*******************************************************\
         constructors & destructors
      \*******************************************************/
      
      // constructor
      bintree() : root(NULL), numItems(0) {}

      // copy constructor
      bintree(const bintree<dataType> &other) : numItems(other.numItems) 
      {
         if (other.root != NULL) 
         {
            root = new binNode<dataType>(*(other.root));
         }
         else 
         {
            root = NULL;
         }
      }
      
      // destructor
      ~bintree() 
      {
         if (root != NULL) delete root;
      }
      
      /*******************************************************\
          tree information functions
      \*******************************************************/
      
      bool empty() const 
      {
         return (numItems == 0);
      }
      
      int size() const 
      {
         // return number of nodes in the tree
		 
         return numItems;
      }
      
      int treeHeight() const 
      {
         // return the maximum height of the tree
		 
         if (root == NULL) 
         {
            return 0;
         } 
         else 
         {
            return root->getHeight();
         }
      }

      int maxTreeDepth() const 
      {
         if (root == NULL) 
         {
            return 0;
         } 
         else 
         {
            return root->maxTreeDepth();
         }
      }

      int numLeafNodes() const 
      {
         // return the number of leaf nodes in the tree
		 
         if (root == NULL) 
         {
            return 0;
         } 
         else 
         {
            return root->numLeafNodes();
         }
      }

      double balance() const 
      {
         // Returns a measure of how balanced a tree is.
         // A value of 1 is a prefectly balanced tree.
         // The closer to 0 the value is the more unbalanced
         // the tree.
         // A value < 0.5 indicates a tree that is seriously unbalanced
         
         // case of no nodes in tree
         if (numItems == 0) return 1.0;
         
         // calculate balance
         double log2numItems = log10(numItems + 1) / log10(2);
         return log2numItems / maxTreeDepth() * (numLeafNodes() * 2) / (numItems + 1); 
      }

      bool verifyTree(char *error) const
      {
	    // verify the tree is correcttly structured and nothing is broken
		 
         if (root == NULL) return true;
         else 
         {
            if (numNodes() != numItems) 
            {
               strcpy(error, "Num nodes not the same as size");
               return false;
            }
            return root->verifyTree(error);
         }
      }
      
      /*******************************************************\
         insertion, erasure and find functions
      \*******************************************************/
      
      void insert(const dataType& newData) 
      {
	     // insert the newData into the tree
		 
         if (root == NULL) 
         {
            root = new binNode<dataType>(newData);
         } 
         else 
         {
            root->insert(root, newData);
         }
         numItems++;
      }
      
      void erase(const dataType& delData) 
      {
	 // find where delData is in tree and erase it
		 
         if (root == NULL) 
         {
            throw std::invalid_argument("data does not exist in tree to erase");
         }
         
         root->erase(root, delData);
         
         numItems--;
      }
      
      dataType* find(const dataType &findData)
      {
         // this function looks for findData in the tree.
	     // If it finds the data it will return the address of the data 
	     // in the tree. otherwise it will return NULL
	     
         if (root == NULL) return NULL;
         else return root->find(findData);
      }
	  
	  const dataType* findConst(const dataType& findData) const
	  {
         if (root == NULL) return NULL;
         else return root->findConst(findData);
	  }

      /*******************************************************\
         overloaded operators 
      \*******************************************************/

      bintree<dataType>& operator = (const bintree<dataType> &other) 
      {
	     // make this tree equal to other. 
		 // erases the entire current contents of the tree doing this
		 
         if (root != NULL) 
         {
            delete root;
            numItems = 0;
         }
         if (other.root != NULL) 
         {
            root = new binNode<dataType>(*(other.root));
            numItems = other.numItems;
         }
         return *this;
      }


      /*******************************************************\
         print function for assignment. 
		 assumes dataType has toString() function 
      \*******************************************************/
	  
	  void print() const {
	     if (root != NULL) root->print();
      }
};

#endif
