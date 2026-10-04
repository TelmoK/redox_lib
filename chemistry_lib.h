#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <initializer_list>
#include <stdexcept>
#include <math.h>
#include <numeric>

namespace chm
{
	namespace utils
	{
		bool chr_is_number(char c)
		{
			return c > 47 && c < 58;
		}

		bool chr_is_uppercase(char c)
		{
			return c > 64 && c < 91;
		}

		bool chr_is_lowercase(char c)
		{
			return c > 96 && c < 123;
		}

		int chr_to_int(char c)
		{
			return (int)(c - 48);
		}

		template<class T>
		bool is_value_in(T value, std::initializer_list<T> candidates)
		{
			bool found = false;
			auto it = candidates.begin();

			while (!found && it != candidates.end()) {
				found = (*it == value); // the requested value is in the list
				++it; 
			}

			return found;
		}
	}

	namespace math
	{
		void decimal_to_fraccion(float decimal, int& numerator, int& denominator, int maxNumerator = 1000) 
		{
			// Separate de sign from the integer part
			float sign = (decimal < 0) ? -1.0 : 1.0;
			decimal = std::abs(decimal);
			
			// If the number is almost zero we avoid dividing by zero
			if (decimal < 1e-5f) {
				numerator = 0;
				denominator = 1;
				return;
			}
			
			int i = 1;
			bool numbersFound = false;

			float inverseDecimal = 1.0f / decimal;

			while(i < maxNumerator && !numbersFound)
			{
				// Getting the denominator by formula sqrt(inverseDecimal * decimal) * numerator^2
				numerator = i;
				float den = inverseDecimal * numerator; // simplification of the formula, better to use as it doesn't have square root

				// Check if the posible denomiator is an integer, we check with a numeric noise tolerance 
				float diff = den - std::round(den);
				
				if(std::abs(diff) < 1e-4f) {
					numbersFound = true;
					numerator *= static_cast<int>(sign);
					denominator = static_cast<int>(std::round(den));
				}
				
				++i;
			}
		}

		long long lcm(const std::vector<int>& values) 
		{
			// We use long long for computational precission safety
			long long globalLCM = 1;

			for (int val : values) {
				if (val > 0) { // Ignoring zeros for safety
					globalLCM = std::lcm(globalLCM, val);
				}
			}
			return globalLCM;
		}

		template<class T>
		class Vector
		{
			std::vector<T> _elements;

		public:

			Vector(int size)
			{
				_elements.resize(size, 0);
			}

			Vector() : Vector(0){}

			Vector(std::initializer_list<T> elementList)
				: _elements{elementList}
			{
			}

			Vector(std::vector<T> stdVec)
				: _elements(stdVec)
			{
			}

			int dimensions()
			{
				return _elements.size();
			}

			T& operator[](std::size_t idx)
			{
				return _elements[idx]; 
			}

			Vector<T>& operator/=(const T& scalar)
			{
				for(T& elem : _elements)
					elem /= scalar;

				return *this;
			}

			Vector<T> operator/(const T& scalar)
			{
				Vector<T> vec(dimensions());

				for(int i = 0; i < _elements.size(); ++i)
					vec[i] = _elements[i] / scalar;

				return vec;
			}

			Vector<T>& operator*=(const T& scalar)
			{
				for(T& elem : _elements)
					elem *= scalar;

				return *this;
			}

			Vector<T> operator*(const T& scalar)
			{
				Vector<T> vec(dimensions());

				for(int i = 0; i < _elements.size(); ++i)
					vec[i] = _elements[i] * scalar;

				return vec;
			}

			Vector<T>& operator+=(const Vector<T>& other)
			{
				if(dimensions() != other.dimensions())
					throw std::runtime_error("Vector<T> error: Vectors must have the same dimensions");

				for(int i = 0; i < _elements.size(); ++i)
					_elements[i] += other[i];

				return *this;
			}

			Vector<T> operator+(const Vector<T>& other)
			{
				if(dimensions() != other.dimensions())
					throw std::runtime_error("Vector<T> error: Vectors must have the same dimensions");

				Vector<T> vec(dimensions());

				for(int i = 0; i < _elements.size(); ++i)
					vec[i] = _elements[i] + other[i];

				return vec;
			}

			Vector<T> operator-()
			{
				return (*this * -1);
			}

			Vector<T>& operator-=(const Vector<T>& other)
			{
				return (*this += -other);
			}

			Vector<T> operator-(const Vector<T>& other)
			{
				return (*this + (-other));
			}

			// Forwarding the internal vector interator to work with for-each loops
			auto begin() { return _elements.begin(); }
			auto end() { return _elements.end(); }

			// Const overloads so it works when the Team object is const
			auto begin() const { return _elements.begin(); }
			auto end() const { return _elements.end(); }

			std::vector<T> toStd()
			{
				return _elements;
			}
		};

		template<class T>
		class Matrix
		{
		protected:
			std::vector<std::vector<T>> _matrix;
			
