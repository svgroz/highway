#define BOOST_TEST_MODULE your_test_module

#include <string>
#include <unordered_map>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include <kafka.hpp>

BOOST_AUTO_TEST_CASE(your_test_case) {
  using namespace highway::kafka;

  ConsumerProperties props;
  props.id = "test";
  props.topics = std::vector<std::string>{"test_1"};
  props.properties = std::unordered_map<std::string, std::string>{
      {"bootstrap.servers", "10.8.0.1"}, {"group.id", "test_consumer_id"}};

  auto c = Consumer(props);

  BOOST_TEST(ConsumerCodes::OK == c.connect());
  BOOST_TEST(ConsumerCodes::TARGET_STATE_UNREACHEBLE == c.connect());
}
