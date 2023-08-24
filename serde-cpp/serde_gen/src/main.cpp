#include <iostream>
#include "init.h"

using namespace serde_gen::init;

///////////////////////////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
try {
    int ret = 0;
    auto option_list = create_option_list();
    const auto options = option_list.parse(argc, argv);

    ret = handle_help_version(option_list, options);
    if (ret)
        return 0;

    ret = validate_options(options);
    if (ret)
        return ret;

    ret = run_serde_generator(options);
    if (ret)
        return ret;

    return 0;
}
catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
}
