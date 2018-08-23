#include "./util.h"

using namespace std;
using namespace ftp;
using namespace sfsm;

void testBasics() {
    ftp_test::testParseString({{"identity", "[a-z]*", "0"}},
                  {{"abc", {{"identity", "abc"}}}});
}

void testJsonStyle() {
    ftp_test::testParseString(
      {
          //
          {"string", CommonTokenReg::jsonStringExpStr, "1"},
          {"number", CommonTokenReg::jsonNumberExpStr, "1"},
          {"{", "\\{", "1"},
          {"}", "\\}", "2"},
          {"[", "\\[", "2"},
          {"]", "\\]", "2"},
          {",", "\\,", "2"},
          {":", "\\:", "2"},
          {"whitespace", "[ \n\r]*", "2"},
          {"true", "true", "2"},
          {"false", "false", "2"},
          {"null", "null", "2"}
          //
      },
      {
          //
          {"\"hello\"", {{"string", "\"hello\""}}}, //
          {"123", {{"number", "123"}}},
          {"{}", {{"{", "{"}, {"}", "}"}}},
          {"true", {{"true", "true"}}},
          {"false", {{"false", "false"}}},
          {"null", {{"null", "null"}}},

          {"[1,2,3]",
           {{"[", "["},
            {"number", "1"},
            {",", ","},
            {"number", "2"},
            {",", ","},
            {"number", "3"},
            {"]", "]"}}},

          {"{\"a\":1,\"b\":2}",
           {
               {"{", "{"},
               {"string", "\"a\""},
               {"number", "1"},
               {":", ":"},
               {",", ","},
               {"string", "\"b\""},
               {":", ":"},
               {"number", "2"},
           }},

          {"[1,{\"attra\":{\"attrb\":10}}]",
           {{"[", "["},
            {",", ","},
            {"{", "{"},
            {"string", "\"attra\""},
            {":", ":"},
            {"{", "{"},
            {"string", "\"attrb\""},
            {":", ":"},
            {"number", "10"},
            {"}", "}"},
            {"}", "}"},
            {"]", "]"}}}
          //
      });
};

int main() {
  testBasics();
  testJsonStyle();
  return 0;
}
