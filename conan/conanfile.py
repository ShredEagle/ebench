from conans import ConanFile, CMake, tools

from os import path


class EbenchConan(ConanFile):
    """ Conan recipe for Ebench """

    name = "ebench"
    license = "MIT License"
    url = "https://github.com/Adnn/ebench"
    description = "Benchmark game engine designs."
    topics = ("game", "benchmark")
    settings = ("os", "compiler", "build_type", "arch")
    options = {
        "build_tests": [True, False],
        "shared": [True, False],
        "visibility": ["default", "hidden"],
    }
    default_options = {
        "build_tests": False,
        "shared": False,
        "visibility": "hidden"
    }

    requires = ("aunteater/0.8.0",)

    build_requires = ("cmake/[>=3.16]",)

    build_policy = "missing"
    generators = "cmake_paths", "cmake"
    revision_mode = "scm"

    scm = {
        "type": "git",
        # Not using auto url: Azure CI clones via https, so auto exported recipe would use https
        "url": "git@github.com:Adnn/ebench.git",
        "revision": "auto",
        "submodule": "recursive",
    }


    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions["BUILD_tests"] = self.options.build_tests
        cmake.definitions["CMAKE_CXX_VISIBILITY_PRESET"] = self.options.visibility
        cmake.definitions["CMAKE_PROJECT_Ebench_INCLUDE"] = \
            path.join(self.source_folder, "cmake", "conan", "customconan.cmake")
        cmake.configure()
        return cmake


    def build(self):
        cmake = self._configure_cmake()
        cmake.build()


    def package(self):
        cmake = self._configure_cmake()
        cmake.install()


    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
