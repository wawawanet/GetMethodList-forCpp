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
  string str;
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
    }
    if(str.find("}") != string::npos) {
      counter--;
      if(counter < (int)ns.size()) {
        ns.pop_back();
      }
    }

    if(str.find("namespace") == 0) {
      vector<string> v;
      boost::algorithm::split(v, str, boost::is_any_of(" "));
      cout << v[1] << endl;
      if(counter > 0) {
        cout << v[1] << endl;
        ns.push_back(v[1]);
      }
      v.clear();
    }

    if(str.find("class") == string::npos) {
      string name;
      for(int i = 0; i < (int)ns.size(); ++i) {
        name += ns[i] + "::";
      }

      vector<string> v;
      boost::algorithm::split(v, str, boost::is_any_of(" "));
      name += v[1];
      ofs << name << "\n" << endl;
      v.clear();
    }
  } // end while

  ifs.close();
  ofs.close();
  ns.clear();
} // end getMethodName

int main(int argc, char* argv[]) {
  const fs::path path(argv[1]);
  BOOST_FOREACH(const fs::path& p, std::make_pair(fs::recursive_directory_iterator(path), fs::recursive_directory_iterator())) {
    if (!fs::is_directory(p)) {
      // std::cout << p << std::endl;
      string fname = p.generic_string();
      getMethodName(fname);
    }
  }
  // cout << argv[1] << endl;
  // getMethodName(argv[1]);
} // end main