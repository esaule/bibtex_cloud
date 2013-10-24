#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

//#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

#include "bibtexreader.hpp"
#include "bibtexwriter.hpp"

#include "util.hpp"

using bibtex::BibTeXEntry;
typedef std::vector<std::string> ValueVector;
typedef std::pair<std::string, ValueVector> KeyValue;
std::vector<BibTeXEntry> entries;

  std::set<std::string> acceptable;


int extract_key_keywords(std::string filename, std::map<std::string, std::vector<std::string> > & out) {

  bool verbose = false;

  acceptable.clear();
  acceptable.insert("article");
  acceptable.insert("book");
  acceptable.insert("inbook");
  acceptable.insert("incollection");
  acceptable.insert("inproceedings");
  acceptable.insert("mastersthesis");
  acceptable.insert("misc");
  acceptable.insert("phdthesis");
  acceptable.insert("proceedings");
  acceptable.insert("techreport");

  std::ifstream in(filename);
  
  if (!in) {
    std::cerr << "error: could not open " << filename << "\n";
    return EXIT_FAILURE;
  }

  bool result = read(in, entries);

  if (verbose) {
    std::cout << "found " << entries.size() << " entries\n"; 
  }
  
  std::set<std::string> existing_tags;

  for (BibTeXEntry b : entries) {
    std::string t = b.tag;
    t = str_tolower(t);
    existing_tags.insert(t);
  }

  if (verbose) {
    std::cout<<"===================================="<<std::endl;
    std::cout<<"List of existing tags"<<std::endl;
    for (std::string tag : existing_tags) {
      std::cout<< tag<<std::endl;
    }
    std::cout<<"===================================="<<std::endl;
  }

  for (BibTeXEntry b : entries) {
    std::string t = b.tag;
    std::transform (t.begin(), t.end(), t.begin(), (int(*)(int))std::tolower);
    if (std::find(acceptable.begin(), acceptable.end(), t) != acceptable.end()) {
      std::string key = b.key.get();
      
      std::vector<std::string> keywords;

      std::string keyword_raw;
      for (KeyValue kv : b.fields) {
	if (kv.first.compare("keywords") == 0) {
	  for (std::string s : kv.second) {
	    keyword_raw.append(s);
	  }
	}
      }

      std::stringstream ss (keyword_raw);
      while (ss) {
	std::string s;
	std::getline(ss, s, ',');
	if (ss) {
	  //transform all white space into actual space
	  std::transform(s.begin(), s.end(), s.begin(), [](char x)->char{if (x == '\n' || x == '\r' || x == '\t') return ' ';  return x;} );

	  //remove white space at the beginning or at the end
	  s = trim(s);  
	  
	  //this compresses all the internal spaces in a string "foo    bar" => "foo bar"
	  for (int i=0; i< s.length(); ++i) {
	    if (s[i] == ' ' && s[i+1] == ' ') {
	      s = s.erase(i, 1);
	      --i;
	    }
	  }	    
	  keywords.push_back(s);
	}
      }
      
      if (verbose) {
	std::cout<<"key="<<key<<" keywords=";
	for (std::string s : keywords)
	  std::cout<<s<<"|";
	std::cout<<std::endl;
      }
      out[key]=keywords;
    }
  }
  
  return 0;
}

///use the database read from the previous call to parse.
void print_to_stdout (const std::set<std::string>& keys) {
  for (auto ent : entries) {
    
    std::string t = ent.tag;
    std::transform (t.begin(), t.end(), t.begin(), (int(*)(int))std::tolower);
    if (std::find(acceptable.begin(), acceptable.end(), t) != acceptable.end()) {
      if (std::find (keys.begin(), keys.end(), ent.key.get()) != keys.end()) {
	//      bibtex::write(std::cout, ent);
	//std::cout<<ent;
	std::cout<<"@"<<ent.tag<<"{"<<ent.key.get();
	for (auto f : ent.fields) {
	  std::cout<<",\n    "<<f.first<<" = \""<<f.second[0]<<"\"";
	}
	std::cout<<"\n}\n"<<std::flush;
      }
    }
  }
}
