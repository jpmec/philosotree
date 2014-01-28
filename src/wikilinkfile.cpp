#include "WikiLinkWeb.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <boost/algorithm/string/join.hpp>
#include <boost/program_options.hpp>

#include "iojson.hpp"


#define VERSION "0.0.0"




struct get_links
{
    get_links(std::set<std::string>& links)
    : links(links)
    {
    }

    void operator() (const std::set<std::string>& l)
    {
        links = l;
    }

    std::set<std::string>& links;
};




int main(int argc, char* argv[])
{
    boost::program_options::options_description options("options");
    boost::program_options::positional_options_description args;

    std::vector< std::string > path_vector;
    bool verbose(false);
    bool add_cin(false);

    std::string filename("linkfile.txt");


    try
    {
        options.add_options()
            ("version", "print version")
            ("help", "print help")
            ("verbose", "verbose output")
            ("file,f", "filename")
            ("add,a", "add from cin")
            ("path", boost::program_options::value< std::vector<std::string> >(&path_vector), "path")
        ;

        args.add("path", -1);


        boost::program_options::variables_map vm;
        boost::program_options::store(
            boost::program_options::command_line_parser(argc, argv)
            .options(options)
            .positional(args)
            .run()
            , vm
        );
        boost::program_options::notify(vm);

        if (vm.count("version")) {
            std::cout << VERSION << std::endl;
            return 1;
        }

        if (vm.count("help")) {
            std::cout << options << std::endl;
            return 1;
        }

        if (vm.count("verbose")) {
            verbose = true;
        }

        if (vm.count("add")) {
            add_cin = true;
        }
    }
    catch( const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cout << options << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cout << "unexpected argument error" << std::endl;
        return 1;
    }

    std::string path = boost::algorithm::join(path_vector, " ");

    try
    {
        WikiLinkWeb web;

        std::ifstream file(filename.c_str());
        if (file.is_open())
        {
            file >> web;
            file.close();
        }

        if (add_cin)
        {
            std::string line;

            std::cin.sync_with_stdio(false);
            while (std::cin) {
                std::getline(std::cin, line);

                if (line.size())
                {
                    web.connect(path, line);
                }
            };
        }

        std::cout << web << std::endl;

    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "unexpected error" << std::endl;
        return 1;
    }

    return 0;
}