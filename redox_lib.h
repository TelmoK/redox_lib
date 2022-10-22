#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

namespace rdx
{
	class Element{
	public:

		std::string nomenclature;
		std::string type;
		int mols = 1;
		std::vector<int> ox_numbers;
		int current_ox_n;

		Element(std::string _nomenclature, std::vector<int> _ox_numbers, std::string _type) 
			: nomenclature(_nomenclature), ox_numbers(_ox_numbers), type(_type)
		{
			if(_ox_numbers.size() == 1)
				current_ox_n = _ox_numbers[0];
		}
		void setCurrentOxNum(int n){
			if(std::find(ox_numbers.begin(), ox_numbers.end(), n) != ox_numbers.end())
				current_ox_n = n;
			else
				std::cout << n << " is not a posible oxidation number for " << nomenclature << std::endl;
		}

		void checkCurrentOxNum(){
			std::sort(ox_numbers.begin(), ox_numbers.end());
			int n;
			for(int ox_num : ox_numbers){
				if(ox_num <= current_ox_n){
					n = ox_num;
				}
			}
			current_ox_n = n;

			if(current_ox_n < ox_numbers[0])
				current_ox_n = ox_numbers[0];
		}

		bool operator==(Element& elem){
			if(elem.nomenclature == this->nomenclature && elem.ox_numbers == this->ox_numbers && elem.current_ox_n == this->current_ox_n)
				return true;
			else
				return false;
		}
	};

	Element getPTElement(std::string nomenclature){

		std::vector<Element> elements;

		elements.push_back(Element("H",  {1,-1},      "hydrogen"));
		elements.push_back(Element("Li", {1},         "metal"));
		elements.push_back(Element("Na", {1},         "metal"));
		elements.push_back(Element("K",  {1},         "metal"));
		elements.push_back(Element("Rb", {1},         "metal"));
		elements.push_back(Element("Cs", {1},         "metal"));
		elements.push_back(Element("Fr", {1},         "metal"));
		elements.push_back(Element("Be", {2},         "metal"));
		elements.push_back(Element("Mg", {2},         "metal"));
		elements.push_back(Element("Ca", {2},         "metal"));
		elements.push_back(Element("Sr", {2},         "metal"));
		elements.push_back(Element("Ba", {2},         "metal"));
		elements.push_back(Element("Ra", {2},         "metal"));
		elements.push_back(Element("V",  {2,3,4,5},   "metal"));
		elements.push_back(Element("Cr", {2,3,6},     "metal"));
		elements.push_back(Element("Mn", {2,3,4,6,7}, "metal"));
		elements.push_back(Element("Fe", {2,3},       "metal"));
		elements.push_back(Element("Co", {2,3},       "metal"));
		elements.push_back(Element("Ni", {2,3},       "metal"));
		elements.push_back(Element("Pd", {2,4},       "metal"));
		elements.push_back(Element("Pt", {2,4},       "metal"));
		elements.push_back(Element("Cu", {1,2},       "metal"));
		elements.push_back(Element("Ag", {1},         "metal"));
		elements.push_back(Element("Au", {1,3},       "metal"));
		elements.push_back(Element("Zn", {2},         "metal"));
		elements.push_back(Element("Cd", {2},         "metal"));
		elements.push_back(Element("Hg", {1,2},       "metal"));
		elements.push_back(Element("B",  {3},         "non metal"));
		elements.push_back(Element("Al", {3},         "metal"));
		elements.push_back(Element("Ga", {3},         "metal"));
		elements.push_back(Element("In", {3},         "metal"));
		elements.push_back(Element("Tl", {1,3},       "metal"));
		elements.push_back(Element("C",  {2,4,-4},    "non metal"));
		elements.push_back(Element("Si", {4,-4},      "non metal"));
		elements.push_back(Element("Ge", {4},         "non metal"));
		elements.push_back(Element("Sn", {2,4},       "metal"));
		elements.push_back(Element("Pb", {2,4},       "metal"));
		elements.push_back(Element("N",  {2,3,-3,4,5},"non metal"));
		elements.push_back(Element("P",  {3,-3,4,5},  "non metal"));
		elements.push_back(Element("As", {3,-3,5},    "non metal"));
		elements.push_back(Element("Sb", {3,-3,5},    "non metal"));
		elements.push_back(Element("Bi", {3,5},       "non metal"));
		elements.push_back(Element("O",  {-2},        "oxygen"));
		elements.push_back(Element("S",  {2,-2,4,6},  "non metal"));
		elements.push_back(Element("Se", {-2,4,6},    "non metal"));
		elements.push_back(Element("Te", {-2,4,6},    "non metal"));
		elements.push_back(Element("Po", {2,4,6},     "non metal"));
		elements.push_back(Element("F",  {-1},        "non metal"));
		elements.push_back(Element("Cl", {1,-1,3,5,7},"non metal"));
		elements.push_back(Element("Br", {1,-1,3,5,7},"non metal"));
		elements.push_back(Element("I",  {1,-1,3,5,7},"non metal"));
		elements.push_back(Element("At", {1,-1,3,5,7},"non metal"));

		(elements[0]).current_ox_n = 1;

		for(Element e : elements){
			if(e.nomenclature == nomenclature)
				return e;
		}

		std::cout << nomenclature << " is not in the periodic table\n";
		return Element("Null",{},"");
	}

