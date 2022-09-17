/*
 *=====================
 *   Sudoku Solver
 *=====================
 *
 * Objective - Inputs Sudoku puzzles from the user, solves it using the backtracking algorithm and outputs the solution.
*/

#include<iostream>
#include<vector>
#include<map>
#include <fstream>

using namespace std;

/*This class provides a data structure to hold and manipulate the values in a sudoku puzzle. */

class SudokuFrame{
  
  int n = 9;
  vector<vector<char>> frame;
  
  /*This constructor calls the menu() func to provide the menu.*/
  public: SudokuFrame(){
      frame = vector<vector<char>>(n, vector<char>(n, '0'));
      menu();
  }
  
  /*Displays a menu to the user at the start of the program to display all possible options to the user.*/
  private:void menu(){
		
		cout<<"\n======================\n";
		cout<<"    Sudoku Solver\n";
		cout<<"======================\n\n";

		cout<<"Welcome to Sudoku Solver!\n";
		cout<<"Before we start, you will have to input the puzzle into this program.\n\n";
		cout<<"You can either:-\n";
		cout<<"\t1. Input the puzzle by entering the values manually. (Enter 1)\n";
		cout<<"\t2. Input the puzzle by reading a file with values in it. (Enter 2)\n";
		cout<<"\t   The file must not have a name > 20 characters.\n";
		cout<<"\t   The file must be in the same directory as this C++ file.\n";
		cout<<"\t   The file must have all 81 values seperated with spaces.\n";
		cout<<"\t   Blank cells must be filled in as 0 (eg; 1 0 0 2 0 0 ...).\n";
		cout<<"\t   --> ";

		int option;
		cin>>option;

		if(option==1) readFrameValues();
		else if(option==2) readFrameValuesFile();
		else{
			while(true){
				cout<<"\nYou seem to have entered an invalid option. Try again.\n";
				cout<<"\t   --> ";
				cin>>option;

				if(option==1) readFrameValues();
				else if(option==2) readFrameValuesFile();
				else continue;

				break;
			}
		}

	}

  /*
	  *	@desc Reads the values for the Sudoku Frame cell-by-cell.
	  *	@param none *@return none
  */	
  private:void readFrameValues(){
		cout<<"\nEnter the specified value when prompted.\n";
		cout<<"Enter 0 if cell is empty.\n\n";
		
		int rowIter, colIter;

		for(rowIter=0; rowIter<n; rowIter++){ 
			for(colIter=0; colIter<n; colIter++){
				int enteredValue;

				cout<<"Enter value for cell["<<rowIter+1<<"]["<<colIter+1<<"] --> ";
				cin>>enteredValue;

				if(!(enteredValue>=0 && enteredValue<=9)){ //Checking for bounds in input.
					while(true){ 
						cout<<"Oops! You seem to have entered a wrong value! Try again.\n";
						cout<<"Enter value for cell ["<<rowIter+1<<"]["<<colIter+1<<"] --> ";
						cin>>enteredValue;

						if(enteredValue>=0 && enteredValue<=9) break;
					}
				}

				frame[rowIter][colIter]=(char)(enteredValue+'0');
			}
			cout<<"-------\n"; //Display a break after every row for convenience.
		}
	}

  /**
	  *	@desc Reads the values from a file containing the frame values seperated by whitespaces.
	  *	@param none *	@return none
	*/	
  private:void readFrameValuesFile(){

		char filename[30]; //Getting filename.

		cout<<"\nEnter the name of the file that contains the Sudoku Puzzle.\n";
		cout<<"\t   --> ";
		cin>>filename;

		ifstream sudokuFile; //Opening file for reading.
		sudokuFile.open(filename,ios::in);
		
		int rowIter, colIter;

		for(rowIter=0; rowIter<n; rowIter++){ //Iterating over file values.
			for(colIter=0; colIter<n; colIter++){
				int readValue;
		
				sudokuFile>>readValue;

				if(!(readValue>=0 && readValue<=9)){ //Checking bounds for input.
					cout<<"\nValue "<<((rowIter*9)+colIter+1)<<" in "<<filename;
					cout<<" seems to be wrong! Correct the value and try again!\n";
					exit(EXIT_FAILURE);
				}

				frame[rowIter][colIter]=(char)(readValue+'0');

				// if(sudokuFrame[rowIter][colIter]==0) editableFrame[rowIter][colIter]=0;
				// else editableFrame[rowIter][colIter]=1;

				if(sudokuFile.eof()) break; //Breaking if EOF is reached.
			}
		}
		
		//Gotta have a line which lets us check the frame for any errors

		sudokuFile.close();
	}
  
