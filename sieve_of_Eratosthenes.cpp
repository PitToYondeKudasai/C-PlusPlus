#include <iostream>
#include <vector>

// This code implements the sieve of Eratosthenes to compute the prime numbers
// in a given interval
int main(){
  std::cout << "Please enter an integer..." << '\n';
  int upper_limit;
  std::cin >> upper_limit;
  std::vector<int> integers(upper_limit);
  for(int i = 1; i <= integers.size() ; i++){
    integers[i] = i;
  }

  for(int i = 2; i <= upper_limit; i++){
    if(integers[i] > 0){
      for(int ii = i + 1; ii <= integers.size(); ii++){
        if(integers[ii]%integers[i] == 0){
          integers[ii] = -1;
        }
      }
    }
  }

  for(int x: integers){
    if(x > 0){
      std::cout << x << '\n';
    }
  }
  return 0;
}
