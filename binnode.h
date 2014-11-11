#ifndef BINNODE_H
#define BINNODE_H

#include <math.h>
#include <iostream> 
#include <stdexcept>
#include <assert.h>
#include <string.h>

#define NDEBUG

/********************************************************\
   template node class for binary tree
\********************************************************/

template <typename dataType> class binNode 
{
   private:
      // private data ====================================
      dataType nodeData;
      binNode<dataType> *left, *right;
      
      // maximum height of this node and it's subtrees
      int height;
      
      // private functions ===============================
      
      void addTreeToLeft(binNode<dataType>* &root, binNode<dataType> *tree)
      {
          assert(tree != NULL);
          assert(root == this);
            
          if (left == NULL)
          {
             left = tree;
          }
          else 
          {
             left->addTreeToLeft(left, tree); 
          }
          
          rebalance(root);
      }
            
      void deleteNode(binNode<dataType>* &root) 
      {
         assert(root == this);
         if (left == NULL && right == NULL) 
         {
            // leaf node
            root = NULL;
         } 
         else if (left == NULL) 
         {
            // right branch but no left branch
            root = right;
         } 
         else if (right == NULL) 
         {
            // left branch but no right branch
            root = left;
         } 
         else 
         { 
            // make left most node of right subtree point to left subtree
            right->addTreeToLeft(right, left);
            
            // make root point to right branch
            root = right;
         }
         left = NULL;
         right = NULL;
         delete (this); 
      }
      
      /********************************************************\
         rebalance functions
      \********************************************************/
      
      void updateHeight()
      {
         height = 1 + maxSubtreeHeight();
      }
      
      // rotations ===============================================
      
      void rotateClockwise(binNode<dataType>* &root)
      {
         assert(root != NULL);
         assert(left != NULL);
         
         // rotate clockwise
         root = left;
         left = left->right;
         root->right = this;
         
         assert(root != NULL);
         
         this->updateHeight();
         root->updateHeight();
      }
      
      void rotateAntiClockwise(binNode<dataType>* &root)
      { 
         assert(root != NULL);
         assert(right != NULL);
         
         // rotate anticlockwise
         root = right;
         right = right->left;
         root->left = this;
         
         assert(root != NULL);
         
         this->updateHeight();
         root->updateHeight();
      }
      
      // rebalance ============================================
      
      void rebalanceClockwise(binNode<dataType>* &root)
      {
         assert(root != NULL);
         assert(slopedLeft()) ;
         assert(left != NULL);
         
         // left is higher than right
            
         if (left->tiltedRight() > 0) 
         {
            // the left hand tree is higher than the right hand tree
            // but the right hand of the left subtree is higher than the 
            // left hand of the left subtree.
            // rotate the left subtree anticlockwise.
            // This will make clockwise rotation work better
            left->rotateAntiClockwise(left);
         }
         rotateClockwise(root);
            
         assert(root != NULL);
         assert(root->right != NULL);
      }
      
      void rebalanceAntiClockwise(binNode<dataType>* &root)
      {
         assert(root != NULL);
         assert(slopedRight());
         assert(right != NULL);
         
         // right is higher than left
            
         if (right->tiltedLeft() < 0) 
         {
            right->rotateClockwise(right);
         }
         rotateAntiClockwise(root);
        
         assert(root != NULL);
         assert(root->left != NULL);
      }
           
   public:
      
      /********************************************************\
         constructors and destructors
      \********************************************************/

      // constructors
      binNode() : left(NULL), right(NULL), height(1) 
      {
      }
   
      binNode(const dataType& dataItem) :
         nodeData(dataItem), left(NULL), right(NULL), height(1) 
      {
      }

      // copy constructor
      binNode(const binNode<dataType> &other) : nodeData(other.nodeData) 
      {
          if (other.left != NULL) 
          {
             left = new binNode<dataType>(*(other.left));
          }
          else 
          {
             left = NULL;
          }
          if (other.right != NULL) 
          {
             right = new binNode<dataType>(*(other.right));
          }
          else 
          {
             right = NULL;
          }
          height = other.height;
      }
   
      // destructor
      ~binNode() 
      {
         if (left != NULL) delete left;
         if (right != NULL) delete right;
      }
   
      /********************************************************\
         insert, delete and find
      \********************************************************/

      void insert(binNode<dataType>* &root, const dataType& dataItem) 
      {
         if (nodeData == dataItem) 
         {
            throw std::invalid_argument("dataItem already in tree");
         }
      
         if (dataItem < nodeData) 
         {
            if (left == NULL) 
            {
               left = new binNode(dataItem);
            } 
            else 
            {
               left->insert(left, dataItem);
            }
         } 
         else 
         {
            if (right == NULL) 
            {
               right = new binNode(dataItem);
            } 
            else 
            {
               right->insert(right, dataItem);
            }
         }
         rebalance(root);
      }
      
      void erase(binNode<dataType>* &root, const dataType &delData) 
      {
         if (delData == nodeData) 
         {
            deleteNode(root);
         } 
         else 
         {
            if (delData < nodeData) 
            {
               if (left == NULL) 
               {
                  throw std::invalid_argument("delItem not in tree");
               } 
               else 
               {
                  left->erase(left, delData);
               }
            } 
            else 
            {
               if (right == NULL) 
               {
                  throw std::invalid_argument("delItem not in tree");
               } 
               else 
               {
                  right->erase(right, delData);
               }
            }
            rebalance(root);
         }
      }
      
      dataType* find(const dataType &findData)
      {
         if (findData == nodeData) 
         {
            return &nodeData;
         } 
         else if (findData < nodeData) 
         {
            if (left == NULL) return NULL;
            else return left->find(findData);
         } 
         else 
         {
            if (right == NULL) return NULL;
            else return right->find(findData);
         }
      }
	  
	  const dataType* findConst(const dataType &findData) const
      {
         if (findData == nodeData) 
         {
            return &nodeData;
         } 
         else if (findData < nodeData) 
         {
            if (left == NULL) return NULL;
            else return left->findConst(findData);
         } 
         else 
         {
            if (right == NULL) return NULL;
            else return right->findConst(findData);
         }
      }
      
      void rebalance(binNode<dataType>* &root) 
      {
         /*******************************************************\ 
            This is called wherever a change is made to the tree.
            Such as inserting and erasing items from the tree.
            It uses rotations to try and keep the tree mostly 
            balanced.
         \*******************************************************/
         
         assert(root != NULL);
         
         if (slopedLeft())
         {
            rebalanceClockwise(root);
         }
         else if (slopedRight())
         {
            rebalanceAntiClockwise(root);
         }
         else
         {
            updateHeight();
         }
      }
  
      /********************************************************\
         tree information
      \********************************************************/

      // tree statistic functions
	  
      int numLeafNodes() const 
      {
         if (height == 1) return 1;
         
         int numLeaf = 0;
         if (left != NULL) numLeaf += left->numLeafNodes();
         if (right != NULL) numLeaf += right->numLeafNodes();
         return numLeaf;
      }


      int maxTreeDepth() const 
      {
         int rdepth = 0, ldepth = 0;
         
         if (left != NULL) ldepth = left->maxTreeDepth();
         if (right != NULL) rdepth = right->maxTreeDepth();
         
         if (ldepth > rdepth) return 1 + ldepth;
         else return 1 + rdepth;
      }

      int numNodes() const 
      {  
         int numNode = 1;
         if (left != NULL) numNode += left->numNodes();
         if (right != NULL) numNode += right->numNodes();
         return numNode;
      }
      
      void print() const 
      {
         if (left != NULL) left->print();
         std::cout << nodeData.toString() << "\n";
         if (right != NULL) right->print();
      }

      int getHeight() const 
      {
         return height;
      }
      
      int leftTreeHeight() const 
      {
         // return height of left subtree
         
         if (left == NULL) return 0;
         else return left->getHeight();
      }
      
      int rightTreeHeight() const 
      {
         // return height of right subtree
         
         if (right == NULL) return 0;
         else return right->getHeight();
      }
      
      int maxSubtreeHeight() const
      {
         if (leftTreeHeight() >= rightTreeHeight())
            return leftTreeHeight();
         else
            return rightTreeHeight();
      }
      
      int treeSlope() const 
      {
         // if negative means left is higher then right
         // if positive means right is higher then left.
         // if zero means left and right have same height

         return leftTreeHeight() - rightTreeHeight();
      }
      
      bool tiltedLeft() const
      {
         return (treeSlope() > 0);
      }
      
      bool slopedLeft() const
      {
         return (treeSlope() > 1);
      }
      
      bool tiltedRight() const
      {
         return (treeSlope() < 0);
      }
      
      bool slopedRight() const
      {
         return (treeSlope() < -1);
      }
         
      bool verifyTree(char *error) const
      {
          // check subtree height is correct in relation to subtree
          int max = leftTreeHeight();
          if (rightTreeHeight() > max) max = rightTreeHeight();
          
          if (height != max + 1)
          {
             strcpy(error, "Invalid height in tree");
             return false;
          }
          
          // check left subtree
          if (left != NULL)
          {
             if (left->nodeData >= nodeData)
             {
                strcpy(error, "Left node out of order");
                return false;
             }
             if (!left->verifyTree(error)) return false;
          }
          
          // check right subtree
          if (right != NULL)
          {
             if (right->nodeData <= nodeData)
             {
                strcpy(error, "Right node out of order");
                return false;
             }
             if (!right->verifyTree(error)) return false;
          }
             
          // no falses created so this node and its subtrees are ok
          return true;
      }
      
         
      /********************************************************\
         overloaded operators
      \********************************************************/

      // overloaded dereference operator
      const dataType& operator * () const 
      {
         return nodeData;
      }

      binNode<dataType>& operator = (const binNode<dataType> &other) 
      {
         // remove current subtrees
         if (left != NULL) delete left;
         if (right != NULL) delete right;

         // make subtrees equal to subtree of other
         if (other.left != NULL) 
         {
            left = new binNode<dataType>(*(other.left));
         }
         else 
         {
            left = NULL;
         }
         if (other.right != NULL) 
         {
            right = new binNode<dataType>(*(other.right));
         }
         else 
         {
            right = NULL;
         }

         // make nodedata equal nodedata of other
         nodeData = other.nodeData;
      }
};

#endif
