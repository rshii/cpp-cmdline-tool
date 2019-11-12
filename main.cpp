// Example program
#include <iostream>
#include <string>
#include <regex>
#include <cstdarg>
#include <vector>
#include <type_traits>
#include <sstream>

using namespace std;

void parse(string s, vector<string> & v) {
  v.clear();
  regex words_regex (R"(".*?"|\S+)");
  auto words_begin = sregex_iterator(s.begin(), s.end(), words_regex);
  auto words_end = sregex_iterator();
  for (auto i = words_begin; i != words_end; ++i) {
      smatch match = *i;                                                 
      string match_str = match.str();
      auto doublequote = match_str.find('"');
      if (doublequote!=string::npos) {
        doublequote = match_str.find('"',doublequote+1);
        if (doublequote!=string::npos && match_str.length() > 2)
          v.push_back(match_str.substr(1,match_str.length()-2));
      }
      else v.push_back(match_str);
  }
}

template <typename T>
bool validateData(const string s, T & x) {
  stringstream ss (s);
  if (is_same<T, bool>::value) {
    if (!(ss >> boolalpha >> x)) return false;
  }
  else if (!(ss >> x)) return false;
  return true;
}

template <typename T>
bool validateData(int i, const vector<string> & v, T & x) {
  if (v.size()==0) return false;
  return validateData(v[i], x);
}

template <typename T, typename ... Args>
bool validateData(int i, const vector<string> & v, T & x, Args & ... args) {
  if (v.size() == 0 || i > v.size() - 1) {
    cout << "not enough parsed strings to match ... args " << endl;
    return false;
  }
  if (!validateData(v[i], x)) return false;
  return validateData(i+1, v, args...);
}

int main ()
{
  string s;
  vector<string> v;
  while (s!="quit") {
    cout << "INPUT albumTitle(string) artist(string) numberOfTracks(int) year(int) price(float) stock available(bool):" << endl;
    cout << endl;
    getline(cin,s);
    parse(s,v);

    string albumTitle;
    string artist;
    int numberOfTracks;
    int year;
    double price;
    bool stockAvailable;

    if (validateData(
      0, v, albumTitle, artist, numberOfTracks, year, price, stockAvailable
      )) {
      cout << "It worked!! Data is clean!" << endl;
      cout << albumTitle << endl;
      cout << artist << endl;
      cout << numberOfTracks << endl;
      cout << year << endl;
      cout << price << endl;
      cout << stockAvailable << endl;
    }
 
    else {
      cout << "Invalid input" << endl << endl;
    }
  }
  return 0;
}

// something of interest: vector<T>.size() is unsigned
// if v is empty, then v.size() - 1 would overflow
// can be the culprit of terrible bugs