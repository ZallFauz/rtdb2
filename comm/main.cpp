/*
 * main.cpp
 *
 *  Created on: 2018-12-08
 *      Author: Jan Feitsma
 */

#include "comm.hpp"
#include "../../rtdb2/RtDB2Context.h"
#include "boost/program_options.hpp"

namespace po = boost::program_options;

int main(int argc, char **argv)
{
    int agent;
    std::string network;
    std::string dbpath;

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("agent", po::value<int>(&agent)->default_value(-1), "agent")
        ("network", po::value<std::string>(&network)->default_value("default"), "network name")
        ("dbpath", po::value<std::string>(&dbpath)->default_value(RTDB2_DEFAULT_PATH), "database storage directory");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return 1;
    }

    if (agent < 0) {
        char *envc = getenv("AGENT");
        agent = envc == NULL ? 0 : atoi(envc);
    }

    std::cout << "Starting network: " << network << std::endl;

    RtDB2Context context = RtDB2Context::Builder(RtDB2ProcessType::comm)
                               .withRootPath(dbpath)
                               .withNetwork(network)
                               .build();
    Comm c(agent, context);
    c.run();

    return 0;
}
