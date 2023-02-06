#include <lib/parser.h>

#include <gtest/gtest.h>
#include <sstream>

using namespace omfl;

TEST(ParamTestSuite, ParamString) {
    std::stringstream stream("\"value1\" <some trash>");
    Param param(stream);

    ASSERT_TRUE(param.IsValid());
    ASSERT_TRUE(param.IsString());
    ASSERT_EQ(param.AsString(), "value1");
}

TEST(ParamTestSuite, ParamInt) {
    std::stringstream stream("228 <some trash>");
    Param param(stream);

    ASSERT_TRUE(param.IsValid());
    ASSERT_TRUE(param.IsInt());
    ASSERT_EQ(param.AsInt(), 228);
}

TEST(ParamTestSuite, ParamFloat) {
    std::stringstream stream("2.28 <some trash>");
    Param param(stream);

    ASSERT_TRUE(param.IsValid());
    ASSERT_TRUE(param.IsFloat());
    ASSERT_EQ(param.AsFloat(), 2.28f);
}

TEST(ParamTestSuite, ParamBool) {
    std::stringstream stream("true <some trash>");
    Param param(stream);

    ASSERT_TRUE(param.IsValid());
    ASSERT_TRUE(param.IsBool());
    ASSERT_EQ(param.AsBool(), true);
}

TEST(ParamTestSuite, ParamArray) {

    std::stringstream stream("[true, 1.5, [\"value\", 228]] <some trash>");
    Param param(stream);

    ASSERT_TRUE(param.IsValid());
    ASSERT_TRUE(param.IsArray());
    ASSERT_EQ(param[0].AsBool(), true);
    ASSERT_EQ(param[1].AsFloat(), 1.5f);
    ASSERT_EQ(param[2][0].AsString(), "value");
    ASSERT_EQ(param[2][1].AsInt(), 228);
}