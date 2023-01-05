#include <string>
#include <iostream>

#include <serde_yaml/serde_yaml.h>
#include "serde/test_serde.h"
#include "mytypes.h"

struct [[serde, serde::deny_unkown_fields, serde::rename_all(serialize="UPPERCASE")]]
Options
{
  bool [[serde::skip]] debug;
  int [[serde::rename("LINE")]] line;
  std::string [[using serde: alias("fn"), alias("FUNC")]] func;
};

int main()
{
  Options opts{true, 856, "main"};
  auto str = serde_yaml::to_string(opts).value();
  std::cout << str << std::endl;
  return 0;
}

//namespace foo {
struct [[serde, serde::deny_unkown_fields]]
Hello {
  int value;
  std::string when;
};
//}
