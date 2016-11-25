#include <gtest/gtest.h>

#include <pool/InflatePool.h>


TEST (MemPoolTest, CreateTest) {
  qsl::InflatePool inflatePool;
  EXPECT_EQ (0, inflatePool.create());
}
