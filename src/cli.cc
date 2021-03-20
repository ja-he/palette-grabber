#include "cli.hh"

#include <map>
#include <set>
#include <string>

#include <regex>

#include <cassert>

std::map<std::string, std::string>
parse_args(int argc, char* argv[])
{
  std::map<std::string, std::string> m;
  std::string                        last_key = "";
  for (int arg_index = 1; arg_index < argc; arg_index++) {
    std::string arg{ argv[arg_index] };

    std::smatch match;
    if (std::regex_search(arg, match, std::regex("^--([a-z\\-]+)$"))) {
      last_key    = match[1];
      m[last_key] = "";
    } else if (std::regex_search(
                 arg, match, std::regex("^--([a-z\\-]+)=(.*)$"))) {
      m[match[1]] = match[2];
      last_key    = "";
    } else if (std::regex_match(arg, std::regex("^-[a-z]+$"))) {
      for (size_t key_index = 1; key_index < arg.length(); key_index++) {
        last_key    = std::string(1, arg[key_index]);
        m[last_key] = "";
      }
    } else {
      assert(last_key != "");
      m[last_key] = arg;
      last_key    = "";
    }
  }
  for (const auto& [k, v] : m) {
    std::cerr << std::setw(10) << k << " : " << v << '\n';
  }
  return m;
}

struct OptionSpec
{
public:
  std::map<std::string, std::string> short_to_long{ { "i", "input" },
                                                    { "d", "dump-colors" },
                                                    { "n", "naive" } };
  std::map<std::string, std::string> long_to_short{ { "input", "i" },
                                                    { "dump-colors", "d" },
                                                    { "naive", "n" } };

  std::map<std::string, std::vector<std::string>> incompatible_counterparts{
    { "input", {} },
    { "dump-colors", { "naive" } },
    { "naive", { "dump-colors" } },
  };

  std::map<std::string, std::string> description{
    { "input", "The input file. Must be given a valid input file name." },
    { "dump-colors", "Dump all colors from the image." },
    { "naive",
      "Naively get unique (by euclidean distance) color values from image to "
      "form palette. Can be given a distance threshold value, default is 100.0 "
      "if none provided." }
  };

  std::map<std::string, std::string> option_arguments{ { "input", "<file>" },
                                                       { "dump-colors", "" },
                                                       { "naive",
                                                         "[<threshold>]" } };

  std::set<std::string> required{ "input" };
};

const OptionSpec spec;

Options::Options(std::map<std::string, std::string> m)
{
  assert(!m.contains(""));

  // migrate values to long option names, erase any short ones
  for (const auto& [s, l] : spec.short_to_long) {
    if (m.contains(s)) {
      assert(!m.contains(l));
      m[l] = m[s];
      m.erase(s);
    }
  }

  // TODO(ja-he): verify existence of required options
  for (const auto& required : spec.required) {
    if (!m.contains(required)) {
      std::cerr << "Required argument '" << required << "' missing. Exiting."
                << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  // TODO(ja-he): verify no incompatible options
  for (const auto& [option, _] : m) {
    if (!spec.description.contains(option)) {
      std::cerr << "Unrecognized option '" << option << "'. Exiting."
                << std::endl;
      exit(EXIT_FAILURE);
    }
    for (const auto& counterpart : spec.incompatible_counterparts.at(option)) {
      if (m.contains(counterpart)) {
        std::cerr << "Incompatible arguments '" << option << "' and '"
                  << counterpart << "'. Exiting." << std::endl;
        exit(EXIT_FAILURE);
      }
    }
  }

  // parse all options
  if (m.contains("naive")) {
    this->naive = true;
    if (m["naive"] == "") {
      this->naive_threshold = 100.0f;
    } else {
      try {
        this->naive_threshold = std::stof(m["naive"]);
      } catch (const std::exception& e) {
        std::cerr << "error: value '" << m["naive"]
                  << "' provided for 'naive' could not be parsed: " << e.what()
                  << std::endl;
        this->parser_error = true;
      };
    }
  }
  if (m.contains("dump-colors")) {
    this->dump_colors = true;
    assert(m["dump-colors"] == "");
  }
  if (m.contains("input")) {
    assert(m["input"] != "");
    this->input_file = m["input"];
  }
}