	class Reaction_Obj{
	public:
		int mols = 1;
		Reaction_Obj(){}
		virtual std::string reaction_obj_type() = 0;
		virtual int valence() = 0;
		virtual std::string nomenclature() = 0;
	};

	class Compound : public Reaction_Obj{

	public:
		std::string type;
		std::vector<Element> elements;

		//std::string type(){ return type; }

		std::string reaction_obj_type() override{
			return "compound";
		}

		Compound(std::vector<Element> _elements) : elements(_elements){}

		Compound(const Compound& _compound){

			this->type = _compound.type;
			this->elements = _compound.elements;
		}

		Compound(std::string formulation){

			//  SEPARATING ELEMENTS

			std::string currentElement;
			int parenthesis[2] = {-1,-1};
			int parenthIndex = 1;

			for(int i = 0; i < formulation.size(); i++){

				if(formulation[i] == '(') parenthesis[0] = elements.size(); //open parenthesis
				if(formulation[i] == ')')								  //close parenthesis
				{
					parenthesis[1] = elements.size() - 1;
					if(formulation[i+1] > 47 && formulation[i+1] < 58){
						parenthIndex = (int)(formulation[i+1] - 48);
						i++;
						continue;
					}
				}

				if(formulation[i] > 47 && formulation[i] < 58){ //is number

					if(formulation[i+1] > 47 && formulation[i+1] < 58) //if the next char is another number
						currentElement += formulation[i];
					else
					{
						if(elements.size() == 0){ // mols of all compound
							currentElement += formulation[i];
							if(currentElement != "") this->mols = std::stoi(currentElement);
							else this->mols = (int)(formulation[i] - 48);
						}
						else{ // mols of current element
							currentElement += formulation[i];
							if(currentElement != "") (elements[elements.size()-1]).mols = std::stoi(currentElement);
							else this->mols = (elements[elements.size()-1]).mols = (int)(formulation[i] - 48);
						}
						
						currentElement = "";
					}
					continue;
				}

				if(formulation[i] > 64 && formulation[i] < 91){ //capital letter

					if(formulation[i+1] > 96 && formulation[i+1] < 123){ //lower case letter
						currentElement += formulation[i];
						currentElement += formulation[i+1];
						Element elem = getPTElement(currentElement);
						currentElement = "";
						elements.push_back(elem);
						i++;
						continue;
					}
					else{
						currentElement += formulation[i];
						Element elem = getPTElement(currentElement);
						currentElement = "";
						elements.push_back(elem);
						continue;
					}
				}

				
			}

			//apply parenthesis index
			if(parenthesis[0] != -1 && parenthesis[1] != -1){
				for(int i = parenthesis[0]; i < parenthesis[1]+1; i++){
					(elements[i]).mols *= parenthIndex;
				}
			}
		}

