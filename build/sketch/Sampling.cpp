#include "Sampling.h"

float data_sampling(int data, int interval){
    delay(10);
    int sum_data = 0;
    for(int i=0; i<interval; i++){
        sum_data +=data;
        delay(1);
    }
    return sum_data/interval;
}