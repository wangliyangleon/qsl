#include <gtest/gtest.h>

#include <pool/InflatePool.h>
#include <TextBuffer.h>

/// TODO: ugly duplicate codes :(

TEST(TextBufferWithMemPoolTest, NewBufferTest) {
  qsl::InflatePool inflatePool;
  EXPECT_EQ(0, inflatePool.create());
  qsl::TextBuffer textBuffer(inflatePool, 1024);
  EXPECT_EQ(static_cast<size_t>(1024), textBuffer.capacity());
  EXPECT_EQ(static_cast<size_t>(0), textBuffer.size());
  EXPECT_TRUE(textBuffer.empty());
  EXPECT_FALSE(textBuffer.full());
  EXPECT_FALSE(textBuffer.truncated());
  ASSERT_STREQ("", textBuffer.c_str());
}

TEST(TextBufferWithMemPoolTest, ReserveTest) {
  qsl::InflatePool inflatePool;
  EXPECT_EQ(0, inflatePool.create());
  qsl::TextBuffer textBuffer(inflatePool, 32);
  EXPECT_EQ(static_cast<size_t>(32), textBuffer.capacity());

  EXPECT_TRUE(textBuffer.reserve(33));
   // at least double previous capacity, or double the new capacity
  EXPECT_EQ(static_cast<size_t>(66), textBuffer.capacity());

  EXPECT_TRUE(textBuffer.reserve(256));
  EXPECT_EQ(static_cast<size_t>(256), textBuffer.capacity());

  EXPECT_TRUE(textBuffer.reserve(32));
  // does not decrease capacity
  EXPECT_EQ(static_cast<size_t>(256), textBuffer.capacity());

  EXPECT_TRUE(textBuffer.reserve(0));
   // does not decrease capacity, even 0
  EXPECT_EQ(static_cast<size_t>(256), textBuffer.capacity());
}

TEST(TextBufferWithMemPoolTest, PushTest) {
  qsl::InflatePool inflatePool;
  EXPECT_EQ(0, inflatePool.create());
  qsl::TextBuffer textBuffer(inflatePool, 16);
  EXPECT_EQ(static_cast<size_t>(16), textBuffer.capacity());

  textBuffer.push('S');
  EXPECT_EQ(static_cast<size_t>(1), textBuffer.size());
  EXPECT_FALSE(textBuffer.empty());
  EXPECT_FALSE(textBuffer.full());
  EXPECT_FALSE(textBuffer.truncated());
  ASSERT_STREQ("S", textBuffer.c_str());

  textBuffer.push("ta");
  EXPECT_EQ(static_cast<size_t>(3), textBuffer.size());
  EXPECT_FALSE(textBuffer.empty());
  EXPECT_FALSE(textBuffer.full());
  EXPECT_FALSE(textBuffer.truncated());
  ASSERT_STREQ("Sta", textBuffer.c_str());

  textBuffer.push("rt", 1);
  EXPECT_EQ(static_cast<size_t>(4), textBuffer.size());
  EXPECT_FALSE(textBuffer.empty());
  EXPECT_FALSE(textBuffer.full());
  EXPECT_FALSE(textBuffer.truncated());
  ASSERT_STREQ("Star", textBuffer.c_str());

  textBuffer.pushf("%c", 't');
  EXPECT_EQ(static_cast<size_t>(5), textBuffer.size());
  EXPECT_FALSE(textBuffer.empty());
  EXPECT_FALSE(textBuffer.full());
  EXPECT_FALSE(textBuffer.truncated());
  ASSERT_STREQ("Start", textBuffer.c_str());

  textBuffer << 2;
  EXPECT_EQ(static_cast<size_t>(6), textBuffer.size());
  EXPECT_FALSE(textBuffer.empty());
  EXPECT_FALSE(textBuffer.full());
  EXPECT_FALSE(textBuffer.truncated());
  ASSERT_STREQ("Start2", textBuffer.c_str());

  textBuffer << "Rock!!!";
  EXPECT_EQ(static_cast<size_t>(13), textBuffer.size());
  EXPECT_FALSE(textBuffer.empty());
  EXPECT_FALSE(textBuffer.full());
  EXPECT_FALSE(textBuffer.truncated());
  ASSERT_STREQ("Start2Rock!!!", textBuffer.c_str());

  EXPECT_EQ(static_cast<size_t>(16), textBuffer.capacity());
}

