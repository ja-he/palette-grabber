#include "cli.hh"

#include <map>
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

std::map<std::string, std::string> short_to_long{ { "i", "input" },
                                                  { "d", "dump-colors" },
                                                  { "n", "naive" } };

std::map<std::string, std::vector<std::string>> incompatible_counterparts{
  { "input", {} },
  { "dump-colors", { "naive" } },
  { "naive", { "dump-colors" } },
};

Options::Options(std::map<std::string, std::string> m)
{
  assert(!m.contains(""));

  // migrate values to long option names, erase any short ones
  for (const auto& [s, l] : short_to_long) {
    if (m.contains(s)) {
      assert(!m.contains(l));
      m[l] = m[s];
      m.erase(s);
    }
  }

  // TODO(ja-he): verify existence of required options

  // TODO(ja-he): verify no incompatible options

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
