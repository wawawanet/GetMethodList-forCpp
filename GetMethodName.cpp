#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

using namespace std;
namespace fs = boost::filesystem;

void getMethodName(string filename) {
  vector<string> ns;
  vector<string> tmpname;
  vector<string> tmpclass;
  ns.reserve(5);
  tmpname.reserve(5);
  tmpclass.reserve(5);
  string str;
  string name;
  int counter = 0;

  // Open Files
  ifstream ifs(filename);
  if(ifs.fail()) {
    cerr << "File Read Error" << endl;
    exit(1);
  }
  ofstream ofs("MethodList.txt");
  if(ofs.fail()) {
    cerr << "File Write Error" << endl;
    exit(1);
  }

  // 1行ずつ読み出し, 1行ずつ書き出し
  while(getline(ifs, str)) {
    boost::algorithm::trim(str);

    if(str.find("{") != string::npos) {
      counter++;
      cout << "Incremented. Counter: " << counter << endl;
    }
    if(str.find("}") != string::npos) {
      counter--;
      cout << "Decremented. Counter: " << counter << endl;
      if(counter < (int)ns.size()) {
        ns.pop_back();
      }
    }

    if(str.find("namespace") == 0) {
      boost::algorithm::split(tmpname, str, boost::is_any_of(" "));
      if(counter > 0) {
        cout << tmpname[1] << endl;
        ns.push_back(tmpname[1]);
      }
      tmpname.clear();
      vector<string>().swap(tmpname);
    }

    if(str.find("class") == string::npos) {
      for(int i = 0; i < (int)ns.size(); ++i) {
        name += ns[i] + "::";
      }
      boost::algorithm::split(tmpclass, str, boost::is_any_of(" "));
      name += tmpclass[1];
      ofs << name << "\n" << endl;
      tmpclass.clear();
      vector<string>().swap(tmpname);
      name = "";
    }
  } // end while

  ifs.close();
  ofs.close();
  ns.clear();
  tmpname.clear();
  tmpclass.clear();

  vector<string>().swap(ns);
  vector<string>().swap(tmpname);
  vector<string>().swap(tmpclass);
} // end getMethodName

int main(int argc, char* argv[]) {
  // const fs::path path(argv[1]);
  // BOOST_FOREACH(const fs::path& p, std::make_pair(fs::recursive_directory_iterator(path), fs::recursive_directory_iterator())) {
  //   if (!fs::is_directory(p)) {
  //     // std::cout << p << std::endl;
  //     string fname = p.generic_string();
  //     getMethodName(fname);
  //   }
  // }
  cout << argv[1] << endl;
  getMethodName(argv[1]);
} // end main