			int _columnNum;
			int _rowNum;

		public:

			Matrix(int rows, int columns)
			{
				_matrix.resize(rows);

				for(std::vector<T>& row : _matrix)
					row.resize(columns);

				_columnNum = columns;
				_rowNum = rows;
			}

			Matrix() : Matrix(0, 0)
			{
			}

			int rowNum()
			{
				return _rowNum;
			}

			int colNum()
			{
				return _columnNum;
			}

			std::pair<T, T> dimensions()
			{
				return {_rowNum, _columnNum};
			}

			T& operator[](std::size_t idx)
			{
				return _matrix[idx]; 
			}

			/**
			 * Returns the indexed column as a vector in a top to bottom order
			 */
			Vector<T> col(int columnIndex)
			{
				Vector<T> vec(_rowNum);

				if(columnIndex < 0 || columnIndex >= _columnNum)
					return vec;

				for(int i = 0; i < _matrix.size(); ++i) {
					std::vector<T>& row = _matrix[i];
					vec[i] = row[columnIndex];
				}

				return vec;
			}

			Vector<T> row(int rowIndex)
			{
				if(rowIndex < 0 || rowIndex >= _columnNum) {
					Vector<T> vec;
					return vec;
				}

				return Vector<T>(_matrix[rowIndex]);
			}

			void setCol(int columnIndex, Vector<T> columnVector)
			{
				if(columnIndex < 0 || columnIndex >= _columnNum || columnIndex < 0 || columnIndex >= _columnNum)
					throw std::out_of_range("Matrix<T> error: column index was out of range");

				for(int i = 0; i < _matrix.size(); ++i) {
					std::vector<T>& row = _matrix[i];
					row[columnIndex] = columnVector[i];
				}
			}

			void setRow(int rowIndex, Vector<T> rowVector)
			{
				if(rowIndex < 0 || rowIndex >= _columnNum || rowIndex < 0 || rowIndex >= _columnNum)
					throw std::out_of_range("Matrix<T> error: column index was out of range");

				std::vector<T> vec = rowVector.toStd();

				int sizeDiff = rowVector.dimensions() - _columnNum;
				
				// If the given vector is bigger we clamp it
				if(sizeDiff > 0)
					for(int i = 0; i < sizeDiff; ++i)
						vec.pop_back();

				_matrix[rowIndex] = rowVector.toStd();
			}

			Matrix<T>& operator+=(const Matrix<T>& other)
			{
				if(dimensions() != other.dimensions())
					throw std::runtime_error("Matrix<T> error: Matrices must have the same dimensions");

				for(int i = 0; i < _matrix.size(); ++i)
					_matrix[i] += other[i];

				return *this;
			}

			Matrix<T> operator+(const Vector<T>& other)
			{
				if(dimensions() != other.dimensions())
					throw std::runtime_error("Matrix<T> error: Matrices must have the same dimensions");

				Vector<T> vec(dimensions());

				for(int i = 0; i < _matrix.size(); ++i)
					vec[i] = _matrix[i] + other[i];

				return vec;
			}

			void pushBackColumn()
			{
				++_columnNum;

				for(std::vector<T>& row : _matrix)
					row.resize(_columnNum, 0); // Add a new position with a 0
			}

			void swapRows(int r1, int r2)
			{
				if(r1 < 0 || r1 >= _rowNum || r2 < 0 || r2 >= _rowNum)
					throw std::out_of_range("Matrix<T> error: row index was out of range");

				if(r1 == r2)
					return;

				_matrix[r1].swap(r2);
			}

			void swapCols(int c1, int c2)
			{
				if(c1 < 0 || c1 >= _columnNum || c2 < 0 || c2 >= _columnNum)
					throw std::out_of_range("Matrix<T> error: column index was out of range");

				if(c1 == c2)
					return;

				for(std::vector<T>& row : _matrix)
					std::swap(row[c1], row[c2]);
			}
		};

		template<class T>
		class EquationSystemSolver
		{
		public:
			virtual ~EquationSystemSolver(){}
			std::vector<T> solve(Matrix<T> system_matrix) = 0;
		};

		template<class T>
		class GaussJordan_EqSystemSolver : public EquationSystemSolver<T>
		{
		public:
			GaussJordan_EqSystemSolver(){}
			~GaussJordan_EqSystemSolver(){}
		
		private:
			
