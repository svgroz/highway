#include <string>
#include <unordered_map>
#include <vector>
#define BOOST_TEST_MODULE your_test_module
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
  c.connect();

  BOOST_TEST(c.id() == "test");
}
