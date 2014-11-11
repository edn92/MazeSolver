#include <iostream>
#include <fstream>
#include <sstream>

#include "bintree.h"

using namespace std;

class MazePoint
{
   private:
   char value;
   int x;

   public:
      
   MazePoint()
   {
      value = ' ';
      x = 0;
   }

   MazePoint(int i)
   {
      value = ' ';
      x = i;
   }

   char getValue() const
   {
      return value;
   }
      
   void setValue(char c)
   {
      value = c;
   }

   int getX() const
   {
      return x;
   }

   //overloaded operators

   bool operator < (const MazePoint &other) const
   {
      return x < other.x;
   }

   bool operator == (const MazePoint &other) const
   {
      return x == other.x;
   }
};

class MazeRow
{
   private:
   bintree<MazePoint> mazePoints;
   int y;

   public:

   MazeRow()
   {
      y = 0;
   }
      
   MazeRow(int i)
   {
      y = i;
   }

   int getY() const
   {
      return y;
   }
   
   int rowLength() const
   {
      return mazePoints.size();
   }

   string toString() const
   {
      //prints out each MazePoint in this MazeRow
      stringstream ss;

      for (int i = 0; i < mazePoints.size(); i++)
      {
         MazePoint mP(i);
         const MazePoint *mPoint = mazePoints.findConst(mP);
         if (mPoint != NULL)
         {
            mP.setValue(mPoint->getValue());
         }
         ss << mP.getValue();
      }
      return ss.str();
   }

   char searchMazePoint(int x) const
   {
      /*Used for searching for a specific MazePoint
      using its x coordinate.*/
      char c;

      MazePoint mP(x);
      const MazePoint *mPoint = mazePoints.findConst(mP);
      if (mPoint != NULL)
      {
         mP.setValue(mPoint->getValue());
         c = mP.getValue();
      }

      return c;
   }
   
   void changeMazePoint(const int x, const char &c)
   {
      /*Used to mark the maze with breadcrumbs
      and the actual path.*/
      MazePoint mP(x);

      MazePoint *mPoint = mazePoints.find(mP);
      if (mPoint != NULL)
      {
         char sc = mPoint->getValue();

         if (sc != 's')
         {
            mP.setValue(c);
            mazePoints.erase(mP);
            mazePoints.insert(mP);
         }
      }
   }
   
   void insertMazePointsIntoRow(const string &line)
   {
      /*Will only insert a char into the MazeRow
      if it is a '#', ' ', 's', 'f' or '\n'.
      Otherwise it returns an error and exits the program.*/
      for(unsigned int i = 0; i <line.length(); i++)
      {
         if (line[i] == '#' || line[i] == ' ' || line[i] == 's' || 
                  line[i] == 'f' || line[i] == '\n')
         {
            MazePoint mazePoint(i);
            mazePoint.setValue(line[i]);
            mazePoints.insert(mazePoint);
         } 
         else
         {
            cout << "Invalid character in maze\n";
            exit(0);
         }
      }
   }

   //overloaded operators

   bool operator < (const MazeRow &other) const
   {
      return y < other.y;
   }

   bool operator == (const MazeRow &other) const
   {
      return y == other.y;
   }
};

class Maze
{
   private:
   bintree<MazeRow> mazeRows;
   int startX, startY, finishX, finishY;

   public:

   void loadMaze(const char *filename)
   {
      string line;
      ifstream fin;

      fin.open(filename);
      if (fin == NULL)
      {
         cout << "Unable to load maze " << filename << "\n";
         exit(0);
      }

      int rowNumber = 0;

      while (getline(fin, line) !=NULL)
      {
         insertRowsIntoTree(line, rowNumber);
         rowNumber++;
      }
   }

   void insertRowsIntoTree(const string &line, int rowNumber)
   {
      MazeRow mazeRow(rowNumber);
      mazeRow.insertMazePointsIntoRow(line);
      mazeRows.insert(mazeRow);
   }
   
