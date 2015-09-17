#include <cmath>
#include <iostream>

 
int order = 2;
int size = 256;
 
int main()
{
    long max_nodes = (long)(pow(order,log(size)/log(order/2.0)-1) + 1);
}