		void set_valences(){

			if(elements.size() == 1) //DIATOMIC COMPOUNDS
			{
				this->type = "single element";
				(elements[0]).current_ox_n = 0; // all single elements have a valence of 0
			}
			else if(elements.size() == 2) //BINARY COMPOUNDS  //QUEDA REVISAR QUE EL NÚMERO DE OXIDACIÓN PERTENEZCA AL ELEMENTO
			{
				//	Oxides
				if((elements[0]).type == "metal" && (elements[1]).type == "oxygen"){

					this->type = "oxyde";

					(elements[0]).current_ox_n = (-((elements[1]).current_ox_n) * elements[1].mols) / (elements[0]).mols;

					//  Peroxides
					if(std::find((elements[0]).ox_numbers.begin(), (elements[0]).ox_numbers.end(), (elements[0]).current_ox_n) == (elements[0]).ox_numbers.end()){
						(elements[1]).current_ox_n = -1;  // O's oxidation number is -1 in this case
						(elements[0]).current_ox_n = (-((elements[1]).current_ox_n) * elements[1].mols) / (elements[0]).mols; 
					}

					(elements[0]).checkCurrentOxNum();
				}
				else if((elements[0]).type == "non metal" && (elements[1]).type == "oxygen"){
					
					this->type = "oxyde";

					(elements[0]).current_ox_n = (-((elements[1]).current_ox_n) * elements[1].mols) / (elements[0]).mols;

					(elements[0]).checkCurrentOxNum();
				}
				else if((elements[0]).type == "oxygen" && (elements[1]).type == "non metal"){

					this->type = "oxyde";

					(elements[1]).current_ox_n = (-((elements[0]).current_ox_n) * elements[0].mols) / (elements[1]).mols;

					(elements[1]).checkCurrentOxNum();
				}

				//	Hydrides
				if((elements[0]).type == "hydrogen" && (elements[1]).type == "non metal"){

					this->type = "hydride";

					(elements[1]).current_ox_n = 1; // H is positive when combining with non metals
					(elements[1]).current_ox_n = -(((elements[0]).current_ox_n * (elements[0]).mols) / (elements[1]).mols);

					(elements[1]).checkCurrentOxNum();
				}
				else if((elements[0]).type == "metal" && (elements[1]).type == "hydrogen"){

					this->type = "hydride";

					(elements[1]).current_ox_n = -1;
					(elements[0]).current_ox_n = (-((elements[1]).current_ox_n) * elements[1].mols) / (elements[0]).mols;

					(elements[0]).checkCurrentOxNum();
				}

				//	Binary salts
				if((elements[0]).type == "metal" && (elements[1]).type == "non metal"){

					this->type = "binary salt";

					int smallestDiff = -1;
					int final_metal_ox_n, final_non_metal_ox_n;

					bool found = false;
					for(int metal_ox_n : (elements[0]).ox_numbers){
						for(int non_metal_ox_n : (elements[1]).ox_numbers){

							if(-non_metal_ox_n * elements[1].mols == metal_ox_n * (elements[0]).mols){ //total sum = 0 found

								(elements[0]).current_ox_n = metal_ox_n;
								(elements[1]).current_ox_n = non_metal_ox_n;
								found = true; break;
							}

							int diff = non_metal_ox_n * elements[1].mols + metal_ox_n * (elements[0]).mols;
							diff *= (diff < 0)? -1 : 1; // abs value

							if(smallestDiff == -1){//fist difference calculation
								smallestDiff = diff;
								final_metal_ox_n = metal_ox_n;
								final_non_metal_ox_n = non_metal_ox_n;
								continue;
							}
							if(diff < smallestDiff){//most stable numbers
								smallestDiff = diff;
								final_metal_ox_n = metal_ox_n;
								final_non_metal_ox_n = non_metal_ox_n;
								continue;
							}
						}
						if(found == true) break;
					}

					if(found == false){ // in case not fully stable numbers were found
						(elements[0]).current_ox_n = final_metal_ox_n;
						(elements[1]).current_ox_n = final_non_metal_ox_n;
					}
				}
				//	Non metal - non metal
				else if((elements[0]).type == "non metal" && (elements[1]).type == "non metal"){

					this->type = "non metal - non metal";

					int smallestDiff = -1;
					int final_non_metal1_ox_n, final_non_metal2_ox_n;

					bool found = false;
					for(int non_metal1_ox_n : (elements[0]).ox_numbers){
						for(int non_metal2_ox_n : (elements[1]).ox_numbers){

							if(-non_metal2_ox_n * elements[1].mols == non_metal1_ox_n * (elements[0]).mols){//total sum = 0 found

								(elements[0]).current_ox_n = non_metal1_ox_n;
								(elements[1]).current_ox_n = non_metal2_ox_n;
								found = true; break;
							}

							int diff = non_metal1_ox_n * elements[1].mols + non_metal2_ox_n * (elements[0]).mols;
							diff *= (diff < 0)? -1 : 1; // abs value

							if(smallestDiff == -1){//fist difference calculation
								smallestDiff = diff;
								final_non_metal1_ox_n = non_metal1_ox_n;
								final_non_metal2_ox_n = non_metal2_ox_n;
								continue;
							}
							if(diff < smallestDiff){//most stable numbers
								smallestDiff = diff;
								final_non_metal1_ox_n = non_metal1_ox_n;
								final_non_metal2_ox_n = non_metal2_ox_n;
								continue;
							}
						}
						if(found == true) break;
					}

					if(found == false){ // in case not fully stable numbers were found
						(elements[0]).current_ox_n = final_non_metal1_ox_n;
						(elements[1]).current_ox_n = final_non_metal2_ox_n;
					}
				}
			}
			else if(elements.size() == 3) //TERNARY COMPOUNDS  //QUEDA REVISAR QUE EL NÚMERO DE OXIDACIÓN PERTENEZCA AL ELEMENTO
			{
				//	Hydroxides
				if((elements[0]).type == "metal" && (elements[1]).type == "oxygen" && (elements[2]).type == "hydrogen"){

					this->type = "hydroxide";

					(elements[0]).current_ox_n = -((elements[1]).current_ox_n * (elements[1]).mols + (elements[2]).current_ox_n * (elements[2]).mols) / (elements[0]).mols;

					(elements[0]).checkCurrentOxNum();
				}

				//  Oxoacids
				else if((elements[0]).type == "hydrogen" && ((elements[1]).type == "metal" || (elements[1]).type == "non metal") && (elements[2]).type == "oxygen"){

					this->type = "oxoacid";

					(elements[1]).current_ox_n = -((elements[0]).current_ox_n * (elements[0]).mols + (elements[2]).current_ox_n * (elements[2]).mols) / (elements[1]).mols;

					(elements[1]).checkCurrentOxNum();
				}
				//  Neutral oxisals
				else if((elements[0]).type == "metal" 
					&& ((elements[1]).type == "non metal" || (elements[1]).nomenclature == "V" || (elements[1]).nomenclature == "Cr" || (elements[1]).nomenclature == "Mn") 
					&& (elements[2]).type == "oxygen")
				{
					this->type = "neutral oxysalt";

					int smallestDiff = -1;
					int final_metal_ox_n, final_non_metal_ox_n;

					bool found = false;
					for(int metal_ox_n : (elements[0]).ox_numbers){
						for(int non_metal_ox_n : (elements[1]).ox_numbers){
							
							//total sum = 0 found
							if(metal_ox_n * (elements[0]).mols + non_metal_ox_n * elements[1].mols == -(elements[2]).current_ox_n * (elements[2]).mols){ 

								(elements[0]).current_ox_n = metal_ox_n;
								(elements[1]).current_ox_n = non_metal_ox_n;
								found = true; break;
							}

							int diff = non_metal_ox_n * elements[1].mols + metal_ox_n * (elements[0]).mols + (elements[2]).current_ox_n * (elements[2]).mols;
							diff *= (diff < 0)? -1 : 1; // abs value

							if(smallestDiff == -1){//fist difference calculation
								smallestDiff = diff;
								final_metal_ox_n = metal_ox_n;
								final_non_metal_ox_n = non_metal_ox_n;
								continue;
							}
							if(diff < smallestDiff){//most stable numbers
								smallestDiff = diff;
								final_metal_ox_n = metal_ox_n;
								final_non_metal_ox_n = non_metal_ox_n;
								continue;
							}
						}
						if(found == true) break;
					}

					if(found == false){ // in case not fully stable numbers were found
						(elements[0]).current_ox_n = final_metal_ox_n;
						(elements[1]).current_ox_n = final_non_metal_ox_n;
					}
				}
			}
			else if(elements.size() == 4){

				//  Acid oxisals
				if((elements[0]).type == "metal" 
					&& (elements[1]).type == "hydrogen"
					&& ((elements[2]).type == "non metal" || (elements[1]).nomenclature == "V" || (elements[1]).nomenclature == "Cr" || (elements[1]).nomenclature == "Mn") 
					&& (elements[3]).type == "oxygen")
				{
					this->type = "acid oxysalt";

					int smallestDiff = -1;
					int final_metal_ox_n, final_non_metal_ox_n;

					bool found = false;
					for(int metal_ox_n : (elements[0]).ox_numbers){
						for(int non_metal_ox_n : (elements[2]).ox_numbers){
							
							//total sum = 0 found
							if(metal_ox_n * (elements[0]).mols + non_metal_ox_n * elements[2].mols == -( (elements[1]).current_ox_n * (elements[1]).mols + (elements[3]).current_ox_n * (elements[3]).mols)){ 

								(elements[0]).current_ox_n = metal_ox_n;
								(elements[2]).current_ox_n = non_metal_ox_n;
								found = true; break;
							}

							int diff = non_metal_ox_n * elements[1].mols + metal_ox_n * (elements[0]).mols + (elements[2]).current_ox_n * (elements[2]).mols + (elements[3]).current_ox_n * (elements[3]).mols;
							diff *= (diff < 0)? -1 : 1; // abs value

							if(smallestDiff == -1){//fist difference calculation
								smallestDiff = diff;
								final_metal_ox_n = metal_ox_n;
								final_non_metal_ox_n = non_metal_ox_n;
								continue;
							}
							if(diff < smallestDiff){//most stable numbers
								smallestDiff = diff;
								final_metal_ox_n = metal_ox_n;
								final_non_metal_ox_n = non_metal_ox_n;
								continue;
							}
						}
						if(found == true) break;
					}

					if(found == false){ // in case not fully stable numbers were found
						(elements[0]).current_ox_n = final_metal_ox_n;
						(elements[2]).current_ox_n = final_non_metal_ox_n;
					}
				}
			}
		}

