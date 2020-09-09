#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>    
#include <vector>  
#include <cmath>
#include <bits/stdc++.h> 
#include <numeric>

int main () {


	//VARIABLES DECLARATION
	
	std::string line, substring, fluents, probfluents;	//line=substring: fluents | probluents

	int n;	
	
	int n_areas = 12;
	int n_vehicles=2;
	
	bool first_round=true;
	
	std::vector <int> present_wait (n_areas, 0);
	
	std::vector <int> present_state (n_areas);
	std::vector <int> previous_state (n_areas, 0);
	std::vector <int> present_priority (n_areas);
	std::vector <int> past_priority (n_areas);
	
	
	std::vector <int> p1_total_wait;
	std::vector <int> p2_total_wait;
	std::vector <int> p3_total_wait;
	
	
	//GENERATION OF MDP FILE

	std::ifstream input ("input/input.txt", std::ios::in);
	std::ofstream output ("output/output.txt", std::ios::out | std::ios::trunc);
	

		
	while ( getline (input,line) ) {
	
		std::stringstream s_line(line); 
		
		getline(s_line, substring, ':');
		
		
		if (substring=="Current state") {
		
		
			getline(s_line, fluents, '|');
			std::stringstream s_fluents(fluents); 
			
			getline(s_line, probfluents, '|');
			std::stringstream s_probfluents(probfluents); 
			
			//Parsing patient states
			int index=0;
			while(s_fluents>>n) {
		
				if (index<n_areas) {
					present_state[index]=n;
				}
				index++;
				//output<<n<<" ";
			}
			
			//Parsing patient priorities
			index=0;
			while(s_probfluents>>n) {
		
		
				if (index>=n_areas) {
					present_priority[index-n_areas]=n;
				}
				index++;
				//output<<i<<" ";
			
			}
			
			
			
			//Compute waiting times
			
			if (first_round) {
				
				first_round=false;
			}
			
			else {
			
				for (int i=0; i<n_areas; i++) {
				
					if (present_state[i]==1) {present_wait[i]=present_wait[i]+1; }
					if (present_state[i]==0 and previous_state[i]==1 ) {
						
						if (past_priority[i]==0) {p1_total_wait.push_back(present_wait[i]);}   
						if (past_priority[i]==1) {p2_total_wait.push_back(present_wait[i]);} 
						if (past_priority[i]==2) {p3_total_wait.push_back(present_wait[i]);}       
						
						present_wait[i]=0; 
						
					}
					
					
				}
				
				//Test 
				/*
				for (int i=0; i<present_state.size(); i++) {output<<present_state[i]<<" ";}
				output<<" | ";
				for (int i=0; i<previous_state.size(); i++) {output<<previous_state[i]<<" ";}
				output<<" | ";
				for (int i=0; i<present_wait.size(); i++) {output<<present_wait[i]<<" ";}
				output<<" | ";
				for (int i=0; i<present_priority.size(); i++) {output<<present_priority[i]<<" ";}
				output<<" | ";
				for (int i=0; i<p1_total_wait.size(); i++) {output<<p1_total_wait[i]<<" ";}
				output<<" | ";
				for (int i=0; i<p2_total_wait.size(); i++) {output<<p2_total_wait[i]<<" ";}
				output<<" | ";
				for (int i=0; i<p3_total_wait.size(); i++) {output<<p3_total_wait[i]<<" ";}
				output<<'\n';
				*/
				
			}
			
			previous_state=present_state;
			past_priority=present_priority;
			

		}
		

				
	
		 
		
		
		
	}
	
	//Compute metrics
	double p1_average, p2_average, p3_average;
	int p1_pos, p2_pos, p3_pos;
	double p1_perc, p2_perc, p3_perc;
	
	
	
	if (p1_total_wait.size()>0) {
		p1_average = accumulate(p1_total_wait.begin(), p1_total_wait.end(), 0.0) / p1_total_wait.size(); 
		sort(p1_total_wait.begin(), p1_total_wait.end());
		p1_pos=floor(p1_total_wait.size()*0.9);
	}
	else p1_average = 0;
	
	if (p2_total_wait.size()>0) {
		p2_average = accumulate(p2_total_wait.begin(), p2_total_wait.end(), 0.0) / p2_total_wait.size(); 
		sort(p2_total_wait.begin(), p2_total_wait.end());
		p2_pos=floor(p2_total_wait.size()*0.9);
	}
	else p2_average = 0;
	
	if (p3_total_wait.size()>0) {
		p3_average = accumulate(p3_total_wait.begin(), p3_total_wait.end(), 0.0) / p3_total_wait.size(); 
		sort(p3_total_wait.begin(), p3_total_wait.end());
		p3_pos=floor(p3_total_wait.size()*0.9);
	}
	else p3_average = 0;
	
	
	
	
	
	
	
	
	//Generate output
	
	output<<"The computed metrics for the simulation are: "<<'\n'<<'\n';
	
	if (p1_total_wait.size()>0) {
		output<<"P1 average wait: "<<p1_average<<" over "<<p1_total_wait.size()<<" emergencies."<<'\n';
		output<<"P1 90 percentile: "<<p1_total_wait[p1_pos]<<"."<<'\n';
		output<<'\n';
	}
	
	else {output<<"No P1 cases."<<'\n'<<'\n';}
	
	
	if (p2_total_wait.size()>0) {
		output<<"P2 average wait: "<<p2_average<<" over "<<p2_total_wait.size()<<" emergencies."<<'\n';
		output<<"P2 90 percentile: "<<p2_total_wait[p2_pos]<<"."<<'\n';
		output<<'\n';
	}
	
	else {output<<"No P2 cases."<<'\n'<<'\n';}
	
	
	if (p3_total_wait.size()>0) {
		output<<"P3 average wait: "<<p3_average<<" over "<<p3_total_wait.size()<<" emergencies."<<'\n';
		output<<"P3 90 percentile: "<<p3_total_wait[p3_pos]<<"."<<'\n';
		output<<'\n';
	}
	
	else {output<<"No P3 cases."<<'\n'<<'\n';}
	
	
	
	//Test
	/*
	for (int i=0; i<p1_total_wait.size(); i++) {output<<p1_total_wait[i]<<" ";}
	output<<'\n';
	for (int i=0; i<p2_total_wait.size(); i++) {output<<p2_total_wait[i]<<" ";}
	output<<'\n';
 	for (int i=0; i<p3_total_wait.size(); i++) {output<<p3_total_wait[i]<<" ";}
 	*/
 	
 	
	input.close();
	output.close();
	
	
	return 0;
}

