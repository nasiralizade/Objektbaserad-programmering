/*
Name: Nasir Alizade
2022-03-23
DT06G
Labb1

*/



#include <iostream>
#include <string>
#include<Windows.h>
#include <fstream>
#include <vector>
#include<algorithm>
#include <sstream>
#include <regex>
#include<locale>
using namespace std;
struct Address
{
    string street;
    int zip;
    string city;
};
struct Person
{
    string name;
    string id;
    Address location{};
};
string rtrim( const string& s );

string toUpper(string str);
istream& operator>>(istream& in, Person& p);
vector<Person> read_file(string filename);
size_t find_in_names(const vector<Person>& haystack, string name_part);
vector<Person> find_person_from_city(const vector<Person>& haystack, string city);



int main()
{
    SetConsoleCP(1252);//för svenska bokstäver
    SetConsoleOutputCP(1252);
    locale::global(locale(""));
    vector<Person> list=read_file("names.txt");
   string name_part,input;
   int option=0;
   while (option!=3)
   {
       cout << "1 - Sök del av personnam!" << endl;
       cout << "2 - Sök efter personer i stad!" << endl;
       cout << "3 - Avsluta!" <<endl;
       do
       {
          cout << "Skriv här: ";
          getline( cin ,input);
       } while (input.empty() || !isdigit(input.front()));//input får inte vara tom eller bokstäver
       option = input.front()-'0';
       if (option==1)
       {
           cout << "Skriv ett namn att söka efter: ";
           getline(cin, name_part);
           size_t count = find_in_names(list, name_part);
           cout << "Hittade " << count << " personer med liknande namn." << endl;
          

       }
       else if (option==2)
       {
           cout << "Skriv en stad att söka efter: ";
           getline(cin, name_part);
           vector<Person> sublist = find_person_from_city(list, name_part);
         
           vector< Person>::const_iterator it;  
           for(it=sublist.begin();it!=sublist.end();it++ )
           {
               cout<<it->id << ", " << it->name << ", " << it->location.zip << ", " << it->location.city << '\n';
           };
          /* for (const auto& per : sublist)
           {
               cout << per.id<<", "<<per.name<<", "<<per.location.zip << ", " << per.location.city<< '\n';
           };*/
           if (sublist.empty())
           {
               cout << "Hittade inga personer som bor i "<<name_part << endl;
           }
       }
   }
    return 0;
       
}

istream& operator>>(istream& in, Person& p)
{
    std::string zip1, zip2;
    std::getline(in, p.name);
    getline(in, p.id);
    getline(in, p.location.street, ',');
    in.ignore();//ignorera whitespace
    getline(in, zip1, ' ');//ta första delen av postnummer
    getline(in, zip2, ' ');//ta andra delen av postnummer
    stringstream omv(zip1 +  zip2);//slår ihop
    omv >> p.location.zip;
    in.ignore();
    string citySecond;
    getline(in,citySecond); ;
    p.location.city = rtrim(citySecond);//funktionen trim tar bort whitespace som kan förekomma
    return in;
}
vector<Person> read_file(string filename)
{
    ifstream file(filename.c_str());
    vector<Person> haystack;

    if (!file.is_open())
    {
        return haystack;
    }
    Person p;
    while (file >>p)//plocka in alla personer stegvis ur filen in i vector
    {
        haystack.push_back(p);
    }
    return haystack;
}
size_t find_in_names(const vector<Person> &haystack, string name_part)
{
    name_part = toUpper(name_part);
    //räkna bara de som har söksträngen som en substräng inom sig
    return	count_if(haystack.begin(), haystack.end(), [&](const Person &p)
        {
            string tmp = p.name;
            tmp = toUpper(tmp);
            return tmp.find(name_part) != string::npos;//::om den inte hittar någon  matchning så returnerar npos, att den har ingen position
        });
}
string toUpper(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    while (str.find("å") != std::string::npos)
        str.replace(str.find("å"), 2, "Å");
    while (str.find("ä") != std::string::npos)
        str.replace(str.find("ä"), 2, "Ä");
    while (str.find("ö") != std::string::npos)
        str.replace(str.find("ö"), 2, "Ö");
        
    return str;
}
vector<Person> find_person_from_city(const vector<Person>& haystack, string city)
{
    vector<Person> city_found;
    city = toUpper(city);

    for (const auto& p : haystack)
    {
        string tmp = p.location.city;
        tmp = toUpper(tmp);
        
        if (tmp == city)//om nuvarande staden matchar med vår vårterm, så ska denna person med
        {
            city_found.push_back(p);
        }

    }
    return city_found;
}

//funktioner används för att ta bort mellanslag 
string rtrim( const string& s ) {
    return regex_replace( s, regex( "\\s+$" ), string( "" ) );
}