		bool operator==(Compound compound){

			bool equal = true;

			for(Element compound_element : compound.elements){

				bool found = false;

				for(Element this_compound_element : elements){

					if(compound_element.nomenclature == this_compound_element.nomenclature 
						&& compound_element.mols * compound.mols == this_compound_element.mols * this->mols){

						found = true;
						break;
					}
				}
				if(found == false){ equal = false; break; } 
			}

			return equal;
		}

		void showCompoundInfo(){

			std::cout << this->mols << " mol" << ((this->mols > 1)? "s" : "") <<" of:\n";
			for(Element element : elements){
				std::cout << element.nomenclature << element.mols << ((element.current_ox_n < 0)? "" : "+") << element.current_ox_n << "\n";
			}
		}

		std::string nomenclature() override{

			std::string nm = (this->mols > 1)? std::to_string(this->mols) : "";

			for(Element element : elements){
				nm += element.nomenclature;
				if(element.mols > 1) nm += std::to_string(element.mols);
			}

			if(this->valence() != 0){

				if(valence() > 0){
					nm += "+" + ( (valence() == 1)? "" : std::to_string(valence()) );
				}
				else if(valence() < 0){
					nm += "-" + ( (valence() == -1)? "" : std::to_string( valence()*(-1) ) );
				}
			}

			return nm;
		}

