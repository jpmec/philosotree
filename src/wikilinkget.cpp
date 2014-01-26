#include "WikiLinkGetter.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <boost/algorithm/string/join.hpp>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>


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

    bool print_status_code(false);
    bool print_status_message(false);
    bool print_header(false);
    bool print_body(false);


    try
    {
        options.add_options()
            ("version", "print version")
            ("help", "print help")
            ("verbose,v", "verbose output")
            ("status_code,c", "status code")
            ("status_message,m", "status message")
            ("header,h", "header")
            ("body,b", "body")
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

        if (vm.count("status_code")) {
            print_status_code = true;
        }

        if (vm.count("status_message")) {
            print_status_message = true;
        }

        if (vm.count("header")) {
            print_header = true;
        }

        if (vm.count("body")) {
            print_body = true;
        }

        if (!vm.count("path")) {
            std::cout << options << std::endl;
            return 1;
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

    if (verbose)
    {
        std::cout << path << std::endl;
    }


    try
    {
        boost::asio::io_service io_service;
        WikiLinkGetter getter(io_service);

        std::set<std::string> links;
        get_links f(links);

        getter.verbose(verbose);

        getter.get(path.c_str(), f);

        io_service.run();


        for (std::set<std::string>::iterator l = links.begin(); l != links.end(); ++l)
        {
          std::cout << *l << std::endl;
        }
    }
    catch (...)
    {
        std::cout << "unexpected error" << std::endl;
        return 1;
    }

    return 0;
}