#pragma once

#include <iomanip>
#include <iostream>

#include <map>
#include <string>

std::map<std::string, std::string>
parse_args(int argc, char* argv[]);

class Options
{
public:
  Options(std::map<std::string, std::string>);

  bool parser_error = false;

  std::string input_file;

  bool  naive           = false;
  float naive_threshold = 100.0f;
  int   kmeans          = 5;

  bool dump_colors = false;
};