		int valence() override{
			int val = 0;
			for(Element element : elements)
				val += element.current_ox_n * element.mols;
			
			return val * this->mols;
		}

		//std::vector<Compound> get_dissociation()

		Compound dissociate(Element element){

			Element foundElement("Null",{},"");

			bool f = false;
			for(Element e : elements){
				if(e.nomenclature == element.nomenclature){
					foundElement = e;
					f = true; break;
				}
			}
			if(f == false){
				std::cout << "Element " << element.nomenclature << " is not in " << this->nomenclature();
				return *this;
			}

			//DISSOCIATION CONDITIONS

			if(this->type == "oxyde" || this->type == "non metal - non metal" || this->type == "single element"){
				return *this;
			}
			else if(this->type == "hydride" || this->type == "binary salt"){
				Compound new_compound({foundElement});
				new_compound.mols = foundElement.mols;
				(new_compound.elements[0]).mols = 1;
				return new_compound;
			}
			else if(this->type == "oxoacid"){
				if(element.nomenclature == "H"){
					Compound new_compound({getPTElement("H")});
					new_compound.mols = element.mols;
					return new_compound;
				}
				else{
					std::vector<Element> els = {(elements[1]), (elements[2])};
					Compound new_compound(els);
					return new_compound;
				}
			}
			else if(this->type == "hydroxide"){
				if(element.type == "metal"){
					Compound new_compound({foundElement});
					new_compound.mols = element.mols;
					(new_compound.elements[0]).mols = 1;
					return new_compound;
				}
				else{
					std::vector<Element> els = {getPTElement("H"), getPTElement("O")};
					Compound new_compound(els);
					new_compound.mols = (elements[1]).mols;
					return new_compound;
				}
			}
			else if(this->type == "neutral oxysalt"){

				if(element.type == "metal" && (elements[0]) == foundElement){
					Compound new_compound({foundElement});
					new_compound.mols = foundElement.mols;
					(new_compound.elements[0]).mols = 1;
					return new_compound;
				}
				else if(foundElement.type == "non metal" || foundElement.nomenclature == "V" || foundElement.nomenclature == "Cr" || foundElement.nomenclature == "Mn"){
					
					std::vector<Element> els = {foundElement, (elements[2])};
					Compound new_compound(els);
					return new_compound;
				}
			}
			else if(this->type == "acid oxysalt"){

				if(element.type == "metal"){
					Compound new_compound({foundElement});
					new_compound.mols = foundElement.mols;
					(new_compound.elements[0]).mols = 1;
					return new_compound;
				}
				else{
					std::vector<Element> els = {(elements[1]), foundElement, (elements[2])};
					Compound new_compound(els);
					return new_compound;
				}
			}
		}
	};