TEST(TextBufferWithMemPoolTest, PushLongTest) {
  qsl::InflatePool inflatePool;
  EXPECT_EQ(0, inflatePool.create());
  qsl::TextBuffer textBuffer(inflatePool, 8);
  EXPECT_EQ(static_cast<size_t>(8), textBuffer.capacity());

  textBuffer.push("abcdefgh");
  EXPECT_EQ(static_cast<size_t>(8), textBuffer.capacity());
  EXPECT_EQ(static_cast<size_t>(8), textBuffer.size());
  EXPECT_FALSE(textBuffer.empty());
  EXPECT_TRUE(textBuffer.full());
  EXPECT_FALSE(textBuffer.truncated());
  ASSERT_STREQ("abcdefgh", textBuffer.c_str());

  textBuffer.push("hijk");
  EXPECT_EQ(static_cast<size_t>(12), textBuffer.size());
  EXPECT_EQ(static_cast<size_t>(24), textBuffer.capacity());
  EXPECT_FALSE(textBuffer.empty());
  EXPECT_FALSE(textBuffer.full());
  EXPECT_FALSE(textBuffer.truncated());
  ASSERT_STREQ("abcdefghhijk", textBuffer.c_str());

  textBuffer << true;
  EXPECT_EQ(static_cast<size_t>(16), textBuffer.size());
  EXPECT_EQ(static_cast<size_t>(24), textBuffer.capacity());
  EXPECT_FALSE(textBuffer.empty());
  EXPECT_FALSE(textBuffer.full());
  EXPECT_FALSE(textBuffer.truncated());
  ASSERT_STREQ("abcdefghhijktrue", textBuffer.c_str());
}

TEST(TextBufferWithMemPoolTest, SwapTransferTest) {
  qsl::InflatePool inflatePool;
  EXPECT_EQ(0, inflatePool.create());
  qsl::TextBuffer textBufferA(inflatePool, 8);
  qsl::TextBuffer textBufferB(inflatePool, 16);
  textBufferA << "I am A";
  textBufferB << "I am B ah";
  EXPECT_EQ(static_cast<size_t>(6), textBufferA.size());
  EXPECT_EQ(static_cast<size_t>(8), textBufferA.capacity());
  ASSERT_STREQ("I am A", textBufferA.c_str());
  EXPECT_EQ(static_cast<size_t>(9), textBufferB.size());
  EXPECT_EQ(static_cast<size_t>(16), textBufferB.capacity());
  ASSERT_STREQ("I am B ah", textBufferB.c_str());

  textBufferA.swap(textBufferB);
  EXPECT_EQ(static_cast<size_t>(6), textBufferB.size());
  EXPECT_EQ(static_cast<size_t>(8), textBufferB.capacity());
  ASSERT_STREQ("I am A", textBufferB.c_str());
  EXPECT_EQ(static_cast<size_t>(9), textBufferA.size());
  EXPECT_EQ(static_cast<size_t>(16), textBufferA.capacity());
  ASSERT_STREQ("I am B ah", textBufferA.c_str());

  textBufferA.transferTextBuffer(textBufferB);
  EXPECT_EQ(static_cast<size_t>(6), textBufferA.size());
  EXPECT_EQ(static_cast<size_t>(8), textBufferA.capacity());
  ASSERT_STREQ("I am A", textBufferA.c_str());
  EXPECT_EQ(static_cast<size_t>(0), textBufferB.size());
  EXPECT_EQ(static_cast<size_t>(0), textBufferB.capacity());
  ASSERT_STREQ("", textBufferB.c_str());
}

TEST(TextBufferWithMemPoolTest, ClearTest) {
  qsl::InflatePool inflatePool;
  EXPECT_EQ(0, inflatePool.create());
  qsl::TextBuffer textBuffer(inflatePool, 4);
  textBuffer << "test";
  EXPECT_EQ(static_cast<size_t>(4), textBuffer.capacity());
  EXPECT_EQ(static_cast<size_t>(4), textBuffer.size());
  EXPECT_FALSE(textBuffer.empty());
  EXPECT_TRUE(textBuffer.full());
  EXPECT_FALSE(textBuffer.truncated());
  ASSERT_STREQ("test", textBuffer.c_str());

  textBuffer.clear();
  EXPECT_EQ(static_cast<size_t>(4), textBuffer.capacity());
  EXPECT_EQ(static_cast<size_t>(0), textBuffer.size());
  EXPECT_TRUE(textBuffer.empty());
  EXPECT_FALSE(textBuffer.full());
  EXPECT_FALSE(textBuffer.truncated());
  ASSERT_STREQ("", textBuffer.c_str());
}
