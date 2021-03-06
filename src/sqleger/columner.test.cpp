#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/sqleger.hpp>


using namespace sqleger;
using namespace sqleger::string_span_literals;


TEST_CASE("A columner can be used to retrieve column data", "[columner]")
{
  SECTION("single value")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto const int1 = 234;

    auto const [r, idx] = bind(s2, int1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    auto c = columner(s3);

    auto const int2 = c.get<int>();
    REQUIRE(int2 == int1);

    REQUIRE(c.index() == 0);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("multiple values")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL, y REAL NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1, ?2)"_ss);

    auto const int1 = 234;
    auto const double1 = 4.25;

    auto const [r, idx] = bind(s2, int1, double1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 2);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x, y FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    auto c = columner(s3);

    auto const [int2, double2] = c.get<int, double>();
    REQUIRE(int2 == int1);
    REQUIRE(double2 == double1);

    REQUIRE(c.index() == 1);

    REQUIRE(s3.step() == result::done);
  }
}

TEST_CASE(
  "A columner can be used to retrieve column data through a stream interface",
  "[columner]")
{
  SECTION("single value")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto const int1 = 234;

    auto const [r, idx] = bind(s2, int1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    auto c = columner(s3);

    auto const int2 = [&]() {
      auto i = 0;
      c >> i;
      return i;
    }();
    REQUIRE(int2 == int1);

    REQUIRE(c.index() == 0);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("multiple values")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL, y REAL NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1, ?2)"_ss);

    auto const int1 = 234;
    auto const double1 = 4.25;

    auto const [r, idx] = bind(s2, int1, double1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 2);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x, y FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    auto c = columner(s3);

    auto const [int2, double2] = [&]() {
      auto i = 0;
      auto dbl = 0.0;
      c >> i >> dbl;
      return std::make_tuple(i, dbl);
    }();
    REQUIRE(int2 == int1);
    REQUIRE(double2 == double1);

    REQUIRE(c.index() == 1);

    REQUIRE(s3.step() == result::done);
  }
}

TEST_CASE(
  "Values can be retrieved from a column through a generic free function",
  "[columner]")
{
  SECTION("single value")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto const int1 = 234;

    auto const [r, idx] = bind(s2, int1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    auto const int2 = column<int>(s3);
    REQUIRE(int2 == int1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("multiple values")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL, y REAL NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1, ?2)"_ss);

    auto const int1 = 234;
    auto const double1 = 4.25;

    auto const [r, idx] = bind(s2, int1, double1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 2);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x, y FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    auto const [int2, double2] = column<int, double>(s3);
    REQUIRE(int2 == int1);
    REQUIRE(double2 == double1);

    REQUIRE(s3.step() == result::done);
  }
}
