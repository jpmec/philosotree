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

        if (!vm.count("server")) {
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

        if (   !print_status_code
            && !print_status_message
            && !print_header
            && !print_body)
        {
            std::cout << response << std::endl;
        }
        else
        {
            if (print_status_code)
            {
                std::cout << response.status_code << std::endl;
            }

            if (print_status_message)
            {
                std::cout << response.status_message << std::endl;
            }

            if (print_header)
            {
                std::map<std::string, std::string>::const_iterator i = response.header.begin();
                for (; i != response.header.end(); ++i)
                {
                    std::cout << i->first << ": " << i->second << std::endl;
                }
            }

            if (print_body)
            {
                std::cout << response.body << std::endl;
            }
        }

    }
    catch (...)
    {
        std::cout << "unexpected error" << std::endl;
        return 1;
    }

    return 0;
}