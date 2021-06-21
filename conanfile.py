from conans import ConanFile

# conan with bazel (since Conan 1.37.0)
# https://github.com/conan-io/conan/issues/6235
# may/2021
# https://github.com/conan-io/conan/pull/8991
# https://github.com/conan-io/docs/pull/2109/files (jun/2021)
# https://docs.conan.io/en/latest/integrations/build_system/bazel.html?highlight=bazel

# ============
# INSTRUCTIONS
# ============
# pip install conan
# conan search tl-expected -r conan-center
# conan inspect tl-expected/20190710
# > write local conanfile.py (this file)
# conan install .
# > will generate 'conandeps/' folder
# > update WORKSPACE with dependency rule
# > bazel should work normally with target @tl-expected://tl-expected


class BazelExampleConan(ConanFile):
    name = "scanner-example"
    #
    # see: https://docs.conan.io/en/latest/reference/conanfile/tools/google.html
    generators = "BazelDeps", "BazelToolchain"
    build_requires = ["tl-expected/20190710", "benchmark/1.5.3"]
