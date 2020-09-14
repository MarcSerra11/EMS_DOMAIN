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
	int number, area, vehicle, hospital, base, x, y, std_time, on_site_time, drop_off_time;
	double stabilisation_p1, stabilisation_p2, stabilisation_p3, hospitalisation_p1, hospitalisation_p2, hospitalisation_p3;  	
	
	//INPUT VARIABLES
	x=4;
	y=2;

	number=10;
	vehicle=3;
	hospital=2;
	base=2;
	
	on_site_time=5; 
	drop_off_time=1;
	
	stabilisation_p1=0; 
	stabilisation_p2=0;
	stabilisation_p3=0; 
	
	hospitalisation_p1=0; 
	hospitalisation_p2=0; 
	hospitalisation_p3=0.0000001;
	
	
	
	
	
	//CONSTANTS SET
	area=x*y;	
	srand (time(NULL));
	std_time=(on_site_time+drop_off_time)/2;
	if (std_time==0) {std_time=1;} 
	
	
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
		 	for (int i=1; i<=number; i++) { mdp<<", @"<<i; }
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
		 	for (int i=1; i<number; i++) { mdp<<"						else if (a_patientActive(?a) & a_patientWaitingTime(?a)==@"<<i-1<<") then @"<<i<<'\n'; }
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
		
		if (line == "	max-nondef-actions =" ) {
		 	replace=true;
		 	inst << "	max-nondef-actions = "<<vehicle<<";"<<'\n';
	
		 };
		
		
		
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
		 
		 
		 if (line == "		n_PreStabilisation" ) {
		 	replace=true;
		 	
		 	double punishment=hospitalisation_p3;
		 	double previous_punishment=0;
		 	double min=0.000001;
		 	inst<< std::fixed;
		 	
		 	for (int i=0; i<=number; i++) { 
		 	
		 		inst<<"		n_PreHospitalisation(@"<<i<<", @p3)=-"<<punishment<<";"<<'\n'; 
		 		previous_punishment=previous_punishment+punishment;
		 		punishment=previous_punishment+min;
		 	}
		 	inst<<'\n';
		 	
		 	if (hospitalisation_p2 != 0) {punishment=hospitalisation_p2; previous_punishment=0;}
		 	for (int i=0; i<=number; i++) { 
		 	
		 		inst<<"		n_PreHospitalisation(@"<<i<<", @p2)=-"<<punishment<<";"<<'\n'; 
		 		previous_punishment=previous_punishment+punishment;
		 		punishment=previous_punishment+min;
		 	}
		 	inst<<'\n';
		 	
		 	if (hospitalisation_p1 != 0) {punishment=hospitalisation_p1; previous_punishment=0;}
		 	for (int i=0; i<=number; i++) { 
		 	
		 		inst<<"		n_PreHospitalisation(@"<<i<<", @p1)=-"<<punishment<<";"<<'\n'; 
		 		previous_punishment=previous_punishment+punishment;
		 		punishment=previous_punishment+min;
		 	}
		 	inst<<'\n';
		 	
		 	if (stabilisation_p3 != 0) {punishment=stabilisation_p3; previous_punishment=0;}
		 	for (int i=0; i<=number; i++) { 
		 	
		 		inst<<"		n_PreStabilisation(@"<<i<<", @p3)=-"<<punishment<<";"<<'\n'; 
		 		previous_punishment=previous_punishment+punishment;
		 		punishment=previous_punishment+min;
		 	}
		 	inst<<'\n';
		 	
		 	if (stabilisation_p2 != 0) {punishment=stabilisation_p2; previous_punishment=0;}
		 	for (int i=0; i<=number; i++) { 
		 	
		 		inst<<"		n_PreStabilisation(@"<<i<<", @p2)=-"<<punishment<<";"<<'\n'; 
		 		previous_punishment=previous_punishment+punishment;
		 		punishment=previous_punishment+min;
		 	}
		 	inst<<'\n';
		 	
		 	if (stabilisation_p1 != 0) {punishment=stabilisation_p1; previous_punishment=0;}
		 	for (int i=0; i<=number; i++) { 
		 	
		 		inst<<"		n_PreStabilisation(@"<<i<<", @p1)=-"<<punishment<<";"<<'\n'; 
		 		previous_punishment=previous_punishment+punishment;
		 		punishment=previous_punishment+min;
		 	}
		 	inst<<'\n';
		 	
		 };
		
		
	
		if (replace==false) {inst << line << '\n';}
		
			
	}
	
	
	
	o_inst.close();
	inst.close();
	
	return 0;
}