			void applyGaussJordan(Matrix<T>& system_matrix, int& firstFreeVariable)
			{
				// Default value if the system is consistent independent, the index of the free variable is the number of variables
				firstFreeVariable = system_matrix.colNum() - 1;

				// The system can be consistent dependent if we find a row of zeros
				bool rowOfZerosFound = false; 
				int row = 0;
				
				while(row < system_matrix.rowNum() && !rowOfZerosFound)
				{
					// Look for the maximum value in the lower part of the column
					Vector<T> column = system_matrix.col(row);
					int betterPivotColValue = column[row];
					int betterPivotRow = row;
					
					for(int r = row + 1; r < column.dimensions(); ++r)
					{
						if(abs(column[r]) > abs(betterPivotColValue)) {
							betterPivotColValue = column[r];
							betterPivotRow = r;
						}
					}

					// Check if we have a consistent dependent system if we only find zeros in the rest of the column
					if(betterPivotColValue < 1e-5f)
					{
						rowOfZerosFound = true;
						firstFreeVariable = row; // The index of the first free variable of the consistent dependet system
					}
					else // Otherwise we continue with the Gauss-Jordan algorithm
					{
						// Swap row with better candidate if there is one
						system_matrix.swapRows(row, betterPivotRow);

						// Normalize de row turning the diagonal column to 1
						Vector<T> normalizedRow = system_matrix.row(row) / betterPivotColValue;
						system_matrix.setRow(row, normalizedRow);

						// Clean the column in the rest of rows making zero their value
						for(int r = 0; r < system_matrix.rowNum(); ++r)
						{
							if(r != row)
							{
								int currentCol = row; // just for clarity

								T factor = system_matrix[r][currentCol];
								Vector<T> cleanedRow = system_matrix.row(r) - system_matrix.row(row) * factor;
							}
						}
					
						++row;						
					}
				}

				// If the system has more variables than equations
				if(!rowOfZerosFound) {
					firstFreeVariable = system_matrix.rowNum();
				}
			}

			std::vector<T> getDenominators(std::vector<T> decimalList)
			{
				std::vector<T> denominators;

				for (T val : decimalList) 
				{
					if (val > 1e-5) { // We ignore the zeros (considering that the can be numeric noise)
						int num, den;
						decimal_to_fraccion(val, num, den);
						denominators.push_back(den);
					}
				}

				return denominators;
			}

			std::vector<T> obtainSolution(Matrix<T>& system_matrix, int firstFreeVariable)
			{
				int numOfVariables = system_matrix.colNum() - 1;

				std::vector<T> solution(numOfVariables, 0);

				// If the system is consisten independent we are done
				if(firstFreeVariable >= numOfVariables) 
				{
					for(int i = 0; i < firstFreeVariable; ++i)
					{
						const int freeTermCol = system_matrix.colNum() - 1;
						solution[i] = system_matrix[i][freeTermCol];
					}

					return solution;
				}

				// If the system is consistent dependent
				int freeVariables = numOfVariables - firstFreeVariable;

				// Fill the solution vector as a combination of solutions with a free variable equal to 1 each (one-hot)
				for(int freeVar = 0; freeVar < freeVariables; ++freeVar)
				{
					for(int i = 0; i < firstFreeVariable; ++i)
					{
						const int freeTermCol = system_matrix.colNum() - 1;
						const int freeVarCol = firstFreeVariable + freeVar;

						solution[i] += system_matrix[i][freeTermCol];
						solution[i] -= system_matrix[i][freeVarCol];
					}
				}

				// The solution may have decimals, we want to find a least common multiple for the denominators of those
				std::vector<T> decimalDenominators = getDenominators(solution);

				long long denominatorLcm = ::chm::math::lcm(decimalDenominators);

				// We multiply the whole solution vector by the lcm to obtain all values as integers
				for (int i = 0; i < solution.size(); ++i) {
					solution[i] = std::round(solution[i] * denominatorLcm);
				}

				return solution;
			}
		
		public:

			std::vector<T> solve(Matrix<T> system_matrix) override
			{
				// Add column with de constant term that is meant to be zero
				system_matrix.pushBackColumn();

				int firstFreeVariable;
				applyGaussJordan(system_matrix, firstFreeVariable);

				return obtainSolution(system_matrix, firstFreeVariable);
			}
		};
	}

	class Element
	{
	public:

		std::string nomenclature;
		std::string type;
		int mols = 1;
		float molar_mass;
		std::vector<int> ox_numbers;
		int current_ox_n;
		std::string current_matter_state;

		Element(std::string _nomenclature, std::vector<int> _ox_numbers, std::string _type) 
			: nomenclature(_nomenclature), ox_numbers(_ox_numbers), type(_type)
		{
			if(_ox_numbers.size() == 1)
				current_ox_n = _ox_numbers[0];
		}

		Element(std::string _nomenclature, std::vector<int> _ox_numbers, std::string _type, float _molar_mass) 
			: nomenclature(_nomenclature), ox_numbers(_ox_numbers), type(_type), molar_mass(_molar_mass)
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

