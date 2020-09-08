#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>    
#include <vector>  
#include <cmath>
#include <bits/stdc++.h> 

int main () {


	//VARIABLES DECLARATION
	
	std::string line, substring, fluents, probfluents;	
	std::vector<int> parser;
	
	
	int n_areas, n_vehicles, i;
	
	n_areas=2;
	n_vehicles=1;
	
	
	
	//GENERATION OF MDP FILE

	std::ifstream input ("input/input.txt", std::ios::in);
	std::ofstream output ("output/output.txt", std::ios::out | std::ios::trunc);
	
	bool first=true;
	std::vector <int> v_old;
		
	while ( getline (input,line) ) {
	
		std::stringstream s_line(line); 
		
		getline(s_line, substring, ':');
		
		
		if (substring=="Current state") {
		
		
			getline(s_line, fluents, '|');
			std::stringstream s_fluents(fluents); 
			
			getline(s_line, probfluents, '|');
			std::stringstream s_probfluents(probfluents); 
			
			std::vector <int> v;
			
			while(s_fluents>>i) {
		
				//output<<i<<" ";
				v.push_back(i); 
			
			}
			
			if (first) {
				
				v_old=v;
				first=false;
			}
			
			else {
			
				for (int i=0; i<n_areas; i++) {
				
					if (v[i+n_areas]==1 & v_old[i+n_areas]==1)
				}
				
			}
			
			state_fluents.push_back(v); 
			
			output<<"|"; 
			
			std::vector <int> p;
			
			while(s_probfluents>>i) {
		
				output<<i<<" ";
				p.push_back(i); 
			
			}
			
			state_prob.push_back(p); 
			
			output<<"\n";
		
		}
		
		output<<state_fluents[1][1];

				
	
		 
		
		
		
	}
 
	input.close();
	output.close();
	
	
	
	
	return 0;
}

