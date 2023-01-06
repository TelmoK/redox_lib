#include <iostream>
//#include "redox_lib.h"
//#include "cinetic_lib.h"
#include "chemistry_lib.h"
using namespace chm;
int main(){

   /* rdx::Compound c("BeCO3");
    c.set_valences(); 
    c.showCompoundInfo();
    std::cout << c.valence() << std::endl;*/

   std::vector<Reaction_Obj*> r = {new Compound("K2Cr2O7"), new Compound("HI"), new Compound("H2SO4")};
   std::vector<Reaction_Obj*> p = {new Compound("K2SO4"), new Compound("I2"), new Compound("Cr2(SO4)3"), new Compound("H2O")};
/*
   std::vector<rdx::Reaction_Obj*> r = {new rdx::Compound("K2Cr2O7"), new rdx::Compound("6HI"), new rdx::Compound("H2SO4")};
   std::vector<rdx::Reaction_Obj*> p = {new rdx::Compound("K2SO4"), new rdx::Compound("3I2"), new rdx::Compound("2Cr2(ClO4)3"), new rdx::Compound("7H2O")};*/
/*   std::vector<Reaction_Obj*> r = {new Compound("Fe"), new Compound("CuSO4")};
   std::vector<Reaction_Obj*> p = {new Compound("Cu"), new Compound("FeSO4")};*/
   Reaction R(r,p);
   R.valance();
   R.showReaction();
  // std::cout << std::endl<< cmt::get_reaction_speed_equation(R);
 /*  rdx::Redox_Valancer R(rdx::Reaction(r,p));
   R.set_semireactions();
   R.showSemireactions();
   R.valance_semireactions();
   R.showSemireactions();
   R.mix_semireactions();*/
  

    std::cin.get();
    
}