	Element getPTElement(std::string nomenclature)
	{
		std::vector<Element> elements;

		elements.push_back(Element("H",  {1,-1},      "hydrogen",    1.0079));
		elements.push_back(Element("Li", {1},         "metal",       6.941));
		elements.push_back(Element("Na", {1},         "metal",       22.99));
		elements.push_back(Element("K",  {1},         "metal",       39.098));
		elements.push_back(Element("Rb", {1},         "metal",       85.468));
		elements.push_back(Element("Cs", {1},         "metal",       132.91));
		elements.push_back(Element("Fr", {1},         "metal",       233));
		elements.push_back(Element("Be", {2},         "metal",       9.0122));
		elements.push_back(Element("Mg", {2},         "metal",       24.305));
		elements.push_back(Element("Ca", {2},         "metal",       40.078));
		elements.push_back(Element("Sr", {2},         "metal",       87.62));
		elements.push_back(Element("Ba", {2},         "metal",       137.33));
		elements.push_back(Element("Ra", {2},         "metal",       226));
		elements.push_back(Element("V",  {2,3,4,5},   "metal",       50.942));
		elements.push_back(Element("Cr", {2,3,6},     "metal",       51.996));
		elements.push_back(Element("Mn", {2,3,4,6,7}, "metal",       54.938));
		elements.push_back(Element("Fe", {2,3},       "metal",       55.845));
		elements.push_back(Element("Co", {2,3},       "metal",       58.933));
		elements.push_back(Element("Ni", {2,3},       "metal",       58.693));
		elements.push_back(Element("Pd", {2,4},       "metal",       106.42));
		elements.push_back(Element("Pt", {2,4},       "metal",       195.08));
		elements.push_back(Element("Cu", {1,2},       "metal",       63.546));
		elements.push_back(Element("Ag", {1},         "metal",       107.87));
		elements.push_back(Element("Au", {1,3},       "metal",       196.97));
		elements.push_back(Element("Zn", {2},         "metal",       65.38));
		elements.push_back(Element("Cd", {2},         "metal",       112.41));
		elements.push_back(Element("Hg", {1,2},       "metal",       200.59));
		elements.push_back(Element("B",  {3},         "non metal",   10.811));
		elements.push_back(Element("Al", {3},         "metal",       26.982));
		elements.push_back(Element("Ga", {3},         "metal",       69.723));
		elements.push_back(Element("In", {3},         "metal",       114.82));
		elements.push_back(Element("Tl", {1,3},       "metal",       204.38));
		elements.push_back(Element("C",  {2,4,-4},    "non metal",   12.011));
		elements.push_back(Element("Si", {4,-4},      "non metal",   28.086));
		elements.push_back(Element("Ge", {4},         "non metal",   72.64));
		elements.push_back(Element("Sn", {2,4},       "metal",       118.71));
		elements.push_back(Element("Pb", {2,4},       "metal",       207.2));
		elements.push_back(Element("N",  {2,3,-3,4,5},"non metal",   14.007));
		elements.push_back(Element("P",  {3,-3,4,5},  "non metal",   30.974));
		elements.push_back(Element("As", {3,-3,5},    "non metal",   74.922));
		elements.push_back(Element("Sb", {3,-3,5},    "non metal",   121.76));
		elements.push_back(Element("Bi", {3,5},       "non metal",   208.98));
		elements.push_back(Element("O",  {-2},        "oxygen",      15.999));
		elements.push_back(Element("S",  {2,-2,4,6},  "non metal",   32.065));
		elements.push_back(Element("Se", {-2,4,6},    "non metal",   78.96));
		elements.push_back(Element("Te", {-2,4,6},    "non metal",   127.6));
		elements.push_back(Element("Po", {2,4,6},     "non metal",   209));
		elements.push_back(Element("F",  {-1},        "non metal",   18.998));
		elements.push_back(Element("Cl", {1,-1,3,5,7},"non metal",   35.453));
		elements.push_back(Element("Br", {1,-1,3,5,7},"non metal",   79.904));
		elements.push_back(Element("I",  {1,-1,3,5,7},"non metal",   126.9));
		elements.push_back(Element("At", {1,-1,3,5,7},"non metal",   210));

		(elements[0]).current_ox_n = 1;

		for(Element e : elements){
			if(e.nomenclature == nomenclature)
				return e;
		}

		std::cout << nomenclature << " is not in the periodic table\n";
		return Element("Null",{},"",0);
	}

	class Reaction_Obj
	{
	public:
		float mols = 1;
		Reaction_Obj(){}
		virtual std::string reaction_obj_type() = 0;
		virtual float valence() = 0;
		virtual std::string nomenclature() = 0;
	};

	class Compound : public Reaction_Obj
	{
	public:
		std::string type;
		std::vector<Element> elements;

		//std::string type(){ return type; }

		std::string reaction_obj_type() override {
			return "compound";
		}

		Compound(std::vector<Element> _elements) : elements(_elements){}

		Compound(const Compound& _compound)
		{
			this->type = _compound.type;
			this->elements = _compound.elements;
		}

