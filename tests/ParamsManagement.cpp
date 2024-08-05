#define BOOST_TEST_MODULE ParametersManagement
#include <boost/test/unit_test.hpp>
#include <telecraft/core/ParamsManagement.hpp>

struct FirstParam : telegram::core::ParametersBuilder<int, "first_param"> {
  using ParametersBuilder::ParametersBuilder;
};

struct SecondParam : telegram::core::ParametersBuilder<std::string, "second_param"> {
  using ParametersBuilder::ParametersBuilder;
};

struct ThirdParam : telegram::core::ParametersBuilder<double, "third_param"> {
  using ParametersBuilder::ParametersBuilder;
};

BOOST_AUTO_TEST_SUITE(ParamsManagement)

  BOOST_AUTO_TEST_CASE(TestParamBuilder) {
    FirstParam first;
    SecondParam second("4141");

    first.set(31311);
    BOOST_CHECK_EQUAL(first.get(), 31311);
    BOOST_CHECK_EQUAL(second.get(), "4141");

    first.serialize("31311");
    BOOST_CHECK_EQUAL(first.get(), 31311);

    second.serialize("121212");
    BOOST_CHECK_EQUAL(second.get(), "121212");

    BOOST_CHECK_EQUAL(first.name, "first_param");
  }

  BOOST_AUTO_TEST_CASE(TestParamContainer) {
    telegram::core::ParametersContainer<FirstParam, SecondParam> container;

    BOOST_TEST((std::is_same_v<decltype(container.at<FirstParam>()), FirstParam&>));
    BOOST_TEST(container.contains<FirstParam>());
    BOOST_TEST(!container.contains<ThirdParam>());

    container.at<FirstParam>().set(42);
    container.at<SecondParam>().set("Hello");

    container.forEach([](auto& param) {
      using ParamType = std::decay_t<decltype(param)>;

      if constexpr (std::is_same_v<typename ParamType::ValueType, int>) {
        param.set(param.get() + 1);
      } else if constexpr (std::is_same_v<typename ParamType::ValueType, std::string>) {
        param.set(param.get() + " World");
      }
    });

    BOOST_TEST(container.at<FirstParam>().get() == 43);
    BOOST_TEST(container.at<SecondParam>().get() == "Hello World");
  }
BOOST_AUTO_TEST_SUITE_END()
