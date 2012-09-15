#include <iostream>
#include <vector>
#include "../src/Filter.h"
#include "../src/Progression.h"

using namespace std; // Don't do this.


bool mod2(int i) { return i%2 == 0; }
//bool mod3(int i) { return i%3 == 0; }


int main() {
  // Progression
  auto thirtym = FIter::Progression(30, -2);
  auto thirtymit = thirtym.begin();
  for(int i=0; i<5; ++i) {
    cout << *thirtymit << endl;
    ++thirtymit;
  }
  cout << endl;


  // Filter
  std::vector<int> v;
  for(int i=0; i<30; i++) v.push_back(i);
  
  auto Mod3F = FIter::Filter([](int x){return x%3==0;})(v.begin(), v.end());
  auto Mod6F = FIter::Filter(mod2)(Mod3F.begin(), Mod3F.end());
  
  for(auto item : Mod6F) {
    std::cout << item << std::endl;
  }
  
  
  
}