	class Electron : public Reaction_Obj{
	public:

		std::string reaction_obj_type() override{
			return "electron";
		}

		Electron(int _mols){
			this->mols = _mols;
		}

		std::string nomenclature() override{
			return std::to_string(this->mols) + "e-";
		}

		int valence() override{
			return -(this->mols);
		}
	};

	class Reaction{
	public:

		std::vector<Reaction_Obj*> reactants;
		std::vector<Reaction_Obj*> products;

		Reaction(){}

		Reaction(std::vector<Reaction_Obj*> _reactants, std::vector<Reaction_Obj*> _products): reactants(_reactants), products(_products){}

		void showReaction(){

			for(int i = 0; i < reactants.size(); i++){
				std::cout << (reactants[i])->nomenclature() << ((i < reactants.size()-1)? " + " : " -> ");
			}
			for(int i = 0; i < products.size(); i++){
				std::cout << (products[i])->nomenclature() << ((i < products.size()-1)? " + " : "");
			}
		}

		void operator*=(int n){
			for(Reaction_Obj* reactant : reactants)
				reactant->mols *= n;
			for(Reaction_Obj* product : products)
				product->mols *= n;
		}

		Reaction operator+(Reaction reaction){

/*			std::vector<Reaction_Obj*> final_reactants;
			for(Reaction_Obj* reactant : reaction.reactants){
				for(Reaction_Obj* this_products : this->products){

				}
			}*/
		}

