#include <string>
#include <iostream>

#include <serde_yaml/serde_yaml.h>
#include "test_serde.h"
#include "mytypes.h"

struct [[serde]] Options
{
  bool debug;
  int line;
  std::string func;
};

int main()
{
  Options opts{true, 856, "main"};
  auto str = serde_yaml::to_string(opts).value();
  std::cout << str << std::endl;
  return 0;
}

//namespace foo {
struct [[serde]] Hello {
  int value;
  std::string when;
};
//}
