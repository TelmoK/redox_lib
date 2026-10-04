#include <iostream>
//#include "redox_lib.h"
//#include "cinetic_lib.h"
#include "chemistry_lib.h"

using namespace chm;

#include <iostream>
#include <cmath>

// Encuentra la fracción exacta (numerador y denominador) usando fracciones continuas
void decimalAFraccion(float decimal, int& numerador, int& denominador, int maxNumerador = 1000) {
   // Manejar el signo y separar la parte entera
   float signo = (decimal < 0) ? -1.0 : 1.0;
   decimal = std::abs(decimal);
   
   // Si el número ya es prácticamente cero, evitamos dividir por cero
   if (decimal < 1e-5f) {
      numerador = 0;
      denominador = 1;
      return;
   }
   
   int i = 1;
   bool numbersFound = false;

   float inverse_decimal = 1.0f / decimal;

   while(i < maxNumerador && !numbersFound)
   {
      numerador = i;
      float den = inverse_decimal * numerador;

      // Usamos una pequeña tolerancia en lugar de == 0.0 para que el ruido binario no se lo salte
      float diff = den - std::round(den);
      
      if(std::abs(diff) < 1e-4f) {
         numbersFound = true;
         numerador *= static_cast<int>(signo);
         denominador = static_cast<int>(std::round(den));
      }
      
      ++i;
   }
}


int main(){

   /* rdx::Compound c("BeCO3");
    c.set_valences(); 
    c.showCompoundInfo();
    std::cout << c.valence() << std::endl;*/
/*
   std::vector<Reaction_Obj*> r = {new Compound("K2Cr2O7"), new Compound("HI"), new Compound("H2SO4")};
   std::vector<Reaction_Obj*> p = {new Compound("K2SO4"), new Compound("I2"), new Compound("Cr2(SO4)3"), new Compound("H2O")};*/
/*
   std::vector<rdx::Reaction_Obj*> r = {new rdx::Compound("K2Cr2O7"), new rdx::Compound("6HI"), new rdx::Compound("H2SO4")};
   std::vector<rdx::Reaction_Obj*> p = {new rdx::Compound("K2SO4"), new rdx::Compound("3I2"), new rdx::Compound("2Cr2(ClO4)3"), new rdx::Compound("7H2O")};*/
/*   std::vector<Reaction_Obj*> r = {new Compound("Fe"), new Compound("CuSO4")};
   std::vector<Reaction_Obj*> p = {new Compound("Cu"), new Compound("FeSO4")};*/

   std::vector<Reaction_Obj*> r = {new Compound("H2O")};
   std::vector<Reaction_Obj*> p = {new Compound("H2"), new Compound("O2")};

   Reaction R(r,p);
   std::cout << "Unvalanced:\n";
   R.showReaction();

   R.valance();
   
   std::cout << "Valanced:\n";
   R.showReaction();
  // std::cout << std::endl<< cmt::get_reaction_speed_equation(R);
 /*  rdx::Redox_Valancer R(rdx::Reaction(r,p));
   R.set_semireactions();
   R.showSemireactions();
   R.valance_semireactions();
   R.showSemireactions();
   R.mix_semireactions();*/
   int num, den = 333;
   float n = 56.0/174.0;
   decimalAFraccion(n, num, den);

  std::cout << "num: " << num << "  denom: " << den;
    std::cin.get();
    
}