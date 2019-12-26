#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <boost/any.hpp>

#include <errno.h>

#include "options.hpp"

// private members

string Options::make_usage_string(const string& name, 
                                  const po::options_description& od) const 
{
    ostringstream oss;
    oss << "Usage: " << name << " [options]" << endl 
        << "    At least one config file is required" << endl << endl
        << od << endl;

    return oss.str();
}

void Options::opp_add_general_options(po::options_description& od) const
{   
   od.add_options()("help",   "Display this message") 
                   ("verbose,v", po::value<int>()->default_value(0)->implicit_value(1),
                   "Verbosity level")
                   ("config", po::value<vector<string>>()->required()->composing(),
                    "Config file, multiple allowed");
}

void Options::opp_add_config_file(po::options_description& od) const
{
    od.add_options()("setting", po::value<string>(), "some setting")
                    ("sec1.set1", po::value<string>(), "some setting")
                    ("sec1.set2", po::value<string>(), "some setting")
                    ("sec1.set3", po::value<string>(), "some setting")
                    ("sec1.set4", po::value<string>(), "some setting")
                    ("sec2.set1", po::value<string>(), "some setting")
                    ("sec2.set2", po::value<string>(), "some setting")
                    ("sec2.set3", po::value<string>(), "some setting")
                    ("sec2.set4", po::value<string>(), "some setting");

}

bool Options::read_from_config_files(po::options_description& all_options) {

    for(auto config_file: vm["config"].as<vector<string>>()) {
        cout << "config_file: " << config_file  << endl;
        try {
            ifstream infile(config_file.c_str());
            if(!infile.is_open()) {
                cerr << "ERROR: Cannot open " << config_file << ": "
                     << strerror(errno) << endl;
                return false;
            }

            po::store(po::parse_config_file(infile, all_options),vm);
            notify(vm);
        } catch (const ifstream::failure& ex) {
            cerr << "Exception opening/reading/closing file"
                 << ex.what() << endl;
            return false;
        } catch(const po::error &ex) {
            cerr << "parse error: " << ex.what() << endl;
            return false;
        }
    }
    return true;
}

// from https://gist.github.com/gesquive/8673796
string Options::DumpVariableMap() const {
    ostringstream oss;

    for (auto it = vm.begin(); it != vm.end(); it++) {
        oss << "> " << it->first;
        if (((boost::any)it->second.value()).empty()) {
            oss << "(empty)";
        }
        if (vm[it->first].defaulted() || it->second.defaulted()) {
            oss << "(default)";
        }
        oss << "=";

        bool is_char;
        try {
            boost::any_cast<const char *>(it->second.value());
            is_char = true;
        } catch (const boost::bad_any_cast &) {
            is_char = false;
        }
        bool is_str;
        try {
            boost::any_cast<std::string>(it->second.value());
            is_str = true;
        } catch (const boost::bad_any_cast &) {
            is_str = false;
        }

        if (((boost::any)it->second.value()).type() == typeid(int)) {
            oss << vm[it->first].as<int>() << endl;
        } else if (((boost::any)it->second.value()).type() == typeid(bool)) {
            oss << vm[it->first].as<bool>() << endl;
        } else if (((boost::any)it->second.value()).type() == typeid(double)) {
            oss << vm[it->first].as<double>() << endl;
        } else if (is_char) {
            oss << vm[it->first].as<const char * >() << endl;
        } else if (is_str) {
            std::string temp = vm[it->first].as<std::string>();
            if (temp.size()) {
                oss << temp << endl;
            } else {
                oss << "true" << endl;
            }
        } else { // Assumes that the only remainder is vector<string>
            try {
                std::vector<std::string> vect = vm[it->first].as<std::vector<std::string> >();
                uint i = 0;
                for (std::vector<std::string>::iterator oit=vect.begin();
                     oit != vect.end(); oit++, ++i) {
                    oss << "\r> " << it->first << "[" << i << "]=" << (*oit) << endl;
                }
            } catch (const boost::bad_any_cast &) {
                oss << "UnknownType(" << ((boost::any)it->second.value()).type().name() << ")" << endl;
            }
        }
    }

    return oss.str();
}


//  public members

bool Options::parse(int argc, char** argv) {
    vector<string> config_files;

    po::options_description genopts("General Options");
    opp_add_general_options(genopts);

    po::options_description confile("Config File");
    opp_add_config_file(confile);

    po::options_description all_options;
    all_options.add(genopts); all_options.add(confile);

    //bringing it all together for parsing
    vector<string> unknown_options;
    try {
        po::parsed_options parsedOptions =
                po::command_line_parser(argc, argv)
                .style(po::command_line_style::unix_style)
                .options(all_options).allow_unregistered().run();
        po::store(parsedOptions, vm);
        unknown_options = po::collect_unrecognized(parsedOptions.options,
                                                    po::include_positional);
        po::notify(vm);
    } catch(const po::error &ex) {
        cerr << "parse error: " << ex.what() << endl;
        return false;
    }

    if(vm.count("help") || !unknown_options.empty() || !vm.count("config")) {  
        cerr << make_usage_string(argv[0], all_options) << endl;
        return false;
    }

    return read_from_config_files(all_options);
}





