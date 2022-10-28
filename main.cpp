#include <iostream>
#include "redox_lib.h"

int main(){

    rdx::Compound c("BeCO3");
    c.set_valences(); 
    c.showCompoundInfo();
    std::cout << c.valence() << std::endl;

/*    std::vector<rdx::Reaction_Obj*> r = {new rdx::Compound("K2Cr2O7"), new rdx::Compound("KI"), new rdx::Compound("H2SO4")};
    std::vector<rdx::Reaction_Obj*> p = {new rdx::Compound("K2SO4"), new rdx::Compound("I2"), new rdx::Compound("Cr2(SO4)3"), new rdx::Compound("H2O")};*/
  /*  std::vector<rdx::Reaction_Obj*> r = {new rdx::Compound("ZnS"), new rdx::Compound("HNO3")};
    std::vector<rdx::Reaction_Obj*> p = {new rdx::Compound("ZnSO4"), new rdx::Compound("NO2"), new rdx::Compound("H2O")};*/
    std::vector<rdx::Reaction_Obj*> r = {new rdx::Compound("KMnO4"), new rdx::Compound("KAsO2"), new rdx::Compound("KOH")};
    std::vector<rdx::Reaction_Obj*> p = {new rdx::Compound("MnO2"), new rdx::Compound("K3AsO4"), new rdx::Compound("H2O")};

    rdx::Redox_Valancer R(rdx::Reaction(r,p));
    R.set_semireactions();
    R.showSeireactions();
    R.valance_semireactions();
    R.showSeireactions();/*
    std::vector<rdx::Reaction_Obj*> rr = {new rdx::Compound("HCl"), new rdx::Compound("MnO2")};
    std::vector<rdx::Reaction_Obj*> p = {new rdx::Compound("MnCl2"), new rdx::Compound("H2O"), new rdx::Compound("Cl2")};
    rdx::Reaction r(rdx::Reaction(rr,p));
    r.valance();
    r.showReaction();*/
    
    std::cin.get();
    
}