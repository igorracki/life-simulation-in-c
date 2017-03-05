// functions
void populateArray();
void traverseArray();
int moveFish(int row, int col);
void printArray();
void getInput(int argc, char* argv[]);
int randomDirection(int neighbors[]);
//

// variables
int* arr; // 0 = empty, odd = fish, even = shark

int rows;
int cols;

int number_of_sharks;
int number_of_fishes;
int sea_size;
int year_no;



// controlFlag == 1 no output only time of execution
// controlFlag == 2 output for Danislav program graph 
// controlFlag == 3 output for Danislav program fieldState
int controlFlag;

//to purpose of this program shark age is always multi-played by 2  
static const int SHARK_LIFE = 140;
static const int SHARK_BIRTH_TIME = 40;
static const int SHARK_START_AGE = 30;
