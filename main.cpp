#include <iostream>
#include "redox_lib.h"

int main(){

   /* rdx::Compound c("BeCO3");
    c.set_valences(); 
    c.showCompoundInfo();
    std::cout << c.valence() << std::endl;*/
/*
   std::vector<rdx::Reaction_Obj*> r = {new rdx::Compound("K2Cr2O7"), new rdx::Compound("HI"), new rdx::Compound("H2SO4")};
   std::vector<rdx::Reaction_Obj*> p = {new rdx::Compound("K2SO4"), new rdx::Compound("I2"), new rdx::Compound("Cr2(SO4)3"), new rdx::Compound("H2O")};
*/
   std::vector<rdx::Reaction_Obj*> r = {new rdx::Compound("K2Cr2O7"), new rdx::Compound("6HI"), new rdx::Compound("H2SO4")};
   std::vector<rdx::Reaction_Obj*> p = {new rdx::Compound("K2SO4"), new rdx::Compound("3I2"), new rdx::Compound("2Cr2(SO4)3"), new rdx::Compound("7H2O")};
   rdx::Reaction R(r,p);
   R.valance();
   R.showReaction();
 /*  rdx::Redox_Valancer R(rdx::Reaction(r,p));
   R.set_semireactions();
   R.showSemireactions();
   R.valance_semireactions();
   R.showSemireactions();
   R.mix_semireactions();*/
  

    std::cin.get();
    
}