  /* Function to display the frame */
  public:void displayFrame(){

		cout<<"\033[0;36m++=====================================++";
		int rowIter, colIter;

		for(rowIter=0; rowIter<n; rowIter++){
			int cellIter=1;

			cout<<"\n\033[0;36m||";
			for(colIter=0; colIter<n; colIter++){
				if(cellIter==3){
					cout<<"\033[0m "<<frame[rowIter][colIter]<<" ";
					cout<<"\033[0;36m||";
					cellIter=1;
				}
				else{
					cout<<"\033[0m "<<frame[rowIter][colIter]<<"  ";
					cellIter++;	
				}
			}

			if(rowIter%3!=2) cout<<"\n\033[0;36m++-----------++-----------++-----------++";
			else cout<<"\n\033[0;36m++=====================================++";
		}

	}
    
  /* Returns the frame for solving */    	
  public:vector<vector<char>> getFrame(){
      return frame;
  }
};

/*This is the main class for solving the sudoku puzzle */
class Solution{
   
    int n=9;
    vector<map<char, bool>> boxes, rows, cols;

    /*This constructor initilizes the data structures needed to hold values in the rows, cols and 3x3 sub boxes */
    public:Solution(){
        boxes = vector<map<char, bool>>(n);
        rows = vector<map<char, bool>>(n);
        cols = vector<map<char, bool>>(n);
    }
   
   /* Returns box id corresponding to row, col */
    private:int getBoxId (int row, int col) {
        int rowVal = (row / 3) * 3;
        int colVal = col / 3;

        return rowVal + colVal;
    }
    
	/* Returns true if the value is present at given row, column or sub box */
    private:bool isValid (int i, int j, char c) {
        
        int boxId = getBoxId(i, j);
        
        if (boxes[boxId][c] || rows[i][c] || cols[j][c]) {
            return false;
          } else {
            return true;
          }
    }
    
	/* hashes the initial values present in the board and calls backtrack function */
    public: void solveSudoku(vector<vector<char>>& board) {

        for(int r = 0; r < n; r++) {
            for(int c = 0; c < n; c++) {
                if(board[r][c] != '0') {
                    int boxId = getBoxId(r, c);
                    char val = board[r][c];
                    boxes[boxId][val] = true;
                    rows[r][val] = true;
                    cols[c][val] = true;
                }
            }
        }
  
        solveBacktrack(board, 0, 0);
    }
   
   // recursive backtrack function
    private: bool solveBacktrack(vector<vector<char>> &board, int i, int j)
    {
        if(i==n) return true;
        if(j==n) return solveBacktrack(board, i+1, 0);
        if(board[i][j] != '0') return solveBacktrack(board, i, j+1);
    
        for(char c='1'; c<='9'; c++)
        {
            if(isValid(i, j, c))
            {
                board[i][j] = c;
                rows[i][c] = 1; cols[j][c] = 1; boxes[getBoxId(i,j)][c] = 1;  
                
                if(solveBacktrack(board, i, j+1)) return true;
                board[i][j] = '0';
                rows[i][c] = 0; cols[j][c] = 0; boxes[getBoxId(i,j)][c] = 0; 
            }
        }
    
        return false;
    }
    
    /* displaying the solved sudoku */
    public: void display(vector<vector<char>>& board){
        for(int r = 0; r < 9; r++){
            for(int c = 0; c < 9; c++){
                cout<<board[r][c]<<" ";
            }
            cout<<endl;
        }
    }
    
};

int main(){
    
    /* test code */

// vector<vector<char>> board = {
//   {'5', '3', '0', '0', '7', '0', '0', '0', '0'},
//   {'6', '0', '0', '1', '9', '5', '0', '0', '0'},
//   {'0', '9', '8', '0', '0', '0', '0', '6', '0'},
//   {'8', '0', '0', '0', '6', '0', '0', '0', '3'},
//   {'4', '0', '0', '8', '0', '3', '0', '0', '1'},
//   {'7', '0', '0', '0', '2', '0', '0', '0', '6'},
//   {'0', '6', '0', '0', '0', '0', '2', '8', '0'},
//   {'0', '0', '0', '4', '1', '9', '0', '0', '5'},
//   {'0', '0', '0', '0', '8', '0', '0', '7', '9'}
// };

    SudokuFrame sf;
    vector<vector<char>> board = sf.getFrame();
    Solution ss;
    ss.solveSudoku(board);
    ss.display(board);
    return 0;
}
