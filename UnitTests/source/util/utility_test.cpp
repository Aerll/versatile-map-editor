#include <catch2.pch>

#include <ddnet/util/utility.hpp>

#include <QByteArray>
#include <QIODevice>

using namespace ddnet;

std::variant<short, int, float, double> v;
static_assert(util::typeIndex<short>(v) == 0);
static_assert(util::typeIndex<int>(v) == 1);
static_assert(util::typeIndex<float>(v) == 2);
static_assert(util::typeIndex<double>(v) == 3);

enum class Enum_qint16 : qint16 { Value };
enum class Enum_qint64 : qint64 { Value };
static_assert(std::is_same_v<decltype(util::toUnderlying(Enum_qint16::Value)), qint16>);
static_assert(std::is_same_v<decltype(util::toUnderlying(Enum_qint64::Value)), qint64>);

static_assert(util::flag(0) == 1 << 0);
static_assert(util::flag(5) == 1 << 5);

TEST_CASE("util::byteSize") {
    SECTION("Size of the std::vector in bytes") {
        std::vector<qint16> v1;
        v1.resize(10);
        CHECK(util::byteSize(v1) == 20);

        std::vector<qint32> v2;
        v2.resize(10);
        CHECK(util::byteSize(v2) == 40);
    }

    SECTION("Size of the QByteArray in bytes") {
        QByteArray b;
        b.resize(50);
        CHECK(util::byteSize(b) == 50);
    }
}

TEST_CASE("util::totalSize") {
    SECTION("Total size of the std::vector<QByteArray>") {
        std::vector<QByteArray> v;
        v.reserve(3);
        v.push_back(QByteArray{ "a string" }); // 8
        v.push_back(QByteArray{ "another" });  // 7
        v.push_back(QByteArray{ "last" });     // 4
        CHECK(util::totalSize(v) == 19);
    }
}

TEST_CASE("util::readRawData & util::writeRawData") {
    SECTION("Compare contents") {
        QByteArray buffer;
        QDataStream dstream{ &buffer, QIODevice::ReadWrite };

        qint64 val = 88;
        util::writeRawData(dstream, &val, sizeof(qint64));
        CHECK(dstream.device()->size() == sizeof(qint64));

        dstream.device()->reset();
        qint64 test_val;

        util::readRawData(dstream, &test_val, sizeof(qint64));
        CHECK(test_val == val);
    }
}
