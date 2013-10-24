#ifndef PARSE_HPP_BIBTEX_
#define PARSE_HPP_BIBTEX_

#include <map>
#include <set>
#include <vector>
#include <string>

int extract_key_keywords(std::string filename, std::map<std::string, std::vector<std::string> > & out);

void print_to_stdout (const std::set<std::string>& keys);

#endif
