#include "HttpGetter.hpp"

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>


#define VERSION "0.0.0"




struct get_response
{
    get_response(HttpResponse& r)
    : response(r)
    {
    }

    void operator() (const HttpResponse& r)
    {
        response = r;
    }

    HttpResponse& response;
};




int main(int argc, char* argv[])
{
    boost::program_options::options_description options("options");
    boost::program_options::positional_options_description args;

    std::string server;
    std::string path("/");
    bool verbose(false);

    try
    {
        options.add_options()
            ("version", "print version")
            ("help", "print help")
            ("verbose,v", "verbose output")
            ("server", boost::program_options::value<std::string>(&server), "server")
            ("path", boost::program_options::value<std::string>(&path), "path")
        ;

        args.add("server", 1);
        args.add("path", 2);

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


    if (verbose)
    {
        std::cout << server << std::endl;
        std::cout << path << std::endl;
    }


    try
    {
        boost::asio::io_service io_service;
        HttpGetter getter(io_service);

        HttpResponse response;
        get_response f(response);

        getter.get(server.c_str(), path.c_str(), f);

        io_service.run();

        std::cout << response << std::endl;
    }
    catch (...)
    {
        std::cout << "unexpected error" << std::endl;
        return 1;
    }

    return 0;
}