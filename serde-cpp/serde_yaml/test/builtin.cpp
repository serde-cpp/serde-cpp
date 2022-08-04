#include <gtest/gtest.h>

#include "serde/serde.h"
#include "serde_yaml/serde_yaml.h"

///////////////////////////////////////////////////////////////////////////////
// Builtin types
///////////////////////////////////////////////////////////////////////////////

TEST(Builtin, Bool_True)
{
  bool val = true;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "1\n");
  auto de_val = serde_yaml::from_str<bool>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Builtin, Bool_False)
{
  bool val = false;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "0\n");
  auto de_val = serde_yaml::from_str<bool>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Builtin, Int_Positive)
{
  int val = 42631;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "42631\n");
  auto de_val = serde_yaml::from_str<int>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Builtin, Int_Negative)
{
  int val = -42631;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "-42631\n");
  auto de_val = serde_yaml::from_str<int>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Builtin, ShortInt_Positive)
{
  short int val = 16535;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "16535\n");
  auto de_val = serde_yaml::from_str<short int>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Builtin, ShortInt_Negative)
{
  short int val = -16535;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "-16535\n");
  auto de_val = serde_yaml::from_str<short int>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Builtin, LongInt_Positive)
{
  long int val = 98694223l;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "98694223\n");
  auto de_val = serde_yaml::from_str<long int>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Builtin, LongInt_Negative)
{
  long int val = -98694223l;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "-98694223\n");
  auto de_val = serde_yaml::from_str<long int>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Builtin, LongLongInt_Positive)
{
  long long int val = 98694223111ll;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "98694223111\n");
  auto de_val = serde_yaml::from_str<long long int>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Builtin, LongLongInt_Negative)
{
  long long int val = -98694223111ll;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "-98694223111\n");
  auto de_val = serde_yaml::from_str<long long int>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Builtin, UnsignedInt)
{
  unsigned int val = 42631u;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "42631\n");
  auto de_val = serde_yaml::from_str<unsigned int>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Builtin, UnsignedShortInt)
{
  unsigned short int val = 16535u;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "16535\n");
  auto de_val = serde_yaml::from_str<unsigned short int>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Builtin, UnsignedLongInt)
{
  unsigned long int val = 98694223lu;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "98694223\n");
  auto de_val = serde_yaml::from_str<unsigned long int>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Builtin, UnsignedLongLongInt)
{
  unsigned long long int val = 98694223111llu;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "98694223111\n");
  auto de_val = serde_yaml::from_str<unsigned long long int>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Builtin, Float)
{
  float val = 3.14159f;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "3.14159\n");
  auto de_val = serde_yaml::from_str<float>(std::move(str)).value();
  EXPECT_FLOAT_EQ(de_val, val);
}

TEST(Builtin, Double)
{
  double val = 3.14159;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "3.14159\n");
  auto de_val = serde_yaml::from_str<double>(std::move(str)).value();
  EXPECT_DOUBLE_EQ(de_val, val);
}

TEST(Builtin, Char)
{
  char val = 'A';
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "A\n");
  auto de_val = serde_yaml::from_str<char>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Builtin, SignedChar)
{
  signed char val = 'B';
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "B\n");
  auto de_val = serde_yaml::from_str<char>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Builtin, UnsignedChar)
{
  unsigned char val = 250;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "250\n");
  auto de_val = serde_yaml::from_str<unsigned char>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Builtin, ConstCharPtr)
{
  const char* val = "FooBar";
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "FooBar\n");
  EXPECT_THROW(std::ignore = serde_yaml::from_str<const char*>(std::move(str)), std::logic_error);
}

TEST(Builtin, CharPtr)
{
  char cstr[10] = "Wiggle";
  char* val = cstr;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "Wiggle\n");
  EXPECT_THROW(std::ignore = serde_yaml::from_str<char*>(std::move(str)), std::logic_error);
}

TEST(Builtin, LiteralCharArray)
{
  struct Struct {
    char val[10] = "Wiggle";
    void serialize(serde::Serializer& ser) const { ser.serialize(val); }
    void deserialize(serde::Deserializer& de) { de.deserialize(val); }
  } val;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "Wiggle\n");
  auto de_val = serde_yaml::from_str<Struct>(std::move(str)).value();
  EXPECT_STREQ(de_val.val, "Wiggle");
}

// TODO: wchar_t
