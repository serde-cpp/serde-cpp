#include <string>
#include <iostream>

#include <serde_yaml/serde_yaml.h>

struct [[serde]] Options
{
  bool debug;
  int line;
  std::string func;
};

int main()
{
  Options opts{true, 856, "main"};
  auto str = serde_yaml::to_string(opts).unwrap();
  std::cout << str << std::endl;
  return 0;
}

