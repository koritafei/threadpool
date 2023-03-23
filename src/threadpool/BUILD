# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
  name = "thread_pool",
  srcs = [
    "threadpool.cc",
    "thread.cc",
  ],
  hdrs = [ 
    "threadpool.h",
    "thread.h",
  ],
)

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
  name = "threadpooltest",
  srcs = [
    "main.cc",
    ],
  copts = [],
  deps = [":thread_pool"],
)