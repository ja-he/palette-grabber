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

    // TODO(ja-he): due to how we do this, an argument like `--i asdf` will get
    //              parsed the same `-i asdf` would. Do we consider this a
    //              problem? I don't see name clash potential because we'll
    //              never want single letter long names but I guess it's a bit
    //              dirty to also parse `--...` as a short option.

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
  return m;
}

struct OptionSpec
{
public:
  void print(void) const
  {
    std::cerr << program_name << ": " << '\n';
    std::cerr << "  " << program_description << '\n';
    std::cerr << '\n';

    std::cerr << "Usage: " << '\n';
    std::cerr << "$ " << program_name << ' ';
    for (const auto& option : required) {
      std::cerr << "--" << option
                << (option_arguments.at(option).length()
                      ? " " + option_arguments.at(option)
                      : "")
                << ' ';
    }
    std::cerr << "[optional options]";
    std::cerr << '\n';
    std::cerr << '\n';

    std::cerr << "Options:" << '\n';
    size_t maxlen = 0;
    for (const auto& [option, arg] : option_arguments) {
      maxlen = std::max(
        maxlen, option.length() + (arg.length() ? (1 + arg.length()) : 0));
    }
    maxlen += 5; // 5 because of the "-x,--" prefix

    for (const auto& [option, arg] : option_arguments) {
      std::cerr << std::setw(maxlen) << std::left
                << (("-" + long_to_short.at(option)) + ',' +
                    ("--" + option + (arg.length() ? " " + arg : "")))
                << " : ";
      int               column = maxlen + 3;
      std::stringstream ss(description.at(option));
      std::string       word;
      while (std::getline(ss, word, ' ')) {
        if (column + word.length() > 80) {
          std::cerr << '\n';
          std::cerr << std::string(maxlen + 3, ' ');
          column = maxlen + 3;
        }
        std::cerr << word << ' ';
        column += word.length();
      }
      std::cerr << '\n';
    }
  };

  std::string program_name = "palette-grabber";
  std::string program_description =
    "Generate a color palette from an image file.";

  std::map<std::string, std::string> short_to_long{ { "h", "help" },
                                                    { "i", "input" },
                                                    { "k", "k-means" } };
  std::map<std::string, std::string> long_to_short{ { "help", "h" },
                                                    { "input", "i" },
                                                    { "k-means", "k" } };

  std::map<std::string, std::vector<std::string>> incompatible_counterparts{
    { "help", {} },
    { "input", {} },
    { "k-means", {} },
  };

  std::map<std::string, std::string> description{
    { "help", "Print this help information." },
    { "input", "The input file. Must be given a valid input file name." },
    { "k-means", "Specify 'k' for k-Means clustering. (Default: 10)" },
  };

  std::map<std::string, std::string> option_arguments{ { "help", "" },
                                                       { "input", "<file>" },
                                                       { "k-means", "[<k>]" } };

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

  // check if --help was given
  if (m.contains("help")) {
    spec.print();
    exit(EXIT_SUCCESS);
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
  if (m.contains("k-means")) {
    if (m["k-means"] == "") {
      this->kmeans = 5;
    } else {
      this->kmeans = std::stoul(m["k-means"]);
    }
  }
  if (m.contains("input")) {
    assert(m["input"] != "");
    this->input_file = m["input"];
  }
}