		Compound(std::string formulation)
		{
			//  SEPARATING ELEMENTS

			std::string currentElement; // buffer for the current element read
			int parenthesis[2] = {-1,-1};
			int parenthIndex = 1;

			for(int i = 0; i < formulation.size(); i++)
			{
				if(formulation[i] == '(') 
				{
					parenthesis[0] = elements.size(); //open parenthesis
				}

				else if(formulation[i] == ')') //close parenthesis
				{
					parenthesis[1] = elements.size() - 1;

					if(utils::chr_is_number(formulation[i+1])) {
						parenthIndex = utils::chr_to_int(formulation[i+1]);
						i++; // Skip step
					}
				}

				else if(utils::chr_is_number(formulation[i])) // if it is number
				{
					currentElement += formulation[i];
					
					if(!utils::chr_is_number(formulation[i+1])) // if the next char is not another number
					{
						if(elements.size() == 0) // mols of all compound
						{
							if(currentElement != "") 
								this->mols = std::stoi(currentElement);
							else 
								this->mols = utils::chr_to_int(formulation[i]);
						}
						else // mols of current element
						{
							Element& lastLoadedElement = (elements[elements.size()-1]);

							if(currentElement != "") 
								lastLoadedElement.mols = std::stoi(currentElement);
							else 
								this->mols = lastLoadedElement.mols = utils::chr_to_int(formulation[i]);
						}
						
						currentElement = ""; // reset buffer
					}
				}

				else if(utils::chr_is_uppercase(formulation[i])) // if we find an uppercase letter
				{
					if(utils::chr_is_lowercase(formulation[i+1])) { // if the element has 2 letters
						currentElement += formulation[i];   // First letter
						currentElement += formulation[i+1]; // Second letter

						Element elem = getPTElement(currentElement);
						currentElement = "";

						elements.push_back(elem);
						i++; // Skip step
					}
					else {
						currentElement += formulation[i]; // Get the letter

						Element elem = getPTElement(currentElement);
						currentElement = "";

						elements.push_back(elem);
					}
				}				
			}

			// apply parenthesis index
			if(parenthesis[0] != -1 && parenthesis[1] != -1) {
				for(int i = parenthesis[0]; i < parenthesis[1] + 1; i++){
					(elements[i]).mols *= parenthIndex;
				}
			}
		}

		friend std::vector<int> is_elemet_of_type(std::string e_type, Compound compound){
			std::vector<int> indexes;
			for(int i = 0; i < compound.elements.size(); i++){
				Element element = compound.elements[i];
				if(element.type == e_type)
					indexes.push_back(i);
			}

			return indexes;
		}

		void set_valences()
		{
			if(elements.size() == 1) //DIATOMIC COMPOUNDS
			{
				this->type = "single element";
				(elements[0]).current_ox_n = 0; // all single elements have a valence of 0
			}
			else if(elements.size() == 2) //BINARY COMPOUNDS 
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

			if(this->elements.size() != compound.elements.size()) return false;

			bool equal = true;

			for(Element compound_element : compound.elements){

				bool found = false;

				for(Element this_compound_element : elements){

					if(compound_element.nomenclature == this_compound_element.nomenclature 
						&& compound_element.mols == this_compound_element.mols){

						found = true;
						break;
					}
				}
				if(found == false){ equal = false; break; } 
			}

			return equal;
		}

		float operator%(Compound compound){

			if(*this == compound) return this->mols / compound.mols;
			else return -1;
		}

		void showCompoundInfo(){

			std::cout << this->mols << " mol" << ((this->mols > 1)? "s" : "") <<" of:\n";
			for(Element element : elements){
				std::cout << element.nomenclature << element.mols << ((element.current_ox_n < 0)? "" : "+") << element.current_ox_n << "\n";
			}
		}

		std::string nomenclature() override{

			std::string nm;

			if(this->mols - (int)this->mols == 0) nm = (this->mols > 1)? std::to_string((int)this->mols) : "";
			else nm = (this->mols > 1)? std::to_string(this->mols) : "";

			for(Element element : elements){
				nm += element.nomenclature;
				if(element.mols > 1){
					if(element.mols - (int)element.mols == 0) nm += std::to_string((int)(element.mols));
					else nm += std::to_string(element.mols);
				}
			}

			this->set_valences();

			if(this->valence() != 0){

				if(valence() > 0){
					if(valence() - (int)valence() == 0) nm += "+" + ( (valence() == 1)? "" : std::to_string( (int)valence() ));
					else nm += "+" + ( (valence() == 1)? "" : std::to_string(valence()) );
				}
				else if(valence() < 0){
					if(valence() - (int)valence() == 0) nm += "-" + ( (valence() == -1)? "" : std::to_string( (int)valence() * (-1) ));
					else nm += "-" + ( (valence() == -1)? "" : std::to_string( valence()*(-1) ) );
				}
			}

			return nm;
		}

		float valence() override{
			float val = 0;
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
			}else{return *this;}return *this;
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
			if(this->mols - (int)this->mols == 0) return std::to_string((int)this->mols) + "e-";
			else return std::to_string(this->mols) + "e-";
		}

		float valence() override{
			return -(this->mols);
		}

		bool operator==(Electron electron){
			return (this->mols == electron.mols)? true : false;
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
			
			std::cout << "\n";
		}

		void operator*=(int n){
			for(Reaction_Obj* reactant : reactants)
				reactant->mols *= n;
			for(Reaction_Obj* product : products)
				product->mols *= n;
		}