		void valance(){

			//the reaction will be valanced using the algebraic method
			//for solving the linear equations this function applies the Gauss elimination and Gauss Jordan method

		/*	int num_of_coeficents = 4;
			int num_of_equations = 4;*/
		/*	int num_of_coeficents = 3;
			int num_of_equations = 3;*/
			int num_of_coeficents = 5;
			int num_of_equations = 5;

			float** matrix = new float*[ num_of_equations ];
			for(int i = 0; i < num_of_equations; i++)
				matrix[i] = new float[ num_of_coeficents+1 ];

		/*	matrix[0][0] = 0;   matrix[0][1] =  0;   matrix[0][2] = 1;   matrix[0][3] =  0;   matrix[0][4] = 1;
			matrix[1][0] = 0;   matrix[1][1] =  0;   matrix[1][2] = 2;   matrix[1][3] =  1;   matrix[1][4] = 3;
			matrix[2][0] = 1;   matrix[2][1] =  0;   matrix[2][2] = 0;   matrix[2][3] = -2;   matrix[2][4] = 0;
			matrix[3][0] = 1;   matrix[3][1] = -1;   matrix[3][2] = 0;   matrix[3][3] =  0;   matrix[3][4] = 0;*/
		/*	matrix[0][0] = 1;   matrix[0][1] =  -2;   matrix[0][2] = 3;   matrix[0][3] =  11;
			matrix[1][0] = 4;   matrix[1][1] =  1;   matrix[1][2] = -1;   matrix[1][3] =  4;
			matrix[2][0] = 2;   matrix[2][1] =  -1;   matrix[2][2] = 3;   matrix[2][3] = 10;*/
			matrix[0][0] = 0.0; matrix[0][1] = 0.0; matrix[0][2] = 0.0; matrix[0][3] =  1.0; matrix[0][4] = 0.0; matrix[0][5] = 2.0; 
			matrix[1][0] = 0.0; matrix[1][1] = 0.0; matrix[1][2] = 1.0; matrix[1][3] =  0.0; matrix[1][4] = 0.0; matrix[1][5] = 5.0; 
			matrix[2][0] = 1.0; matrix[2][1] = 2.0; matrix[2][2] = -2.0; matrix[2][3] = -3.0; matrix[2][4] = 0.0; matrix[2][5] = 0.0;
			matrix[3][0] = 1.0; matrix[3][1] = 0.0; matrix[3][2] = 0.0; matrix[3][3] =  0.0; matrix[3][4] = -1.0; matrix[3][5] = 0.0; 
			matrix[4][0] = 3.0; matrix[4][1] = 1.0; matrix[4][2] = -4.0; matrix[4][3] = -4.0; matrix[4][4] = -1.0; matrix[4][5] = 0.0;

			for(int i = 0; i < num_of_equations; i++){
				for(int j = 0; j <num_of_coeficents+1; j++){
					std::cout << matrix[i][j] << "  ";
				}
				std::cout << "\n\n";
			}std::cout << "\n----------------------\n";
			//ensure that all the diagonal values are != 0
			for(int i = 0; i < num_of_equations; i++){

				if(matrix[i][i] != 0) continue;

				//if the diagonal coeficent is 0
				// search for a matrix row to swap
				for(int j = i+1; j < num_of_equations; j++){

					if(matrix[j][i] == 0) continue;

					//if a row was found
					//swaping the rows
					for(int k = 0; k < num_of_coeficents+1; k++){

						float aux = matrix[j][k];
						matrix[j][k] = matrix[i][k];
						matrix[i][k] = aux;
					}
					break;
				}
			}

			gauss_jordan_reduction:

			for(int i = 0; i < num_of_equations; i++){
				for(int j = 0; j <num_of_coeficents+1; j++){
					std::cout << matrix[i][j] << "  ";
				}
				std::cout << "\n\n";
			}std::cout << "\n----------------------\n";

			//convert the coeficients under the diagonal into 0
			for(int i = 0; i < num_of_coeficents; i++){
				for(int j = i; j < num_of_equations; j++){

					if(j > i && matrix[j][i] != 0){

						int best_scalonated_num_of_coef = num_of_coeficents;
						int best_scalonated_row = 0;

						for(int k = 0; k < num_of_equations; k++){//searching for row to operate

							if(k != j && matrix[k][i] != 0){ //posible found row

								int aprox_index = 0;
								for(int s = 0; s < num_of_coeficents; s++){
									if(matrix[k][s] != 0) break;
									aprox_index++;
								}

								//setting the best row to operate
								if(best_scalonated_num_of_coef > num_of_coeficents-aprox_index){
									best_scalonated_row = k;
									best_scalonated_num_of_coef = num_of_coeficents-aprox_index;
								}

							}
						}

						//start reduction operation
						float oposite_coef = -matrix[j][i];
						for(int n = 0; n < (num_of_coeficents+1); n++){
							matrix[j][n] = matrix[j][n] * matrix[best_scalonated_row][i] +matrix[best_scalonated_row][n] * oposite_coef; //reducing
						}
						std::cin.get();
						goto gauss_jordan_reduction;
					}
				}
			}

			//convert the coeficients on top of the diagonal into 0
			for(int i = 0; i < num_of_coeficents; i++){
				for(int j = 0; j < num_of_equations; j++){

					if(j < i && matrix[j][i] != 0){

						int best_scalonated_num_of_coef = num_of_coeficents;
						int best_scalonated_row = 0;

						for(int k = 0; k < num_of_equations; k++){//searching for row to operate

							if(k != j && matrix[k][i] != 0){ //posible found row

								int aprox_index = 0;
								for(int s = 0; s < num_of_coeficents; s++){
									if(matrix[k][s] != 0) break;
									aprox_index++;
								}

								//setting the best row to operate
								if(best_scalonated_num_of_coef > num_of_coeficents-aprox_index){
									best_scalonated_row = k;
									best_scalonated_num_of_coef = num_of_coeficents-aprox_index;
								}

							}
						}

						//start reduction operation
						float oposite_coef = -matrix[j][i];
						for(int n = 0; n < (num_of_coeficents+1); n++){
							matrix[j][n] = matrix[j][n] * matrix[best_scalonated_row][i] +matrix[best_scalonated_row][n] * oposite_coef; //reducing
						}
						std::cin.get();
						goto gauss_jordan_reduction;
					}
				}
			}

			//setting coeficents to 1
			for(int i = 0; i < num_of_equations; i++){

				float index_factor = matrix[i][i];
				for(int j = 0; j <num_of_coeficents+1; j++)
					matrix[i][j] = matrix[i][j] / index_factor;
				
			}

			for(int i = 0; i < num_of_equations; i++){
				for(int j = 0; j <num_of_coeficents+1; j++){
					std::cout << matrix[i][j] << "  ";
				}
				std::cout << "\n\n";
			}std::cout << "\n----------------------\n";
		}
	};

