#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
#include<sstream>
#include <math.h> 
using namespace std;
#define MemSize 1000 

vector<string> PC;
vector<string> Taken;
vector<string> predict;
int m;
int k;

void ReadTrace() {
	ifstream trace;
	string line;
	int i = 0;
	trace.open("trace.txt");
	if (trace.is_open()) {
		while (getline(trace, line)) {
			vector<string> words;
			string result;
			stringstream input(line);
			while (input >> result)
				words.push_back(result);
			PC.push_back(words[0]);
			Taken.push_back(words[1]);
			i++;
		}
	}
	else cout << "Unable to open file";
	trace.close();
}

void ReadConfig() {
	ifstream config;
	string line;
	int i = 0;
	config.open("config.txt");
	if (config.is_open())
	{
		getline(config, line);
		stringstream conv1(line);
		conv1 >> m;
		getline(config, line);
		stringstream conv2(line);
		conv2 >> k;
		cout << m << " " << k << endl;
	}
	else cout << "Unable to open file";
	config.close();
}

void PrintPred(vector<string> predict) {
	ofstream printPred("trace.txt.out", ios::out);
	if (printPred.is_open()) {
		for (int i = 0; i < PC.size(); i++) {
			printPred << predict[i] << endl;
		}
	}
	else cout << "Unable to open file";
	printPred.close();
}

int Conv16to10(string input) {
	int output = 0;
	int curbit;
	for (int i = 0; i < input.length(); i++) {
		if (input[i] == 'a') curbit = 10;
		else if (input[i] == 'b') curbit = 11;
		else if (input[i] == 'c') curbit = 12;
		else if (input[i] == 'd') curbit = 13;
		else if (input[i] == 'e') curbit = 14;
		else if (input[i] == 'f') curbit = 15;
		else if (input[i] == '0') curbit = 0;
		else if (input[i] == '1') curbit = 1;
		else if (input[i] == '2') curbit = 2;
		else if (input[i] == '3') curbit = 3;
		else if (input[i] == '4') curbit = 4;
		else if (input[i] == '5') curbit = 5;
		else if (input[i] == '6') curbit = 6;
		else if (input[i] == '7') curbit = 7;
		else if (input[i] == '8') curbit = 8;
		else if (input[i] == '9') curbit = 9;
		output += curbit * pow(16, input.length() - i - 1);
	}
	return output;
}

int main() {
	ReadTrace();
	ReadConfig();
	int* BHR;
	BHR = (int*)malloc(sizeof(int) * k);
	for (int i = 0; i < k; i++) {
		BHR[i] = 1;
	}
	int totalrow = pow(2, m);
	string flag = "1";
	int curBHR = 0;
	vector<string> predict(PC.size(), "1");
	int** PHT;
	int column = pow(2,k);
	PHT = (int**)malloc(sizeof(int*) * column); 
	for (int i = 0; i < column; i++) { 
		PHT[i] = (int*)malloc(sizeof(int) * totalrow);
	}
	for (int i = 0; i < column; i++) {
		for (int j = 0; j < totalrow; j++) {
			PHT[i][j] = 11;
		}
	}
	int index = 0;

	for (int i = 0; i < PC.size(); i++) {
		// find position of state in PHT
		int row = Conv16to10(PC[i].substr(8 - m / 4, 8));
		index = 0;
		for (int a = 0; a < k; a++) {
			index += BHR[a] * pow(2, k - 1 - a);
		}

		// determine its prediction result
		if (PHT[index][row] == 00 || PHT[index][row] == 01) {
			predict[i] = "0";
		}
		else predict[i] = "1";

		// update PHT & BHR according to new coming instruction
		if (Taken[i] == "0" && PHT[index][row] == 11) {
			PHT[index][row] = 10;
			curBHR = 0;
		}
		else if (Taken[i] == "0" && PHT[index][row] == 10) {
			PHT[index][row] = 00;
			curBHR = 0;
		}
		else if (Taken[i] == "1" && PHT[index][row] == 10) {
			PHT[index][row] = 11;
			curBHR = 1;
		}
		else if (Taken[i] == "0" && PHT[index][row] == 01) {
			PHT[index][row] = 00;
			curBHR = 0;
		}
		else if (Taken[i] == "0" && PHT[index][row] == 00) {
			PHT[index][row] = 00;
			curBHR = 0;
		}
		else if (Taken[i] == "1" && PHT[index][row] == 01) {
			PHT[index][row] = 11;
			curBHR = 1;
		}
		else if (Taken[i] == "1" && PHT[index][row] == 00) {
			PHT[index][row] = 01;
			curBHR = 1;
		}
		else if (Taken[i] == "1" && PHT[index][row] == 11) {
			PHT[index][row] = 11;
			curBHR = 1;
		}
		// move younger one in and older one shifted right in BHR
		for (int j = 0; j < k; j++)
		{
			char temp = BHR[j];
			BHR[j] = curBHR;
			curBHR = temp;
		}
	}

	PrintPred(predict);
	return 0;
}