		Reaction operator+(Reaction reaction){

			Reaction new_equivalent_reaction;

			//mixing the reactions
			new_equivalent_reaction.reactants.insert( new_equivalent_reaction.reactants.end(), reaction.reactants.begin(), reaction.reactants.end());
			new_equivalent_reaction.reactants.insert( new_equivalent_reaction.reactants.end(), this->reactants.begin(), this->reactants.end());

			new_equivalent_reaction.products.insert(  new_equivalent_reaction.products.end(), reaction.products.begin(),  reaction.products.end());
			new_equivalent_reaction.products.insert(  new_equivalent_reaction.products.end(), this->products.begin(),  this->products.end());
			

			new_equivalent_reaction.showReaction();
			printf("\nreduced reaction\n------------------------\n");

			//reducing the reaction
			
			reduce:

			for(int i = 0; i < new_equivalent_reaction.reactants.size(); i++){
				for(int j = 0; j < new_equivalent_reaction.products.size(); j++){

					Reaction_Obj* reactant = new_equivalent_reaction.reactants[i];
					Reaction_Obj* product = new_equivalent_reaction.products[j];

					if(reactant->reaction_obj_type() == "compound" && product->reaction_obj_type() == "compound"){

						if(*dynamic_cast<Compound*>(reactant) == *dynamic_cast<Compound*>(product)){
							if(reactant->mols > product->mols){
								reactant->mols -= product->mols;
								new_equivalent_reaction.products.erase(new_equivalent_reaction.products.begin() + j);
								goto reduce;
							}
							if(reactant->mols < product->mols){
								product->mols -= reactant->mols;
								new_equivalent_reaction.reactants.erase(new_equivalent_reaction.reactants.begin() + i);
								goto reduce;
							}
							if(reactant->mols == product->mols){
								new_equivalent_reaction.reactants.erase(new_equivalent_reaction.reactants.begin() + i);
								new_equivalent_reaction.products.erase(new_equivalent_reaction.products.begin() + j);
								goto reduce;
							}
						}
						
					}
					else if(reactant->reaction_obj_type() == "electron" && product->reaction_obj_type() == "electron"){

						if(reactant->mols > product->mols){
							reactant->mols -= product->mols;
							new_equivalent_reaction.products.erase(new_equivalent_reaction.products.begin() + j);
							goto reduce;
						}
						if(reactant->mols < product->mols){
							product->mols -= reactant->mols;
							new_equivalent_reaction.reactants.erase(new_equivalent_reaction.reactants.begin() + i);
							goto reduce;
						}
						if(reactant->mols == product->mols){
							new_equivalent_reaction.reactants.erase(new_equivalent_reaction.reactants.begin() + i);
							new_equivalent_reaction.products.erase(new_equivalent_reaction.products.begin() + j);
							goto reduce;
						}
					}
				}
			}
			printf("\n");
			new_equivalent_reaction.showReaction();
			return new_equivalent_reaction;
		}

