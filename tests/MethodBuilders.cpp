#define BOOST_TEST_MODULE MethodBuilders
#include <boost/test/unit_test.hpp>
#include <telecraft/core/MethodBuilders.hpp>



struct FirstMethod : telegram::core::MethodInfo<"first_method"> {
  void build() override {}
  void deserialize(const std::string& json) override {};
  std::string getBody() override { return "";}
};

struct SecondMethod : telegram::core::MethodInfo<"second_method",
                      telegram::core::http::ContentType::FormData,
                      telegram::core::http::Method::GET>
{
  void build() override {}
  void deserialize(const std::string& json) override {};
  std::string getBody() override { return "";}
};

BOOST_AUTO_TEST_SUITE(MethodBuilders)

  BOOST_AUTO_TEST_CASE(TestMethodInfo) {
    FirstMethod first;
    SecondMethod second;

    BOOST_CHECK_EQUAL(first.getName(), "first_method");
    BOOST_CHECK_EQUAL(second.getName(), "second_method");

    BOOST_CHECK(first.getContentType() == telegram::core::http::ContentType::Json);
    BOOST_CHECK(second.getContentType() == telegram::core::http::ContentType::FormData);

    BOOST_CHECK(first.getMethod() == telegram::core::http::Method::POST);
    BOOST_CHECK(second.getMethod() == telegram::core::http::Method::GET);
  }

BOOST_AUTO_TEST_SUITE_END()
