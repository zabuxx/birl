#ifndef __options_hpp
#define __options_hpp

#include <string>

#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

class Options {
    private:
        po::variables_map vm;

        string basename(const string&) const;
        string make_usage_string(const string&, 
                                 const po::options_description&) const;


        void opp_add_general_options(po::options_description&) const;
        void opp_add_config_file(po::options_description& ) const;


        bool read_from_config_files(po::options_description&);
    public:
        Options() { }

        bool parse(int, char**);

        string DumpVariableMap() const;
};

#endif