		void valance(std::vector<std::string> specif_els_to_valance = {}, bool exclude_specif_els = false){

			//the reaction will be valanced using the algebraic method
			//for solving the linear equations this function applies the Gauss elimination and Gauss Jordan method

			//differenciating the present elements
			std::vector<Element> present_elements;

			// 1. Obtaining list of all the implicated elements in the reaction
			for(Reaction_Obj* reactObj : reactants) // TODO: Scan de product side too in search of posible inconsistencies
			{
				if(reactObj->reaction_obj_type() == "compound")
				{
					Compound current_compound = *(dynamic_cast<Compound*>(reactObj));
					
					for(Element el : current_compound.elements) 
					{
						bool excludeElem = false;

						for(Element pr_el : present_elements) {
							if(el.nomenclature == pr_el.nomenclature 
								|| (!specif_els_to_valance.empty() && (
									(exclude_specif_els == false   && std::find(specif_els_to_valance.begin(), specif_els_to_valance.end(), el.nomenclature) != specif_els_to_valance.end())
									|| (exclude_specif_els == true && std::find(specif_els_to_valance.begin(), specif_els_to_valance.end(), el.nomenclature) == specif_els_to_valance.end())
									)
								))
							{
								excludeElem = true;
								break;
							}
						}

						if(excludeElem == false) 
							present_elements.push_back(el);
					}
				}
			}

			// 2. Creating the matrix that represents the ecuation system
			// Ex: xCl + yNa -> zNaCl is translated to an ecuation x + y -z = 0 for each row
			float** first_matrix = new float*[ present_elements.size() ];// the number of ecuations corresponds to de number of the unrepited different elements
			for(int i = 0; i < present_elements.size(); i++)
				first_matrix[i] = new float[ reactants.size() + products.size() ]; //a b c ... coefficents depend on de number of compounds in the reaction

			for(int i = 0; i < present_elements.size(); i++)
			{
				for(int j = 0; j < reactants.size(); j++) {

					int current_compound_mols = (reactants[j])->mols;
					int element_mols_count = 0;

					Reaction_Obj* reactObj = reactants[j];

					if(reactObj->reaction_obj_type() == "compound")
					{
						Compound current_compound = *(dynamic_cast<Compound*>(reactObj));

						for(Element pr_el : current_compound.elements)
							if(pr_el.nomenclature == (present_elements[i]).nomenclature){
								element_mols_count += pr_el.mols;
							}
						
					}

					first_matrix[i][j] = element_mols_count * current_compound_mols;
				}

				for(int j = 0; j < products.size(); j++){

					int current_compound_mols = (products[j])->mols;
					int element_mols_count = 0;

					Reaction_Obj* reactObj = products[j];

					if(reactObj->reaction_obj_type() == "compound")
					{
						Compound current_compound = *(dynamic_cast<Compound*>(reactObj));

						for(Element pr_el : current_compound.elements)
							if(pr_el.nomenclature == (present_elements[i]).nomenclature){
								element_mols_count -= pr_el.mols;
							}
						
					}

					first_matrix[i][reactants.size() + j] = element_mols_count * current_compound_mols;
				}
			}

			//-------------------------------------------------------------------------------------------------------------------------
			// 3. Elimitating repeated equivalent equations
			//-------------------------------------------------------------------------------------------------------------------------
			std::vector<float> row_factor;
			for(int i = 0; i < present_elements.size(); i++){
				for(int j = 0; j < reactants.size() + products.size(); j++){
	
					if(first_matrix[i][j] != 0){ row_factor.push_back(first_matrix[i][j]); break;} 
					if(j == reactants.size() + products.size() -1) row_factor.push_back(0);
				}
			}

			std::vector<int> excluded;

			for(int i = 0; i < present_elements.size(); i++){

				if(std::find(excluded.begin(), excluded.end(), i) != excluded.end()) continue; //is this row already excluded?

				for(int j = 0; j < present_elements.size(); j++){

					if(std::find(excluded.begin(), excluded.end(), j) != excluded.end() || j == i) continue; //is this row already excluded or the same compared to?

					bool differ = false;
					for(int k = 0; k < (reactants.size() + products.size()); k++){
						if(first_matrix[i][k] * row_factor[j] != first_matrix[j][k] * row_factor[i]){
							differ = true;
							break;
						}
					}
					if(differ == false){ 
						excluded.push_back(j); 
						break;
					}
				}
			}
			//-------------------------------------------------------------------------------------------------------------------------
			//-------------------------------------------------------------------------------------------------------------------------

			//assuming coefficent 'a' is 1 we reduce the number of coefficents and set an independent term column
			//we create a second matrix for solving the rest of unknowns
			int num_of_coeficents = products.size() + reactants.size();
			int num_of_equations = present_elements.size() /**/-excluded.size();

			float** matrix = new float*[ num_of_equations ];
			for(int i = 0; i < num_of_equations; i++)
				matrix[i] = new float[ num_of_coeficents + 1 ];

			
int skip = 0;//RETOCAR
			for(int i = 0; i < num_of_equations; i++){
				for(int j = 0; j < num_of_coeficents-1; j++){
					/**/if(std::find(excluded.begin(), excluded.end(), i) != excluded.end()) skip++;
					matrix[i][j] = first_matrix[i/**/+skip/**/][j+1];
				}
				matrix[i][num_of_coeficents-1] = -first_matrix[i][0];
			}
			num_of_coeficents--;

			ensure:
			auto SHOW_MATRIX_STATE = [&](){
				for(int i = 0; i < num_of_equations; i++){  // SHOW MATRIX STATE
					for(int j = 0; j < num_of_coeficents+1; j++){
						std::cout << ((j == num_of_coeficents)? "| " : "") << matrix[i][j] << "  ";
					}
					std::cout << "\n\n";
				}std::cout << "\n----------------------\n";
			};
			
			SHOW_MATRIX_STATE();

			//First ensure that all the diagonal values are != 0
			for(int i = 0; i < num_of_equations; i++){

				if(matrix[i][i] != 0) continue;

				//if the diagonal coeficent is 0
				// search for a matrix row to swap
				for(int j = i + 1; j < num_of_equations; j++){

					if(matrix[j][i] == 0) continue;

					//if a row was found
					//swaping the rows
					for(int k = 0; k < num_of_coeficents + 1; k++){

						float aux = matrix[j][k];
						matrix[j][k] = matrix[i][k];
						matrix[i][k] = aux;
					}
					std::cout << "\nCHANGED\n";
					SHOW_MATRIX_STATE();
					break;
				}
				

			}

			// 4. Ensure that the diagonals have found a good ordering in diagonal terms, otherwise repeat
			bool allOk = true;
			int err_index;

			for(int i = 0; i < num_of_equations; i++) {
				if(matrix[i][i] == 0) {
					allOk = false;
					err_index = i;
					break;
				}
			}

			if(allOk == false) {

				for(int i = 0; i < num_of_equations; i++) {

					if(matrix[i][err_index] == 0) continue;
					if(matrix[err_index][i] == 0) continue;

					for(int k = 0; k < num_of_coeficents+1; k++) {
						float aux = matrix[i][k];
						matrix[i][k] = matrix[err_index][k];
						matrix[err_index][k] = aux;
					}
					break;
				}
				
				goto ensure;
			}

			// 5. convert the coeficients under the diagonal into 0
			gauss_jordan_elimination:

			SHOW_MATRIX_STATE();

			//convert the coeficients under the diagonal into 0
			for(int i = 0; i < num_of_coeficents; i++){
				for(int j = i; j < num_of_equations; j++){

					if(j > i && matrix[j][i] != 0){

						int best_scalonated_num_of_coef = num_of_coeficents;
						int best_scalonated_row = 0;

						// searching for row to operate
						for(int k = 0; k < num_of_equations; k++) 
						{
							if(k != j && matrix[k][i] != 0){ //posible found row

								int aprox_index = 0;
								for(int s = 0; s < num_of_coeficents; s++){ // search first non cero column of the row
									if(matrix[k][s] != 0) break;
									aprox_index++;
								}

								//setting the best row to operate
								if(best_scalonated_num_of_coef > num_of_coeficents - aprox_index){
									best_scalonated_row = k;
									best_scalonated_num_of_coef = num_of_coeficents - aprox_index;
								}

							}
						}

						//start elimination operation
						float oposite_coef = -matrix[j][i];
						for(int n = 0; n < (num_of_coeficents + 1); n++){
							matrix[j][n] = matrix[j][n] * matrix[best_scalonated_row][i] + matrix[best_scalonated_row][n] * oposite_coef; //reducing
						}
						std::cin.get();
						goto gauss_jordan_elimination;
					}
				}
			}

			// 6. convert the coeficients on top of the diagonal into 0
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

						//start elimination operation
						float oposite_coef = -matrix[j][i];
						for(int n = 0; n < (num_of_coeficents+1); n++){
							matrix[j][n] = matrix[j][n] * matrix[best_scalonated_row][i] + matrix[best_scalonated_row][n] * oposite_coef; //reducing
						}
						std::cin.get();
						goto gauss_jordan_elimination;
					}
				}
			}

			// 7. setting coeficents to 1
			for(int i = 0; i < num_of_equations; i++){

				float index_factor = matrix[i][i];
				for(int j = 0; j < num_of_coeficents+1; j++)
					matrix[i][j] = matrix[i][j] / index_factor;
				
			}

			SHOW_MATRIX_STATE();

			int lower_mol_value = 1;

			setting_mols: std::cout << lower_mol_value << "\n";

			(reactants[0])->mols = lower_mol_value;

			int coeff_index = 0;
			for(int i = 0; i < reactants.size(); i++) {

				if(i == 0) continue; // is a setted value
				
				Reaction_Obj* reactant = reactants[i];
				float setted_reactant_mols = reactant->mols * matrix[coeff_index][num_of_coeficents] * lower_mol_value;
				
				if(setted_reactant_mols - (int)(setted_reactant_mols) != 0){//if mols are not integers repeat mol setting
					lower_mol_value++;
					goto setting_mols;
				}
		
				reactant->mols *= setted_reactant_mols;

				coeff_index++;
			}

			for(Reaction_Obj* product: products) {

				float setted_product_mols = product->mols * matrix[coeff_index][num_of_coeficents] * lower_mol_value;

				if(setted_product_mols - (int)(setted_product_mols) != 0){//if mols are not integers repeat mol setting
					lower_mol_value++;
					goto setting_mols;
				}

				product->mols *= setted_product_mols;

				coeff_index++;
			}
		}
	};

	float molar_mass_of(Compound compound){
		float total_molar_mass = 0;
		for(Element element : compound.elements){
			total_molar_mass += element.molar_mass * element.mols;
		}
		total_molar_mass *= compound.mols;
		return total_molar_mass;
	}

	float gTo_mols_of(Compound compound, float grams){

		return grams * (1 / molar_mass_of(compound));
	}

	float gTo_mols_of(Element element, float grams){
		float total_molar_mass = element.molar_mass * element.mols;
		return grams * (1 / total_molar_mass);
	}

	float molarity(float mols, float liters){
		return mols / liters;
	}

	float mols_in_solution(float liters, float molarity){
		return molarity * liters;
	}

	float liters_in_solution(float mols, float molarity){
		return mols / molarity;
	}

	float density(float mass, float liters){
		return mass / liters;
	}

	float molarity_to_density_of(Compound compound, float molarity){
		return molarity * molar_mass_of(compound);
	}

	float density_to_molarity_of(Compound compound, float density){
		return density / molar_mass_of(compound);
	}

	namespace rdx
	{

	}
	
}

/*
	iterar hacia abajo y si no se encuentra una buena fila debajo se swapea buscando desde arriba un buen 
	candidato para esa posición y se continúa la iteración, al acabar se repite la iteración
 */

