#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>    
#include <vector>  
#include <cmath>

int main () {


	//VARIABLES DECLARATION
	
	std::string line;
	int number, area, vehicle, hospital, base, x, y, std_time;
	
	
	//INPUT VARIABLES
	x=5;
	y=3;

	number=19;
	vehicle=1;
	hospital=2;
	base=1;
	std_time=1;
	
	
	//CONSTANTS SETTING
	area=x*y;	
	srand (time(NULL));
	
	
	//GENERATION OF MDP FILE

	std::ifstream o_mdp ("input/mdp.rddl", std::ios::in);
	std::ofstream mdp ("output/ems_mdp.rddl", std::ios::out | std::ios::trunc);
		
	while ( getline (o_mdp,line) ) {
	
		bool replace = false;
	
		 if (line == "		area :" ) {
		 	replace=true;
		 	mdp << "		area : { @a0";
		 	for (int i=1; i<area; i++) { mdp<<", @a"<<i; }
		 	mdp<<"};"<<'\n';
		 };
		 
		 if (line == "		number :" ) {
		 	replace=true;
		 	mdp << "		number : { @0";
		 	for (int i=1; i<number; i++) { mdp<<", @"<<i; }
		 	mdp<<"};"<<'\n';
		 };
		 
		 
		 if (line == "						else if (v_attendPatient(?v, @a1)) then @a1" ) {
		 	replace=true;
		 	for (int i=1; i<area; i++) { mdp<<"						else if (v_attendPatient(?v, @a"<<i<<")) then @a"<<i<<'\n'; }
		 };
		 
		 if (line == "						else if (v_travelToHsopital(?v, @a1)) then @a1" ) {
		 	replace=true;
		 	for (int i=1; i<area; i++) { mdp<<"						else if (v_travelToHsopital(?v, @a"<<i<<")) then @a"<<i<<'\n'; }
		 };
		 
		 if (line == "					else if (v_travelToBase(?v, @a1)) then @a1" ) {
		 	replace=true;
		 	for (int i=1; i<area; i++) { mdp<<"						else if (v_travelToBase(?v, @a"<<i<<")) then @a"<<i<<'\n'; }
		 };
		 
		 if (line == "						else if (v_remainingActionTime(?v)==@2) then @1" ) {
		 	replace=true;
		 	for (int i=2; i<number; i++) { mdp<<"						else if (v_remainingActionTime(?v)==@"<<i<<") then @"<<i-1<<'\n'; }
		 };
		 
		 if (line == "						else if (v_position(?v)==@a0 & (v_attendPatient(?v, @a0) | v_travelToHsopital(?v, @a0) | v_travelToBase(?v, @a0) ) ) then ( aa_travelTime(@a0, @a0) )" ) {
		 	replace=true;
		 	for (int i=0; i<area; i++) { 
		 		
		 		for (int j=0; j<area; j++) { 
		 			mdp<<"						else if (v_position(?v)==@a"<<i<<" & (v_attendPatient(?v, @a"<<j<<") | v_travelToHsopital(?v, @a"<<j<<") | v_travelToBase(?v, @a"<<j<<") ) ) then ( aa_travelTime(@a"<<i<<", @a"<<j<<") )"<<'\n';
		 		}
		 		mdp<<'\n';
		 	}	
		 	
		 };
		 
		 
		 if (line == "						else if (a_patientActive(?a) & a_patientWaitingTime(?a)==@0) then @1" ) {
		 	replace=true;
		 	for (int i=2; i<number; i++) { mdp<<"						else if (a_patientActive(?a) & a_patientWaitingTime(?a)==@"<<i-1<<") then @"<<i<<'\n'; }
		 };
		 
		 
		 
		 if (replace==false) {mdp << line << '\n';}
		
		
		
	}
 
	o_mdp.close();
	mdp.close();
	
	
	//GENERATION OF INST FILE

	std::ifstream o_inst ("input/inst.rddl", std::ios::in);
	std::ofstream inst ("output/ems_inst.rddl", std::ios::out | std::ios::trunc);
	
	std::vector<int> hospital_locations (hospital);
		 	
 	for (int i=0; i<hospital; i++) {
 	 	hospital_locations[i] = rand() % area;
 		for (int j=0; j<i; j++) {
 			if (hospital_locations[i]==hospital_locations[j]) {
 				i--;
 				break;
 			}
 		}
 	}
 	
 	std::vector<int> base_locations (base);
		 	
 	for (int i=0; i<base; i++) {
 	 	base_locations[i] = rand() % area;
 		for (int j=0; j<i; j++) {
 			if (base_locations[i]==base_locations[j]) {
 				i--;
 				break;
 			}
 		}
 	}
 	
 	
	
	
	while ( getline (o_inst,line) ) {
	
		bool replace = false;
		
		if (line == "		vehicle :" ) {
		 	replace=true;
		 	inst << "		vehicle : { v0";
		 	for (int i=1; i<vehicle; i++) { inst<<", v"<<i; }
		 	inst<<"};"<<'\n';
		 };
		
		
		if (line == "		a_hospital" ) {
		 	replace=true;
		 	
		 	for (int i=0; i<hospital; i++) { inst<<"		a_hospital(@a"<<hospital_locations[i]<<")=true;"<<'\n'; }
		 };
		 
		 
		 if (line == "		a_base" ) {
		 	replace=true;
		 	
		 	for (int i=0; i<base; i++) { inst<<"		a_base(@a"<<base_locations[i]<<")=true;"<<'\n'; }
		 };
		 
		 
		 if (line == "		aa_travelTime" ) {
		 	replace=true;
		 	for (int i=0; i<x; i++) {
		 	
		 		for (int j=0; j<y; j++) {
		 		
		 			int a=j+i*y;
		 		
		 			for (int m=0; m<x; m++) {
		 	
		 				for (int n=0; n<y; n++) {
		 		
		 					int b=n+m*y;
		 					int d= std::abs(i-m) + std::abs(j-n) + std_time;
		 
		 					inst<<"		aa_travelTime(@a"<<a<<", @a"<<b<<")=@"<<d<<";"<<'\n';
		 				}
		 			}

		 		}
		 	}

		 };
		 
		 
		 if (line == "		v_position" ) {
		 	replace=true;
		 	int b=0;
		 	
		 	for (int i=0; i<vehicle; i++) { 
		 		inst<<"		v_position(v"<<i<<")=@a"<<base_locations[b]<<";"<<'\n'; 
		 		b++;
		 		if (b==base) {b=0;}
		 	}
		 };
		
		
	
		if (replace==false) {inst << line << '\n';}
		
			
	}
	
	
	
	o_inst.close();
	inst.close();
	
	return 0;
}

