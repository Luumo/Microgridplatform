#include "Sampling.h"

float data_sampling(int data, int interval){
    long sum_data = 0;
    for(int i=0; i<interval; i++){
        sum_data +=data;
        delay(1);
    }
    return float(sum_data/interval);
}