	class Redox_Valancer{

		Reaction raw_reaction;
		std::vector<Reaction> semireactions; 

	public:

		Redox_Valancer(){}

		Redox_Valancer(Reaction _reaction) : raw_reaction(_reaction){}

		void set_semireactions(){

			for(Reaction_Obj* R : raw_reaction.reactants){
				for(Reaction_Obj* P : raw_reaction.products){

					if(R->reaction_obj_type() == "compound" && P->reaction_obj_type() == "compound"){

						Compound reactant = *(dynamic_cast<Compound*>(R));
						reactant.set_valences();
						Compound product = *(dynamic_cast<Compound*>(P));
						product.set_valences();
						
						for(Element reactant_element : reactant.elements){
							for(Element product_element : product.elements){
								
								if(reactant_element.nomenclature == product_element.nomenclature &&
									reactant_element.current_ox_n != product_element.current_ox_n)  //element has changed its oxidation number
								{
									std::vector<Reaction_Obj*> new_semireaction_reactants = {new Compound(reactant.dissociate(reactant_element))};
									std::vector<Reaction_Obj*> new_semireaction_products = {new Compound(product.dissociate(product_element))};
									
									Reaction new_semireaction(new_semireaction_reactants, new_semireaction_products);

									semireactions.push_back(new_semireaction);
								}
							}
						}
					}
				}
			}
		}

		void showSeireactions(){

			for(Reaction sr : semireactions){
				sr.showReaction(); 
				std::cout << std::endl;
			}
		}

		/*void valance_semireactions(){
	
			for(Reaction sr : semireactions)
				sr.valance(); 
			
			//add H2Os
		}*/
	};
}