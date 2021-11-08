from conans import ConanFile

class TFGAKSAlgorithm(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    # Comma-separated list of requirements
    requires = [
        "benchmark/1.6.0",
        "catch2/2.13.7",
        "fmt/8.0.1",
        "gmp/6.2.1",
        "magic_enum/0.7.3",
        "mpfr/4.1.0",
    ]

    generators = ["cmake_find_package", "cmake_paths"]
