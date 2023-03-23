# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
  name = "thread_pool",
  srcs = [
    "threadpool.cc",
    "task.cc",
    "thread.cc",
    "result.cc",
    "semaphore.cc",
  ],
  hdrs = [ 
    "threadpool.h",
    "task.h",
    "thread.h",
    "any.h",
    "result.h",
    "semaphore.h",
  ],
)

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
  name = "threadpooltest",
  srcs = [
    "main.cc",
    "mytask.h"
    ],
  copts = [],
  deps = [":thread_pool"],
)