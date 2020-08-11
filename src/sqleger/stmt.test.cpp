#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/db.hpp>
#include <sqleger/stmt.hpp>


using namespace sqleger;

using namespace std::string_literals;
using namespace std::string_view_literals;


TEST_CASE("A stmt can be prepared and finalized", "[stmt]")
{
  SECTION("prepare v2, zstring")
  {
    auto d = db(":memory:");

    const auto q = "CREATE TABLE t(x INTEGER)"s;

    stmt s;
    const auto r1 = d.prepare_v2(q.c_str(), s);

    REQUIRE(r1 == result_t::ok);
    REQUIRE(s.c_ptr() != nullptr);
    REQUIRE(s.sql().c_str() == q);

    const auto r2 = s.finalize();

    REQUIRE(r2 == result_t::ok);
    REQUIRE(s.c_ptr() == nullptr);
  }

  SECTION("prepare v2, bounded string")
  {
    auto d = db(":memory:");

    const auto q1 = "CREATE TABLE t(x INTEGER)garbage"s;
    const auto q2 = std::string_view(q1.c_str(), 25);

    stmt s;
    const auto r1 = d.prepare_v2(q2, s);

    REQUIRE(r1 == result_t::ok);
    REQUIRE(s.c_ptr() != nullptr);
    REQUIRE(s.sql().c_str() == q2);

    const auto r2 = s.finalize();

    REQUIRE(r2 == result_t::ok);
    REQUIRE(s.c_ptr() == nullptr);
  }

  SECTION("prepare v2, failure")
  {
    auto d = db(":memory:");

    stmt s;
    const auto r = d.prepare_v2("I'm not SQL!", s);

    REQUIRE(is_error(r));
    REQUIRE(s.c_ptr() == nullptr);
  }
}

TEST_CASE("A stmt can be prepared through a constructor-exception interface",
          "[stmt]")
{
  SECTION("prepare v2, zstring")
  {
    auto d = db(":memory:");

    const auto q = "CREATE TABLE t(x INTEGER)"s;

    auto s = stmt(d, q.c_str());

    REQUIRE(s.c_ptr() != nullptr);
    REQUIRE(s.sql().c_str() == q);
  }

  SECTION("prepare v2, bounded string")
  {
    auto d = db(":memory:");

    const auto q1 = "CREATE TABLE t(x INTEGER)garbage"s;
    const auto q2 = std::string_view(q1.c_str(), 25);
 
    auto s = stmt(d, q2);

    REQUIRE(s.c_ptr() != nullptr);
    REQUIRE(s.sql().c_str() == q2);
  }

  SECTION("prepare v2, failure")
  {
    auto d = db(":memory:");

    try
    {
      auto s = stmt(d, "I'm not SQL!");
    }
    catch (const result_exception& e)
    {
      REQUIRE(is_error(e.code()));
    }
  }
}

TEST_CASE("A C handle can be taken from a stmt", "[stmt]")
{
  auto d = db(":memory:");
  auto s = stmt(d, "CREATE TABLE t(x INTEGER)"sv);

  const auto* const p1 = d.c_ptr();
  const auto* const p2 = d.take_c_ptr();

  REQUIRE(p1 == p2);
  REQUIRE(d.c_ptr() == nullptr);
}

TEST_CASE("A stmt can be moved", "[stmt]")
{
  SECTION("construction")
  {
    auto d = db(":memory:");
    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"sv);

    const auto* const p = s1.c_ptr();

    auto s2 = stmt(std::move(s1));

    REQUIRE(s2.c_ptr() == p);
    REQUIRE(s1.c_ptr() == nullptr);
  }

  SECTION("assignment into null")
  {
    auto d = db(":memory:");
    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"sv);

    const auto* const p = s1.c_ptr();

    stmt s2;
    s2 = std::move(s1);

    REQUIRE(s2.c_ptr() == p);
    REQUIRE(s1.c_ptr() == nullptr);
  }

  SECTION("assignment into prepared stmt")
  {
    auto d = db(":memory:");
    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"sv);

    const auto* const p = s1.c_ptr();

    auto s2 = stmt(d, "CREATE TABLE u(y INTEGER)"sv);
    s2 = std::move(s1);

    REQUIRE(s2.c_ptr() == p);
    REQUIRE(s1.c_ptr() == nullptr);
  }
}