   void checkMaze(const char *mazeFile)
   {
      /*Check the maze
      Number of start and finish should be equal to 1
      Also checks if the start and finish points 
      are located outside of the maze
      
      The (x,y) coordinates of the starting point
      is saved for finding the path later*/
      int numStart = 0 , numFinish = 0;

      for (int y = 0; y <mazeRows.size(); y++)
      {
         MazeRow mR(y);
         const MazeRow *mRow = mazeRows.findConst(mR);
         if (mRow != NULL)
         {
            for (int x = 0; x < mRow->rowLength(); x++)
            {
               char c = mRow->searchMazePoint(x);

               if (c == 's')
               {
                  numStart++;

                  startX = x;
                  startY = y;
               }

               if (c == 'f')
               {
                  numFinish++;

                  finishX = x;
                  finishY = y;
               }
            }
         }
      }
      
      if (ifOutside('s', startX, startY) == true)
      {
         cout << "Error - start declared outside of maze\n";
         cout << "Unable to load maze " << mazeFile << "\n";
         exit(0);
      }
      if (ifOutside('f', finishX, finishY) == true)
      {
         cout << "Error - finish declared outside of maze\n";
         cout << "Unable to load maze " << mazeFile << "\n";
         exit(0);
      }
      checkStart(numStart);
      checkFinish(numFinish);
   }
   
   void checkStart(int i)
   {
      if (i == 0)
      {
         cout << "Error - no start found in maze\n";
         exit(0);
      }

      if (i > 1)
      {
         cout << "Error - multiple start found in maze\n";
         exit(0);
      }
   }
   
   void checkFinish(int i)
   {
      if (i == 0)
      {
         cout << "Error - no finish found in maze\n";
         exit(0);
      }

      if (i >1)
      {
         cout << "Error - multiple finish found in maze\n";
         exit(0);
      }
   }
   
   bool ifOutside(const char c, int x, int y)
   {
      /*Find position of character
      Find the first hash in a line 
      If no hash or the character is before a hash
      then its outside.
      
      Assumes the maze is not U-shaped and
      the spec says we don't have to check the right
      of a maze*/
      int i = 0;
      int positionOfChar = x;
      int positionOfHash = 0;

      MazeRow mR(y);
      const MazeRow *mRow = mazeRows.findConst(mR);
      if (mRow != NULL)
      {
         while(mRow->searchMazePoint(i) != '#')
         {
            i++;
            positionOfHash++;
         }
      }

      if (positionOfChar < positionOfHash)
      {
         return true;
      }

      return false;
   }
   
   void findPathThroughMaze()
   {
      //Give the starting point to the move function
      move(startX, startY);
   }
   
   bool move(int x, int y)
   {
      /*Check if on finishing point
      If yes, return true all the way up the stack
      Clean the maze up and change the path from spaces to a '.'
      If not, move down, up, right, left and leave breadcrumb
      */
      if (x < 0 || y < 0)
      {
         return false;
      }

      MazeRow mR(y);
      MazeRow *mRow = mazeRows.find(mR);

      if (mRow != NULL)
      {
         char c = mRow->searchMazePoint(x);

         if (c == 'f')
         {
            return true;
         }

         if (c == ' ' || c == 's')
         {
            mRow->changeMazePoint(x, '!');

            if (move(x, y+1) == true)
            {
               cleanUpMaze();
               mRow->changeMazePoint(x, '.');
               return true;
            }
            if (move(x, y-1) == true)
            {
               cleanUpMaze();
               mRow->changeMazePoint(x, '.');
               return true;
            }
            if (move(x+1, y) == true)
            {
               cleanUpMaze();
               mRow->changeMazePoint(x, '.');
               return true;
            }
            if (move(x-1, y) == true)
            {
               cleanUpMaze();
               mRow->changeMazePoint(x, '.');
               return true;
            }
         }
      }
      return false;
   }
   
   void cleanUpMaze()
   {
      for (int y = 0; y < mazeRows.size(); y++)
      {
         MazeRow mR(y);
         MazeRow *mRow = mazeRows.find(mR);
         if (mRow != NULL)
         {
            for (int x = 0; x < mRow->rowLength(); x++)
            {
               char c = mRow->searchMazePoint(x);

               if (c == '!')
               {
                  mRow->changeMazePoint(x, ' ');
               }
            }
         }
      }
   }
   
   void printMaze() const
   {
      mazeRows.print();
   }
};

int main(int argc, char *argv[])
{
   Maze maze;
   
   if (argc != 2)
   {
      cout << "Must supply 1 argument to this program\n";
      return 0;
   }
   
   maze.loadMaze(argv[1]);
   maze.checkMaze(argv[1]);
   maze.findPathThroughMaze();
   maze.printMaze();
   
   return 0;
}
