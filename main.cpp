#include <iostream>
#include "redox_lib.h"

int main(){

//    rdx::Compound c("BeCO3");
//    c.set_valences(); 
//    c.showCompoundInfo();
//    std::cout << c.valence() << std::endl;
/*
    std::vector<rdx::Reaction_Obj*> r = {new rdx::Compound("K2Cr2O7"), new rdx::Compound("KI"), new rdx::Compound("H2SO4")};
    std::vector<rdx::Reaction_Obj*> p = {new rdx::Compound("K2SO4"), new rdx::Compound("I2"), new rdx::Compound("Cr2(SO4)3"), new rdx::Compound("H2O")};


    rdx::Redox_Valancer R(rdx::Reaction(r,p));
    R.set_semireactions();
    R.showSeireactions();*/
    std::vector<rdx::Reaction_Obj*> rr = {new rdx::Compound("P2S5"), new rdx::Compound("HNO3"), new rdx::Compound("H2O")};
    std::vector<rdx::Reaction_Obj*> p = {new rdx::Compound("H2SO4"), new rdx::Compound("H3PO4"), new rdx::Compound("NO")};
    rdx::Reaction r(rdx::Reaction(rr,p));
    r.valance();
    
    std::cin.get();
    
}