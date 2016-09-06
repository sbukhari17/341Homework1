#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_map>

//
// Chicago Crime Analysis: top-5 crimes
//
// Syed Bukhari
// U. of Illinois, Chicago
// CS341, Fall 2016
// HW #01
//

using namespace std;

class Crime {
public:
	string DateTime;
	string IUCR;
	bool Arrest;
	bool Domestic;
	int Beat;
	int District;
	int Ward;
	int Community;
	int Year;


	Crime::Crime(string dateTime, string iucr, bool arrest, bool domestic,
		int beat, int district, int ward, int community, int year) :
		DateTime(dateTime), IUCR(iucr), Arrest(arrest), Domestic(domestic),
		Beat(beat), District(district), Ward(ward), Community(community), Year(year) {}
};

class CrimeCode {
public:
	string IUCR;
	string Primary_description;
	string Secondary_description;
	int Occurrence;
	CrimeCode::CrimeCode() {};
	CrimeCode::CrimeCode(string iucr, string primary_description, string secondary_description) :
		IUCR(iucr), Primary_description(primary_description), Secondary_description(secondary_description), Occurrence(0) {}
};

int main() {
	
	ifstream crimeInputStream("crimes-2.csv");
	ifstream crimeCodeInputStream("crime-codes.csv");
	string crimeheader;
	string crimeCodeHeader;
	string input;

	vector<Crime> crimes;
	vector<CrimeCode> crimeCodes;

	unordered_map <string, CrimeCode> crimeCodeCount;
	getline(crimeCodeInputStream, crimeCodeHeader);
	while (getline(crimeCodeInputStream, input)) { //crime-code parsing loop
		string IUCR, primaryDescription, secondaryDescription;
		stringstream ss(input);

		getline(ss, IUCR, ',');
		getline(ss, primaryDescription, ',');
		getline(ss, secondaryDescription, ',');

		

		//create crime-code object and push to back of vector of crime-codes
		CrimeCode C(IUCR, primaryDescription, secondaryDescription);
		crimeCodes.push_back(C);
		crimeCodeCount[IUCR] = C;
	}
	

															   //sort crimeCodes
	sort(crimeCodes.begin(), crimeCodes.end(),
		[](CrimeCode c1, CrimeCode c2) {
		return c1.IUCR < c2.IUCR;
	}
);

	getline(crimeInputStream, crimeheader);
	while (getline(crimeInputStream, input)) { //crime parsing loop
		string dateTime, IUCR, arrest, domestic,
			beat, district, ward, community, year;
		stringstream ss(input);

		getline(ss, dateTime, ',');
		getline(ss, IUCR, ',');
		getline(ss, arrest, ',');
		getline(ss, domestic, ',');
		getline(ss, beat, ',');
		getline(ss, district, ',');
		getline(ss, ward, ',');
		getline(ss, community, ',');
		getline(ss, year, ',');

		crimeCodeCount[IUCR].Occurrence++;

		bool Arrest, Domestic;
		//parse booleans as booleans
		if (arrest == "TRUE") {
			Arrest = true;
		}
		else {
			Arrest = false;
		}

		if (domestic == "TRUE") {
			Domestic = true;
		}
		else {
			Domestic = false;
		}
		
		//create crime object and push to back of vector of crimes
		Crime C(dateTime, IUCR, Arrest, Domestic, stoi(beat), stoi(district), stoi(ward), stoi(community), stoi(year));
		crimes.push_back(C);
	}
	cout << "** Crime Analysis **" << endl;
	cout << endl;
	cout << "Date range: " + crimes.front().DateTime + " - " + crimes.back().DateTime << endl; //output date range
	cout << endl;
	cout << "# of crime codes: " << crimeCodes.size() << endl; // # of crime codes

	for (int i = 0; i < 3; i++) {
		cout << crimeCodes.at(i).IUCR + ":\t" + crimeCodes.at(i).Primary_description + ":" + crimeCodes.at(i).Secondary_description << endl;
	}
	cout << "..." << endl;
	for (int i = crimeCodes.size() - 3; i < crimeCodes.size(); i++) {
		cout << crimeCodes.at(i).IUCR + ":\t" + crimeCodes.at(i).Primary_description + ":" + crimeCodes.at(i).Secondary_description << endl;
	}
	cout << endl;
	cout << "# of crimes: " << crimes.size() << endl;
	
	for (int i = 0; i < 3; i++) {
		cout << crimes.at(i).IUCR + ":\t" << crimes.at(i).DateTime + ", " << crimes.at(i).Beat << ", " << crimes.at(i).District <<
			 ", " << crimes.at(i).Ward << ", " << crimes.at(i).Community <<
			(crimes.at(i).Arrest ? ", arrested" : "") << (crimes.at(i).Domestic ? ", domestic violence" : "") << endl;
	}

	cout << "..." << endl;

	for (int i = crimes.size() - 3; i < crimes.size(); i++) {
		cout << crimes.at(i).IUCR + ":\t" << crimes.at(i).DateTime + ", " << crimes.at(i).Beat << ", " << crimes.at(i).District <<
			", " << crimes.at(i).Ward << ", " << crimes.at(i).Community <<
			(crimes.at(i).Arrest ? ", arrested" : "") << (crimes.at(i).Domestic ? ", domestic violence" : "") << endl;
	}
	
	//copy values of map (now incremmented to their proper occurrences to a set/vector)
	vector <CrimeCode> sortByOccurrence;
	
	for (unordered_map<string, CrimeCode>::iterator it = crimeCodeCount.begin(); it != crimeCodeCount.end(); ++it) {
		sortByOccurrence.push_back(it->second);
	}
	//sort vector of crimeCodes by occurrence
	
	sort(sortByOccurrence.begin(),sortByOccurrence.end(),
		[](CrimeCode c1, CrimeCode c2) {
		return c1.Occurrence > c2.Occurrence;
	});

	cout << endl;
	cout << "** Top-5 Crimes **" << endl;
	cout << "Code:\tTotal, Description" << endl;
	for (int i = 0;i < 5; i++) {
		cout << sortByOccurrence.at(i).IUCR + ":\t" << sortByOccurrence.at(i).Occurrence << ", " << sortByOccurrence.at(i).Primary_description + ": " + sortByOccurrence.at(i).Secondary_description << endl;
	}
	cout << endl;
	cout << "** Done **" << endl;
	crimeInputStream.close();
	crimeCodeInputStream.close();


	return 0;
}	
