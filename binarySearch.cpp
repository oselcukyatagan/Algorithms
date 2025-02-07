#include <iostream>
using namespace std;


class search{
    public:

    static int binarySearch(int data[], int size, int target){

        int lower = 0;
        int upper = size - 1;
        
    
        while(lower <= upper){
            int middle = lower + (upper-lower) / 2;
            if(data[middle] == target){
                return middle;
            }
            else if(data[middle] < target) lower = middle + 1;
            else upper = middle - 1;
        }
        return -1;
    }
        
};


int main(){

    int array[100];

    for (int i = 0; i < 100; i ++)
    {
        array[i] = i * 2;
    }
    
    cout << search::binarySearch(array, 100, 96) << endl;


    cout << "selam" << endl;
    return 0;
}